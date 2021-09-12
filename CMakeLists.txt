cmake_minimum_required(VERSION 3.15)
project(deadlock_proof_mutex)

set(CMAKE_CXX_STANDARD 17)

# Deadlock Proof Mutex - Library
set(DPM_LIB ${CMAKE_PROJECT_NAME})
set(
        DPM_SOURCES
        src/Mutex.cpp
        src/MutexManager.cpp
        src/DeadlockException.cpp
)
add_library(${DPM_LIB} ${DPM_SOURCES})
target_include_directories(${DPM_LIB} PUBLIC src)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${DPM_LIB} PRIVATE Threads::Threads)

# Testing
set(DPM_TEST test_${DPM_LIB})
set(
        DPM_TEST_SOURCES
        test/test.cpp
        test/overall.cpp
)
add_executable(${DPM_TEST} ${DPM_TEST_SOURCES})
target_include_directories(${DPM_TEST} PUBLIC src)
target_include_directories(${DPM_TEST} PUBLIC extern/doctest/doctest)
target_link_libraries(${DPM_TEST} ${DPM_LIB})
target_compile_definitions(${DPM_TEST} PUBLIC THROW_ON_DEADLOCK=1)
enable_testing()
add_test(Overall ${DPM_TEST})
