alias b := build

# Builds the library
build:
    cmake -S . -G Ninja -O build
    cmake --build build

alias bms := build-manual-server
[group("Examples")]
build-manual-server:
    cmake -G Ninja -S examples/tcpEchoManualServer -O examples/tcpEchoManualServer/build
    cmake --build examples/tcpEchoManualServer/build

alias rms := run-manual-server
[group("Examples")]
run-manual-server: build-manual-server
    examples/tcpEchoManualServer/build/ShiraNetTest

alias bss := build-shiranet-server
[group("Examples")]
build-shiranet-server:
    cmake -G Ninja -S examples/tcpEchoShiraNetServer -O examples/tcpEchoShiraNetServer/build
    cmake --build examples/tcpEchoShiraNetServer/build

alias rss := run-shiranet-server
[group("Examples")]
run-shiranet-server: build-shiranet-server
    examples/tcpEchoShiraNetServer/build/ShiraNetTest