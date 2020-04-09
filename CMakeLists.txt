cmake_minimum_required(VERSION 3.15)
project(Judge_client C)

set(CMAKE_C_STANDARD 99)

add_executable(Judge_client main.c src/net/register.c src/net/channel.c src/net/socket_base.c src/vender/pthreadpool.c src/inc/ojp.h src/judge/handler.c src/inc/judge.h src/judge/judge_client.c)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)