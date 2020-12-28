#include<header.h>
int Update_imx25_BootImages(char *BootImagesPath)
{
	FILE *fp= NULL;
	int Ubootmd5_ret,Kernelmd5_ret,Kernel_ret,Uboot_ret;
	char file[360];
	char cmd[460];
	chdir(BootImagesPath);
	memset(file,0,sizeof(file));
	sprintf(file,"%s/imx25_u-boot.bin",BootImagesPath);
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
			mkdir_p("/vision/");
					
			system("fw_printenv > /vision/env_file.txt");
			fprintf(stdout,"Installing...  GL11 iMX25  U-boot Image\n");
		/*	system("echo 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config");
			system("dd if=/dev/zero of=/dev/mmcblk0 bs=512 seek=1536 count=8");
			system("echo 0 > /sys/block/mmcblk0boot0/force_ro");
			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"dd if=%s of=/dev/mmcblk0boot0 bs=512 seek=2 skip=2",file);
			fprintf(stdout,"Running Uboot Command %s\n",cmd);
			Uboot_ret = system(cmd);
			system("echo 1 > /sys/block/mmcblk0boot0/force_ro");
			system("echo 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config"); */
			memset(cmd,0,sizeof(cmd));
			strcpy(cmd,"flash_eraseall /dev/mtd0");
			fprintf(stdout,"Installing...  GL11 iMX25  Uboot Image\nErasing... Running Kernel Command %s",cmd);
			system(cmd);
			sleep(2);
			sync();
			

			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"nandwrite -p /dev/mtd0 %s",file);
			fprintf(stdout,"Writing...  GL11 iMX25  Uboot Image\nRunning Kernel Command %s",cmd);
			system(cmd);
			sleep(2);
			sync();
			

			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"/home/differ /dev/mtd0 %s",file);
			fprintf(stdout,"checking...  GL11 iMX25  uboot Image\nRunning Kernel Command %s",cmd);
			Uboot_ret = system(cmd);
			sleep(2);


			if ( Uboot_ret == 0 )
			{
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
			sync();
		}
		else fprintf(stderr,"uboot.md5 file not found\n");
	}
	else 
		fprintf(stdout,"U-boot Image, imx25_u-boot.bin file not found\n"); 



	memset(file,0,sizeof(file));
	sprintf(file,"%s/imx25_kernel.IMG",BootImagesPath);
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
			strcpy(cmd,"flash_eraseall /dev/mtd1");
			fprintf(stdout,"Installing...  GL11 iMX25  Kernel Image\nErasing... Running Kernel Command %s",cmd);
			system(cmd);
			sleep(2);
			sync();
			

			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"nandwrite -p /dev/mtd1 %s",file);
			fprintf(stdout,"Writing...  GL11 iMX25  Kernel Image\nRunning Kernel Command %s",cmd);
			system(cmd);
			sleep(2);
			sync();
			

			memset(cmd,0,sizeof(cmd));
			sprintf(cmd,"/home/differ /dev/mtd1 %s",file);
			fprintf(stdout,"checking...  GL11 iMX25  Kernel Image\nRunning Kernel Command %s",cmd);
			Kernel_ret = system(cmd);
			sleep(2);


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
		fprintf(stdout,"Kernel Image, imx25_kernel.IMG file not Found \n");

	if ( Uboot_ret == 0 || Kernel_ret == 0 )
		return 0;
	else 
	{
		fprintf(stderr," U-boot or Kernel patch file not found\n");
			return -1;
	}
}
