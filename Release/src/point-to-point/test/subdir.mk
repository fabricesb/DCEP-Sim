################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/point-to-point/test/point-to-point-test.cc 

OBJS += \
./src/point-to-point/test/point-to-point-test.o 

CC_DEPS += \
./src/point-to-point/test/point-to-point-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/point-to-point/test/%.o: ../src/point-to-point/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


