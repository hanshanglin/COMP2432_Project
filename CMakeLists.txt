cmake_minimum_required(VERSION 3.13)
project(COMP2432_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(COMP2432_Project input.c mytime.c mytime.h record.c record.h log.c FCFS.h FCFS.c DDL.c DDL.h Priority.c  Priority.h list.c list.h)