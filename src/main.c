#include<header.h>
int Install_patches_lock()
{
	int fd,ret=0;

	fd = open("/tmp/.Install_patches_lock",O_CREAT | O_CLOEXEC);

	ret = flock(fd,LOCK_EX|LOCK_NB);

	if (ret != 0)
	{
		close(fd);
		return -1;
	}
	return 0;

}
int main()
{
	int Success_Installation=0;
	int Apps_Downloads,Firmware_Downloads;
	int ret;
	int Installer_Current_Version=7;
	ret = Install_patches_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Installer_FirmwareAndApps Application is already Running\n");
		return -1;
	}

	Write_Current_Version(Installer_Current_Version);

	if ( access("/vision/.RHMS_Uboot_Update",F_OK) == 0 )
	{
		fprintf(stdout,"Updating Last U-boot Environment Variables to Updated U-boot\n");
		system("/vision/DeviceManagement/set_env > /dev/null 2>&1");
		remove("/vision/.RHMS_Uboot_Update");
	}

	Firmware_Downloads = Get_Total_Downloaded_Updates(FIRMWARE);
	Apps_Downloads = Get_Total_Downloaded_Updates(APPLICATION);
	
	if ( Firmware_Downloads > 0 || Apps_Downloads > 0 ) 
	{
		ret  = system("grep Hardware /proc/cpuinfo  |grep MX25 -q");
		if ( ret == 0 )
		{
			fprintf(stdout,"This is Imx25 device, So inserting iocontrol module on 235 number\n");
			system("insmod /home/imx_iocontrol.ko");
			system("mknod /dev/ioc c 235 0");
			sleep(2);
		}

	}
	if ( Firmware_Downloads > 0 )
	{
		fprintf(stdout,"%d Firmware Installations are need to Install \n",Firmware_Downloads);
		system("/vision/DeviceManagement/lcd_bkl &");
		ret = Install_Firmware();	
		if ( ret == 1 )
			fprintf(stdout,"Firmware Installation Failed due to  Battery must and Should Require for patch installation\n");
		else if ( ret == 0 )
		{
			fprintf(stdout,"Successfully Installed Firmware Patch, reboot required for the patch changes\n");
			Success_Installation=1;
		}
		else
		{
			fprintf(stdout,"Firmware Installation Failed\n");
			system("cat /vision/DeviceManagement/logos/FirmwareInstallationFailed.png > /dev/fb0");
			sleep(2);

		}
		system("killall  /vision/DeviceManagement/lcd_bkl");
	}
	else 
		fprintf(stdout,"No Firmware Installations Found\n");

	//Apps_Downloads = Get_Total_Downloaded_Updates(APPLICATION);
	if ( Apps_Downloads > 0 )
	{
		fprintf(stdout,"%d Application Installations are need to Install \n",Apps_Downloads);
		system("/vision/DeviceManagement/lcd_bkl &");
		ret = Install_Apps(Apps_Downloads);	
		if ( ret > 0 )
		{
			fprintf(stdout,"Successfully Installed %d Applications, reboot required for the patch changes\n",ret);
			Success_Installation =1;
		}
		system("killall  /vision/DeviceManagement/lcd_bkl");
	}
	else 
		fprintf(stdout,"No Application Installations Found\n");


	if ( Firmware_Downloads <= 0  && Apps_Downloads <= 0 )
	{
		fprintf(stdout,"\n***** No Software Updates Found *****\n");
		return 0;
	}
	else if ( Success_Installation == 1 )
	{
		fprintf(stdout,"Rebooting the POS device  for the patch installation changes\n");
		system("reboot");
		sleep(12); // Avoid startx running
	}

	return ret;
}

