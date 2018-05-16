################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/olsr/helper/olsr-helper.cc 

OBJS += \
./src/olsr/helper/olsr-helper.o 

CC_DEPS += \
./src/olsr/helper/olsr-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/olsr/helper/%.o: ../src/olsr/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


