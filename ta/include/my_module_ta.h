#ifndef MY_MODULE_H
#define MY_MODULE_H

// 4f465115-69c4-43b3-8a67-bbf4e58a5045
#define MY_MODULE_UUID {0x4f465115, 0x69c4, 0x43b3, \ 
    {0x8a, 0x67, 0xbb, 0xf4, 0xe5, 0x8a, 0x50, 0x45} }

/* The function IDs implemented in this TA */
#define TA_HELLO_WORLD_CMD_INC_VALUE		0
#define TA_HELLO_WORLD_CMD_DEC_VALUE		1
#define TA_HELLO_WORLD_CMD_WRITE_HW         3

#define SHARED_MEM_SIZE  64

#endif