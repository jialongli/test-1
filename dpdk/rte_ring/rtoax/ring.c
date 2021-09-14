/***********************************************************************************\
 *  文件名   ：ring.c
 *
 *  描述：实现无锁多生产者多消费者 ring 队列
 *  作者：荣涛
 *  日期：2021年1月22日
 *  
\***********************************************************************************/

#ifndef __x86_64__
# error "Not support Your Arch, just support x86-64"
#endif

 
#include <errno.h>
#include <assert.h>
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>

#include "ring.h"


enum {
//                           tail
//                            |
//    ------------------------+--------------- RING_EMPTY
//                            |
//                           head
    RING_EMPTY,
    
//                           tail
//                            |
//    -----------------*******+--------------- RING_PARTIAL1
//                     |
//                    head
    RING_PARTIAL1,
    
//                    tail
//                     |
//    *****************+------**************** RING_PARTIAL2
//                            |
//                          head
    RING_PARTIAL2,
    
//                           tail
//                            |
//    **************************************** RING_FULL
//                            |
//                           head
    RING_FULL,

    RING_ENQUEUING, //正在入队
    RING_DEQUEUING, //正在出队
    RING_PARAMING,  //正在修改参数
};


#ifndef likely
#define likely(exp) __builtin_expect(!!exp, 1)
#endif
#ifndef unlikely
#define unlikely(exp) __builtin_expect(!!exp, 0)
#endif
 
#ifdef RING_DEBUG

#define debug_log_enqueue(fmt...)  do{                                  \
        printf("\033[33m[%d: %s-%d]", getpid(), __func__, __LINE__);    \
        printf(fmt);printf("\033[m");                                   \
    }while(0)
    
#define debug_log_dequeue(fmt...)  do{                                  \
        printf("\033[35m[%d: %s-%d]", getpid(), __func__, __LINE__);    \
        printf(fmt);printf("\033[m");                                   \
    }while(0)

#define TAIL_CHECK(ring)  do{                                           \
        if(atomic64_read(&ring->tail) > ring->elem_nr) {                \
            debug_log_enqueue("tail error. head %ld, tail %ld\n",       \
            atomic64_read(&ring->head), atomic64_read(&ring->tail));    \
            abort();                                                    \
        }                                                               \
    }while(0)

#define HEAD_CHECK(ring)  do{                                           \
        if(atomic64_read(&ring->head) > ring->elem_nr) {                \
            debug_log_dequeue("head error. head %ld, tail %ld\n",       \
            atomic64_read(&ring->head), atomic64_read(&ring->tail));    \
            abort();                                                    \
        }                                                               \
    }while(0)

#else

 #define debug_log_enqueue(fmt...) do{}while(0)
 #define debug_log_dequeue(fmt...) do{}while(0)
 #define TAIL_CHECK(ring) do{}while(0)
 #define HEAD_CHECK(ring) do{}while(0)
 
#endif

always_inline int ring_init(struct ring_struct *ring, const char *name, size_t elem_num)
{
    if(unlikely(!ring) || unlikely(!elem_num)) {
        fprintf(stderr, "[%s:%d]null pointer error.\n", __func__, __LINE__);
        return RING_IS_ERR;
    }
    int i;

    if(name) {
        strncpy(ring->name, name, 64);
    } else {
        strncpy(ring->name, RING_DEFAULT_NAME, 64);
    }

    atomic64_set(&ring->stat, RING_EMPTY);
    atomic64_init(&ring->head);
    atomic64_init(&ring->tail);
    atomic64_init(&ring->count_enqueue);
    atomic64_init(&ring->count_dequeue);
    
//    atomic64_set(&ring->head, elem_num/2);
//    atomic64_set(&ring->tail, elem_num/2);

//    atomic64_set(&ring->head, elem_num-1);
//    atomic64_set(&ring->tail, elem_num-1);

    ring->elem_nr = elem_num;

    for(i=0; i<ring->elem_nr; i++) {
        ring->elems[i].data = NULL;
    }
    
    return RING_IS_OK;
}



always_inline struct ring_struct *ring_create(const char *name, size_t elem_num)
{
    if(unlikely(!elem_num)) {
        fprintf(stderr, "[%s:%d]wrong parameter error, use default %ld.\n", \
                            __func__, __LINE__, RING_DEFAULT_SIZE);
        elem_num = RING_DEFAULT_SIZE;
    }
    const unsigned long read_size = sizeof(struct ring_struct) + sizeof(struct ring_element)*elem_num;
    struct ring_struct *ring = malloc(read_size);
    assert(ring);
    
    memset(ring, 0x00, read_size);
    
    if(RING_IS_ERR == ring_init(ring, name, elem_num)) {
        free(ring);
        return NULL;
    }

    return ring;
}

