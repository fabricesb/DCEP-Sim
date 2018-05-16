################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/internet-apps/model/ping6.cc \
../src/internet-apps/model/radvd-interface.cc \
../src/internet-apps/model/radvd-prefix.cc \
../src/internet-apps/model/radvd.cc \
../src/internet-apps/model/v4ping.cc 

OBJS += \
./src/internet-apps/model/ping6.o \
./src/internet-apps/model/radvd-interface.o \
./src/internet-apps/model/radvd-prefix.o \
./src/internet-apps/model/radvd.o \
./src/internet-apps/model/v4ping.o 

CC_DEPS += \
./src/internet-apps/model/ping6.d \
./src/internet-apps/model/radvd-interface.d \
./src/internet-apps/model/radvd-prefix.d \
./src/internet-apps/model/radvd.d \
./src/internet-apps/model/v4ping.d 


# Each subdirectory must supply rules for building sources it contributes
src/internet-apps/model/%.o: ../src/internet-apps/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


