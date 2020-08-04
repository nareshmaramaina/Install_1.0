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
	int Apps_Downloads,Firmware_Downloads;
	int ret;
	ret = Install_patches_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Install_patches_lock Application is already Running\n");
		return -1;
	}
	Firmware_Downloads = Get_Total_Downloaded_Updates(FIRMWARE);
	if ( Firmware_Downloads > 0 )
	{
		fprintf(stdout,"%d Firmware Installations are need to Install \n",Firmware_Downloads);
		ret = Install_Firmware();	
		if ( ret == 1 )
			fprintf(stdout,"Firmware Installation Failed due to  Battery must and Should Require for patch installation\n");
		else if ( ret == 0 )
		{
			fprintf(stdout,"Successfully Installed Firmware Patch, reboot required for the patch changes\n");
			system("reboot");
			return 0;
		}
		else fprintf(stdout,"Firmware Installation Failed\n");
	}
	else 
		fprintf(stdout,"No Firmware Installations Found\n");

	Apps_Downloads = Get_Total_Downloaded_Updates(APPLICATION);
	if ( Apps_Downloads > 0 )
	{
		fprintf(stdout,"%d Application Installations are need to Install \n",Apps_Downloads);
		ret = Install_Apps();	
		if ( ret > 0 )
		{
			fprintf(stdout,"Successfully Installed %d Applications, reboot required for the patch changes\n",ret);
			system("reboot");
		}
	}
	else 
		fprintf(stdout,"No Application Installations Found\n");


	if ( Firmware_Downloads <= 0  && Apps_Downloads <= 0 )
	{
		fprintf(stdout,"\n***** No Software Updates Found *****\n");
		return 0;
	}

	return 0;
}
