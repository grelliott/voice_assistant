/*
 * Filename: /home/grant/src/test_c++/src/main.cpp
 * Path: /home/grant/src/test_c++/src
 * Created Date: Saturday, August 24th 2024, 5:59:00 am
 * Author: Grant Elliott
 *
 * Copyright (c) 2024 Grant Elliott
 */

#include <argparse/argparse.hpp>
#include <exception>

#include "application.hpp"
#include "log.hpp"
#include "version.hpp"

using namespace ca::grantelliott;

auto main(int argc, char* argv[]) -> int {
    argparse::ArgumentParser args("test_app", APP_VERSION);
    args.add_description(APP_DESCRIPTION);
    // add logging-related arguments

    try {
        args.parse_args(argc, argv);
    } catch (const std::exception& err) {
        log::critical("Failed to parse command-line arguments - {}",
                      err.what());
        return 1;
    }

    // initialize logging
    log::initialize();

    ca::grantelliott::testapp::Application app{};
    app.init();

    app.start();

    log::info("Terminating program");
    app.terminate();
}
