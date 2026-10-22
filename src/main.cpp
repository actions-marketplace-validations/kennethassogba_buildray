#include "buildray/cli.hpp"
#include "buildray/core.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    buildray::install_signal_handlers();
    const std::vector<std::string> args(argv + 1, argv + argc);
    if (args.empty() || args.front() == "--help" || args.front() == "-h" ||
        (args.size() == 2 && args[1] == "--help") || args.front() == "--help-all") {
        std::cout << buildray::usage(args.front() == "--help-all" ||
            (args.size() == 2 && args.front() != "measure" && args.front() != "clean" && args.front() != "clean-deps"));
        return 0;
    }
    if (args.front() == "--version") {
        std::cout << "buildray " BUILDRAY_VERSION "\n";
        return 0;
    }
    if (args.front() == "check") {
        buildray::CheckOptions options;
        try {
            options = buildray::parse_check(args);
        } catch (const std::exception& error) {
            std::cerr << "buildray: " << error.what() << '\n';
            return 2;
        }
        try {
            return buildray::check_reports(options);
        } catch (const std::exception& error) {
            std::cerr << "buildray: " << error.what() << '\n';
            return 1;
        }
    }
    if (args.front() == "_launch" || args.front() == "_trace-launch" || args.front() == "_deps-launch" || args.front() == "_link-launch") {
        try {
            if (args.front() == "_link-launch") return buildray::launch_link({args.begin() + 1, args.end()});
            return buildray::launch({args.begin() + 1, args.end()}, args.front() == "_trace-launch",
                                    args.front() == "_deps-launch");
        } catch (const std::exception& error) {
            std::cerr << "buildray launcher: " << error.what() << '\n';
            return 1;
        }
    }
    buildray::Options options;
    try {
        options = buildray::parse(args);
    } catch (const std::exception& error) {
        std::cerr << "buildray: " << error.what() << "\nUse buildray --help\n";
        return 2;
    }
    try {
        return buildray::pipeline(options, buildray::fs::read_symlink("/proc/self/exe"));
    } catch (const std::exception& error) {
        std::cerr << "buildray: " << error.what() << '\n';
        return 1;
    }
}
