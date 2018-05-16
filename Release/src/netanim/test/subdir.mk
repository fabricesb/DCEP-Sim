################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/netanim/test/netanim-test.cc 

OBJS += \
./src/netanim/test/netanim-test.o 

CC_DEPS += \
./src/netanim/test/netanim-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/netanim/test/%.o: ../src/netanim/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


