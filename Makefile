# Compiler
CC = gcc
AFL_CC = afl-clang-fast  # AFL's compiler for fuzz testing
CFLAGS = -Wall -I./include -fPIE  # Position independent code for both fuzzing and unit testing
LDFLAGS = -pie  # Generate position independent executables

# Targets
all: unit_test fuzz_test

# Unit test target
unit_test: gpNvm.o gpNvmTest.o
	$(CC) $(CFLAGS) $(LDFLAGS) gpNvm.o gpNvmTest.o -o gpNvmTest

# Fuzz test target using AFL
fuzz_test: gpNvm.o gpNvmFuzzTest.o
	$(AFL_CC) $(CFLAGS) $(LDFLAGS) gpNvm.o gpNvmFuzzTest.o -o gpNvmFuzzTest

# Object file for SDK source
gpNvm.o: src/gpNvm.c include/gpNvm.h
	$(CC) $(CFLAGS) -c src/gpNvm.c -o gpNvm.o

# Object file for unit tests
gpNvmTest.o: test/gpNvmTest.c
	$(CC) $(CFLAGS) -c test/gpNvmTest.c -o gpNvmTest.o

# Object file for fuzz testing
gpNvmFuzzTest.o: test/gpNvmFuzzTest.c
	$(AFL_CC) $(CFLAGS) -c test/gpNvmFuzzTest.c -o gpNvmFuzzTest.o

# Clean target to remove generated files
clean:
	rm -f *.o gpNvmTest gpNvmFuzzTest
