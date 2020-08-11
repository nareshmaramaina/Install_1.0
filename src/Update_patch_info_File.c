#include<header.h>
extern char *Standard_Apps_path;
extern char *Standard_Firmwares_path;
extern char *Firmware_history_file;
extern char *Application_history_file;
void Update_Firmware_patch_info_File(char *FirmwareName,float Version,char *md5sum)
{
	char date[48];
	char Remove_DownloadCompletedFile[350];

	memset(date,0,sizeof(date));	

	FILE *fp=NULL;
	fp = fopen("/etc/vision/RHMS/Firmware/FirmwareUpdated.info","w");
	if ( fp == NULL)
	{
		fprintf(stderr,"/etc/vision/RHMS/Firmware/FirmwareUpdated.info file not found \n");
		return;
	}

	Update_Current_Date_with_Time(date);

	fprintf(fp,"FirmwareName:%s\nVersion:%.1f\nFirmware_patch_md5sum=%s\nInstalled_DateAndTime=%s\n",FirmwareName,Version,md5sum,date);

	fclose(fp);

	memset(Remove_DownloadCompletedFile,0,sizeof(Remove_DownloadCompletedFile));
	sprintf(Remove_DownloadCompletedFile,"%s/%s/%.1f_DownloadCompleted", Standard_Firmwares_path,FirmwareName,Version);
	
	remove(Remove_DownloadCompletedFile);
	fprintf(stdout,"Removed %s file\n",Remove_DownloadCompletedFile);
	fp = fopen(Firmware_history_file,"a");
	if ( fp == NULL)
	{
		fprintf(stderr,"%s append Error \n",Firmware_history_file);
		return;
	}
	fprintf(fp,"FirmwareName:%s\nVersion:%.1f\nFirmware_patch_md5sum=%s\nInstalled_DateAndTime=%s\n",FirmwareName,Version,md5sum,date);
	fprintf(fp,"\n*********************************************\n\n");

	fclose(fp);

	fprintf(fp,"Updated patch info, FirmwareName:%s\nVersion:%.1f\nFirmware_md5sum=%s\nInstalled_DateAndTime=%s\n",FirmwareName,Version,md5sum,date);

	return;
}
void Update_Application_patch_info_File(char *app_patch_info_file,char *ApplicationType,char *ApplicationName,float Version,char *md5sum)
{
	char Remove_DownloadCompletedFile[450];
	char date[48];

	memset(date,0,sizeof(date));	
	
	FILE *fp=NULL;
	fp = fopen(app_patch_info_file,"w");
	if ( fp == NULL)
	{
		fprintf(stderr,"%s file not found \n",app_patch_info_file);
		return;
	}

	Update_Current_Date_with_Time(date);

	fprintf(fp,"ApplicationType:%s\nApplicationName:%s\nVersion:%.1f\nApplication_patch_md5sum=%s\nInstalled_DateAndTime=%s\n",ApplicationType,ApplicationName,Version,md5sum,date);

	fclose(fp);

	memset(Remove_DownloadCompletedFile,0,sizeof(Remove_DownloadCompletedFile));
	sprintf(Remove_DownloadCompletedFile,"%s/%s/%s/DownloadCompleted",Standard_Apps_path,ApplicationType,ApplicationName);
	
	remove(Remove_DownloadCompletedFile);
	fprintf(stdout,"Removed %s file\n",Remove_DownloadCompletedFile);

	fp = fopen(Application_history_file,"a");
	if ( fp == NULL)
	{
		fprintf(stderr,"%s append Error \n",Application_history_file);
		return;
	}
	fprintf(fp,"ApplicationType:%s\nApplicationName:%s\nVersion:%.1f\nApplication_patch_md5sum=%s\nInstalled_DateAndTime=%s\n",ApplicationType,ApplicationName,Version,md5sum,date);
	fprintf(fp,"\n*********************************************\n\n");

	fclose(fp);

	fprintf(fp,"Updated patch info, ApplicationType:%s\nApplicationName:%s\nVersion:%.1f\nApplication_patch_md5sum=%s\nInstalled_DateAndTime=%s\n",ApplicationType,ApplicationName,Version,md5sum,date);

	
	return;
}


void get_md5sum(char *file,char  *md5sum)
{
	FILE *fp=NULL;
	fp = fopen(file,"r");  // Writing version file to /root/version file
	if (fp == NULL)
	{
		fprintf(stderr,"%s file not found \n",file);
		return;
	}

	fscanf(fp,"%s",md5sum);

	fclose(fp);

	return;
}
void Update_Current_Date_with_Time(char *Date_time)
{
	struct tm *Today=NULL;
	struct timeval tv;

	gettimeofday (&tv,NULL);

	Today = localtime (&tv.tv_sec) ;

	sprintf(Date_time,"%04d-%02d-%02dT%02d:%02d:%02d",Today->tm_year+1900,Today->tm_mon+1,Today->tm_mday,Today->tm_hour,Today->tm_min,Today->tm_sec);
	fprintf(stdout,"Today Date and Time, %s \n",Date_time);

	return;
}

