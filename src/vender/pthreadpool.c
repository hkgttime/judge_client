//
// Created by httime on 3/29/20.
//

#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    void *(*handler)(void*);
    void *ages;
} threadpool_task_t;

typedef struct threadpool_t{
    pthread_mutex_t lock;               /*用于锁住当前这个结构体体taskpoll*/
    pthread_mutex_t thread_counter;     /*记录忙状态线程个数*/
    pthread_cond_t queue_not_full;      /*当任务队列满时，添加任 务的线程阻塞，等待此条件变量*/
    pthread_cond_t queue_not_empty;     /*任务队列里不为空时，通知等待任务的线程*/
    pthread_t *threads;                 /*保存工作线程tid的数组*/
    pthread_t adjust_tid;               /*管理线程tid*/
    threadpool_task_t *task_queue;      /*任务队列*/
    int min_thr_num;                    /*线程组内默认最小线程数*/
    int max_thr_num;                    /*线程组内默认最大线程数*/
    int live_thr_num;                   /*当前存活线程个数*/
    int busy_thr_num;                   /*忙状态线程个数*/
    int wait_exit_thr_num;              /*要销毁的线程个数*/
    int queue_front;                    /*队头索引下标*/
    int queue_rear;                     /*队未索引下标*/
    int queue_size;                     /*队中元素个数*/
    int queue_max_size;                 /*队列中最大容纳个数*/
    int shutdown;                       /*线程池使用状态，true或false*/
} threadpool_t;


threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size){
    threadpool_t *pool = NULL;
    do {
     pool = (threadpool_t*) malloc(sizeof(threadpool_t));
    if (pool == NULL){
        perror("create threadpool_t err");
        break;
    }
    pool->min_thr_num = min_thr_num;
    pool->max_thr_num = max_thr_num;
    pool->live_thr_num = min_thr_num;
    pool->busy_thr_num = 0;
    pool->queue_front = 0;
    pool->queue_rear = 0;
    pool->queue_size = 0;
    pool->queue_max_size = queue_max_size;
    pool->shutdown = 0;


    }while (0);

    free(pool);
    return NULL;
}




