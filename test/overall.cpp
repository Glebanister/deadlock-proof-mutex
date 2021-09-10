//
// Created by Глеб Марьин on 10.09.2021.
//

#include <doctest.h>
#include <chrono>
#include <thread>

#include "Mutex.hpp"
#include "MutexManager.hpp"

void sleepMillis(std::size_t millis) {
    using namespace std::chrono;
    std::this_thread::sleep_for(duration_cast<nanoseconds>(milliseconds(millis)));
}

TEST_SUITE("Deadlock Proof Mutex Overall") {
    TEST_CASE("One thread lock & unlock") {
        CHECK_EQ(dpm::detail::MutexManager::instance().getMutexDataSize(), 0U);
        {
            dpm::Mutex m;
            m.lock();
            m.unlock();
        }
        CHECK_EQ(dpm::detail::MutexManager::instance().getMutexDataSize(), 0U);
    }

    TEST_CASE("Unlock non locked lock") {
        dpm::Mutex m;
        CHECK_THROWS_AS(m.unlock(), std::logic_error);
    }

    TEST_CASE("Unlock other thread's lock") {
        dpm::Mutex m;

        auto locker = [&m]() {
            m.lock();
        };
        auto unLocker = [&m]() {
            sleepMillis(50);
            CHECK_THROWS_AS(m.unlock(), std::logic_error);
        };

        std::thread tLocker(locker);
        std::thread tUnLocker(unLocker);
        tUnLocker.join();
        tLocker.join();
    }

    TEST_CASE("Usable with std::unique_lock") {
        dpm::Mutex m;
        std::unique_lock lock(m);
    }

    TEST_CASE("No deadlock process") {
        dpm::Mutex m;
        std::size_t totalCount = 0;
        auto inc = [&m, &totalCount](std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) {
                std::unique_lock lock(m);
                totalCount += 1;
            }
        };

        std::thread tInc1(inc, 10000), tInc2(inc, 20000);
        tInc2.join();
        tInc1.join();

        CHECK_EQ(30000, totalCount);
    }
}
