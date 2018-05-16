################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mesh/helper/flame/flame-installer.cc 

OBJS += \
./src/mesh/helper/flame/flame-installer.o 

CC_DEPS += \
./src/mesh/helper/flame/flame-installer.d 


# Each subdirectory must supply rules for building sources it contributes
src/mesh/helper/flame/%.o: ../src/mesh/helper/flame/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


