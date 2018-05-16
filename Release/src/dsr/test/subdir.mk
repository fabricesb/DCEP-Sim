################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/dsr/test/dsr-test-suite.cc 

OBJS += \
./src/dsr/test/dsr-test-suite.o 

CC_DEPS += \
./src/dsr/test/dsr-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
src/dsr/test/%.o: ../src/dsr/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


