#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/livepatch.h>
#include <linux/sched.h>
#include <linux/sched/loadavg.h>
#include <linux/sched/stat.h>
#include <linux/pid_namespace.h>
#include <linux/proc_fs.h>

#include <linux/seq_file.h>

#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

static int livepatch_loadavg_proc_show(struct seq_file *m, void *v) 
{
	    unsigned long avnrun[3];

#if 0
	    get_avenrun(avnrun, FIXED_1/200, 0); 

		seq_printf(m, "RongTao: %lu.%02lu %lu.%02lu %lu.%02lu %ld/%d %d\n",
	        LOAD_INT(avnrun[0]), LOAD_FRAC(avnrun[0]),
	        LOAD_INT(avnrun[1]), LOAD_FRAC(avnrun[1]),
	        LOAD_INT(avnrun[2]), LOAD_FRAC(avnrun[2]),
	        nr_running(), nr_threads,
	        idr_get_cursor(&task_active_pid_ns(current)->idr) - 1); 
#elif 0
	seq_printf(m, "RongTao: %lu.%02lu %lu.%02lu %lu.%02lu\n",
		   LOAD_INT(avnrun[0]), LOAD_FRAC(avnrun[0]),
		   LOAD_INT(avnrun[1]), LOAD_FRAC(avnrun[1]),
		   LOAD_INT(avnrun[2]), LOAD_FRAC(avnrun[2]));
#else
	seq_printf(m, "RongTao: \n");
#endif
	    return 0;
}

static struct klp_func funcs[] = {
	{
		.old_name = "loadavg_proc_show",
		.new_func = livepatch_loadavg_proc_show,
	}, { }
};

static struct klp_object objs[] = {
	{
		/* name being NULL means vmlinux */
		.funcs = funcs,
	}, { }
};

static struct klp_patch patch = {
	.mod = THIS_MODULE,
	.objs = objs,
};

static int livepatch_init(void)
{
	int ret;

	ret = klp_enable_patch(&patch);
	if (ret) {
		return ret;
	}
	printk(KERN_INFO "livepatch insert.");
	return 0;
}

static void livepatch_exit(void)
{
	printk(KERN_INFO "livepatch exit.");
}

module_init(livepatch_init);
module_exit(livepatch_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");
