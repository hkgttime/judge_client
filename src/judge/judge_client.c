//
// Created by httime on 4/3/20.
//

#include <stddef.h>
#include <termio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <wait.h>
#include <sys/stat.h>
#include<sys/ptrace.h>
#include <stdlib.h>
#include "../inc/judge.h"

#define STD_MB 1048576

static char lang_ext[13][8] = { "c", "cc", "pas", "java", "rb", "sh", "py",
                                "php", "pl", "cs", "m", "bas", "scm" };

void setProcessLimit(long time_limit, long memory_limit, int lang) {

    //在Linux系统中，Resouce limit指在一个进程的执行过程中，它所能得到的资源的限制，
    //比如进程的core file的最大值，虚拟内存的最大值等 ，这是运行时间，内存大小实现的关键
    /*
    结构体中 rlim_cur是要取得或设置的资源软限制的值，rlim_max是硬限制
    这两个值的设置有一个小的约束：
    1） 任何进程可以将软限制改为小于或等于硬限制
    2）任何进程都可以将硬限制降低，但普通用户降低了就无法提高，该值必须等于或大于软限制
    3） 只有超级用户可以提高硬限制

    setrlimit(int resource,const struct rlimit rlptr);返回：若成功为0，出错为非0
    RLIMIT_CPU：CPU时间的最大量值（秒），当超过此软限制时向该进程发送SIGXCPU信号
    RLIMIT_FSIZE:可以创建的文件的最大字节长度，当超过此软限制时向进程发送SIGXFSZ
    */
    struct rlimit LIM;
    LIM.rlim_max = time_limit;
    LIM.rlim_cur = time_limit;
    setrlimit(RLIMIT_CPU, &LIM);//cpu运行时间限制

    LIM.rlim_max = 80 * STD_MB;
    LIM.rlim_cur = 80 * STD_MB;
    setrlimit(RLIMIT_FSIZE, &LIM);//可文件大小限制，防止恶意程序的吗？

    LIM.rlim_max = memory_limit;
    LIM.rlim_cur = memory_limit;
    setrlimit(RLIMIT_AS, &LIM);//最大运行的虚拟内存大小限制
    // set the stack
    LIM.rlim_cur = STD_MB << 6;
    LIM.rlim_max = STD_MB << 6;
    setrlimit(RLIMIT_STACK, &LIM);

    // proc limit
    switch (lang) {
        case 3:  //java
        case 12:
            LIM.rlim_cur = LIM.rlim_max = 50;
            break;
        case 5: //bash
            LIM.rlim_cur = LIM.rlim_max = 3;
            break;
        case 9: //C#
            LIM.rlim_cur = LIM.rlim_max = 50;
            break;
        default:
            LIM.rlim_cur = LIM.rlim_max = 1;
    }
    setrlimit(RLIMIT_NPROC, &LIM);//每个实际用户ID所拥有的最大子进程数，这些都是为了防止恶意程序的吧？？
}

int get_file(const char *path){
    struct stat fd_stat;
    if (stat(path, &fd_stat) == -1){
        return 0;
    }
    return fd_stat.st_size;
}

void init_problem(const char *buf, problem *problem){


    char src_pth[BUFF_SIZE], in_pth[BUFF_SIZE];
    sprintf(src_pth, "%s/run/Main.%s", "/home/oj_home", lang_ext[problem->language]);
    FILE *fd_src = fopen(src_pth, "w");
    fprintf(fd_src, "%s", problem->data);
    fclose(fd_src);
}


