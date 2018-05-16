################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/wifi/examples/ideal-wifi-manager-example.cc \
../src/wifi/examples/minstrel-ht-wifi-manager-example.cc \
../src/wifi/examples/test-interference-helper.cc \
../src/wifi/examples/wifi-phy-configuration.cc \
../src/wifi/examples/wifi-phy-test.cc 

OBJS += \
./src/wifi/examples/ideal-wifi-manager-example.o \
./src/wifi/examples/minstrel-ht-wifi-manager-example.o \
./src/wifi/examples/test-interference-helper.o \
./src/wifi/examples/wifi-phy-configuration.o \
./src/wifi/examples/wifi-phy-test.o 

CC_DEPS += \
./src/wifi/examples/ideal-wifi-manager-example.d \
./src/wifi/examples/minstrel-ht-wifi-manager-example.d \
./src/wifi/examples/test-interference-helper.d \
./src/wifi/examples/wifi-phy-configuration.d \
./src/wifi/examples/wifi-phy-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/wifi/examples/%.o: ../src/wifi/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


