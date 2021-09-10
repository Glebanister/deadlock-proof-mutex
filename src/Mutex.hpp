//
// Created by Gleb Marin on 10.09.2021.
//

#ifndef DEADLOCK_PROOF_MUTEX_MUTEX_HPP
#define DEADLOCK_PROOF_MUTEX_MUTEX_HPP


#include "MutexManager.hpp"


namespace dpm {

    /**
     * @brief A deadlock proof mutex. It could be used as a regular mutex (@p std::mutex).
     * But the executor is not permanently blocked if a deadlock occurs on dpm::Mutex.
     * Instead, @p dpm::DeadlockException is thrown if @p THROW_ON_DEADLOCK
     * is defined, @p std::abort is called otherwise.
     *
     * @p Mutex is just a wrapper of @p MutexManager calls.
     *
     * @p Mutex is movable, but not copyable.
     */
    class Mutex {
    public:
        /**
         * Creates unlocked Mutex.
         */
        Mutex();

        Mutex(Mutex&& other) noexcept;

        Mutex& operator=(Mutex&& other) noexcept;

        Mutex(const Mutex& other) = delete;

        Mutex& operator=(const Mutex& other) = delete;

        /**
         * Locks the mutex.
         * If a cyclic lock (deadlock) occurs in the process,
         * an @p dpm::DeadlockException is thrown if @p THROW_ON_DEADLOCK
         * is defined, `std::abort` is called otherwise.
         */
        void lock() const;

        /**
         * Unlocks the mutex.
         */
        void unlock() const;

        /**
         * @brief Set @p THROW_ON_DEADLOCK flag.
         * @param f New flag value
         */
        static void setThrowOnDeadlock(bool f) noexcept;

        ~Mutex();

    private:
        detail::MutexManager::MutexId mutexId_;
    };

} // namespace dpm


#endif //DEADLOCK_PROOF_MUTEX_MUTEX_HPP
