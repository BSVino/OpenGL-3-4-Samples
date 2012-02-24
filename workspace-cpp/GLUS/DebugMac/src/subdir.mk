################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/glus_glfw.c \
../src/glus_load.c \
../src/glus_math.c \
../src/glus_matrix.c \
../src/glus_plane.c \
../src/glus_point.c \
../src/glus_quaternion.c \
../src/glus_shaderprogram.c \
../src/glus_shape.c \
../src/glus_vector.c \
../src/glus_version.c \
../src/glus_viewprojection.c \
../src/glus_wavefront.c 

OBJS += \
./src/glus_glfw.o \
./src/glus_load.o \
./src/glus_math.o \
./src/glus_matrix.o \
./src/glus_plane.o \
./src/glus_point.o \
./src/glus_quaternion.o \
./src/glus_shaderprogram.o \
./src/glus_shape.o \
./src/glus_vector.o \
./src/glus_version.o \
./src/glus_viewprojection.o \
./src/glus_wavefront.o 

C_DEPS += \
./src/glus_glfw.d \
./src/glus_load.d \
./src/glus_math.d \
./src/glus_matrix.d \
./src/glus_plane.d \
./src/glus_point.d \
./src/glus_quaternion.d \
./src/glus_shaderprogram.d \
./src/glus_shape.d \
./src/glus_vector.d \
./src/glus_version.d \
./src/glus_viewprojection.d \
./src/glus_wavefront.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/nopper/workspace-cpp/External/mac/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


