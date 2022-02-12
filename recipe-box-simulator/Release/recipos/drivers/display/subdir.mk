################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../recipos/drivers/display/bitmapdrawer.cpp 

C_SRCS += \
../recipos/drivers/display/font.c 

OBJS += \
./recipos/drivers/display/bitmapdrawer.o \
./recipos/drivers/display/font.o 

CPP_DEPS += \
./recipos/drivers/display/bitmapdrawer.d 

C_DEPS += \
./recipos/drivers/display/font.d 


# Each subdirectory must supply rules for building sources it contributes
recipos/drivers/display/%.o: ../recipos/drivers/display/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

recipos/drivers/display/%.o: ../recipos/drivers/display/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


