//
// Created by httime on 4/1/20.

/*
 *  const int *p        //p本身不是const的，而p指向的变量是const的
    int const *p        //p本身不是const的，而p指向的变量是const的
    int * const p;      //p 本身是 const 的，而 p 指向的变量不是 const 的
    const int * const p;//p 本身是 const 的，而 p 指向的变量也是 const 的*/
//

#ifndef JUDGE_CLIENT_JUDGE_H
#define JUDGE_CLIENT_JUDGE_H

#define OJ_WT0 0
#define OJ_WT1 1
#define OJ_CI 2
#define OJ_RI 3
#define OJ_AC 4
#define OJ_PE 5
#define OJ_WA 6
#define OJ_TL 7
#define OJ_ML 8
#define OJ_OL 9
#define OJ_RE 10
#define OJ_CE 11
#define OJ_CO 12

#define BUFF_SIZE 1024

#define LG_C 1
#define LG_JAVA 2
#define LG_BASH 3
#define LG_PHP 4
#define LG_GO 5

typedef unsigned language;

typedef void* (*handfun)(void);
typedef const struct {
    handfun handler;
    void* ages;
} handler;

void handlers(char *buf);

typedef struct {
    char rid[32];
    char uid[32];
    char data[1024];
    language  language;
    char in[64];
    char out[64];
    char md5[128];
    long time_limit;
    long memory_limit;
    char work_dir[BUFF_SIZE];
} problem;

typedef struct {
    char work_dir[BUFF_SIZE];
    char oj_home[BUFF_SIZE];
    int  max_process;

} judge_conf;

typedef struct {

} e_result;
#endif //JUDGE_CLIENT_JUDGE_H
