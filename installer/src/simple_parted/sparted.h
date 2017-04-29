#ifndef DEF_SPARTED
#define DEF_SPARTED

#include <parted/parted.h>
#include <parted/device.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

typedef struct SPedDevice{

        PedDevice** device;
        int device_count;

}SPedDevice;


typedef struct SPedDisk{
        PedDisk** disk;
        int disk_count;
}SPedDisk;

typedef struct SPedPartition{

    PedPartition** partition;
    int partition_count;

}SPedPartition;

typedef struct SPedPartitionInfo{
	char* name;
	char* path;
	char* fs;
	char* type;
	long long size;

}SPedPartitionInfo;

typedef struct SPedDiskInfo{

	PedDiskTypeFeature feature;
	SPedPartitionInfo* partition_info;
	int partition_info_count;
	long long size;

}SPedDiskInfo;

SPedDevice sped_get_device();
SPedDisk sped_get_disk();
SPedPartition sped_get_partition(PedDisk* d);
SPedDiskInfo sped_get_disk_info(PedDisk* disk);
PedDisk* sped_get_disk_by_name(char* name);

#endif