cmake_minimum_required(VERSION 3.14)
project(parallel_workloads)

set(CMAKE_CXX_STANDARD 17)

add_executable(parallel_workloads main.cpp arrangement.h chart.h job.h read.h timestamp.h)