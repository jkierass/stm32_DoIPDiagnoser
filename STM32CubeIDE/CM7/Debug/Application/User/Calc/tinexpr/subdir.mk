################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/Calc/tinexpr/tinyexpr.c 

C_DEPS += \
./Application/User/Calc/tinexpr/tinyexpr.d 

OBJS += \
./Application/User/Calc/tinexpr/tinyexpr.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Calc/tinexpr/%.o Application/User/Calc/tinexpr/%.su Application/User/Calc/tinexpr/%.cyclo: ../Application/User/Calc/tinexpr/%.c Application/User/Calc/tinexpr/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DUSE_BPP=24 -DSTM32H747xx -c -I../../../CM7/Core/Inc -I../../../CM7/TouchGFX/App -I../../../CM7/TouchGFX/target/generated -I../../../CM7/TouchGFX/target -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/STM32H747I-Discovery -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -I../../../Drivers/BSP/Components -I../../../CM7/LIBJPEG/App -I../../../CM7/LIBJPEG/Target -I../../../Utilities/JPEG -I../../../Middlewares/Third_Party/LibJPEG/include -I../../CM7/../../CM7/Middlewares/ST/touchgfx/framework/include -I../../CM7/../../CM7/TouchGFX/generated/fonts/include -I../../CM7/../../CM7/TouchGFX/generated/gui_generated/include -I../../CM7/../../CM7/TouchGFX/generated/images/include -I../../CM7/../../CM7/TouchGFX/generated/texts/include -I../../CM7/../../CM7/TouchGFX/generated/videos/include -I../../CM7/../../CM7/TouchGFX/gui/include -I"C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/STM32CubeIDE/CM7/Application/User/EventManager/inc" -I"C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/STM32CubeIDE/CM7/Application/User/Calc/inc" -I"C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/STM32CubeIDE/CM7/Application/User/Calc/tinexpr" -I"C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/STM32CubeIDE/CM7/Application/User/EdiabasDaemonProxy/inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-Calc-2f-tinexpr

clean-Application-2f-User-2f-Calc-2f-tinexpr:
	-$(RM) ./Application/User/Calc/tinexpr/tinyexpr.cyclo ./Application/User/Calc/tinexpr/tinyexpr.d ./Application/User/Calc/tinexpr/tinyexpr.o ./Application/User/Calc/tinexpr/tinyexpr.su

.PHONY: clean-Application-2f-User-2f-Calc-2f-tinexpr

