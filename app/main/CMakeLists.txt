cmake_minimum_required(VERSION 3.9)
project(main)

set(CMAKE_CXX_STANDARD 11)

add_executable(main
        controlwindow.cpp
        controlwindow.h
        kernel.cpp
        scopewindow.cpp
        scopewindow.h)
