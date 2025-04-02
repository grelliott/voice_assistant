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
    SDL2,
}:

gccStdenv.mkDerivation rec {
    pname = "voice_assistant_app";
    version = "0.0.2";
    src = ./.;

    nativeBuildInputs = [ cmake ninja git gtest ];
    buildInputs = [ spdlog argparse SDL2 ];
    checkInputs = [ cppcheck cpplint ];
    doCheck = true;
    cmakeFlags = ["-DBUILD_TESTING:BOOL=TRUE"];

    meta = with lib; {
        license = licenses.mit;
    };
}
