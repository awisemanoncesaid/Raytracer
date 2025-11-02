
#include "Program.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <cstdlib>


Program::Params::Params(const CommandLineParser &parser)
{
    if (parser.containsFlag('h')) {
        runMode = RunMode::Help;
    } else {
        runMode = RunMode::Raytracing;
        if (parser.getStandaloneArgs().size() < 1) {
            throw Program::InvalidParamsException("Missing path to the scene file");
        }
        path = parser.getStandaloneArgs()[0];
        if (parser.getStandaloneArgs().size() > 1) {
            output = parser.getStandaloneArgs()[1];
        } else {
            output = "../renderings/output.png";
        }
        if (parser.containsFlag('r')) {
            std::string resolution = parser.getFlagArgs('r')[0];
            if (resolution.find('x') == std::string::npos) {
                throw Program::InvalidParamsException("Invalid resolution format");
            }
            try {
                resolutionX = std::clamp(std::stoi(resolution.substr(0, resolution.find('x'))), 1, 1920);
                resolutionY = std::clamp(std::stoi(resolution.substr(resolution.find('x') + 1)), 1, 1080);
            } catch (const std::exception &e) {
                throw Program::InvalidParamsException("Invalid resolution format");
            }
        }
        if (parser.containsFlag('t')) {
            try {
                nbThreads = std::clamp(std::stoi(parser.getFlagArgs('t')[0]), 1, 64);
            } catch (const std::exception &e) {
                throw Program::InvalidParamsException("Invalid number of threads");
            }
        }
        if (parser.containsFlag('a')) {
            try {
                antiAliasingSize = std::clamp(std::stoi(parser.getFlagArgs('a')[0]), 1, 5);
            } catch (const std::exception &e) {
                throw Program::InvalidParamsException("Invalid anti-aliasing factor");
            }
        }
    }
}
