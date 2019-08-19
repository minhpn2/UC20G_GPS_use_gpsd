# UC20G_GPS_use_gpsd
This project uses GPSD for detect signal from GPS module.

1. Configure GPS port before run this program.
List USB attributes for the device:
- udevadm info --name=/dev/ttyUSB_DATA --attribute-walk
- udevadm info --name=/dev/ttyUSB_COMMAND --attribute-walk
--> ttyUSB_DATA: this port is used get data GPS from GPS Module, it corressponds to ttyUSB* port data.
Note: Port ttyUSB* which contain data GPS can be found by using command: ls -l /dev/ttyUSB*
crw-rw---- 1 root dialout 188, 0 Aug 15 01:17 /dev/ttyUSB0
crw-rw---- 1 root dialout 188, 1 Aug 15 01:19 /dev/ttyUSB1
crw-rw---- 1 root dialout 188, 2 Aug 15 01:19 /dev/ttyUSB2
crw-rw---- 1 root dialout 188, 3 Aug 15 02:07 /dev/ttyUSB3
crw-rw---- 1 root dialout 188, 4 Aug 15 01:19 /dev/ttyUSB4
--> Check one by one port for detect which port contain data and send command AT.
2. In the attribute list you should look for an attribute that is unique for a device (In this project: KERNELS ATTRS{bNumEndpoints})
- Create (or open) a file /etc/udev/rules.d/01-usb-serial.rules
- If you found an unique attribute of the device then write a following line in the rules file:
KERNELS=="1-1.3:1.1", ATTRS{bNumEndpoints}=="02", SYMLINK+="ttyUSB_UC20_DATA"
KERNELS=="1-1.3:1.2", ATTRS{bNumEndpoints}=="02", SYMLINK+="ttyUSB_UC20_COMMAND"
3. Reload your udevadm rules:
- udevadm control --reload-rules

Now, check your port again: ls -l /dev/ttyUSB*
crw-rw---- 1 root dialout 188, 0 Aug 15 01:17 /dev/ttyUSB0
crw-rw---- 1 root dialout 188, 1 Aug 15 01:19 /dev/ttyUSB1
crw-rw---- 1 root dialout 188, 2 Aug 15 02:27 /dev/ttyUSB2
crw-rw---- 1 root dialout 188, 3 Aug 15 02:28 /dev/ttyUSB3
crw-rw---- 1 root dialout 188, 4 Aug 15 01:19 /dev/ttyUSB4
lrwxrwxrwx 1 root root         7 Aug 15 01:19 /dev/ttyUSB_UC20_COMMAND -> ttyUSB3
lrwxrwxrwx 1 root root         7 Aug 15 01:19 /dev/ttyUSB_UC20_DATA -> ttyUSB2

All Done!
