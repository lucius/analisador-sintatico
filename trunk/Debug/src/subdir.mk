################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AnalisadorSintatico.cpp \
../src/ConteudoHash.cpp \
../src/NoArvoreSintatica.cpp 

OBJS += \
./src/AnalisadorSintatico.o \
./src/ConteudoHash.o \
./src/NoArvoreSintatica.o 

CPP_DEPS += \
./src/AnalisadorSintatico.d \
./src/ConteudoHash.d \
./src/NoArvoreSintatica.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


