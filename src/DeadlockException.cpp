//
// Created by Gleb Marin on 10.09.2021.
//

#include "DeadlockException.hpp"


namespace dpm {
    namespace {
        std::string makeDeadlockExceptionMessage(const std::vector<std::thread::id>& threads) {
            std::stringstream messageStream;
            messageStream << "A deadlock has occurred. It includes " << threads.size() << " thread(s):";
            std::vector<std::string> threadStrings;
            std::transform(
                threads.begin(), threads.end(),
                std::back_inserter(threadStrings),
                [](const std::thread::id& threadId) {
                    std::stringstream s;
                    s << threadId;
                    return "\n\t" + s.str();
                }
            );
            std::copy(
                threadStrings.begin(), threadStrings.end(),
                std::ostream_iterator<std::string>(messageStream)
            );
            return messageStream.str();
        }
    }

    DeadlockException::DeadlockException(const std::vector<std::thread::id>& ids)
        : std::runtime_error(makeDeadlockExceptionMessage(ids)) {}

} // namespace dpm
