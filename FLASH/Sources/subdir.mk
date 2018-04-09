################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/CLK.c" \
"../Sources/KBI.c" \
"../Sources/SPI.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/CLK.c \
../Sources/KBI.c \
../Sources/SPI.c \
../Sources/main.c \

OBJS += \
./Sources/CLK.o \
./Sources/KBI.o \
./Sources/SPI.o \
./Sources/main.o \

OBJS_QUOTED += \
"./Sources/CLK.o" \
"./Sources/KBI.o" \
"./Sources/SPI.o" \
"./Sources/main.o" \

C_DEPS += \
./Sources/CLK.d \
./Sources/KBI.d \
./Sources/SPI.d \
./Sources/main.d \

OBJS_OS_FORMAT += \
./Sources/CLK.o \
./Sources/KBI.o \
./Sources/SPI.o \
./Sources/main.o \

C_DEPS_QUOTED += \
"./Sources/CLK.d" \
"./Sources/KBI.d" \
"./Sources/SPI.d" \
"./Sources/main.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/CLK.o: ../Sources/CLK.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/CLK.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/CLK.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/KBI.o: ../Sources/KBI.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/KBI.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/KBI.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SPI.o: ../Sources/SPI.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/SPI.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/SPI.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '


