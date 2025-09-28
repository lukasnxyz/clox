#!/bin/sh

mkdir -p build/
cd build/ || exit 1
cmake ..
cmake --build . -j4 || exit 1

if [ "$1" = "run" ]; then
    ./clox   # replace with the actual binary name
elif [ "$1" = "test" ]; then
    ctest --output-on-failure -V
else
    echo "Usage: $0 [run|test]"
fi
