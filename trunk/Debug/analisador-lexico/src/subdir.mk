################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../analisador-lexico/src/AnalisadorLexico.cpp \
../analisador-lexico/src/AutomatoFD.cpp \
../analisador-lexico/src/ErrosExecucao.cpp \
../analisador-lexico/src/LogErros.cpp \
../analisador-lexico/src/TabelaSimbolos.cpp 

OBJS += \
./analisador-lexico/src/AnalisadorLexico.o \
./analisador-lexico/src/AutomatoFD.o \
./analisador-lexico/src/ErrosExecucao.o \
./analisador-lexico/src/LogErros.o \
./analisador-lexico/src/TabelaSimbolos.o 

CPP_DEPS += \
./analisador-lexico/src/AnalisadorLexico.d \
./analisador-lexico/src/AutomatoFD.d \
./analisador-lexico/src/ErrosExecucao.d \
./analisador-lexico/src/LogErros.d \
./analisador-lexico/src/TabelaSimbolos.d 


# Each subdirectory must supply rules for building sources it contributes
analisador-lexico/src/%.o: ../analisador-lexico/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


