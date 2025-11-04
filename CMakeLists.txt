cmake_minimum_required(VERSION 3.10)
project(InMemoryDB)

set(CMAKE_CXX_STANDARD 17)

add_executable(InMemoryDB
    main.cpp
    store.cpp
)