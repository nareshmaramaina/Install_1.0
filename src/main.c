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
	int Apps_Downloads,Updates;
	int Firmware_Downloads;
	int ret;
	FILE *fp;

	ret = Install_patches_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Install_patches_lock Application is already Running\n");
		return -1;
	}
	Firmware_Downloads = Get_Total_Downloaded_Updates(FIRMWARE);
	if ( Firmware_Downloads > 0 )
		fprintf(stdout,"%d Firmware Downloads are need to Install \n",Firmware_Downloads);
	else 
		fprintf(stdout,"No Firmware Downloads Found\n");

	Apps_Downloads = Get_Total_Downloaded_Updates(APPLICATION);
	if ( Apps_Downloads > 0 )
		fprintf(stdout,"%d Application Downloads are need to Install \n",Apps_Downloads);
	else 
		fprintf(stdout,"No Application Downloads Found\n");


	if ( Firmware_Downloads <= 0  && Apps_Downloads <= 0 )
	{

		fprintf(stdout,"\n***** No Software Updates Found *****\n");
		return 0;
	}

	ret = Check_Battery_and_Minumum_Charge();

	if ( ret != 0 )
		return -1;


	system("/vision/lcd_bkl &");

	system("cat /vision/apply_patch > /dev/fb0");


	return 1;// Mandatory return non zero filesystem purpose 
}

