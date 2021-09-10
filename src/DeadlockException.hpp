//
// Created by Gleb Marin on 10.09.2021.
//

#ifndef DEADLOCK_PROOF_MUTEX_DEADLOCKEXCEPTION_HPP
#define DEADLOCK_PROOF_MUTEX_DEADLOCKEXCEPTION_HPP


#include <stdexcept>
#include <thread>
#include <vector>
#include <sstream>


namespace dpm {

    /**
     * @brief An exception, which is thrown once deadlock on @p dpm::Mutex is occurred.
     */
    class DeadlockException : public std::runtime_error {
    public:
        /**
         * @param ids The Ids of threads, which are stuck in deadlock.
         */
        explicit DeadlockException(const std::vector<std::thread::id>& ids);
    };

} // namespace dpm


#endif //DEADLOCK_PROOF_MUTEX_DEADLOCKEXCEPTION_HPP