always_inline int ring_destroy(struct ring_struct *ring)
{
    if(unlikely(!ring)) {
        fprintf(stderr, "[%s:%d]null pointer error.\n", __func__, __LINE__);
        return RING_IS_ERR;
    }
    if(atomic64_read(&ring->count_enqueue) != atomic64_read(&ring->count_dequeue)) {
        fprintf(stdout, "[%s:%d warning] ring still has msg(%ld) remain to dequeue .\n", __func__, __LINE__,
            atomic64_read(&ring->count_enqueue)-atomic64_read(&ring->count_dequeue));        
    }
    free(ring);
    return RING_IS_OK;
}

always_inline int ring_is_full(struct ring_struct *ring)
{
    return atomic64_cmpset(&ring->stat, RING_FULL, RING_FULL);
}
always_inline int ring_is_empty(struct ring_struct *ring)
{
    return atomic64_cmpset(&ring->stat, RING_EMPTY, RING_EMPTY);
}

always_inline int ring_dump(FILE *fp, struct ring_struct *ring)
{
    if(unlikely(!ring)) {
        fprintf(stderr, "[%s:%d]null pointer error.\n", __func__, __LINE__);
        return RING_IS_ERR;
    }
    fprintf(fp, "[%s]== ring size %ld\n", ring->name, ring->elem_nr);
    fprintf(fp, "[%s]== enqueue %ld\n", ring->name, atomic64_read(&ring->count_enqueue));
    fprintf(fp, "[%s]== dequeue %ld\n", ring->name, atomic64_read(&ring->count_dequeue));
    
    return RING_IS_OK;
}


