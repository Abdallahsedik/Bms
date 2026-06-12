# =============================================================================
# BMS Project Makefile
# Target MCU  : NXP S32K144 (ARM Cortex-M4)
# Toolchains  : arm-none-eabi-gcc (production), gcc (unit test host)
# =============================================================================

# -----------------------------------------------------------------------------
# Toolchains
# -----------------------------------------------------------------------------
CC_ARM    = arm-none-eabi-gcc
AR_ARM    = arm-none-eabi-ar
CC_NATIVE = gcc

# -----------------------------------------------------------------------------
# Include Paths
#
# INC_TEST: stubs path MUST come before mcu/mcal/includes so the compiler
#           finds stub headers first, allowing the real hardware headers to be
#           blocked by the stub's include guard.
# -----------------------------------------------------------------------------
INC_PROD = -I./mcu/mcal/Port      \
           -I./mcu/mcal/           \
           -I./mcu/mcal/includes   \
           -I./mcu/mcal/Adc

INC_TEST = -I./test/stubs          \
           -I./mcu/mcal/Port       \
           -I./mcu/mcal/           \
           -I./mcu/mcal/includes   \
           -I./mcu/mcal/Adc        \
           -I./tools/Unity/src

# -----------------------------------------------------------------------------
# Clean Target
# -----------------------------------------------------------------------------
clean:
	@if exist build rmdir /s /q build

# -----------------------------------------------------------------------------
# Production Build Target
# Cross-compiles all MCAL sources into a static library for the S32K144 target.
# Output: build/BMS_Library.a
# -----------------------------------------------------------------------------
build_target:
	@if not exist build mkdir build
	@echo "Compiling MCAL sources for S32K144..."
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Port/Port.c       -o build/Port.o
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Det/Det.c         -o build/Det.o
	$(CC_ARM) -mcpu=cortex-m4 -mthumb -O0 -Werror $(INC_PROD) -c mcu/mcal/Adc/Adc_PBcfg.c  -o build/Adc_PBcfg.o
	$(AR_ARM) rcs build/BMS_Library.a build/Port.o build/Det.o build/Adc_PBcfg.o
	@echo "Build Complete: build/BMS_Library.a"

# -----------------------------------------------------------------------------
# Native Unit Test Target
# Compiles driver sources + stubs + Unity tests using native GCC.
# Runs the test binary on the host and captures Unity output as JUnit XML
# so Jenkins Stage 4 can parse and display pass/fail results.
# Output: test-results.xml (read by Jenkins JUnit publisher)
# -----------------------------------------------------------------------------
test_host:
	@if not exist build mkdir build
	$(CC_NATIVE) -g -O0 -Werror              \
	    -DUNIT_TESTING -DUNIT_TEST            \
	    $(INC_TEST)                           \
	    mcu/mcal/Port/Port.c                  \
	    test/stubs/S32K144_Registers.c        \
	    test/stubs/Det_Mock.c                 \
	    test/unit/test_Port.c                 \
	    tools/Unity/src/unity.c               \
	    -o build/test_runner.exe
	build\test_runner.exe > test-results.xml
