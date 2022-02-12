################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../recipos/modules/Buttons.cpp \
../recipos/modules/Display.cpp \
../recipos/modules/Printer.cpp \
../recipos/modules/Speaker.cpp \
../recipos/modules/Storage.cpp 

OBJS += \
./recipos/modules/Buttons.o \
./recipos/modules/Display.o \
./recipos/modules/Printer.o \
./recipos/modules/Speaker.o \
./recipos/modules/Storage.o 

CPP_DEPS += \
./recipos/modules/Buttons.d \
./recipos/modules/Display.d \
./recipos/modules/Printer.d \
./recipos/modules/Speaker.d \
./recipos/modules/Storage.d 


# Each subdirectory must supply rules for building sources it contributes
recipos/modules/%.o: ../recipos/modules/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


