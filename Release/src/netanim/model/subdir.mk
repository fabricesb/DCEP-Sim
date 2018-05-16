################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/netanim/model/animation-interface.cc 

OBJS += \
./src/netanim/model/animation-interface.o 

CC_DEPS += \
./src/netanim/model/animation-interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/netanim/model/%.o: ../src/netanim/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


