cmake_minimum_required(VERSION 2.8)
project(caps)

include_directories(./)
add_library(caps STATIC caps.cc reader.cc writer.cc)
