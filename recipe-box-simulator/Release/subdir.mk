################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../arduinostandin.cpp \
../lcdtest.cpp \
../main.cpp \
../terminal.cpp 

OBJS += \
./arduinostandin.o \
./lcdtest.o \
./main.o \
./terminal.o 

CPP_DEPS += \
./arduinostandin.d \
./lcdtest.d \
./main.d \
./terminal.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


