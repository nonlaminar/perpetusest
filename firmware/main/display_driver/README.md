# Display Drivers

These display drivers have been adapted from the waveshare/e-Ink drivers. They have been slimmed down to use a specific set of drivers, and support more specific hardware as well as including some hacks and upgrades to make the eink more performant.

## Overview

### Operating principles

#### Board agnostic
Using the methodology of the waveshare library, we can make the library "board agnostic" by having a device file in the `config` folder wich will wrap the hardware calls behind generic dev functions. In the event that another board with a different hardware library is needed, we can use preprocessor macros to add support for different boards with minimal updates to the driver. 

### Raspberry Pi
- Using the WiringPI gpio library for raspberry pi, as it is most performant


## The changes
