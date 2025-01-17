#include "list.h"

struct list_test {
	struct list_head list;
	int a;
#define LIST_TEST_INITIALIZER(v)   {{NULL, NULL}, v}
};

struct hlist_test {
	struct hlist_head list;
	int a;
#define HLIST_TEST_INITIALIZER(v)   {{NULL}, v}
};



int demo_list_1_add_del_swap_for_each()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

    list_del_init(&lt3.list);
	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_replace_init(&lt4.list, &lt3.list);
	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_swap(&lt1.list, &lt5.list);
	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    return 0;
}



int demo_list_2_move_and_move_tail()
{
    struct list_head lhead1, lhead2;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead1);
    INIT_LIST_HEAD(&lhead2);
    
    list_add_tail(&lt1.list, &lhead1);
    list_add_tail(&lt2.list, &lhead1);
    list_add_tail(&lt3.list, &lhead1);
    list_add_tail(&lt4.list, &lhead2);
    list_add_tail(&lt5.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_move(&lt1.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_move_tail(&lt2.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    return 0;
}


int demo_list_3_bulk_move()
{
    struct list_head lhead1, lhead2;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead1);
    INIT_LIST_HEAD(&lhead2);
    
    list_add_tail(&lt1.list, &lhead1);
    list_add_tail(&lt2.list, &lhead1);
    list_add_tail(&lt3.list, &lhead1);
    list_add_tail(&lt4.list, &lhead2);
    list_add_tail(&lt5.list, &lhead2);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_bulk_move_tail(&lhead2, &lt1.list, &lt2.list);

    list_for_each_entry(iter, &lhead1, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");


    return 0;
}


int demo_list_4_is_first_is_last_is_empty_is_singular()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);

    if(list_empty(&lhead))
    {
        printf("is empty.\n");
    }
    if(list_empty_careful(&lhead))
    {
        printf("is list_empty_careful.\n");
    }
    
    list_add_tail(&lt1.list, &lhead);

    
    if(list_is_singular(&lhead))
    {
        printf("is list_is_singular.\n");
    }
    
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

    
    if(list_empty(&lhead))
    {
        printf("is empty.\n");
    }

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    if(list_is_first(&lt1.list, &lhead))
    {
        printf("is first.\n");
    }
    if(list_is_last(&lt1.list, &lhead))
    {
        printf("is last.\n");
    }
    if(list_is_first(&lt5.list, &lhead))
    {
        printf("is first.\n");
    }
    if(list_is_last(&lt5.list, &lhead))
    {
        printf("is last.\n");
    }

    return 0;
}



int demo_list_5_rotate()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_rotate_left(&lhead);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_rotate_to_front(&lt4.list, &lhead);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
//    list_rotate_left(&lhead);
//
//
//    list_for_each_entry(iter, &lhead, list) {
//		printf("%d - ", iter->a);
//	}printf("\n");
    
    return 0;
}



int demo_list_6_cut()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    struct list_head lhead2;
    
    list_cut_position(&lhead2, &lhead, &lt3.list);
//    1 - 2 - 3 - 4 - 5 - 
//    4 - 5 - 
//    1 - 2 - 3 - 

//    list_cut_before(&lhead2, &lhead, &lt3.list);
//    1 - 2 - 3 - 4 - 5 - 
//    3 - 4 - 5 - 
//    1 - 2 - 


    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    
    return 0;
}


int demo_list_7_splice()//粘贴
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);

    struct list_head lhead2;
    INIT_LIST_HEAD(&lhead2);
    
    list_add_tail(&lt4.list, &lhead2);
    list_add_tail(&lt5.list, &lhead2);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");


#if 1
    list_splice(&lhead2, &lhead);

//    list_splice_tail(&lhead2, &lhead);
    
    list_for_each_entry(iter, &lhead, list) {
        printf("%d - ", iter->a);
    }printf("\n");
#else

    list_splice_init(&lhead2, &lhead);

//    list_splice_tail_init(&lhead2, &lhead);
    
    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry(iter, &lhead2, list) {
		printf("%d - ", iter->a);
	}printf("\n");

#endif

    return 0;
}



