What is SCSI (Small Computer System Interface)

对于短小型计算机系统接口，SCSI发音为“ Scuzzy ”，是这是第一次在完成磁盘驱动器的接口1982年。与竞争标准不同，SCSI能够支持8个设备或16个具有Wide SCSI的设备。但是，使用ID号为07的SCSI主机适配器并从ID 00引导。这将保留六个设备连接的可用性。下图是带有内部和外部连接的SCSI适配器扩展卡的示例。一旦安装在计算机中，此适配器将允许在计算机中安装多个SCSI设备。更高级的主板上也可能有可用的SCSI连接。

小型计算机系统接口（SCSI，Small Computer System Interface）是一种用于计算机及其周边设备之间（硬盘、软驱、光驱、打印机、扫描仪等）系统级接口的独立处理器标准。SCSI标准定义命令、通信协议以及实体的电气特性（换成OSI的说法，就是占据物理层、链接层、套接层、应用层），最大部分的应用是在存储设备上（例如硬盘、磁带机）；但，其实SCSI可以连接的设备包括有扫描仪、光学设备（像CD、DVD）、打印机……等等，SCSI命令中有条列出支持的设备SCSI周边设备。理论上，SCSI不可能连接所有的设备，所以有“1Fh - unknown or no device type”这个参数存在。

--------------------------------------------------
* test/linux/SCSI
* test/linux/cmd/lsscsi
