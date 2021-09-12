[![CMake](https://github.com/Glebanister/deadlock-proof-mutex/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/Glebanister/deadlock-proof-mutex/actions/workflows/cmake.yml)

# Deadlock proof mutex

Test assignment for JetBrains research.

## About

The [`dpm::Mutex`](src/Mutex.hpp) class satisfies all requirements of
[Mutex](https://en.cppreference.com/w/cpp/named_req/Mutex).
Thus, you may use it with `std::unique_lock`
and `std::scoped_lock`.

If a [deadlock](https://en.wikipedia.org/wiki/Deadlock) occurs during code execution, a `dpm::DeadlockException`
is thrown if flag `THROW_ON_DEADLOCK` is set to `true` and `std::abort`
is called otherwise. Its value can be changed with static `Mutex` function call
`dpm::Mutex::setThrowOnDeadlock(<new flag value>)`.

## Installation

First, make sure
`git` is installed,
`cmake` has at least 15 version and
`c++` compiler supports `C++17` standard.

Then type following commands in your terminal

```bash
git clone --recursive https://github.com/Glebanister/deadlock-proof-mutex
cd deadlock-proof-mutex
mkdir build && cd build
cmake ..
```

## Testing

On the main branch you can see the status
of the tests in the header of this document.
If you want to run the tests locally, run the following code

```bash
./test_deadlock_proof_mutex
```

## Documentation

Source code contains doxygen-friendly comments,
so beautiful documentation can be generated
by typing

```bash
doxygen -g Doxygen
doxygen Doxygen
```
