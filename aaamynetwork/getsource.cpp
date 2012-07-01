#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdlib.h>
#include <errno.h>

int GetDiskCount() {
	FILE* f = fopen("/proc/partitions","r");
	if (!f) {
		fprintf(stderr,"fopen /proc/partitions failed!error=%d\n",errno);
		return 0;
	}
	char line[100],ptname[100],devname[120],*s;
	int ma,mi,sz;
	int nDiskCount = 0;

	while (fgets(line,sizeof(line),f)) {
		if (sscanf(line," %u %u %u %[^\n ]",&ma,&mi,&sz,ptname) != 4)
			continue;
		for (s = ptname; *s; s++)
			continue;
		/* note： excluding ‘0’： e.g. mmcblk0 is not a partition name! */
		if (s[-1] >= '1' && s[-1] <= '9')
			continue;
		nDiskCount++;
	}
	fclose(f);
	return nDiskCount;
}

int main(int argc,char * argv[]) {
	printf("DiskCount： %d\n",GetDiskCount());
	return 0;
}
