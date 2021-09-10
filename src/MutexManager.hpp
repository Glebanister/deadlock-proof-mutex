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
        /**
         * @return Reference to global MutexManager.
         */
        static MutexManager& instance();

        /**
         * @brief Initialized a deadlock-proof deadlock proof mutex.
         * @return Identifier of the new mutex.
         */
        MutexId createMutex();

        /**
         * @brief Destroys deadlock-proof mutex.
         * @param id Identifier of mutex which is needed to be destroyed.
         * If no such mutex exists @p std::invalid_argument is thrown.
         */
        void destroyMutex(MutexId id);

        /**
         * @brief Locks deadlock-proof mutex.
         * @param id Identifier of mutex which is needed to be locked.
         * If no such mutex exists @p std::invalid_argument is thrown.
         */
        void lockMutex(MutexId id);

        /**
         * @brief Unlocks deadlock-proof mutex.
         * @param id Identifier of mutex which is needed to be unlocked.
         * If no such mutex exists @p std::invalid_argument is thrown.
         */
        void unlockMutex(MutexId id);

        /**
         * @return Total number of created mutexes (excluding destroyed ones).
         */
        std::size_t getMutexDataSize() const;

        /**
         * @brief Set throw on deadlock flag.
         * @param f If true then exceptions @p dpm::DeadlockException will be thrown,
         * @p std::abort will be called otherwise.
         */
        void setThrowOnDeadlock(bool f) noexcept;

    private:
        [[nodiscard]] bool hasMutexUnsafe(MutexId id) const;

        std::unordered_map<MutexId, MutexData> mutexData_{};
        std::unordered_map<ThreadId, ThreadData> threadData_{};
        MutexId maxMutexId_ = 0;
        mutable std::mutex managerMutex_{};
        bool throwOnDeadlock_ = false;
    };

} // namespace dpm::detail


#endif //DEADLOCK_PROOF_MUTEX_MUTEXMANAGER_HPP
