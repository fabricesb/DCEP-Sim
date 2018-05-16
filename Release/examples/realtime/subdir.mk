################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/realtime/realtime-udp-echo.cc 

OBJS += \
./examples/realtime/realtime-udp-echo.o 

CC_DEPS += \
./examples/realtime/realtime-udp-echo.d 


# Each subdirectory must supply rules for building sources it contributes
examples/realtime/%.o: ../examples/realtime/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


