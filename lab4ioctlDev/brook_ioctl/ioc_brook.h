#ifndef IOC_BROOK_H
#define IOC_BROOK_H

#define BROOK_IOC_MAGIC     'k'
#define BROOK_IOCSETNUM     _IOW(BROOK_IOC_MAGIC,  1, int)
#define BROOK_IOCGETNUM     _IOR(BROOK_IOC_MAGIC,  2, int)
#define BROOK_IOCXNUM       _IOWR(BROOK_IOC_MAGIC, 3, int)
#define BROOK_IOC_MAXNR     3

#endif
