################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Application/User/EventManagerCM4/src/EventBusManager.cpp \
../Application/User/EventManagerCM4/src/EventManagerCM4Task.cpp 

OBJS += \
./Application/User/EventManagerCM4/src/EventBusManager.o \
./Application/User/EventManagerCM4/src/EventManagerCM4Task.o 

CPP_DEPS += \
./Application/User/EventManagerCM4/src/EventBusManager.d \
./Application/User/EventManagerCM4/src/EventManagerCM4Task.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/EventManagerCM4/src/%.o Application/User/EventManagerCM4/src/%.su Application/User/EventManagerCM4/src/%.cyclo: ../Application/User/EventManagerCM4/src/%.cpp Application/User/EventManagerCM4/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../../CM4/Core/Inc -I../../../STM32CubeIDE/CM4/Application/User/EdiabasDaemonNative/inc -I../../../STM32CubeIDE/CM4/Application/User/EventManagerCM4/inc -I../../../Common/Inc -I../../../Drivers/BSP/Components -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-EventManagerCM4-2f-src

clean-Application-2f-User-2f-EventManagerCM4-2f-src:
	-$(RM) ./Application/User/EventManagerCM4/src/EventBusManager.cyclo ./Application/User/EventManagerCM4/src/EventBusManager.d ./Application/User/EventManagerCM4/src/EventBusManager.o ./Application/User/EventManagerCM4/src/EventBusManager.su ./Application/User/EventManagerCM4/src/EventManagerCM4Task.cyclo ./Application/User/EventManagerCM4/src/EventManagerCM4Task.d ./Application/User/EventManagerCM4/src/EventManagerCM4Task.o ./Application/User/EventManagerCM4/src/EventManagerCM4Task.su

.PHONY: clean-Application-2f-User-2f-EventManagerCM4-2f-src