always_inline int ring_try_enqueue(struct ring_struct *ring, void* addr)
{
    /* 已满 */
    //                           tail
    //                            |
    //    ********************************************** RING_FULL
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_FULL, RING_PARAMING)) {
        atomic64_cmpset(&ring->stat, RING_PARAMING, RING_FULL);
        return RING_IS_FULL;
    }
    /* 空 */
    //                           tail
    //                            |
    //    ------------------------+--------------------- RING_EMPTY
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_EMPTY, RING_PARAMING)) {
        debug_log_enqueue("empty -> paraming.\n");
        
        /* 添加 */
        ring->elems[atomic64_read(&ring->tail)].data = addr;    
        atomic64_inc(&ring->tail);  
        atomic64_inc(&ring->count_enqueue); 
        debug_log_enqueue("enqueue %ld add to %ld. head(%ld)\n", 
            atomic64_read(&ring->count_enqueue), atomic64_read(&ring->tail), atomic64_read(&ring->head));
        TAIL_CHECK(ring);

        
        //                                                tail
        //                                                 |
        //    ---------------------------------------------+ RING_EMPTY
        //                                                 |
        //                                                head
        //   tail
        //    |
        //    +--------------------------------------------* RING_PARTIAL2
        //                                                 |
        //                                                head
        if(atomic64_cmpset(&ring->tail, ring->elem_nr, 0)) {
            atomic64_cmpset(&ring->stat, RING_PARAMING, RING_PARTIAL2);
            debug_log_enqueue("paraming -> partial2.\n");
        
        //                           tail
        //                            |
        //    ------------------------+--------------------- RING_EMPTY
        //                            |
        //                           head
        //                            tail
        //                             |
        //    ------------------------*+--------------------- RING_PARTIAL1
        //                            |
        //                           head
        } else {
            atomic64_cmpset(&ring->stat, RING_PARAMING, RING_PARTIAL1);
            debug_log_enqueue("paraming -> partial1.\n");
        }
        return RING_IS_OK;
    }
    
    //                                 tail
    //                                  |
    //    ------------------------******+----------------- RING_PARTIAL1
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_PARTIAL1, RING_ENQUEUING)) {
        debug_log_enqueue("partial1 -> enqueuing.\n");
        /* 添加 */
        ring->elems[atomic64_read(&ring->tail)].data = addr;    
        atomic64_inc(&ring->tail);  
        atomic64_inc(&ring->count_enqueue); 
        debug_log_enqueue("enqueue %ld add to %ld. head(%ld)\n", 
            atomic64_read(&ring->count_enqueue), atomic64_read(&ring->tail), atomic64_read(&ring->head));
        TAIL_CHECK(ring);
        
        /* tail 到头了 */
        //                                                  tail
        //                                                   |
        //    ------------------------***********************+ RING_PARTIAL1
        //                            |
        //                           head
        if(atomic64_cmpset(&ring->tail, ring->elem_nr, 0)) {
            //                                                  tail
            //                                                   |
            //    ***********************************************+ RING_PARTIAL1
            //    |
            //   head
            //   tail
            //    |
            //    ************************************************ RING_FULL
            //    |
            //   head
            if(atomic64_cmpset(&ring->head, 0, 0)) {
                debug_log_enqueue("enqueuing to full.\n");
                atomic64_cmpset(&ring->stat, RING_ENQUEUING, RING_FULL);
            
            //                                                  tail
            //                                                   |
            //    ------------------------***********************+ RING_PARTIAL1
            //                            |
            //                           head
            //  tail
            //    |
            //    +-----------------------************************ RING_PARTIAL2
            //                            |
            //                           head
            } else {
                debug_log_enqueue("enqueuing to partial2.\n");
                atomic64_cmpset(&ring->stat, RING_ENQUEUING, RING_PARTIAL2);
            }
            
        //                                 tail
        //                                  |
        //    ------------------------******+----------------- RING_PARTIAL1
        //                            |
        //                           head
        //                                  tail
        //                                   |
        //    ------------------------*******+---------------- RING_PARTIAL1
        //                            |
        //                           head
        } else {
            debug_log_enqueue("enqueuing to partial1.\n");
            atomic64_cmpset(&ring->stat, RING_ENQUEUING, RING_PARTIAL1);
        }
        
        return RING_IS_OK;
    }
    
    //       tail
    //        |
    //    ****+-------------------************************ RING_PARTIAL2
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_PARTIAL2, RING_ENQUEUING)) {
        debug_log_enqueue("partial2 to enqueuing.\n");
        /* 添加 */
        ring->elems[atomic64_read(&ring->tail)].data = addr;    
        atomic64_inc(&ring->tail);  
        atomic64_inc(&ring->count_enqueue); 
        TAIL_CHECK(ring);
        debug_log_enqueue("enqueue %ld add to %ld. head(%ld)\n", 
            atomic64_read(&ring->count_enqueue), atomic64_read(&ring->tail), atomic64_read(&ring->head));
        
        //                          tail
        //                           |
        //    ***********************+************************ RING_PARTIAL2
        //                            |
        //                           head
        //                           tail
        //                            |
        //    ************************************************ RING_FULL
        //                            |
        //                           head
        if(atomic64_read(&ring->head) == atomic64_read(&ring->tail)) {
            atomic64_set(&ring->stat, RING_FULL);
            debug_log_enqueue("enqueuing to full.\n");
        
        //       tail
        //        |
        //    ****+-------------------************************ RING_PARTIAL2
        //                            |
        //                           head
        //        tail
        //         |
        //    *****+------------------************************ RING_PARTIAL2
        //                            |
        //                           head
        } else {
            debug_log_enqueue("enqueuing to partial2.\n");
            atomic64_cmpset(&ring->stat, RING_ENQUEUING, RING_PARTIAL2);
        }
        return RING_IS_OK;
    }

    return RING_IS_ERR;
}

always_inline int ring_force_enqueue(struct ring_struct *ring, void* addr)
{
    int ret = RING_IS_ERR;
    while(RING_IS_OK != (ret=ring_try_enqueue(ring, addr)));
    return ret;
}


