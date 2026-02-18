#include "error.hpp"

#include <cstring>
#include <sstream>

namespace ShiraNet {

    Exception::Exception(ErrorCode code, const std::string& msg, int sysErr)
      : code(code)
      , message(msg)
      , systemError(sysErr) {
    }

    const char* Exception::what() const noexcept {
        return message.c_str();
    }

    ErrorCode Exception::getCode() const noexcept {
        return code;
    }

    int Exception::getSystemError() const noexcept {
        return systemError;
    }

    std::string Exception::getSystemErrorMessage() const noexcept {
        if (systemError != 0) {
            return std::string(strerror(systemError));
        }
        return "";
    }

}
