InfiniBand（直译为“无限带宽”技术，缩写为IB）是一个用于高性能计算的计算机网络通信标准，它具有极高的吞吐量和极低的延迟，用于计算机与计算机之间的数据互连。InfiniBand也用作服务器与存储系统之间的直接或交换互连，以及存储系统之间的互连。
InfiniBand技术不是用于一般网络连接的，它的主要设计目的是针对服务器端的连接问题的。因此，InfiniBand技术将会被应用于服务器与服务器（比如复制，分布式工作等），服务器和存储设备（比如SAN和直接存储附件）以及服务器和网络之间（比如LAN， WANs和the Internet）的通信。
与目前计算机的I/O子系统不同，InfiniBand是一个功能完善的网络通信系统。InfiniBand贸易组织把这种新的总线结构称为I/O网络，并把它比作开关，因为所给信息寻求其目的地址的路径是由控制校正信息决定的。InfiniBand使用的是网际协议版本6的128位地址空间，因此它能提供近乎无限量的设备扩展性。
通过InfiniBand传送数据时，数据是以数据包方式传输，这些数据包会组合成一条条信息。这些信息的操作方式可能是远程直接内存存取的读写程序，或者是通过信道接受发送的信息，或者是多点传送传输。就像大型机用户所熟悉的信道传输模式，所有的数据传输都是通过信道适配器来开始和结束的。每个处理器（例如个人电脑或数据中心服务器）都有一个主机通道适配器，而每个周边设备都有一个目标通道适配器。通过这些适配器交流信息可以确保在一定服务品质等级下信息能够得到有效可靠的传送。

[https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux_for_real_time/7/html/tuning_guide/infiniband]
Infiniband是一种通信体系结构，通常用于增加带宽并提供服务质量和故障转移。它也可以用于通过远程直接内存访问（RDMA）功能来改善延迟。
在Red Hat Enterprise Linux for Real Time下对Infiniband的支持与在Red Hat Enterprise Linux 7下提供的支持没有区别。

----------------------------------
* test/InfiniBand
* test/c/glibc/infiniband/
* test/linux/InfiniBand
* test/linux/RDMA [远端内存直接访问]
* 
