#pragma once

#include <exception>
#include <string>

namespace ShiraNet {

    enum class ErrorCode {
        SocketCreationFailed,
        ConnectionFailed,
        AcceptFailed,
        BindFailed,
        SendFailed,
        ListenFailed,
        ReceiveFailed,
        ConnectionClosedEarly,
        InvalidAddress,
        BufferTooSmall,
        PartialSend,
        Unknown
    };

    class Exception : public std::exception {
        private:
            ErrorCode code;
            std::string message;
            int systemError;  // errno value

        public:
            Exception(ErrorCode code, const std::string& msg, int sysErr = 0);
            
            const char* what() const noexcept override;
            ErrorCode getCode() const noexcept;
            int getSystemError() const noexcept;
            std::string getSystemErrorMessage() const noexcept;
    };

}
