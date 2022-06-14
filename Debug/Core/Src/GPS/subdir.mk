################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GPS/GPScontroller.c \
../Core/Src/GPS/GPSmodel.c \
../Core/Src/GPS/parser.c 

OBJS += \
./Core/Src/GPS/GPScontroller.o \
./Core/Src/GPS/GPSmodel.o \
./Core/Src/GPS/parser.o 

C_DEPS += \
./Core/Src/GPS/GPScontroller.d \
./Core/Src/GPS/GPSmodel.d \
./Core/Src/GPS/parser.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GPS/%.o Core/Src/GPS/%.su: ../Core/Src/GPS/%.c Core/Src/GPS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GPS

clean-Core-2f-Src-2f-GPS:
	-$(RM) ./Core/Src/GPS/GPScontroller.d ./Core/Src/GPS/GPScontroller.o ./Core/Src/GPS/GPScontroller.su ./Core/Src/GPS/GPSmodel.d ./Core/Src/GPS/GPSmodel.o ./Core/Src/GPS/GPSmodel.su ./Core/Src/GPS/parser.d ./Core/Src/GPS/parser.o ./Core/Src/GPS/parser.su

.PHONY: clean-Core-2f-Src-2f-GPS

