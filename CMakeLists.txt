cmake_minimum_required(VERSION 3.27)
project(3A)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(3A
        bounded_buffer_hw.h
        bounded_buffer_hw_template.cpp
        bounded_buffer_os.h
        bounded_buffer_os_template.cpp
        driver.cpp
        semaphore.cpp
        semaphore.h
        tas.cpp)
