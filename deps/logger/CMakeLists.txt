cmake_minimum_required(VERSION 2.8)
project(logger)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_library(logger STATIC ./logger.c)
