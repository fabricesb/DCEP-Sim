################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/topology-read/examples/topology-example-sim.cc 

OBJS += \
./src/topology-read/examples/topology-example-sim.o 

CC_DEPS += \
./src/topology-read/examples/topology-example-sim.d 


# Each subdirectory must supply rules for building sources it contributes
src/topology-read/examples/%.o: ../src/topology-read/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


