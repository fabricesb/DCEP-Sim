################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/aodv/helper/aodv-helper.cc 

OBJS += \
./src/aodv/helper/aodv-helper.o 

CC_DEPS += \
./src/aodv/helper/aodv-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/aodv/helper/%.o: ../src/aodv/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


