# STM32Ethernet_LAN8720
STM32 Ethernet supports only LAN8742A.   
Other PHYs are not supported.   
But with a little change, You can use LAN8720 PHY with STM32F407.

__Note__   
STM is currently rebuilding Ethernet support with an external PYH.
In the future, Ethernet support using an external PYH will change significantly and may be supported differently.
This is an interim action until officially supported.

# Hardware requirements

STM32F407 Development Board.   
LAN8720 PHY module.   
STM32 NUCLEO Board. Because we need STLink V2.1.   

# Software requirements

STM32 core support for Arduino.   
https://github.com/stm32duino/Arduino_Core_STM32

Arduino library to support Ethernet for STM32 based board.   
https://github.com/stm32duino/STM32Ethernet

Lightweight TCP/IP stack (LwIP) is a small independent implementation of the TCP/IP protocol suite.   
https://github.com/stm32duino/LwIP

# Library update
You have to update your local file.   
File location is Documents\ArduinoData\packages\STM32\hardware\stm32\1.6.1\system\STM32F4xx.

[stm32f4xx_hal_conf_default.h](https://github.com/stm32duino/Arduino_Core_STM32/blob/85fd492c15a87048086e7e82318c555fb6410a41/system/STM32F4xx/stm32f4xx_hal_conf_default.h#L233-L273)

```
/* Section 2: PHY configuration section */
#if !defined  (LAN8742A_PHY_ADDRESS)
/* LAN8742A PHY Address*/
#define LAN8742A_PHY_ADDRESS            0x00U
#endif
```

# Wireing

|LAN8720 module|---|STM32F407|
|:-:|:-:|:-:|
|TX1|---|PB_13|
|TX_EN|---|PC_11|
|TX0|---|PC_12|
|RX0|---|PC_4|
|RX1|---|PC_5|
|nINT/RETCLK|---|PA_1|
|CRS|---|PA_7|
|MDIO|---|PA_2|
|MDC|---|PC_1|
|GND|---|GND|
|VCC|---|+3.3V|

# How to flash

STM32 NUCLEO provides a on-board STLINK-V2-1 USB interface.
Actually the board has two STM32 chips. One is the target of the demo board, and the other is the STLINK.
This STLINK-V2-1 can be used as programmer and debugger for external targets.
Remove the two jumpers from CN2, and place them at the two outermost empty jumper pins marked CN11 and CN12.

Connect the NUCLEO SWD port to the STM32F407.   

|NUCLEO SWD|---|STM32F407|
|:-:|:-:|:-:|
|SWDIO|---|PA_13|
|SWCLK|---|PA_14|
|GND|---|GND|
|+3.3V|---|+3.3V|

# Serial monitor

You need to use a USB-TTL converter to display the serial output.   

|USB-TTL|---|STM32F407|
|:-:|:-:|:-:|
|TX|---|PA_9|
|GND|---|GND|

