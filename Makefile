# Toolchains
CC_ARM = arm-none-eabi-gcc
AR_ARM = arm-none-eabi-ar
CC_NATIVE = gcc

INC_TEST = -I./test/stubs -I./mcu/mcal/Port -I./mcu/mcal/ -I./mcu/mcal/includes -I./mcu/mcal/Adc -I./tools/Unity/src
INC_PROD = -I./mcu/mcal/Port -I./mcu/mcal/ -I./mcu/mcal/includes -I./mcu/mcal/Adc

# Clean Target
clean:
	@if exist build rmdir /s /q build

# Production Build Target
build_target:
	@if not exist build mkdir build
	@echo "Compiling..."
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Port/Port.c -o build/Port.o
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Det/Det.c -o build/Det.o
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Adc/Adc_PBcfg.c -o build/Adc_PBcfg.o
	
	$(AR_ARM) rcs build/BMS_Library.a build/Port.o build/Det.o build/Adc_PBcfg.o
	@echo "Build Complete: BMS_Library.a"

# Test Target
test_host:
	@if not exist build mkdir build
	$(CC_NATIVE) -g -O0 -Werror -DUNIT_TESTING $(INC_TEST) -DUNIT_TEST \
	mcu/mcal/Port/Port.c \
	test/stubs/S32K144_Registers.c \
	test/stubs/Det_Mock.c \
	test/unit/test_Port.c \
	tools/Unity/src/unity.c \
	-o build/test_runner.exe
	build\test_runner.exe