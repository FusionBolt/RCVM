#pragma once
#include <exception>
#include <string>

namespace RCVM
{
class MethodNotFoundException : public std::exception
{
public:
    explicit MethodNotFoundException(const std::string& f)
        : message(std::string("Target Method: ") + f + std::string(" not found")) {}
    ~MethodNotFoundException() noexcept override = default;

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};
}