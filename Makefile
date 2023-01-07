CC = gcc
CFLAGS = -g -Wall

examples:
	$(CC) $(CFLAGS) -o udev_example udev_example.c -ludev
	$(CC) $(CFLAGS) -o blkid_example blkid_example.c -lblkid	
	$(CC) $(CFLAGS) -o sd_device_example sd_device_example.c -lsystemd	

default: examples

all: default

clean:
	rm -f udev_example blkid_example sd_device_example