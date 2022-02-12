################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../recipos/drivers/buttons/ButtonReader.cpp 

OBJS += \
./recipos/drivers/buttons/ButtonReader.o 

CPP_DEPS += \
./recipos/drivers/buttons/ButtonReader.d 


# Each subdirectory must supply rules for building sources it contributes
recipos/drivers/buttons/%.o: ../recipos/drivers/buttons/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


