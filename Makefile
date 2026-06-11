# Toolchains
CC_ARM = arm-none-eabi-gcc
AR_ARM = arm-none-eabi-ar
CC_NATIVE = gcc

# Paths - Added -I./mcu/mcal/includes to INC_TEST
INC_PROD = -I./mcu/mcal/Port -I./mcu/mcal/ -I./mcu/mcal/includes
INC_TEST = -I./mcu/mcal/Port -I./mcu/mcal/ -I./mcu/mcal/includes -I./test/stubs -I./tools/Unity/src
# Clean Target
clean:
	@if exist build rmdir /s /q build

# Production Build Target
build_target:
	@if not exist build mkdir build
	@echo "Compiling..."
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Port/Port.c -o build/Port.o
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Det/Det.c -o build/Det.o
	$(AR_ARM) rcs build/BMS_Library.a build/Port.o build/Det.o
	@echo "Build Complete: BMS_Library.a"

# Test Target
test_host:
	@if not exist build mkdir build
	$(CC_NATIVE) -g -O0 -Werror -DUNIT_TESTING $(INC_TEST) -DUNIT_TEST mcu/mcal/Port/Port.c mcu/mcal/Det/Det.c test/stubs/S32K144_Registers.c test/unit/test_Port.c tools/Unity/src/unity.c -o build/test_runner.exe
	build\test_runner.exe > test-results.xml