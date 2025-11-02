#include "Program.hpp"
#include "lib/Scene3d/Scene/Scene.hpp"
#include "lib/FrameBuffer/FrameBuffer.hpp"
#include "lib/Renderer/Renderer.hpp"

#include "CommandLineParser.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>


void helpModeHandler(const Program::Params &params, const CommandLineParser &parser)
{
    parser.printHelp();
}

void raytracingModeHandler(const Program::Params &params, const CommandLineParser &parser)
{
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
    std::cout << scene << std::endl;
}

static void (*runModeHandlers[])(const Program::Params &, const CommandLineParser &) = {
    [static_cast<size_t>(Program::RunMode::Help)] = &helpModeHandler,
    [static_cast<size_t>(Program::RunMode::Raytracing)] = &raytracingModeHandler,
};

int main(const int argc, const char *argv[], const char *envp[])
{
    CommandLineParser parser(Program::commandLineParserParams);

    try {
        parser.parse(argc, argv);
        const Program::Params params(parser);
        runModeHandlers[static_cast<size_t>(params.runMode)](params, parser);
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
