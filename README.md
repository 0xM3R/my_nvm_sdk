
# My NVM SDK

This project is a simple SDK for managing non-volatile memory (NVM) attributes with unit testing and fuzz testing using AFL.

## Directory Structure
```
/my_nvm_sdk
    /include      # Public header file
    /src          # SDK implementation
    /test         # Unit and fuzz test files
    /inputs       # Seed inputs for AFL fuzzing
    /screenshots  # Compilation screenshots
    Makefile      # Build script
```

## Compilation Instructions

### Prerequisites
- **AFL**: Install via `sudo apt-get install afl` or [AFL GitHub](https://github.com/google/AFL).
- **GCC** or **Clang**: For compiling tests.

### Compiling Unit Tests
```bash
make unit_test
```

### Compiling Fuzz Tests (AFL)
```bash
make fuzz_test
```

## Running the Tests

### Running Unit Tests
```bash
./gpNvmTest
```

### Running AFL Fuzz Tests
```bash
afl-fuzz -i inputs/ -o outputs/ -- ./gpNvmFuzzTest
```

## Cleaning Up
```bash
make clean
```

This will remove all generated files.
