################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mesh/examples/mesh.cc 

OBJS += \
./src/mesh/examples/mesh.o 

CC_DEPS += \
./src/mesh/examples/mesh.d 


# Each subdirectory must supply rules for building sources it contributes
src/mesh/examples/%.o: ../src/mesh/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


