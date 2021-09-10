#include <iostream>
#include <thread>

#include "Mutex.hpp"

void foo() {
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main() {

    std::thread a(foo);
    std::thread b(foo);

    a.join();
    b.join();

    std::cout << "Done :)" << std::endl;

    return 0;
}
