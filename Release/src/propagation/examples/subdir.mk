################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/propagation/examples/jakes-propagation-model-example.cc \
../src/propagation/examples/main-propagation-loss.cc 

OBJS += \
./src/propagation/examples/jakes-propagation-model-example.o \
./src/propagation/examples/main-propagation-loss.o 

CC_DEPS += \
./src/propagation/examples/jakes-propagation-model-example.d \
./src/propagation/examples/main-propagation-loss.d 


# Each subdirectory must supply rules for building sources it contributes
src/propagation/examples/%.o: ../src/propagation/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


