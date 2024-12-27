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

- STM32F407VET6/VGT6 Development Board. About $10 for AliExpress/eBay.   
- LAN8720 PHY module. About $2 for AliExpress/eBay.   
- STM32 NUCLEO Board. Because we need STLink V2.1 for farmware writting.   

- Board parts number is DIYMORE STM32F407VGT.   
![LAN8720-2](https://user-images.githubusercontent.com/6020549/62419501-80a64d00-b6bc-11e9-9cc1-9293446bec45.JPG)

- Board parts number is Black F407VE.    
![LAN8720-11](https://user-images.githubusercontent.com/6020549/62419879-2362c980-b6c5-11e9-8bd9-0fc0ef1444b0.JPG)

- Board parts number is Black F407VE.    
![LAN8720-21](https://user-images.githubusercontent.com/6020549/62815224-ca29e880-bb51-11e9-9197-a6f8a1870501.JPG)

# Software requirements
- Arduino IDE   
 I used V1.8.19   
 ![ArduinoIDE_V1 8 19](https://github.com/nopnop2002/Arduino-STM32-Ethernet-LAN8720/assets/6020549/d0211c18-b77e-436f-b931-8a9759f833eb)

- Arduino core support for STM32 based boards.   
 https://github.com/stm32duino/Arduino_Core_STM32   
 ___Note for Core version___   
 Requires core version 2.3 or higher.   
 Core version 2.8 or later requires Arduino IDE 2.x.   
 I used version 2.7.1.   
 This is the final version available for Arduino IDE 1.x.   
 ![STM32_Core_2 7 1](https://github.com/user-attachments/assets/e135c6b2-61f5-4ac8-a4e5-8878a29a2be4)

- Arduino library to support Ethernet for STM32 based board.   
 https://github.com/stm32duino/STM32Ethernet   
 ___Note for library name___   
 When installed using the ZIP file, it will be ```STM32Ethernet```.   
 When installed using the Library Manager, it will be ```STM32duino_STM32Ethernet```.   
 Installing using the ZIP file and then updating using the Library Manager will result in duplicate libraries.   

- Lightweight TCP/IP stack (LwIP) is a small independent implementation of the TCP/IP protocol suite.   
 https://github.com/stm32duino/LwIP   
 ![Arduino-STM32-Ethernet-LAN8720](https://user-images.githubusercontent.com/6020549/231913627-4294b712-bde5-4735-8569-46199f85e8d9.jpg)

# Library modify
You have to modify your local file.   
The file location is ```C:\Users\user\AppData\Local\Arduino15\packages\STMicroelectronics\hardware\stm32\2.7.1\system\STM32F4xx```.   
___The file location may vary depending on core library version.___   
Please look for ```stm32f4xx_hal_conf_default.h```.   

[stm32f4xx_hal_conf_default.h](https://github.com/stm32duino/Arduino_Core_STM32/blob/85fd492c15a87048086e7e82318c555fb6410a41/system/STM32F4xx/stm32f4xx_hal_conf_default.h#L233-L273)

- Before change
```
/* Section 2: PHY configuration section */
/* LAN8742A PHY Address*/
#define LAN8742A_PHY_ADDRESS            0x00U
```

- After change
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

STM32 NUCLEO provides a on-board STLINK-V2.1 USB interface.   
Actually the board has two STM32 chips. One is the target of the demo board, and the other is the STLINK.   
This STLINK-V2.1 can be used as programmer and debugger for external targets.   
Remove the two jumpers from CN2, and place them at the two outermost empty jumper pins marked CN11 and CN12.   
SWD port pinmap for using the Nucleo as a programming device can be found [here](https://os.mbed.com/questions/7974/F401RE-Cut-off-ST-LINK/).   

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

__Please note the board part number.__   
Some boards do not support Ethernet.   

![LAN8720-Arduino](https://user-images.githubusercontent.com/6020549/113068815-07952b00-91fa-11eb-8e88-1f7aa6a9b79a.JPG)

# Serial monitor

You need to use a USB-TTL converter to display the serial output.   

|USB-TTL|---|STM32F407|
|:-:|:-:|:-:|
|TX|---|PA_9|
|GND|---|GND|

___Note for Virtual COM port___   
STM32F407 can use a Virtual COM port.   
If you enable Virtual COM port, you can use the USB port as a serial port.   
But Ethernet and Virtual COM port cannot be used at the same time.   
Something is probably in conflict.   
Don't use USB Support.   

# Other examples
There is other examples provided by STM.   
To use these you need to add hal_conf_extra.h.   
https://github.com/stm32duino/STM32Ethernet/tree/main/examples
