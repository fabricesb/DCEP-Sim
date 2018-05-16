################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mobility/helper/mobility-helper.cc \
../src/mobility/helper/ns2-mobility-helper.cc 

OBJS += \
./src/mobility/helper/mobility-helper.o \
./src/mobility/helper/ns2-mobility-helper.o 

CC_DEPS += \
./src/mobility/helper/mobility-helper.d \
./src/mobility/helper/ns2-mobility-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/mobility/helper/%.o: ../src/mobility/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


