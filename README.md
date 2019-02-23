# How to access I2C EEPROM 24C01C from LC CH341 USB board

Here is example how to access I2C EEPROM [24C01C]:
from [CH341A USB to UART/IIC/SPI/TTL/ISP adapter EPP/MEM Parallel converter]
using SPI mode.

> WARNING!
>
> Work in progress....

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
> will work with 24C01A or later revisions!!!

# Setup

The CH341A adapter must be setup following way:
* jumper set to `I2C/SPI` mode
* voltage set to 5V TTL logic - the only voltage supported by [24C01C]



[Atmel 24C01]: https://dflund.se/~triad/krad/entrega/at24c01.pdf
[tme.eu - 24C01C]: https://www.tme.eu/en/details/24c01c-i_p/serial-eeprom-memories-integ-circ/microchip-technology/
[24C01C]: http://ww1.microchip.com/downloads/en/devicedoc/21201k.pdf
[CH341A USB to UART/IIC/SPI/TTL/ISP adapter EPP/MEM Parallel converter]:http://www.chinalctech.com/index.php?_m=mod_product&_a=view&p_id=1220
[DollaTek CH341A USB zu UART/IIC/SPI/TTL/ISP Adapter EPP/MEM Parallelwandler]:https://www.amazon.de/gp/product/B07DJZDRKG/

