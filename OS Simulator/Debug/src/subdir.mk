################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OS\ Simulator.cpp 

OBJS += \
./src/OS\ Simulator.o 

CPP_DEPS += \
./src/OS\ Simulator.d 


# Each subdirectory must supply rules for building sources it contributes
src/OS\ Simulator.o: ../src/OS\ Simulator.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/OS Simulator.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


