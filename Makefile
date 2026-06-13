# =============================================================================
# BMS Project Makefile
# Target MCU  : NXP S32K144 (ARM Cortex-M4)
# Toolchains  : arm-none-eabi-gcc (production), gcc (unit test host)
#
# MCAL drivers included:
#   Port  — mcu/mcal/Port/
#   ADC   — mcu/mcal/Adc/
#   DIO   — mcu/mcal/Dio/
#   Det   — mcu/mcal/Det/
#
# Unit test strategy:
#   Each driver has its own test runner and JUnit XML output file.
#   Jenkins JUnit publisher uses pattern: test-results-*.xml
# =============================================================================

# -----------------------------------------------------------------------------
# Toolchains
# -----------------------------------------------------------------------------
CC_ARM    = arm-none-eabi-gcc
AR_ARM    = arm-none-eabi-ar
CC_NATIVE = gcc

# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------
FLAGS_ARM    = -mcpu=cortex-m4 -mthumb -O0 -Werror
FLAGS_NATIVE = -g -O0 -Werror -DUNIT_TESTING -DUNIT_TEST


INC_PROD = -I./mcu/mcal/Port      \
           -I./mcu/mcal/           \
           -I./mcu/mcal/includes   \
           -I./mcu/mcal/Adc        \
           -I./mcu/mcal/Dio

INC_TEST = -I./test/stubs          \
           -I./mcu/mcal/Port       \
           -I./mcu/mcal/           \
           -I./mcu/mcal/includes   \
           -I./mcu/mcal/Adc        \
           -I./mcu/mcal/Dio        \
           -I./tools/Unity/src

STUBS = test/stubs/S32K144_Registers.c \
        test/stubs/Det_Mock.c

UNITY = tools/Unity/src/unity.c

# =============================================================================
# CLEAN
# =============================================================================
clean:
	@if exist build rmdir /s /q build
	@if exist test-results-port.xml del test-results-port.xml
	@if exist test-results-adc.xml  del test-results-adc.xml
	@if exist test-results-dio.xml  del test-results-dio.xml

# =============================================================================
# PRODUCTION BUILD TARGET
# Cross-compiles all MCAL driver sources into a static library.
# Output: build/BMS_Library.a
# =============================================================================
build_target:
	@if not exist build mkdir build
	@echo "Compiling MCAL sources for S32K144..."
	$(CC_ARM) $(FLAGS_ARM) $(INC_PROD) -c mcu/mcal/Port/Port.c      -o build/Port.o
	$(CC_ARM) $(FLAGS_ARM) $(INC_PROD) -c mcu/mcal/Det/Det.c        -o build/Det.o
	$(CC_ARM) $(FLAGS_ARM) $(INC_PROD) -c mcu/mcal/Adc/Adc.c        -o build/Adc.o
	$(CC_ARM) $(FLAGS_ARM) $(INC_PROD) -c mcu/mcal/Adc/Adc_PBcfg.c -o build/Adc_PBcfg.o
	$(CC_ARM) $(FLAGS_ARM) $(INC_PROD) -c mcu/mcal/Dio/Dio.c        -o build/Dio.o
	$(CC_ARM) $(FLAGS_ARM) $(INC_PROD) -c mcu/mcal/Dio/Dio_PBcfg.c -o build/Dio_PBcfg.o
	$(AR_ARM) rcs build/BMS_Library.a  \
	    build/Port.o                   \
	    build/Det.o                    \
	    build/Adc.o                    \
	    build/Adc_PBcfg.o             \
	    build/Dio.o                    \
	    build/Dio_PBcfg.o
	@echo "Build Complete: build/BMS_Library.a"



# -----------------------------------------------------------------------------
# PORT unit tests
# Needs: Port.c + stubs + test_Port.c + Unity
# Does NOT need Port_PBcfg.c — test_Port.c supplies its own config struct.
# -----------------------------------------------------------------------------
test_port:
	@if not exist build mkdir build
	$(CC_NATIVE) $(FLAGS_NATIVE) $(INC_TEST) \
	    mcu/mcal/Port/Port.c                 \
	    $(STUBS)                             \
	    test/unit/test_Port.c               \
	    $(UNITY)                             \
	    -o build/test_port.exe
	build\test_port.exe > test-results-port.xml

# -----------------------------------------------------------------------------
# ADC unit tests
# Needs: Adc.c + stubs + test_Adc.c + Unity
# Does NOT need Adc_PBcfg.c — test_Adc.c supplies TestConfig directly.
# -----------------------------------------------------------------------------
test_adc:
	@if not exist build mkdir build
	$(CC_NATIVE) $(FLAGS_NATIVE) $(INC_TEST) \
	    mcu/mcal/Adc/Adc.c                   \
	    $(STUBS)                             \
	    test/unit/test_Adc.c               \
	    $(UNITY)                             \
	    -o build/test_adc.exe
	build\test_adc.exe > test-results-adc.xml

# -----------------------------------------------------------------------------
# DIO unit tests
# Needs: Dio.c + Dio_PBcfg.c + stubs + test_Dio.c + Unity
# REQUIRES Dio_PBcfg.c because Dio.c directly indexes Dio_ChannelConfigs[]
# and test_Dio.c references Dio_BalancingGroup — both defined in Dio_PBcfg.c.
# -----------------------------------------------------------------------------
test_dio:
	@if not exist build mkdir build
	$(CC_NATIVE) $(FLAGS_NATIVE) $(INC_TEST) \
	    mcu/mcal/Dio/Dio.c                   \
	    mcu/mcal/Dio/Dio_PBcfg.c            \
	    $(STUBS)                             \
	    test/unit/test_Dio.c               \
	    $(UNITY)                             \
	    -o build/test_dio.exe
	build\test_dio.exe > test-results-dio.xml

# -----------------------------------------------------------------------------
# test_host — runs ALL driver tests sequentially.
# Called by Jenkins Stage 4.
# Jenkins JUnit publisher pattern: test-results-*.xml
# -----------------------------------------------------------------------------
test_host: test_port test_adc test_dio
	@echo "All unit tests complete."