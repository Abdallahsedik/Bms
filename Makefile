# Toolchains
CC_ARM = arm-none-eabi-gcc
CC_NATIVE = gcc

# Target Compiler Flags (For S32K144)
CFLAGS_ARM = -mcpu=cortex-m4 -mthumb -O0 -Werror -I./src

# Host Compiler Flags (Injects UNIT_TESTING macro for CMock/Unity)
CFLAGS_NATIVE = -g -O0 -Werror -DUNIT_TESTING -I./src -I./test/stubs -I./tools/Unity/src

# --- Stage 1: Cross-Compile for Hardware ---
build_target:
	@echo "Cross-compiling BMS MCAL..."
	mkdir -p build
	$(CC_ARM) $(CFLAGS_ARM) -c src/Port.c -o build/Port.o
	$(CC_ARM) $(CFLAGS_ARM) -Wl,-Map=build/BMS_Project.map build/Port.o -o build/BMS_Project.elf

# --- Stage 2: Native Compile for Tests ---
test_host:
	@echo "Compiling native tests..."
	mkdir -p build
	# test/stubs/S32K144_Stubs.c contains your RAM-allocated dummy registers
	$(CC_NATIVE) $(CFLAGS_NATIVE) tools/Unity/src/unity.c test/stubs/S32K144_Stubs.c src/Port.c test/test_Port.c -o build/test_runner.out
	./build/test_runner.out

clean:
	rm -rf build/*