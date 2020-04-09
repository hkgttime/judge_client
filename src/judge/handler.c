//
// Created by httime on 4/1/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../inc/judge.h"

extern judge_conf;

init_problem(const char *buf, problem *problem);
int compile(const problem *problem);
void run_code(problem *p);
void watch_app(pid_t pid, const problem *problem);

void handlers(char *buf){
    problem *p = (problem*) malloc(sizeof(problem));
    init_problem(buf, p);
    chdir(p->work_dir);
    int Compile_OK = compile(p);
    if (Compile_OK != 0){ //编译失败则推出判题进程不需后续执行了
        perror("compile err");
    };
    pid_t app = fork();
    if (app == 0){
        run_code(p);
    } else if (app > 0){
        watch_app(app,p);
    } else{
        perror("handlers fork err");
    }
}



