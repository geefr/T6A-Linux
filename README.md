# T6a-Linux
Linux utilities for HK-T6A and compatible transmitters

# libt6a
Library for interfacing with the T6A via the trainer port.

Mostly working
- Reading of channel state

Not started/Not working
- Reading of configuration
- Writing of configration

# t6aJoy
This program is a userspace joystick driver.
The transmitter will be exposed as a regular Linux driver, allowing use in simulators that don't support the T6A.

Tested simulators:
-crrcsim - Has native T6A support, but works with either
-flightgear - Working. Config isn't easy due to current state of driver
-All other simulators will work assuming they can read joystick input on Linux.

This driver will not change the transmitter settings in any way. If a channel is inverted for instance it will appear that way to the simulator

Tricks to get it working
-Plug in transmitter and check dmesg for /dev/ttyUSB0. If it's called something else t6aJoy will need to be modified.
-Ensure you have permissions to access /dev/ttyUSB0. The simplest way is to 'sudo chown yourname:yourname /dev/ttyUSB0' but updating udev would be better
-Ensure uinput is installed
-Ensure you have permissions to access /dev/uinput
-run t6aJoy
-If anything stops working kill t6aJoy and start again

# Links and resources
Excellent python management utility and description of serial protocol
https://github.com/yschaeff/HK-T6Av2

