# Toolchains
CC_ARM = arm-none-eabi-gcc
AR_ARM = arm-none-eabi-ar
CC_NATIVE = gcc

# Paths
INC_PROD = -I./mcu/mcal/Port -I./mcu/mcal/ -I./mcu/mcal/includes
INC_TEST = -I./mcu/mcal/Port -I./mcu/mcal/ -I./test/stubs -I./tools/Unity/src

build_target:
	@if not exist build mkdir build
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Port/Port.c -o build/Port.o
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Det/Det.c -o build/Det.o
	$(AR_ARM) rcs build/BMS_Library.a build/Port.o build/Det.o

test_host:
	@if not exist build mkdir build
	$(CC_NATIVE) -g -O0 -Werror -DUNIT_TESTING $(INC_TEST) -DUNIT_TEST mcu/mcal/Port/Port.c mcu/mcal/Det/Det.c test/stubs/S32K144_Registers.c test/unit/test_Port.c tools/Unity/src/unity.c -o build/test_runner.exe
	build\test_runner.exe