################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/applications/test/udp-client-server-test.cc 

OBJS += \
./src/applications/test/udp-client-server-test.o 

CC_DEPS += \
./src/applications/test/udp-client-server-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/applications/test/%.o: ../src/applications/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


