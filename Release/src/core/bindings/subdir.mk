################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/core/bindings/module_helpers.cc 

OBJS += \
./src/core/bindings/module_helpers.o 

CC_DEPS += \
./src/core/bindings/module_helpers.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/bindings/%.o: ../src/core/bindings/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


