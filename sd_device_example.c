#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <systemd/sd-device.h>

#define FOREACH_DEVICE(enumerator, device)                               \
        for (device = sd_device_enumerator_get_device_first(enumerator); \
             device;                                                     \
             device = sd_device_enumerator_get_device_next(enumerator))

int main(int argc, char *argv[]) {
    __attribute__((cleanup(sd_device_enumerator_unrefp))) sd_device_enumerator *enumerator = NULL;
    sd_device *device;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <devname>\n", argv[0]);
		exit(1);
	}

    sd_device_enumerator_new(&enumerator);
    sd_device_enumerator_allow_uninitialized(enumerator);
    sd_device_enumerator_add_match_subsystem(enumerator, "block", 1);


    FOREACH_DEVICE(enumerator, device) {
        __attribute__((cleanup(sd_device_unrefp))) sd_device *dev = NULL;
        const char *syspath, *sysname, *devname, *label, *uuid, *fstype, *partuuid, *partlabel, *partnum;
        
        sd_device_get_syspath(device, &syspath);
        sd_device_get_sysname(device, &sysname);
        sd_device_new_from_syspath(&dev, syspath);
        sd_device_get_devname(dev,&devname);

        if (!strcmp(argv[1], devname)) {
            sd_device_get_property_value(dev, "ID_FS_UUID", &uuid);
            sd_device_get_property_value(dev, "ID_FS_LABEL", &label);
            sd_device_get_property_value(dev, "ID_FS_TYPE", &fstype);            
            sd_device_get_property_value(dev, "ID_PART_ENTRY_UUID", &partuuid);
            sd_device_get_property_value(dev, "ID_PART_ENTRY_NAME", &partlabel);
            sd_device_get_property_value(dev, "ID_PART_ENTRY_NUMBER", &partnum);    

            printf("UUID\t\t: %s\n", uuid);
            printf("LABEL\t\t: %s\n", label);
            printf("FSTYPE\t\t: %s\n", fstype);
            printf("PARTLABEL\t: %s\n", partlabel);
            printf("PARTUUID\t: %s\n", partuuid);
            printf("PARTNUM\t\t: %s\n", partnum);
            printf("DEVNAME\t\t: %s\n", devname);    
            printf("BLOCKDEVICE\t: %s\n", sysname);         
        }
 
        dev = sd_device_unref(dev);
    }

    return 0;
}