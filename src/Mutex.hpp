//
// Created by Gleb Marin on 10.09.2021.
//

#ifndef DEADLOCK_PROOF_MUTEX_MUTEX_HPP
#define DEADLOCK_PROOF_MUTEX_MUTEX_HPP


#include "MutexManager.hpp"


namespace dpm {

    class Mutex {
    public:
        Mutex();

        Mutex(Mutex&& other) noexcept;

        Mutex& operator=(Mutex&& other) noexcept;

        Mutex(const Mutex& other) = delete;

        Mutex& operator=(const Mutex& other) = delete;

        void lock() const;

        void unlock() const;

        ~Mutex();

    private:
        detail::MutexManager::MutexId mutexId_;
    };

} // namespace dpm


#endif //DEADLOCK_PROOF_MUTEX_MUTEX_HPP
