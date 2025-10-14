#include "Program.hpp"
#include "lib/Scene3d/Scene/Scene.hpp"
#include "lib/FrameBuffer/FrameBuffer.hpp"
#include "lib/Renderer/Renderer.hpp"

#include <CommandLineParser.hpp>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <fstream>

// Params ---------------------------------------------------------------------

static const CommandLineParserParams commandLineParserParams = {
    .standaloneFlags = {
        {
            .name = "PATH",
            .description = "Path to the scene file"
        },
        {
            .name = "OUTPUT",
            .description = "Output file"
        }
    },
    .flags = {
        {
            .shortName = 'h',
            .longName = "help",
            .description = "Print this help message"
        },
        {
            .shortName = 'r',
            .longName = "resolution",
            .description = "Resolution of the output image in the format WIDTHxHEIGHT. Default: 1920x1080, Min: 1x1, Max: 1920x1080",
            .minArgs = 1,
            .maxArgs = 1
        },
        {
            .shortName = 't',
            .longName = "threads",
            .description = "Number of threads to use for rendering. Default: 16, Max: 64, Min: 1",
            .minArgs = 1,
            .maxArgs = 1
        },
        {
            .shortName = 'a',
            .longName = "anti-aliasing",
            .description = "Anti-aliasing factor. Default: 3, Max: 5, Min: 1",
            .minArgs = 1,
            .maxArgs = 1
        }
    }
};

// Program --------------------------------------------------------------------

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

// Main -----------------------------------------------------------------------

int main(int argc, const char *argv[])
{
    CommandLineParser parser(commandLineParserParams);

    try {
        parser.parse(argc, argv);
        const Program::Params params(parser);

        switch (params.runMode) {
            case Program::RunMode::Help: {
                parser.printHelp();
                break;
            }
            case Program::RunMode::Raytracing: {
                Scene3d::Scene scene(params.path);
                Renderer renderer(scene);
                FrameBuffer frameBuffer;
                Renderer::RenderParams renderParams = {
                    .antiAliasingSize = params.antiAliasingSize,
                    .nbThreads = params.nbThreads,
                    .resolution = {params.resolutionX, params.resolutionY}
                };
                renderer.render(frameBuffer, renderParams);
                frameBuffer.toPngFile(params.output);
                break;
            }
            default:
                throw Program::InvalidParamsException("No run mode specified");
        }
    } catch (const FailedParsingException &e) {
        std::cerr << "Failed command line parsing: " << e.what() << std::endl;
        parser.printHelp(std::cerr);
        return Program::ExitCode::InvalidParams;
    } catch (const Program::InvalidParamsException &e) {
        std::cerr << "Invalid parameters: " << e.what() << std::endl;
        parser.printHelp(std::cerr);
        return Program::ExitCode::InvalidParams;
    } catch (const std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return Program::ExitCode::RuntimeError;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return Program::ExitCode::RuntimeError;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return Program::ExitCode::UnknownError;
    }
    return Program::ExitCode::Success;
}
