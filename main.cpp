#include <iostream>
#include <thread>

#include "Mutex.hpp"

int main() {

    std::thread a(foo);
    std::thread b(foo);

    a.join();
    b.join();

    std::cout << "Done :)" << std::endl;

    return 0;
}
