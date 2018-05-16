################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/nix-vector-routing/helper/ipv4-nix-vector-helper.cc 

OBJS += \
./src/nix-vector-routing/helper/ipv4-nix-vector-helper.o 

CC_DEPS += \
./src/nix-vector-routing/helper/ipv4-nix-vector-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/nix-vector-routing/helper/%.o: ../src/nix-vector-routing/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


