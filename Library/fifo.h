#ifndef __FIFO_H__
#define __FIFO_H__

int fifo_init(char *buffer, int len);
int fifo_write(int handle, char *data, int len);
int fifo_read(int handle, char *data, int len);
int fifo_clean(int handle);
int fifo_available(int handle);

#endif//__FIFO_H__
