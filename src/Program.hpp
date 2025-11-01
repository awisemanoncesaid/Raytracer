#pragma once

#include "CommandLineParser.hpp"

#include <vector>
#include <string>
#include <map>


namespace Program {

    class InvalidParamsException : public std::exception {
    public:
        InvalidParamsException(const std::string &message) : _message(message) {}
        ~InvalidParamsException() = default;
        const char *what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
    };

    enum class RunMode {
        Help = 0,
        Raytracing
    };

    struct Params {
        Params(const CommandLineParser &parser);
        ~Params() = default;

        RunMode runMode = RunMode::Raytracing;

        std::string path;
        std::string output;

        uint16_t resolutionX = 1920;
        uint16_t resolutionY = 1080;

        uint8_t nbThreads = 16;

        uint8_t antiAliasingSize = 3;
    };

    enum ExitCode {
        Success = 0,
        InvalidParams = 1,
        RuntimeError = 2,
        UnknownError = 3,
    };

    // int main(int argc, const char *argv[], const char *envp[]);

}
