# Toolchains
CC_ARM = arm-none-eabi-gcc
AR_ARM = arm-none-eabi-ar 
CC_NATIVE = gcc

# path to common headers
INC_PATH = -I./mcu/mcal/Port -I./mcu/mcal/ -I./mcu/mcal/includes

# Target Compiler Flags
CFLAGS_ARM = -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PATH)

# Host Compiler Flags
CFLAGS_NATIVE = -g -O0 -Werror -DUNIT_TESTING $(INC_PATH) -I./tools/Unity/src

# --- Stage 1: Cross-Compile for Hardware ---
build_target:
	@if not exist build mkdir build
	@echo "Compiling..."
	$(CC_ARM) $(CFLAGS_ARM) -c mcu/mcal/Port/Port.c -o build/Port.o
	$(CC_ARM) $(CFLAGS_ARM) -c mcu/mcal/Det/Det.c -o build/Det.o
	@echo "Archiving to library..."
	$(AR_ARM) rcs build/BMS_Library.a build/Port.o build/Det.o
	@echo "Build Complete: BMS_Library.a"

# --- Stage 2: Native Compile for Tests ---
test_host:
	@if not exist build mkdir build
	$(CC_NATIVE) $(CFLAGS_NATIVE) -DUNIT_TEST \
	mcu/mcal/Port/Port.c \
	mcu/mcal/Det/Det.c \
	test/stubs/S32K144_Registers.c \
	test/unit/test_Port.c \
	-o build/test_runner.exe
	build\test_runner.exe
clean:
	@if exist build rmdir /s /q build