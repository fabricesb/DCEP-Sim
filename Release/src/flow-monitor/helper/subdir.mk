################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/flow-monitor/helper/flow-monitor-helper.cc 

OBJS += \
./src/flow-monitor/helper/flow-monitor-helper.o 

CC_DEPS += \
./src/flow-monitor/helper/flow-monitor-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/flow-monitor/helper/%.o: ../src/flow-monitor/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


