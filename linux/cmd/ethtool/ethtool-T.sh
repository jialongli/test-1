#!/bin/bash

#  ethtool -T|--show-time-stamping devname
sudo ethtool -T eth0
#Time stamping parameters for eth0:
#Capabilities:
#	software-transmit     (SOF_TIMESTAMPING_TX_SOFTWARE)
#	software-receive      (SOF_TIMESTAMPING_RX_SOFTWARE)
#	software-system-clock (SOF_TIMESTAMPING_SOFTWARE)
#PTP Hardware Clock: none
#Hardware Transmit Timestamp Modes: none
#Hardware Receive Filter Modes: none
