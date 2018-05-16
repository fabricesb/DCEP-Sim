################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/brite/test/brite-test-topology.cc 

OBJS += \
./src/brite/test/brite-test-topology.o 

CC_DEPS += \
./src/brite/test/brite-test-topology.d 


# Each subdirectory must supply rules for building sources it contributes
src/brite/test/%.o: ../src/brite/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


