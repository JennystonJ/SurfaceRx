################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/utilities/delay.c \
../Core/Src/utilities/map.c 

OBJS += \
./Core/Src/utilities/delay.o \
./Core/Src/utilities/map.o 

C_DEPS += \
./Core/Src/utilities/delay.d \
./Core/Src/utilities/map.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/utilities/%.o Core/Src/utilities/%.su: ../Core/Src/utilities/%.c Core/Src/utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-utilities

clean-Core-2f-Src-2f-utilities:
	-$(RM) ./Core/Src/utilities/delay.d ./Core/Src/utilities/delay.o ./Core/Src/utilities/delay.su ./Core/Src/utilities/map.d ./Core/Src/utilities/map.o ./Core/Src/utilities/map.su

.PHONY: clean-Core-2f-Src-2f-utilities

