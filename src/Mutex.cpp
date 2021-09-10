//
// Created by Gleb Marin on 10.09.2021.
//

#include "Mutex.hpp"


namespace dpm {

    Mutex::Mutex()
        : mutexId_(detail::MutexManager::instance().createMutex()) {
    }

    Mutex::Mutex(Mutex&& other) noexcept
        : mutexId_(other.mutexId_) {
    }

    Mutex& Mutex::operator=(Mutex&& other) noexcept {
        mutexId_ = other.mutexId_;
        return *this;
    }

    void Mutex::unlock() const {
        detail::MutexManager::instance().lockMutex(mutexId_);
    }

    void Mutex::lock() const {
        detail::MutexManager::instance().lockMutex(mutexId_);
    }

    Mutex::~Mutex() {
        detail::MutexManager::instance().destroyMutex(mutexId_);
    }

} // namespace dpm
