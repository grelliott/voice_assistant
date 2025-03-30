# voice_assistant_app.nix
{
    lib,
    gccStdenv,
    clang-tools,
    cmake,
    cppcheck,
    cpplint,
    git,
    gtest,
    ninja,
    spdlog,
    argparse,
}:

gccStdenv.mkDerivation rec {
    pname = "voice_assistant_app";
    version = "0.0.2";
    src = ./.; # set correctly?

    nativeBuildInputs = [clang-tools cppcheck cpplint cmake ninja git gtest];
    buildInputs = [spdlog argparse];

    meta = with lib; {
        license = licenses.mit;
    };
}
