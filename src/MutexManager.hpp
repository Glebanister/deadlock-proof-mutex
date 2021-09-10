//
// Created by Gleb Marin on 08.09.2021.
//

#ifndef DEADLOCK_PROOF_MUTEX_MUTEXMANAGER_HPP
#define DEADLOCK_PROOF_MUTEX_MUTEXMANAGER_HPP


#include <unordered_map>
#include <thread>
#include <set>


namespace dpm::detail {

    class MutexManager {
    public:
        using ThreadId = std::thread::id;
        using MutexId = std::int32_t;

    private:
        struct MutexData {
            std::optional<ThreadId> owner{};
            std::condition_variable condUnlocked;

            [[nodiscard]] bool isUnlocked();
        };

        struct ThreadData {
            std::optional<MutexId> claimingMutex;
        };

    public:
        static MutexManager& instance();

        MutexId createMutex();

        void destroyMutex(MutexId id);

        void lockMutex(MutexId id);

        void unlockMutex(MutexId id);

        bool hasMutex(MutexId id) const;

        std::size_t getMutexDataSize() const;

    private:
        [[nodiscard]] bool hasMutexUnsafe(MutexId id) const;

        std::unordered_map<MutexId, MutexData> mutexData_{};
        std::unordered_map<ThreadId, ThreadData> threadData_{};
        MutexId maxMutexId_{};
        mutable std::mutex managerMutex_{};
    };

} // namespace dpm::detail


#endif //DEADLOCK_PROOF_MUTEX_MUTEXMANAGER_HPP
