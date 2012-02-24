################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/renderBackground.c \
../src/renderWaterTexture.c 

OBJS += \
./src/main.o \
./src/renderBackground.o \
./src/renderWaterTexture.o 

C_DEPS += \
./src/main.d \
./src/renderBackground.d \
./src/renderWaterTexture.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/nopper/Development/workspace-cpp/GLUS/src" -I"/home/nopper/Development/workspace-cpp/External/linux/include" -O0 -g3 -Wall -c -fmessage-length=0 -m32 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


