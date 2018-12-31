cmake_minimum_required(VERSION 2.8)
project(restclient-cpp)

include_directories(include/)
add_library(restclient-cpp STATIC
  source/connection.cc
  source/restclient.cc
  source/helpers.cc
)
