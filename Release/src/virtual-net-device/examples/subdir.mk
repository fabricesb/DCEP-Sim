################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/virtual-net-device/examples/virtual-net-device.cc 

OBJS += \
./src/virtual-net-device/examples/virtual-net-device.o 

CC_DEPS += \
./src/virtual-net-device/examples/virtual-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
src/virtual-net-device/examples/%.o: ../src/virtual-net-device/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


