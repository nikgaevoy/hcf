cmake_minimum_required(VERSION 3.14)
project(hcf)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(LOCAL)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-sanitize-recover")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(hcf main.cpp)

#find_package(Threads REQUIRED)
#target_link_libraries(olymp PRIVATE Threads::Threads)
