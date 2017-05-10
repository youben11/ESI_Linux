#include "sparted.c"

void main(){
    /*PedDevice* p;
    ped_device_probe_all();
    p=ped_device_get_next(NULL);
    printf("%s\n",p->path );
    p=ped_device_get_next(p);
    if(p!=NULL)
        printf("%s\n",p->path );
    //ped_device_probe_all();
    SPedDisk diske=sped_get_disk();
    for(int j=0;j<diske.disk_count;j++){
    printf("%s\n",diske.disk[j]->dev->path);
    SPedPartition part=sped_get_partition(diske.disk[j]);
    for(int i=0;i<part.partition_count;i++)
    	printf("|_%s\n",ped_partition_get_path(part.partition[i]) );}
    sped_get_disk_info(diske.disk[0]);*/

    SPedDisk disk=sped_get_disk();
    SPedDiskInfo dinfo=sped_get_disk_info(disk.disk[0]);

    for(int i=0;i<dinfo.partition_info_count;i++){
        printf("%s %s %s %s %lldMo\n",dinfo.partition_info[i].name,dinfo.partition_info[i].path,dinfo.partition_info[i].type,dinfo.partition_info[i].fs,dinfo.partition_info[i].size/1048576);
    }
    printf("%lld\n",dinfo.size/1024/1024/1024);
    return 0;
}
