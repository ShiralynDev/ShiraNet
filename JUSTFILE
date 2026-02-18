alias b := build

# Builds the library
build:
    cmake -S . -G Ninja -O build
    cmake --build build

alias bms := build-manual-server
[group("Example")]
build-manual-server:
    just build-example tcpEchoManualServer

alias rms := run-manual-server
[group("Example")]
run-manual-server:
    just run-example tcpEchoManualServer

alias bss := build-shiranet-server
[group("Example")]
build-shiranet-server:
    just build-example tcpEchoShiraNetServer

alias rss := run-shiranet-server
[group("Example")]
run-shiranet-server:
    just run-example tcpEchoShiraNetServer


# Generic example builder
[group("Private")]
build-example name:
    cmake -S examples/{{name}} -G Ninja -B examples/{{name}}/build
    cmake --build examples/{{name}}/build

# Generic example runner
[group("Private")]
run-example name: (build-example name)
    ./examples/{{name}}/build/ShiraNetTest
