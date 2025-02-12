FROM ubuntu:latest

ENV TZ=UTC
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build

RUN apt-get install -y --no-install-recommends \
    clang-17 \
    libstdc++-13-dev

RUN apt-get install -y --no-install-recommends \
    libc++abi-17-dev \
    libclang-rt-17-dev

RUN apt-get install -y --no-install-recommends \
    libssl-dev \
    libboost-all-dev \
    libpq-dev \
    libfmt-dev

RUN apt-get install -y --no-install-recommends \
    clang-tidy-17

RUN rm -rf /var/lib/apt/lists/*

WORKDIR /lib

COPY . .

RUN cmake -S . -B build -G Ninja \
    -DCMAKE_CXX_COMPILER=clang++-17 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wmisleading-indentation -Wnull-dereference" \
    -DCMAKE_CXX_CLANG_TIDY="clang-tidy-17;-checks=performance-*,-clang-analyzer-*,bugprone-*"

RUN cmake --build build --config Release -j$(nproc)

CMD ["ninja", "-C", "build"]