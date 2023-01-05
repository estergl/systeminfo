#include <libudev.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

/***
 * http://cholla.mmto.org/computers/usb/OLD/tutorial_usbloger.html
 * https://github.com/robertalks/udev-examples
 * https://gist.github.com/VJSchneid/486c4d4b27b4474b37e3c9b1ddd557af
 * 
*/

int main (int argc, char *argv[])
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;

	/* verify that an argument exist */
	if (argc != 2) {
        fprintf(stderr, "Usage: %s <devname>\n", argv[0]);
        exit(1);
    }
	
	/* create the udev object */
	udev = udev_new();
	if (!udev) {
		fprintf(stderr, "Can't create udev\n");
		exit(1);
	}

	/* create enumerate object */	
	enumerate = udev_enumerate_new(udev);
	if (!enumerate) {
		fprintf(stderr, "Cannot create enumerate.\n");
		exit(1);
	}

	udev_enumerate_add_match_subsystem(enumerate, "block");
	udev_enumerate_scan_devices(enumerate);
	
	/* create a list of the devices */
	devices = udev_enumerate_get_list_entry(enumerate);
	if (!devices) {
		fprintf(stderr, "Failed to get device list.\n");
		exit(1);
	}

	/* For each item enumerated, print out its information */
	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *path;
		/* Get the filename of the /sys entry for the device
		   and create a udev_device object (dev) representing it */
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);
		const char *devnode = udev_device_get_devnode(dev);

		/* check if given argument match with devnode */
		if(!strcmp(argv[1], devnode)) {
			printf("UUID\t\t: %s\n", udev_device_get_property_value(dev, "ID_FS_UUID"));
			printf("LABEL\t\t: %s\n", udev_device_get_property_value(dev, "ID_FS_LABEL"));
			printf("FSTYPE\t\t: %s\n", udev_device_get_property_value(dev, "ID_FS_TYPE"));			
			printf("PARTUUID\t: %s\n", udev_device_get_property_value(dev, "ID_PART_ENTRY_UUID"));
			printf("PARTLABEL\t: %s\n", udev_device_get_property_value(dev, "ID_PART_ENTRY_NAME"));
			printf("PARTNUM\t\t: %s\n", udev_device_get_property_value(dev, "ID_PART_ENTRY_NUMBER"));				
			printf("DEVNAME\t\t: %s\n", udev_device_get_property_value(dev, "DEVNAME"));
			printf("BLOCKDEVICE\t: %s\n", udev_device_get_sysname(dev));		
		}

		/* free dev */
		udev_device_unref(dev);
	}

	/* free enumerate */
	udev_enumerate_unref(enumerate);
	/* free udev*/
	udev_unref(udev);

	return 0;       
}