################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../recipos/RecipOS.cpp 

C_SRCS += \
../recipos/colors.c 

OBJS += \
./recipos/RecipOS.o \
./recipos/colors.o 

CPP_DEPS += \
./recipos/RecipOS.d 

C_DEPS += \
./recipos/colors.d 


# Each subdirectory must supply rules for building sources it contributes
recipos/%.o: ../recipos/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

recipos/%.o: ../recipos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


