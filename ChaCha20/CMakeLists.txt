cmake_minimum_required (VERSION 2.6)

project (ChaCha20)

include_directories(inc)

set(SOURCES src/main.cpp src/chacha20.cpp  src/chacha20block.cpp src/consumer.cpp src/processing.cpp src/producer.cpp src/pthrd.cpp)

add_executable(ChaCha20 ${SOURCES})
target_link_libraries(ChaCha20 pthread rt)