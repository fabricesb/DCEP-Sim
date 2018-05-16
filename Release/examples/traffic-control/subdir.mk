################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/traffic-control/queue-discs-benchmark.cc \
../examples/traffic-control/traffic-control.cc 

OBJS += \
./examples/traffic-control/queue-discs-benchmark.o \
./examples/traffic-control/traffic-control.o 

CC_DEPS += \
./examples/traffic-control/queue-discs-benchmark.d \
./examples/traffic-control/traffic-control.d 


# Each subdirectory must supply rules for building sources it contributes
examples/traffic-control/%.o: ../examples/traffic-control/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