always_inline int ring_try_dequeue(struct ring_struct *ring, void **data)
{
    /* 空 */
    //                           tail
    //                            |
    //    ---------------------------------------------- RING_FULL
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_EMPTY, RING_PARAMING)) {
        atomic64_cmpset(&ring->stat, RING_PARAMING, RING_EMPTY);
        *data = NULL;
        return RING_IS_EMPTY;
    }
    
    /* 满 */
    //                           tail
    //                            |
    //    ************************************************ RING_FULL
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_FULL, RING_DEQUEUING)) {
        
        *data = ring->elems[atomic64_read(&ring->head)].data;
        atomic64_inc(&ring->head);
        atomic64_inc(&ring->count_dequeue); 
        HEAD_CHECK(ring);
        debug_log_dequeue("dequeue %ld from %ld. tail(%ld)\n", 
            atomic64_read(&ring->count_dequeue), atomic64_read(&ring->head), atomic64_read(&ring->tail));
        
        /* head 到头了 */
        //                                                  tail
        //                                                   |
        //    ************************************************ RING_FULL
        //                                                   |
        //                                                  head
        //                                                  tail
        //                                                   |
        //    ***********************************************+ RING_PARTIAL1
        //    |
        //   head
        if(atomic64_cmpset(&ring->head, ring->elem_nr, 0)) {
            atomic64_set(&ring->stat, RING_PARTIAL1);
            debug_log_dequeue("full to partial1.\n");

            
        //                           tail
        //                            |
        //    ************************************************ RING_FULL
        //                            |
        //                           head
        //                           tail
        //                            |
        //    ************************+*********************** RING_PARTIAL2
        //                             |
        //                            head
        } else {
            atomic64_set(&ring->stat, RING_PARTIAL2);
            debug_log_dequeue("full to partial2.\n");
        }        
        return RING_IS_OK;
    }

    //                                 tail
    //                                  |
    //    ------------------------******+----------------- RING_PARTIAL1
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_PARTIAL1, RING_DEQUEUING)) {
        
        *data = ring->elems[atomic64_read(&ring->head)].data;
        atomic64_inc(&ring->head);
        atomic64_inc(&ring->count_dequeue); 
        HEAD_CHECK(ring);
        debug_log_dequeue("dequeue %ld from %ld. tail(%ld)\n", 
            atomic64_read(&ring->count_dequeue), atomic64_read(&ring->head), atomic64_read(&ring->tail));
            
        //                                 tail
        //                                  |
        //    -----------------------------*+----------------- RING_PARTIAL1
        //                                 |
        //                                head
        //                                 tail
        //                                  |
        //    ------------------------------+----------------- RING_EMPTY
        //                                  |
        //                                 head
        if(atomic64_read(&ring->head) == atomic64_read(&ring->tail)) {
            debug_log_dequeue("partial1 to empty.\n");
            atomic64_cmpset(&ring->stat, RING_DEQUEUING, RING_EMPTY);
        
        //                                 tail
        //                                  |
        //    ------------------------******+----------------- RING_PARTIAL1
        //                            |
        //                           head
        //                                 tail
        //                                  |
        //    -------------------------*****+----------------- RING_PARTIAL1
        //                             |
        //                            head
        } else {
            atomic64_cmpset(&ring->stat, RING_DEQUEUING, RING_PARTIAL1);
        }     
        return RING_IS_OK;
    }

    
    //       tail
    //        |
    //    ****+-------------------************************ RING_PARTIAL2
    //                            |
    //                           head
    if(atomic64_cmpset(&ring->stat, RING_PARTIAL2, RING_DEQUEUING)) {
        
        *data = ring->elems[atomic64_read(&ring->head)].data;
        atomic64_inc(&ring->head);
        atomic64_inc(&ring->count_dequeue); 
        HEAD_CHECK(ring);
        debug_log_dequeue("dequeue %ld from %ld. tail(%ld)\n", 
            atomic64_read(&ring->count_dequeue), atomic64_read(&ring->head), atomic64_read(&ring->tail));

        /* head 到头了 */
        //       tail
        //        |
        //    ****+------------------------------------------* RING_PARTIAL2
        //                                                   |
        //                                                  head
        if(atomic64_cmpset(&ring->head, ring->elem_nr, 0)) {
            //   tail
            //    |
            //    +----------------------------------------------* RING_PARTIAL2
            //                                                   |
            //                                                  head
            //   tail
            //    |
            //    +----------------------------------------------- RING_EMPTY
            //    |
            //   head
            if(atomic64_read(&ring->head) == atomic64_read(&ring->tail)) {
                debug_log_dequeue("partial1 to empty.\n");
                atomic64_set(&ring->stat, RING_EMPTY);
            
            //       tail
            //        |
            //    ****+------------------------------------------* RING_PARTIAL2
            //                                                   |
            //                                                  head
            //       tail
            //        |
            //    ****+------------------------------------------- RING_PARTIAL1
            //    |
            //   head
            } else {
                debug_log_dequeue("partial2 to partial1.\n");
                atomic64_set(&ring->stat, RING_PARTIAL1);
            }
        } else {
            atomic64_cmpset(&ring->stat, RING_DEQUEUING, RING_PARTIAL2);
        }    
        return RING_IS_OK;
    }
    
    return RING_IS_ERR;
}

always_inline int ring_force_dequeue(struct ring_struct *ring, void **data)
{
    int ret = RING_IS_ERR;
    while(RING_IS_OK != (ret=ring_try_dequeue(ring, data)));
    return ret;
}

