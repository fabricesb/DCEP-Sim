################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/uan/helper/acoustic-modem-energy-model-helper.cc \
../src/uan/helper/uan-helper.cc 

OBJS += \
./src/uan/helper/acoustic-modem-energy-model-helper.o \
./src/uan/helper/uan-helper.o 

CC_DEPS += \
./src/uan/helper/acoustic-modem-energy-model-helper.d \
./src/uan/helper/uan-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/uan/helper/%.o: ../src/uan/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


