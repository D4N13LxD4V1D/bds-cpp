# bds-cpp

bds is a programming language inspired by C and Rust. It is a compiled language with a focus on performance and safety.

## Contributing

### Building

To build bds, you will need to have CMake 3.27.7 and Clang+LLVM 17.0.3. You can install Clang+LLVM from [here](https://github.com/llvm/llvm-project/releases) and CMake from [here](https://cmake.org/download/).

Once you have installed Clang+LLVM and CMake, you can build bds by running the following commands:

```bash
git clone https://github.com/D4N13LxD4V1D/bds-cpp.git
cd bds-cpp
mkdir build
cd build
cmake ..
make
```

### Running

To run bds with the example program, run the following command:

```bash
./build/bds examples/hello_world.bds
```
