################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Application/User/Core/Logger/src/Logger.cpp 

OBJS += \
./Application/User/Core/Logger/src/Logger.o 

CPP_DEPS += \
./Application/User/Core/Logger/src/Logger.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Core/Logger/src/%.o Application/User/Core/Logger/src/%.su Application/User/Core/Logger/src/%.cyclo: ../Application/User/Core/Logger/src/%.cpp Application/User/Core/Logger/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DUSE_BPP=24 -DSTM32H747xx -c -I../../../CM7/Core/Inc -I../../../Common/Inc -I../../../CM7/TouchGFX/App -I../../../CM7/TouchGFX/target/generated -I../../../CM7/TouchGFX/target -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/STM32H747I-Discovery -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -I../../../Drivers/BSP/Components -I../../../CM7/LIBJPEG/App -I../../../CM7/LIBJPEG/Target -I../../../Utilities/JPEG -I../../../Middlewares/Third_Party/LibJPEG/include -I../../CM7/../../CM7/Middlewares/ST/touchgfx/framework/include -I../../CM7/../../CM7/TouchGFX/generated/fonts/include -I../../CM7/../../CM7/TouchGFX/generated/gui_generated/include -I../../CM7/../../CM7/TouchGFX/generated/images/include -I../../CM7/../../CM7/TouchGFX/generated/texts/include -I../../CM7/../../CM7/TouchGFX/generated/videos/include -I../../CM7/../../CM7/TouchGFX/gui/include -I../../../STM32CubeIDE/CM7/Application/User/Core/GUITaskAdditions/inc -I../../../STM32CubeIDE/CM7/Application/User/Calculator/inc -I../../../STM32CubeIDE/CM7/Application/User/EventManagerCM7/inc -I../../../STM32CubeIDE/CM7/Application/User/Core/Logger/inc -I../../../STM32CubeIDE/CM7/Application/User/Calc/inc -I../../../STM32CubeIDE/CM7/Application/User/Calc/tinyexpr -I../../../STM32CubeIDE/CM7/Application/User/IPCDaemonProxy/inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-Core-2f-Logger-2f-src

clean-Application-2f-User-2f-Core-2f-Logger-2f-src:
	-$(RM) ./Application/User/Core/Logger/src/Logger.cyclo ./Application/User/Core/Logger/src/Logger.d ./Application/User/Core/Logger/src/Logger.o ./Application/User/Core/Logger/src/Logger.su

.PHONY: clean-Application-2f-User-2f-Core-2f-Logger-2f-src

