/*
 * include/linux/spi/spidev.h
 *
 * Copyright (C) 2006 SWAPP
 *	Andrea Paterniani <a.paterniani@swapp-eng.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  */

#ifndef __FP_DRV_H
#define __FP_DRV_H

#include <linux/types.h>

///////////////////////////////////////////////////////////////////////////////////////////

extern int full_fp_chip_name(const char *name);
extern int full_fp_chip_info(const char *name);
extern int read_fpId_pin_value(struct device *dev, char *label);
///////////////////////////////////////////////////////////////////////////////////////////

#define LOG_TAG  "[fingerprint][fp_drv]:" 
#define __FUN(f)   printk(KERN_ERR LOG_TAG "~~~~~~~~~~~~ %s ~~~~~~~~~\n", __FUNCTION__)
#define klog(fmt, args...)    printk(KERN_ERR LOG_TAG fmt, ##args)


#define __HIGH   (1) 
#define __LOW   (2)
#define __HIGH_IMPEDANCE   (3) 
///////////////////////////////////////////////////////////////////////////////////////////

#define ELAN_EVT_LEFT          301
#define ELAN_EVT_RIGHT        302
#define ELAN_EVT_UP              303
#define ELAN_EVT_DOWN        304

#define HW_EVT_DOWN                1
#define HW_EVT_UP                     2
#define HW_EVT_MOVE_UP          3
#define HW_EVT_MOVE_DOWN     4
#define HW_EVT_MOVE_LEFT       5
#define HW_EVT_MOVE_RIGHT     6


#endif /* __FP_DRV_H */
