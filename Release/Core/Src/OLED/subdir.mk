################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/OLED/Screens.c 

OBJS += \
./Core/Src/OLED/Screens.o 

C_DEPS += \
./Core/Src/OLED/Screens.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/OLED/%.o Core/Src/OLED/%.su: ../Core/Src/OLED/%.c Core/Src/OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-OLED

clean-Core-2f-Src-2f-OLED:
	-$(RM) ./Core/Src/OLED/Screens.d ./Core/Src/OLED/Screens.o ./Core/Src/OLED/Screens.su

.PHONY: clean-Core-2f-Src-2f-OLED

