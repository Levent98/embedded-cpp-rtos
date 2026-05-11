FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    cmake \
    ninja-build \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    python3 \
    cppcheck \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace