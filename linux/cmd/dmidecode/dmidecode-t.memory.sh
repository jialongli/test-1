#!/bin/bash
dmidecode -t memory
## dmidecode 3.2
#Getting SMBIOS data from sysfs.
#SMBIOS 2.8 present.
#
#Handle 0x1000, DMI type 16, 23 bytes
#Physical Memory Array
#	Location: Other
#	Use: System Memory
#	Error Correction Type: Multi-bit ECC
#	Maximum Capacity: 8 GB
#	Error Information Handle: Not Provided
#	Number Of Devices: 1
#
#Handle 0x1100, DMI type 17, 40 bytes
#Memory Device
#	Array Handle: 0x1000
#	Error Information Handle: Not Provided
#	Total Width: Unknown
#	Data Width: Unknown
#	Size: 8192 MB
#	Form Factor: DIMM
#	Set: None
#	Locator: DIMM 0
#	Bank Locator: Not Specified
#	Type: RAM
#	Type Detail: Other
#	Speed: Unknown
#	Manufacturer: QEMU
#	Serial Number: Not Specified
#	Asset Tag: Not Specified
#	Part Number: Not Specified
#	Rank: Unknown
#	Configured Memory Speed: Unknown
#	Minimum Voltage: Unknown
#	Maximum Voltage: Unknown
#	Configured Voltage: Unknown
#
