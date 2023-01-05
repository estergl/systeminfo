#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <blkid/blkid.h>

/**
 * https://github.com/util-linux/util-linux/blob/master/misc-utils/lsblk-properties.c
 * https://github.com/util-linux/util-linux/blob/master/libblkid/src/superblocks/superblocks.c
 * https://github.com/util-linux/util-linux/blob/master/libblkid/src/partitions/partitions.c
*/

int main (int argc, char *argv[]) {
   blkid_probe pr;

   const char *uuid;
   const char *label;
   const char *partlabel;
   const char *partuuid;
   const char *fstype;
   const char *partnum;

   if (argc != 2) {
      fprintf(stderr, "Usage: %s <devname>\n", argv[0]);
      exit(1);
   }

   if (getuid() != 0) {
      fprintf(stderr, "no permissions to read from the device\n");
      exit(1);      
   }

   pr = blkid_new_probe_from_filename(argv[1]);
   if (!pr) {
      err(2, "Failed to open %s", argv[1]);
   }

   blkid_probe_enable_superblocks(pr, 1);
   blkid_probe_set_superblocks_flags(pr, BLKID_SUBLKS_TYPE | BLKID_SUBLKS_UUID | BLKID_SUBLKS_LABEL);
   blkid_probe_enable_partitions(pr, 1);
   blkid_probe_set_partitions_flags(pr, BLKID_PARTS_ENTRY_DETAILS);

   if (!blkid_do_safeprobe(pr)) {
      if (!blkid_probe_lookup_value(pr, "UUID", &uuid, NULL))
         printf("UUID\t\t:%s\n", uuid);
      if (!blkid_probe_lookup_value(pr, "LABEL", &label, NULL))
         printf("LABEL\t\t:%s\n", label);
      if (!blkid_probe_lookup_value(pr, "TYPE", &fstype, NULL))
         printf("FSTYPE\t\t:%s\n", fstype);         
      if (!blkid_probe_lookup_value(pr, "PART_ENTRY_NAME", &partlabel, NULL))
         printf("PARTLABEL\t:%s\n", partlabel);   
      if (!blkid_probe_lookup_value(pr, "PART_ENTRY_UUID", &partuuid, NULL))
         printf("PARTUUID\t:%s\n", partuuid);   
      if (!blkid_probe_lookup_value(pr, "PART_ENTRY_NUMBER", &partnum, NULL))
         printf("PARTNUM\t\t:%s\n", partnum);   
   }

   blkid_free_probe(pr);

   return 0;
}