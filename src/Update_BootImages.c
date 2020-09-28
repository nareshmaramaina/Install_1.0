#include<header.h>
int Update_BootImages(char *BootImagesPath)
{
	FILE *fp= NULL;
	int Ubootmd5_ret,Kernelmd5_ret,Kernel_ret,Uboot_ret;
	char file[360];
	char cmd[460];
	chdir(BootImagesPath);
	memset(file,0,sizeof(file));
	sprintf(file,"%s/u-boot.bin",BootImagesPath);
	Uboot_ret = access(file,F_OK);
	if ( Uboot_ret == 0 )
	{
		fprintf(stdout," U-boot Image Found \n");
		Ubootmd5_ret = access("uboot.md5",F_OK);
		if ( Ubootmd5_ret == 0 )
		{
			fprintf(stdout," uboot.md5 also Found\n");
			Ubootmd5_ret = system("md5sum -c uboot.md5");
			if ( Ubootmd5_ret != 0 )
			{
				fprintf(stderr,"Uboot File Corrupted\n");
				return -1;
			}
			fprintf(stdout,"Installing...  GL11 iMX6  U-boot Image\n");
			system("echo 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config");
			system("dd if=/dev/zero of=/dev/mmcblk0 bs=512 seek=1536 count=8");
			system("echo 0 > /sys/block/mmcblk0boot0/force_ro");
			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"dd if=%s of=/dev/mmcblk0boot0 bs=512 seek=2 skip=2",file);
			fprintf(stdout,"Running Uboot Command %s\n",cmd);
			Uboot_ret = system(cmd);
			system("echo 1 > /sys/block/mmcblk0boot0/force_ro");
			system("echo 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config");

			if ( Uboot_ret == 0 )
			{
				mkdir_p("/vision/");
				system("fw_printenv > /vision/env_file.txt");
				fp = fopen("/vision/.RHMS_Uboot_Update","w");
				if ( fp == NULL )
					return -1;
				fclose(fp);	
				fprintf(stdout,"U-Boot Loaded Successfully\n");
			}
			else 
			{
				fprintf(stdout,"Caution: U-boot Load Failed, Your POS may be crashed \n");
				return -1;	
			}
			sleep(2);
			sync();
		}
		else fprintf(stderr,"uboot.md5 file not found\n");
	}
	else 
		fprintf(stdout,"U-boot Image, u-boot.bin file not found\n"); 



	memset(file,0,sizeof(file));
	sprintf(file,"%s/kernel.IMG",BootImagesPath);
	Kernel_ret = access(file,F_OK);
	if ( Kernel_ret == 0 )
	{
		fprintf(stdout," Kernel Image Found \n");
		Kernelmd5_ret = access("kernel.md5",F_OK);
		if ( Kernelmd5_ret == 0 )
		{	
			fprintf(stdout," kernel.md5  also Found\n");
			Kernelmd5_ret = system("md5sum -c kernel.md5");
			if ( Kernelmd5_ret != 0 )
			{
				fprintf(stderr,"Kernel File Corrupted\n");
				return -1;
			}
			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"dd if=%s of=/dev/mmcblk0 bs=1M seek=1 conv=fsync",file);
			fprintf(stdout,"Installing...  GL11 iMX6  Kernel Image\nRunning Kernel Command %s",cmd);
			Kernel_ret = system(cmd);
			sleep(2);
			sync();
			if ( Kernel_ret == 0 )
				fprintf(stdout," Kernel Loaded Successfully\n");
			else
			{	
				fprintf(stdout,"Caution: Kernel Loading Failed, Your POS may be crashed \n");
				return -1;
			}
		}
		else fprintf(stderr,"kernel.md5 file not found\n");
	}
	else 
		fprintf(stdout,"Kernel Image, kernel.IMG file not Found \n");

	return 0;
}
int mkdir_p(char *dirname)
{
        DIR *dp;

        dp = opendir(dirname);
        if ( dp == NULL )
        {
                remove(dirname);
                return mkdir(dirname,0777);
        }

        closedir(dp);
        return 0;
}

