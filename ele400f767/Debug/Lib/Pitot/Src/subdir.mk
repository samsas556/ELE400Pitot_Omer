################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/Pitot/Src/pitot.c 

OBJS += \
./Lib/Pitot/Src/pitot.o 

C_DEPS += \
./Lib/Pitot/Src/pitot.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/Pitot/Src/pitot.o: ../Lib/Pitot/Src/pitot.c Lib/Pitot/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I"/home/rockets/Documents/Code/ele400/ELE400git2/ELE400EclairDeGenie/ele400f767/Lib/Pitot/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"/home/rockets/Documents/Code/ele400/ELE400git2/ELE400EclairDeGenie/ele400f767/Lib/Client_MQTT/Inc" -I"/home/rockets/Documents/Code/ele400/ELE400git2/ELE400EclairDeGenie/ele400f767/Lib/debug_interface/Inc" -I"/home/rockets/Documents/Code/ele400/ELE400git2/ELE400EclairDeGenie/ele400f767/Lib/Node_MQTT/Inc" -I"/home/rockets/Documents/Code/ele400/ELE400git2/ELE400EclairDeGenie/ele400f767/Lib/paho_MQTT/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/Pitot/Src/pitot.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
