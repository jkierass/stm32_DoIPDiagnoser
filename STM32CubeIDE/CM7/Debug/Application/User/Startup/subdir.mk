################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/User/Startup/startup_stm32h747xihx.s 

S_DEPS += \
./Application/User/Startup/startup_stm32h747xihx.d 

OBJS += \
./Application/User/Startup/startup_stm32h747xihx.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Startup/%.o: ../Application/User/Startup/%.s Application/User/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I../../CM7/../../CM7/Middlewares/ST/touchgfx/framework/include -I../../CM7/../../CM7/TouchGFX/generated/fonts/include -I../../CM7/../../CM7/TouchGFX/generated/gui_generated/include -I../../CM7/../../CM7/TouchGFX/generated/images/include -I../../CM7/../../CM7/TouchGFX/generated/texts/include -I../../CM7/../../CM7/TouchGFX/generated/videos/include -I../../CM7/../../CM7/TouchGFX/gui/include -I"C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/STM32CubeIDE/CM7/Application/User/Calc/inc" -I"C:/Users/jakub/Desktop/Praca_inz/stm32_workspace/ediabasdiagnoser/STM32CubeIDE/CM7/Application/User/RTC/inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Application-2f-User-2f-Startup

clean-Application-2f-User-2f-Startup:
	-$(RM) ./Application/User/Startup/startup_stm32h747xihx.d ./Application/User/Startup/startup_stm32h747xihx.o

.PHONY: clean-Application-2f-User-2f-Startup

