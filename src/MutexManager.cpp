//
// Created by Gleb Marin on 08.09.2021.
//

#include <mutex>
#include <iostream>

#include "MutexManager.hpp"
#include "DeadlockException.hpp"


namespace dpm::detail {

    MutexManager::MutexId MutexManager::createMutex() {
        std::scoped_lock lock(managerMutex_);
        using HtType = decltype(mutexData_);
        mutexData_[maxMutexId_].owner = std::nullopt;
        return maxMutexId_++;
    }

    bool MutexManager::hasMutexUnsafe(MutexManager::MutexId id) const {
        return mutexData_.find(id) != mutexData_.end();
    }

    void MutexManager::destroyMutex(MutexManager::MutexId id) {
        std::scoped_lock lock(managerMutex_);
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
        std::thread::id thisThreadId = std::this_thread::get_id();
        MutexData& thisMutexData = mutexData_[mutexId];
        if (thisMutexData.isUnlocked()) {
            thisMutexData.owner = thisThreadId;
            return;
        }
        ThreadData& thisThreadData = threadData_[thisThreadId];
        thisThreadData.claimingMutex = mutexId;

        {
            // Check for deadlock
            MutexId curMutexId = mutexId;
            std::vector<ThreadId> deadlockingThreads = {thisThreadId};
            while (mutexData_[curMutexId].owner.has_value()) {
                ThreadId curMutexOwnerId = mutexData_[curMutexId].owner.value();
                if (curMutexOwnerId == thisThreadId) {
                    if (throwOnDeadlock_) {
                        throw DeadlockException(deadlockingThreads);
                    }
                    std::cerr << DeadlockException(deadlockingThreads).what() << std::endl;
                    std::abort();
                }
                deadlockingThreads.push_back(curMutexOwnerId);
                if (!threadData_[curMutexOwnerId].claimingMutex.has_value()) {
                    break;
                }
                curMutexId = threadData_[curMutexOwnerId].claimingMutex.value();
            }
        }

        thisMutexData.condUnlocked.wait(lock, [&thisMutexData]() { return thisMutexData.isUnlocked(); });
        thisThreadData.claimingMutex = std::nullopt;
        thisMutexData.owner = thisThreadId;
    }

    void MutexManager::unlockMutex(MutexManager::MutexId mutexId) {
        std::scoped_lock lock(managerMutex_);
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

    std::size_t MutexManager::getMutexDataSize() const {
        return mutexData_.size();
    }

    void MutexManager::setThrowOnDeadlock(bool f) noexcept {
        throwOnDeadlock_ = f;
    }

    bool MutexManager::MutexData::isUnlocked() {
        return !owner.has_value();
    }

} // namespace dpm::detail
