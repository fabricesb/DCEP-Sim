################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/config-store/examples/config-store-save.cc 

OBJS += \
./src/config-store/examples/config-store-save.o 

CC_DEPS += \
./src/config-store/examples/config-store-save.d 


# Each subdirectory must supply rules for building sources it contributes
src/config-store/examples/%.o: ../src/config-store/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


