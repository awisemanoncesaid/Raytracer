
#include "Program.hpp"


namespace Program {

    const CommandLineParserParams commandLineParserParams = {
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

} /* namespace Program */
