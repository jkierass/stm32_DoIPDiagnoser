################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c 

C_DEPS += \
./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.d 

OBJS += \
./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.o: C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c Drivers/CMSIS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../../CM4/Core/Inc -I../../../Common/Inc -I../../../Drivers/BSP/Components -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS

clean-Drivers-2f-CMSIS:
	-$(RM) ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.cyclo ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.d ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.o ./Drivers/CMSIS/system_stm32h7xx_dualcore_boot_cm4_cm7.su

.PHONY: clean-Drivers-2f-CMSIS

