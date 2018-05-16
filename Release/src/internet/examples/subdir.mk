################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/internet/examples/main-simple.cc 

OBJS += \
./src/internet/examples/main-simple.o 

CC_DEPS += \
./src/internet/examples/main-simple.d 


# Each subdirectory must supply rules for building sources it contributes
src/internet/examples/%.o: ../src/internet/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


