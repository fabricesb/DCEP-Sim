################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/openflow/examples/openflow-switch.cc 

OBJS += \
./src/openflow/examples/openflow-switch.o 

CC_DEPS += \
./src/openflow/examples/openflow-switch.d 


# Each subdirectory must supply rules for building sources it contributes
src/openflow/examples/%.o: ../src/openflow/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


