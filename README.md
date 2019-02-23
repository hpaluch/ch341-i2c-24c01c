# How to access I2C EEPROM 24C01C from LC CH341 USB board

Here is example how to access I2C EEPROM [24C01C]:
from [CH341A USB to UART/IIC/SPI/TTL/ISP adapter EPP/MEM Parallel converter]
using SPI mode.

> WARNING!
>
> This program will overwrite all contents of your 24C01C EEPROM!
>
> The example program does following:
> 1. Write sample string to EEPROM
> 1. Read back all EEPROM contents and comparing sample string
> 1. Write erase pattern to whole EEPROM
> 1. Read back whole EEPROM and verify contents

Circuit schematic is below:

![Schematic of I2C w 24C01C](https://github.com/hpaluch/ch341-i2c-24c01c/blob/master/ExpressPCB/ch341a-i2c-24c01c.png?raw=true)


Hardware:
* you
  need [CH341A USB to UART/IIC/SPI/TTL/ISP adapter EPP/MEM Parallel converter]
  I got my from Amazon.de as [DollaTek CH341A USB zu UART/IIC/SPI/TTL/ISP Adapter EPP/MEM Parallelwandler]. If you never used this module
  please see my article [Getting started with LC CH341A USB conversion module]
* EEPROM [24C01C] - I bought one on [tme.eu - 24C01C], 100nF ceramic capacitor.

> !!!IMPORTANT WARNING!!!
>
> Do NOT use original [Atmel 24C01]! It uses **different protocol**
> (the 1st byte which should be use for slave type/address is (mis)used
> for EEPROM data address. The official CH341A library routines
> will work with 24C01A or later revisions only!!!

# Setup

The CH341A adapter must be setup following way:
* jumper set to `I2C/SPI` mode
* voltage set to 5V TTL logic - the only voltage supported by [24C01C]

Software setup:
*  Download and install [CH341PAR.ZIP] - USB driver for CH341 chip
   in Parallel mode (EPP, MEM). This driver is valid 
   also for **I2C mode and SPI mode** (yes - even when it is marked _parallel_).
*  install VisualSutdio 2010

Create environment variable `CH341_SDK` that should point to extracted
`CH341PAR.ZIP` header and library files. For example
if you have extracted file:

```
C:\CH341_DRIVER\LIB\C\CH341DLL.H 
```
Then your `CH341_SDK` should be set to `C:\CH341_DRIVER\LIB\C`.

Open and rebuild solution `VS2010/ch341_i2c_24c01c/ch341_i2c_24c01c.sln`
in VisualStudio 2010. There should be no errors.

Connect your `CH341A USB module` to target circuit. Following pins are used:

|PIN Name|Direction|Description|
|--------|---------|-----------|
|GND|N/A|Common ground|
|VCC|N/A|5V supply|
|SDA|Open Drain| I2C Data|
|SCL|Open Drain| I2C Clock|
----


Connect your `CH341 USB module` to your PC. There should
be permanently lighting red LED on USB module.

Now you can run sample program - you should see following output:
```
ch341_i2c_24c01c.exe

CH341 library version: 33
CH341 driver  version: 0
Opening device# 0
Storing string 'Hello!' (including '\0') at address 0x5...
Fetching whole memory content...
Dumping EEPROM contents...
Dump of buffer at 0x00417140,  bytes 128

0x0000 ff ff ff ff ff 48 65 6c 6c 6f 21 00 ff ff ff ff .....Hello!.....
0x0010 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0020 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0030 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0040 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0050 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0060 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0070 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
Cleaning EEPROM contents with 0xff...
Fetching whole memory content...
Dumping EEPROM contents...
Dump of buffer at 0x00417140,  bytes 128

0x0000 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0010 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0020 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0030 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0040 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0050 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0060 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ................
0x0070 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
```



[Atmel 24C01]: https://dflund.se/~triad/krad/entrega/at24c01.pdf
[tme.eu - 24C01C]: https://www.tme.eu/en/details/24c01c-i_p/serial-eeprom-memories-integ-circ/microchip-technology/
[24C01C]: http://ww1.microchip.com/downloads/en/devicedoc/21201k.pdf
[CH341A USB to UART/IIC/SPI/TTL/ISP adapter EPP/MEM Parallel converter]:http://www.chinalctech.com/index.php?_m=mod_product&_a=view&p_id=1220
[DollaTek CH341A USB zu UART/IIC/SPI/TTL/ISP Adapter EPP/MEM Parallelwandler]:https://www.amazon.de/gp/product/B07DJZDRKG/
[Getting started with LC CH341A USB conversion module]:  https://github.com/hpaluch/hpaluch.github.io/wiki/Getting-started-with-LC-CH341A-USB-conversion-module
[CH341PAR.ZIP]: http://www.wch.cn/downloads/file/7.html
