# STM32Ethernet_LAN8720

STM provides an Ethernet library for Arduino_Core_STM32, but it supports only LAN8742A.   
However, it is difficult to get LAN8742 PHY.   
We can get the LAN8720 PHY cheaply instead of the LAN8742 PHY.   
With a little change, You can use LAN8720 PHY with STM32F407.

__Note__   
STM is currently rebuilding external PHY Ethernet support for Arduino_Core_STM32.   
In the future, Ethernet support using external PHYs will change significantly and support may change.   
This is an interim action until officially supported.   
When STM become officially supports external PHY Ethernet, I will delete this repository.

# Hardware requirements

- STM32F407VET6/VGT6 Development Board. About $10 for aliexpress/eBay.   
- LAN8720 PHY module. About $2 for aliexpress/eBay.   
- STM32 NUCLEO Board. Because we need STLink V2.1 for farmware writting.   

![LAN8720-2](https://user-images.githubusercontent.com/6020549/62419501-80a64d00-b6bc-11e9-9cc1-9293446bec45.JPG)

![LAN8720-11](https://user-images.githubusercontent.com/6020549/62419879-2362c980-b6c5-11e9-8bd9-0fc0ef1444b0.JPG)

![LAN8720-21](https://user-images.githubusercontent.com/6020549/62815224-ca29e880-bb51-11e9-9197-a6f8a1870501.JPG)

# Software requirements

- STM32 core support for Arduino.   
https://github.com/stm32duino/Arduino_Core_STM32   
___Note for board name___   
Board name changed from ```STM32``` to ```STMicroelectronics```.   
The location of the package has changed accordingly.   
Previously  .....\packages\STM\hardware\.....   
Current     .....\packages\STMicroelectronics\hardware\.....   

- Arduino library to support Ethernet for STM32 based board.   
https://github.com/stm32duino/STM32Ethernet   
___Note for library name___   
The library name is ```STM32Ethernet``` when installed using the zip file.   
The library name is ```STM32duino_STM32Ethernet``` when installed using the library manager.   
After installing using the zip file, Updating using Library Manager will result in a duplicate library.   

- Lightweight TCP/IP stack (LwIP) is a small independent implementation of the TCP/IP protocol suite.   
https://github.com/stm32duino/LwIP   

# Library update
You have to update your local file.   
The file location is ```C:\Users\user\AppData\Local\Arduino15\packages\STMicroelectronics\hardware\stm32\2.5.0\system\STM32F4xx```.   
___The file location may vary depending on your Preferences.___   
Please look for ```stm32f4xx_hal_conf_default.h```.   

[stm32f4xx_hal_conf_default.h](https://github.com/stm32duino/Arduino_Core_STM32/blob/85fd492c15a87048086e7e82318c555fb6410a41/system/STM32F4xx/stm32f4xx_hal_conf_default.h#L233-L273)

```
/* Section 2: PHY configuration section */
#if !defined  (LAN8742A_PHY_ADDRESS)
/* LAN8742A PHY Address*/
#define LAN8742A_PHY_ADDRESS            0x00U
#endif
```

# Wireing

|LAN8720 PHY|---|STM32F407|
|:-:|:-:|:-:|
|TX1|---|PB_13|
|TX_EN|---|PB_11|
|TX0|---|PB_12|
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
|nRST|---|RESET(*1)|
|GND|---|GND|
|+3.3V|---|+3.3V|

__(*1)If a non-standard BootLoader has already been written to STM32, connection of nRTS pin and RESET pin is required.__

Choose Tool->Upload method "STM32CubeProgrammer(SWD)"

![LAN8720-Arduino](https://user-images.githubusercontent.com/6020549/113068815-07952b00-91fa-11eb-8e88-1f7aa6a9b79a.JPG)

# Serial monitor

You need to use a USB-TTL converter to display the serial output.   

|USB-TTL|---|STM32F407|
|:-:|:-:|:-:|
|TX|---|PA_9|
|GND|---|GND|

# Other examples
There is other examples provided by STM.   
https://github.com/stm32duino/STM32Ethernet/tree/main/examples
