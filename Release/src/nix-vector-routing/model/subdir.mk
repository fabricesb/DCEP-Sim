################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/nix-vector-routing/model/ipv4-nix-vector-routing.cc 

OBJS += \
./src/nix-vector-routing/model/ipv4-nix-vector-routing.o 

CC_DEPS += \
./src/nix-vector-routing/model/ipv4-nix-vector-routing.d 


# Each subdirectory must supply rules for building sources it contributes
src/nix-vector-routing/model/%.o: ../src/nix-vector-routing/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


