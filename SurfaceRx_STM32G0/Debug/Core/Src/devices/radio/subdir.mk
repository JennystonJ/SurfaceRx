################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/devices/radio/receiver.c 

OBJS += \
./Core/Src/devices/radio/receiver.o 

C_DEPS += \
./Core/Src/devices/radio/receiver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/devices/radio/%.o Core/Src/devices/radio/%.su: ../Core/Src/devices/radio/%.c Core/Src/devices/radio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-devices-2f-radio

clean-Core-2f-Src-2f-devices-2f-radio:
	-$(RM) ./Core/Src/devices/radio/receiver.d ./Core/Src/devices/radio/receiver.o ./Core/Src/devices/radio/receiver.su

.PHONY: clean-Core-2f-Src-2f-devices-2f-radio

