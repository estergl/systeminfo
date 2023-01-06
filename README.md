# systeminfo
The file system and partition information (LABEL, UUID, TYPE, PARTUUID, PARTLABEL, PARTNUM)
are printed to the console. This was implemented using libblkid and libudev APIs.


## build
```shell
make
```

## run
```shell
./udev_example /dev/sdaX
```

```shell
sudo ./blkid_example /dev/sdaX
```

[![asciicast](https://asciinema.org/a/dc9LIP4e3gRkMoTDPzu5vj8lq.svg)](https://asciinema.org/a/dc9LIP4e3gRkMoTDPzu5vj8lq)

## useful links

* https://github.com/util-linux/util-linux/blob/master/libblkid/src/superblocks/superblocks.c
* https://github.com/util-linux/util-linux/blob/master/libblkid/src/partitions/partitions.c
* https://github.com/util-linux/util-linux/blob/master/misc-utils/lsblk-properties.c
* https://github.com/systemd/systemd/blob/main/src/libudev/test-libudev.c
