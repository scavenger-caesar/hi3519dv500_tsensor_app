#ifndef __TSENSOR_H
#define __TSENSOR_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define TSENSOR_MAGIC               'g'

/* ioctl numbers for /dev/ot_tsensor */
#define GET_TSENSOR_EN              _IOR(TSENSOR_MAGIC, 0, __u8)
#define SET_TSENSOR_EN              _IOW(TSENSOR_MAGIC, 1, __u8)
#define GET_TSENSOR_MONITOR_EN      _IOR(TSENSOR_MAGIC, 2, __u8)
#define SET_TSENSOR_MONITOR_EN      _IOW(TSENSOR_MAGIC, 3, __u8)


#define TSENSOR_EN                  BIT(31)
#define TSENSOR_MONITOR_EN          BIT(30)

#endif /* __TSENSOR_H */