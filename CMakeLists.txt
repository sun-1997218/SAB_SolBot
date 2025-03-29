cmake_minimum_required(VERSION 3.12)
project(solana_monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -march=native")  # 启用优化

include_directories(${CMAKE_SOURCE_DIR}/include)

set(BOOST_ROOT "/opt/homebrew/opt/boost")
find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(solana_monitor
    src/main.cpp
    src/dex_websocket.cpp
    src/monitor.cpp  # 确保包含此文件
)

target_link_libraries(solana_monitor
    PRIVATE 
    Boost::boost 
    Boost::system
    pthread  # 多线程支持
)