int compile(const problem *problem){
    pid_t pid;
    language language = problem->language;
    //设置编译命令
    const char * CP_C[] = { "gcc", "Main.c", "-o", "Main", "-fno-asm", "-Wall",
                            "-lm", "--static", "-std=c99", "-DONLINE_JUDGE", NULL };
    const char * CP_J[] = { "javac", "-J-Xms32m", "-J-Xmx256m","-encoding","UTF-8", "Main.java",NULL };
    const char * CP_B[] = { "chmod", "+rx", "Main.sh", NULL };
    const char * CP_GO[] = { "chmod", "+rx", "Main.sh", NULL };
    const char * CP_PH[] = { "php", "-l", "Main.php", NULL };

    if ((pid = fork()) == 0){
        freopen("ce.txt", "w", stderr);
        switch (language){
            case LG_C:
                execvp(CP_C[0], (char * const *) CP_C);
                break;
            case LG_PHP:
                execvp(CP_PH[0], (char * const *) CP_PH);
                break;
            case LG_JAVA:
                execvp(CP_J[0],(char * const *) CP_J);
                break;
            case LG_GO:
                execvp(CP_GO[0],(char * const *) CP_GO);
                break;
            case LG_BASH:
                execvp(CP_B[0],(char * const *) CP_B);
                break;
            default:
                printf("nothing to do!\n");
        }
    } else if (pid > 0){
        int status = 0;
        waitpid(pid, &status, 0);
        status = get_file("ce.txt");
        return status;
    };

}

void watch_app(pid_t pid, const problem *problem){

};

void run_code(problem *p){
    /*
	在当前程序运行优先级基础之上调整指定值得到新的程序运行优先级，
	用新的程序运行优先级运行命令行"command [arguments...]"。优先级的范围为-20 ～ 19 等40个等级，
	其中数值越小优先级越高，数值越大优先级越低，既-20的优先级最高， 19的优先级最低。
	若调整后的程序运行优先级高于-20，则就以优先级-20来运行命令行；若调整后的程序运行优先级低于19，
	则就以优先级19来运行命令行。若 nice命令未指定优先级的调整值，则以缺省值10来调整程序运行优先级，
	既在当前程序运行优先级基础之上增加10。

	调低运行级，基本用户都能执行成功，将这个执行子进程的优先级降到最低，为了安全？？？？？？？？？
	*/
    nice(19);
    //使输入输出重定向到文件里，方便最后的评判
    freopen("data.in", "r", stdin);
    freopen("user.out", "w", stdout);
    freopen("error.out", "a+", stderr);

    /*
	ptrace系统函数。 ptrace提供了一种使父进程得以监视和控制其它进程的方式，
	它还能够改变子进程中的寄存器和内核映像，因而可以实现断点调试和系统调用的跟踪。
	使用ptrace，你可以在用户层拦截和修改系统调用(sys call).
    形式：ptrace(PTRACE_TRACEME,0 ,0 ,0)
    描述：本进程被其父进程所跟踪。其父进程应该希望跟踪子进程。
	*/
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    chroot(p->work_dir);
    setProcessLimit(p->time_limit, p->memory_limit, p->language);
    switch (p->language) {
        case 0:
        case 1:
        case 2:
        case 10:
        case 11:
            execl("./Main", "./Main", (char *) NULL);//因为已经chroot(/home/judge/run0所以可以./Main)
            break;
        case 3:
            execl("/usr/bin/java", "/usr/bin/java",
                  "-Djava.security.manager",
                  "-Djava.security.policy=./java.policy", "Main", (char *) NULL);
            break;
        case 4:
            //system("/ruby Main.rb<data.in");
            execl("/ruby", "/ruby", "Main.rb", (char *) NULL);
            break;
        case 5: //bash
            execl("/bin/bash", "/bin/bash", "Main.sh", (char *) NULL);
            break;
        case 6: //Python
            execl("/python", "/python", "Main.py", (char *) NULL);
            break;
        case 7: //php
            execl("/php", "/php", "Main.php", (char *) NULL);
            break;
        case 8: //perl
            execl("/perl", "/perl", "Main.pl", (char *) NULL);
            break;
        case 9: //Mono C#
            execl("/mono", "/mono", "--debug", "Main.exe", (char *) NULL);
            break;
        case 12: //guile
            execl("/guile", "/guile", "Main.scm", (char *) NULL);
            break;

    }
    exit(0);
}


