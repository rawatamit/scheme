cmake_minimum_required(VERSION 2.8.12)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(interp
            Token.cc Reader.cc ReaderException.cc
            Eval.cc Print.cc)
