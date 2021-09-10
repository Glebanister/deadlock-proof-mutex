//
// Created by Gleb Marin on 08.09.2021.
//

#include "MutexManager.hpp"


namespace dpm::detail {

    MutexManager::MutexId MutexManager::createMutex() {
        std::unique_lock lock(managerMutex_);
        using HtType = decltype(mutexData_);
        mutexData_[maxMutexId_].owner = std::nullopt;
        return maxMutexId_++;
    }

    bool MutexManager::hasMutexUnsafe(MutexManager::MutexId id) const {
        return mutexData_.find(id) != mutexData_.end();
    }

    bool MutexManager::hasMutex(MutexId id) const {
        std::unique_lock guard(managerMutex_);
        return hasMutexUnsafe(id);
    }

    void MutexManager::destroyMutex(MutexManager::MutexId id) {
        std::unique_lock lock(managerMutex_);
        if (!hasMutexUnsafe(id)) {
            throw std::invalid_argument("Can not destroy mutex data: it was not created");
        }
        mutexData_.erase(id);
    }

    void MutexManager::lockMutex(MutexManager::MutexId mutexId) {
        std::unique_lock lock(managerMutex_);
        if (!hasMutexUnsafe(mutexId)) {
            throw std::invalid_argument("Can not lock not created mutex");
        }
        MutexData& thisMutexData = mutexData_[mutexId];
        if (thisMutexData.isUnlocked()) {
            thisMutexData.owner = std::this_thread::get_id();
            return;
        }
        ThreadData& thisThreadData = threadData_[std::this_thread::get_id()];
        thisThreadData.claimingMutex = mutexId;
        while (thisMutexData.isUnlocked()) {
            thisMutexData.condUnlocked.wait(lock);
        }
        thisThreadData.claimingMutex = std::nullopt;
        thisMutexData.owner = std::this_thread::get_id();
    }

    void MutexManager::unlockMutex(MutexManager::MutexId mutexId) {
        std::unique_lock lock(managerMutex_);
        if (!hasMutexUnsafe(mutexId)) {
            throw std::invalid_argument("Can not unlock not created mutex");
        }
        MutexData& thisMutexData = mutexData_[mutexId];
        if (thisMutexData.isUnlocked()) {
            throw std::logic_error("Mutex must be locked first");
        }
        if (thisMutexData.owner.value() != std::this_thread::get_id()) {
            throw std::logic_error("Mutex can be unlocked only by owner");
        }
        thisMutexData.owner = std::nullopt;
        thisMutexData.condUnlocked.notify_one();
    }

    MutexManager& MutexManager::instance() {
        static MutexManager globalMutexManager;
        return globalMutexManager;
    }

    bool MutexManager::MutexData::isUnlocked() {
        return !owner.has_value();
    }

} // namespace dpm::detail
