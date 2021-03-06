/*
 * scull.h -- definitions for the char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: scull.h,v 1.15 2004/11/04 17:51:18 rubini Exp $
 *
 * Modified by Keith Shomper, 10/27/2017 for use in CS3320
 *
 */

#ifndef _SCULL_H_
#define _SCULL_H_

#include <linux/ioctl.h>    /* needed for the _IOW etc stuff used later */

#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0       /* dynamic major by default */
#endif

#ifndef SCULL_NR_DEVS       /* original code used four devices scull0 through */
#define SCULL_NR_DEVS 1     /* scull3; example simplified to single device    */
#endif

/*
 * The bare device is a variable-length region of memory.
 * Use a linked list of indirect blocks.
 *
 * "scull_dev->data" points to an array of pointers, each
 * pointer refers to a memory area of SCULL_QUANTUM bytes.
 *
 * The array (quantum-set) is SCULL_QSET long.
 */
#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET    1000
#endif

/*
 * Representation of scull quantum sets.
 */
struct scull_qset {
   void              **data;      /* an array or pointers to bytes    */
   struct scull_qset  *next;      /* the next qset in the linked-list */
};

struct scull_dev {
   struct scull_qset  *data;      /* Pointer to first quantum set     */
   int                 quantum;   /* the current quantum size         */
   int                 qset;      /* the current array size           */
   unsigned long       size;      /* amount of data stored here       */
   struct semaphore    sem;       /* mutual exclusion semaphore       */
   struct cdev         cdev;       /* Char device structure             */
};

/*
 * The different configurable parameters
 */
extern int scull_major;
extern int scull_nr_devs;
extern int scull_quantum;
extern int scull_qset;

/*
 * Prototypes for shared functions
 */
ssize_t scull_read  (struct file *filp, char __user *buf, size_t count,
                     loff_t *f_pos);
ssize_t scull_write (struct file *filp, const char __user *buf, size_t count,
                     loff_t *f_pos);
loff_t  scull_llseek(struct file *filp, loff_t off, int whence);
long    scull_ioctl (struct file *filp, unsigned int cmd, unsigned long arg);


/*
 * Ioctl definitions
 */

/* Use 'k' as magic number - please use a different 8-bit number in your code */
#define SCULL_IOC_MAGIC  'k'
#define SCULL_IOCRESET    _IO(SCULL_IOC_MAGIC,     0)

/*
 * S means "Set"       through a ptr,
 * T means "Tell"      directly with the argument value
 * G means "Get":      reply by setting through a pointer
 * Q means "Query":    response is on the return value
 * X means "eXchange": switch G and S atomically
 * H means "sHift":    switch T and Q atomically
 */
#define SCULL_IOCSQUANTUM _IOW (SCULL_IOC_MAGIC,   1, int)
#define SCULL_IOCSQSET    _IOW (SCULL_IOC_MAGIC,   2, int)
#define SCULL_IOCTQUANTUM _IO  (SCULL_IOC_MAGIC,   3     )
#define SCULL_IOCTQSET    _IO  (SCULL_IOC_MAGIC,   4     )
#define SCULL_IOCGQUANTUM _IOR (SCULL_IOC_MAGIC,   5, int)
#define SCULL_IOCGQSET    _IOR (SCULL_IOC_MAGIC,   6, int)
#define SCULL_IOCQQUANTUM _IO  (SCULL_IOC_MAGIC,   7     )
#define SCULL_IOCQQSET    _IO  (SCULL_IOC_MAGIC,   8     )
#define SCULL_IOCXQUANTUM _IOWR(SCULL_IOC_MAGIC,   9, int)
#define SCULL_IOCXQSET    _IOWR(SCULL_IOC_MAGIC,  10, int)
#define SCULL_IOCHQUANTUM _IO  (SCULL_IOC_MAGIC,  11     )
#define SCULL_IOCHQSET    _IO  (SCULL_IOC_MAGIC,  12     )
#define SCULL_IOC_MAXNR                           12

#endif /* _SCULL_H_ */
