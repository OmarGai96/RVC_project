# STM32F429-DISCO

## 1. Introduction

The STM32F429I-DISCOVERY development board uses the high-performance MCU STM32F429 and can be used to implement advanced graphical user interfaces.
This BSP is compatible with DISCOVERY development boards of STM32F429I-DISCO and STM32F429I-DISC1.

The main resources onboard the core board are as follows: 
| hardware | description |
| -- | -- |
|chip model| STM32F429ZIT6 |
|CPU| ARM Cortex-M4F |
|main frequency| 180MHz |
|On-chip SRAM| 256KB |
|oof-chip SDRAM| 64-Mbit SDRAM |
|On-chip Flash| 2MB |
|display screen| 2.4" QVGA TFT LCD|
|Motion Sensors| L3GD20 |
|LED| 6 |
|button| 2 |

## 2.Compilation instructions

The STM32F429-DISCO board package supports MDK4, MDK5, IAR development environment and GCC compiler. The following is the specific version information: 

| IDE/COMPILER | tested version               |
| ------------ | ---------------------------- |
| MDK4         | MDK4.74                      |
| MDK5         | MDK V5.25.2.0                |
| IAR          | not tested                   |
| GCC          | GCC 5.4.1 20160919 (release) |

## 3.Programming and execution

Power supply mode: use the Mini USB cable to connect the USB ST-LINK port of the computer and the board.

Serial port connection: The onboard debugger of the STM32F429I-DISC1 version of the development board is ST-LINK/V2-B, which supports the VCP virtual serial port, so you can directly use the virtual serial port to output the default console device USART1.

The old version of the STM32F429I-DISCO development board needs to connect the serial port from the pin on the board, and change the console device to the corresponding USART.

Use the terminal software on the PC to connect to the corresponding COM device in the configuration of 115200-N-8-1.

STM32F429-DISCO/DISC1 uses the on-board programmer to download the program.

### 3.1 Running results

If the compilation and programming are correct, after resetting the device, you will see the RT-Thread startup logo information on the serial port: 

```bash
 \ | /
- RT -     Thread Operating System
 / | \     3.1.0 build Aug 31 2018
 2006 - 2018 Copyright by rt-thread team
finsh />

```

## 4. Driver support status and plans 

| drive  | Support  | Remark |
| ------ | ----  | :------:  |
| USART | support | USART1/2/3 |
| GPIO | support |  |
| SPI |  |  |
| IIC |  |  |
| SDRAM |  | |
| LCD |  |  |
| touch screen |  |  |
| USB OTG Host|  |  |
| USB OTG Device|  | |
| L3GD20 |  | |


## 5. Reference 

* [board related information](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f429idiscovery.html)
