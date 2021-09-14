#define _GNU_SOURCE
#include <fastlog_decode.h>
#include <arpa/inet.h>

/**
 *  映射的文件
 */
static struct fastlog_file_mmap ro_metadata_mmap, ro_logdata_mmap;

static struct fastlog_file_header *metadata_header = NULL;
static struct fastlog_file_header *logdata_header = NULL;


int output_open(struct output_struct *output, char *filename)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }

    if(filename) {
        output->filename = strdup(filename);
    }
    
    progress_reset(&pro_bar, filename);

    return output->ops->open(output);
}

int output_header(struct output_struct *output, struct fastlog_file_header *header)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }
    return output->ops->header(output, header);
}

int output_setfilter(struct output_struct *output, struct output_filter *filter, struct output_filter_arg arg)
{
    assert(output && filter && "NULL error.");

    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }

    if(output->filter_num > __LOG__RANGE_FILTER_NUM) {
        assert(0 && "filter number out of range.");
    }

    output->filter[output->filter_num] = filter;
    output->filter_arg[output->filter_num] = arg;
    output->filter_num++;
    
    return 0;
}

bool output_callfilter(struct output_struct *output, struct logdata_decode *logdata)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }
    int i;
    struct output_filter *filter = NULL;
    struct output_filter_arg *filter_arg = NULL;
    
    for(i=0; i<output->filter_num; i++) {
        filter = output->filter[i];
        filter_arg = &output->filter_arg[i];
    
        /* 如果 filter 为空，等同于 匹配成功 */
        if(!filter) continue;

        if(filter->log_range & LOG__RANGE_CONTENT) {
            //不匹配
            if(!filter->match_log_content_ok(filter, logdata, filter_arg->log_buffer, filter_arg->value)) {
                return false;
            }
        } else {
            //不匹配
            if(!filter->match_prefix_ok(filter, logdata, filter_arg->value)) {
                return false;
            }
        }
    }

    return true;
}

int output_updatefilter_arg(struct output_struct *output, char *log_buffer)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }
    int i;
    for(i=0; i<output->filter_num; i++) {
        output->filter_arg[i].log_buffer = log_buffer;
    }
    return 0;
}

int output_clearfilter(struct output_struct *output)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }
    int i;
    for(i=0; i<output->filter_num; i++) {
        output->filter[i] = NULL;
        output->filter_arg[i] = output_filter_arg_null;
    }
    output->filter_num = 0;
    return 0;
}

void progress_output(struct output_struct *output, unsigned long i, unsigned long total_num)
{
    float f = 0.0f;
    int interval = total_num>100?total_num/100:total_num;
    
    //当输出到文件时，显示进度条
    if(output->filename) {
        if(i % interval == 0  || i == total_num) {
            if(total_num >= 100) {
                f = (i*1.0/total_num*100.0f)/100.0f;
            } else {
                f = i*1.0/total_num;
            }
            progress_show(&pro_bar, f);
        }
    }
}

void output_metadata(struct metadata_decode *meta, void *arg)
{
    //printf("metadata_print logID %d\n", meta->log_id);
    
    struct output_struct *output = (struct output_struct *)arg;
    
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return;
    }
    
    output->ops->meta_item(output, meta);

    progress_output(output, output->output_meta_cnt, decoder_config.total_fmeta_num);
}

void output_logdata(struct logdata_decode *logdata, void *arg)
{
    struct output_struct *output = (struct output_struct *)arg;

    if(!output->enable) {
        return ;
    }

    // 从 "Hello, %s, %d" + World\02021 
    // 转化为
    // Hello, World, 2021
    reprintf(logdata, output);
}


//在`reprintf`中被调用
int output_log_item(struct output_struct *output, struct logdata_decode *logdata, char *log)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }

    int ret = 0;
    
    ret = output->ops->log_item(output, logdata, log);

    progress_output(output, output->output_log_cnt, decoder_config.total_flog_num);

    return ret;
}

int output_footer(struct output_struct *output)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }
    return output->ops->footer(output);
}
int output_close(struct output_struct *output)
{
    if(!output->enable) {
        fprintf(stderr, "file type not support.\n");
        return -1;
    }
    int _unused ret = output->ops->close(output);

    if(output->filename) {
        free(output->filename);
        output->filename = NULL;    
        
        printf("\n");   //当输出到文件时，显示进度条，这时需要最后输出一个换行
    }

    output_clearfilter(output);
    
    return 0;
}


int release_file(struct fastlog_file_mmap *mapfile)
{
    return unmmap_fastlog_logfile(mapfile);
}

/* 以只读方式映射一个文件 */
static int load_file(struct fastlog_file_mmap *mapfile, char *file, 
                        struct fastlog_file_header **hdr, unsigned int magic)
{
    int ret;
    
    //元数据文件映射
    ret = mmap_fastlog_logfile_read(mapfile, file);
    if(ret) {
        return -1;
    }
    *hdr = (struct fastlog_file_header *)mapfile->mmapaddr;

    /* 大小端，当解析该日志文件的服务器与生成日志文件的服务器 endian 不同，不可解析 */
    if(ntohl((*hdr)->endian) != FASTLOG_LOG_FILE_ENDIAN_MAGIC) {
        fprintf(stderr, "Endian wrong.\n");
        release_file(mapfile);
        return -1;
    }

    if((*hdr)->magic != magic) {
        fprintf(stderr, "%s is not fastlog metadata file.\n", file);
        release_file(mapfile);
        return -1;
    }

    return 0;
}
                        
struct fastlog_file_mmap *meta_mmapfile()
{
    return &ro_metadata_mmap;
}

struct fastlog_file_mmap *log_mmapfile()
{
    return &ro_logdata_mmap;
}

/* 初始化即可任意访问 */
struct fastlog_file_header *meta_hdr()
{
    return metadata_header;
}

/* 只有在映射了 log 文件时，该接口才可用
    用于提供 可以动态加载多个 日志文件的 功能*/
struct fastlog_file_header *log_hdr()
{
    return logdata_header;
}


int release_metadata_file()
{
    return release_file(&ro_metadata_mmap);
}
int release_logdata_file()
{
    return release_file(&ro_logdata_mmap);
}

int load_metadata_file(char *mmapfile_name)
{
    return load_file(&ro_metadata_mmap, mmapfile_name, &metadata_header, FATSLOG_METADATA_HEADER_MAGIC_NUMBER);
}

int load_logdata_file(char *mmapfile_name)
{
    return load_file(&ro_logdata_mmap, mmapfile_name, &logdata_header, FATSLOG_LOG_HEADER_MAGIC_NUMBER);
}

int match_metadata_and_logdata()
{
    uint64_t metadata_cycles_per_sec = metadata_header->cycles_per_sec;
    uint64_t metadata_start_rdtsc = metadata_header->start_rdtsc;
    uint64_t logdata_cycles_per_sec = logdata_header->cycles_per_sec;
    uint64_t logdata_start_rdtsc = logdata_header->start_rdtsc;

    /* 在写入时，这两个应该数值完全相等，所以这里进行检测 */
    return ((metadata_cycles_per_sec == logdata_cycles_per_sec) 
        && (metadata_start_rdtsc == logdata_start_rdtsc));
}