int demo_list_8_for_each_and_entry()
{
    struct list_head lhead;
    
    struct list_test lt1 = LIST_TEST_INITIALIZER(1);
    struct list_test lt2 = LIST_TEST_INITIALIZER(2);
    struct list_test lt3 = LIST_TEST_INITIALIZER(3);
    struct list_test lt4 = LIST_TEST_INITIALIZER(4);
    struct list_test lt5 = LIST_TEST_INITIALIZER(5);
    
    struct list_test *iter;
    
    INIT_LIST_HEAD(&lhead);
    
    list_add_tail(&lt1.list, &lhead);
    list_add_tail(&lt2.list, &lhead);
    list_add_tail(&lt3.list, &lhead);
    list_add_tail(&lt4.list, &lhead);
    list_add_tail(&lt5.list, &lhead);

	list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = list_first_entry(&lhead, struct list_test, list);
    printf("list_first_entry:   %d - \n", iter->a);    
    
    iter = list_last_entry(&lhead, struct list_test, list);
    printf("list_last_entry:    %d - \n", iter->a);

    
    iter = list_first_entry_or_null(&lhead, struct list_test, list);
    printf("list_first_entry_or_null:   %d - \n", iter->a);

    iter = list_next_entry(iter, list);
    printf("list_next_entry:   %d - \n", iter->a);

    
    iter = list_prev_entry(iter, list);
    printf("list_prev_entry:   %d - \n", iter->a);

    struct list_head *lh_iter;
    
    list_for_each(lh_iter, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");
    
    list_for_each_prev(lh_iter, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");

    struct list_head *lh_n;
    
    list_for_each_safe(lh_iter, lh_n, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");
    
    list_for_each_prev_safe(lh_iter, lh_n, &lhead){
		printf("%p - ", lh_iter);
	}printf("\n");


    list_for_each_entry(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_for_each_entry_reverse(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    list_prepare_entry(iter, &lhead, list); //TODO
	printf("list_prepare_entry:     %d\n", iter->a);
    
    list_for_each_entry_continue(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry_continue_reverse(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    list_for_each_entry_from(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    list_for_each_entry_from_reverse(iter, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    
    struct list_test *iter_next;
    list_for_each_entry_safe(iter,iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    list_for_each_entry_safe(iter,iter_next, &lhead, list) {
		printf("%d - ", iter->a);
        list_safe_reset_next(iter, iter_next, list);
	}printf("\n");

    iter = &lt3;
    list_for_each_entry_safe_continue(iter, iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    iter = &lt3;
    list_for_each_entry_safe_from(iter, iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");
    
    list_for_each_entry_safe_reverse(iter, iter_next, &lhead, list) {
		printf("%d - ", iter->a);
	}printf("\n");

    

    
    return 0;
}


#include <malloc.h>

#define log(fmt, arg...) printf(""fmt,##arg)
#define MAX_ADDR 256

//结点结构体
struct hdata_node {
    unsigned int data;
    struct hlist_node list;
};

int demo_hlist_1_test()
{ 
 
    struct hlist_head htable[MAX_ADDR]; //hash数组
    struct hdata_node *hnode = NULL;
    struct hlist_node *hlist = NULL, *n = NULL;
    int i = 0, quit = 1, opt = 0, key;
    unsigned int data;
 
    /* hlist_head 动态初始化 */
    for(i = 0; i < MAX_ADDR; i++) //hash数组动态初始化
        INIT_HLIST_HEAD(&htable[i]);
        
    log("*********************\n\n"
        "input options:\n"
        "1: insert\n"           //插入
        "2: serach\n"           //查询
        "3: delete\n"           //删除
        "0: quit\n"
        "\n*********************\n");  
        
    while(quit != 0){
        log("\ninput options:");
        scanf("%d",  &opt);
        switch(opt){
            //插入
            case  1:
                //分配结点
                hnode = calloc(1, sizeof(struct hdata_node));
                if(!hnode){
                    log("insert fail \n");
                    break;
                }
                //hlist_node 结点初始化
                INIT_HLIST_NODE(&hnode->list);
                
                log("\ninput data:");
                scanf("%d",  &hnode->data);
                key = hnode->data % MAX_ADDR;
                //添加到链表首部
                hlist_add_head(&hnode->list, &htable[key]);
                break;
 
            //查询
            case  2:
                log("\ninput data:");
                scanf("%d",  &data);         
                key = data % MAX_ADDR;
                if(hlist_empty(&htable[key]))
                    log("data not exist \n");
                else{
                    //遍历对应的槽位，匹配值就打印
                    hlist_for_each_entry(hnode, &htable[key], list){
                        if(hnode->data == data)
                            log("find data : %u \n", hnode->data);
                    }
                }                
                break;
 
            //删除
            case  3:
                log("\ninput data:");
                scanf("%d",  &data);         
                key = data % MAX_ADDR;
                if(hlist_empty(&htable[key]))
                    log("data not exist ，delete fail \n");
                else{
                    //遍历对应的槽，匹配值就删除
                    hlist_for_each_entry_safe(hnode, n, &htable[key], list){
                        if(hnode->data == data){
                            hlist_del(&hnode->list);
                            break;
                        }
                    }
                }                            
                log("delete fail\n");                
                break;
            case 0:
                quit = 0;
                break;
            default:
                log("unrecognized option!");
                break;                
        }
    }
    //退出程序前释放资源
    for(i=0; i < MAX_ADDR; i++){
        //遍历每一个槽，有结点就删除
        hlist_for_each_entry_safe(hnode, n, &htable[i], list){           
            hlist_del(&hnode->list); 
            log("delete %u \n", hnode->data);
            free(hnode);
            hnode = NULL;
        }
    }
    log("exit\n");    
}

int main()
{
    demo_list_1_add_del_swap_for_each();
    demo_list_2_move_and_move_tail();
    demo_list_3_bulk_move();
    demo_list_4_is_first_is_last_is_empty_is_singular();
    demo_list_5_rotate();
    demo_list_6_cut();
    demo_list_7_splice();
    demo_list_8_for_each_and_entry();
    demo_hlist_1_test();


    return 0;
}

