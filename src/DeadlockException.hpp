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

    class DeadlockException : public std::runtime_error {
    public:
        explicit DeadlockException(const std::vector<std::thread::id>& ids);
    };

} // namespace dpm


#endif //DEADLOCK_PROOF_MUTEX_DEADLOCKEXCEPTION_HPP
