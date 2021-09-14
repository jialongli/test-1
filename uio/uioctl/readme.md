
                                _            _   _ 
                          _   _(_) ___   ___| |_| |
                         | | | | |/ _ \ / __| __| |
                         | |_| | | (_) | (__| |_| |
                          \__,_|_|\___/ \___|\__|_|
 

trivial utility for manipulating simple Linux UIO devices ("Userspace I/O").
eg, reading and writing bytes to memory mapped devices or monitoring
interrupts.
can list all devices and their mappings.
little-endian by default.

-------------------------------------------------------------------------------

gcc uioctl.c -o uioctl

-------------------------------------------------------------------------------

./uioctl -h
little-endian by default
    -w  select word width (default: 4)
    -s  size of mmap (default: base + length, rounded up to PAGE_SIZE)

./uioctl /dev/uio0 0 -n 100 -w 1 -r 0
./uioctl /dev/uio0 0x818 0
./uioctl /dev/uio0 -m
./uioctl -l

BUT WAIT?  hexdump -C /dev/uio0 -s 0x

-------------------------------------------------------------------------------

Userspace I/O drivers in a realtime context
https://www.osadl.org/fileadmin/dam/rtlws/12/Koch.pdf

UIO: user-space drivers (2007)
https://lwn.net/Articles/232575/

The Userspace I/O HOWTO (2006-2009)
https://www.kernel.org/doc/htmldocs/uio-howto/

Simple userland drivers for FPGA interfaces (eg, AXI)
http://svenand.blogdrive.com/archive/150.html

devmem2.c: Simple program to read/write from/to any location in memory
http://sources.buildroot.net/devmem2.c

pydevmem
https://github.com/kylemanna/pydevmem

