#include<header.h>
char *Standard_Apps_path="/mnt/sysuser/Software-Upgrade/Applications_Downloads/";
char *Standard_Firmwares_path="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/";
char *Firmware_history_file="/etc/vision/RHMS/Firmware/Installed_patches_history";
int Install_Firmware_patch(char *FirmwarePatchFile)
{
	char Date_time[64];
	char file[128];
	char cmd[400];
	FILE *fp=NULL;
	int ret =0;
	char ExtractPath[330];
	char FirmwarePath[340];
	char RemoveExtractPath[340];
	//	char FirmwarePatchFile[128];
	char FirmwareType[128];
	char Filename[128];
	float Version;
	DIR *dp=NULL;

	memset(FirmwareType,0,sizeof(FirmwareType));
	memset(Filename,0,sizeof(Filename));

	ret = Get_Tokens_of_FirmwarePatchfile(FirmwarePatchFile,FirmwareType,Filename,&Version);

	if( ret  == 0 )
		printf(" FirmwarType = %s Filename = %s Version= %.1f\n" ,FirmwareType,Filename,Version);


	memset(ExtractPath,0,sizeof(ExtractPath));

	sprintf(ExtractPath,"%s/%s/Extract",Standard_Firmwares_path,FirmwareType);
	sprintf(RemoveExtractPath,"rm -rf %s",ExtractPath);	

	if ( access(FirmwarePatchFile,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",FirmwarePatchFile);
		//Delete_From_installation(FirmwarePatchFile,FIRMWARE);
		return -1;
	}


	if( access(ExtractPath,F_OK) == 0 )
		system (RemoveExtractPath); // Removing previous files  

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p %s",ExtractPath);
	system(cmd); // Extract Patch in Particular Folder 

	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"unzip %s -d %s  >> %s",FirmwarePatchFile,ExtractPath,Firmware_history_file);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 

	if(ret != 0)
	{
		fprintf(stderr,"Unzip failed %s\n",FirmwarePatchFile);	
		return -1;
	}
	chdir(ExtractPath);// Changing directory to firmware
	ret = system("md5sum -c patch.md5"); // Md5sum verification

	if( ret != 0)
	{
		fprintf(stderr,"patch.md5 Checksum failed %s\n",FirmwarePatchFile);
		return -1;
	}

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"tar -xvf patch.tar.bz2 >> %s",Firmware_history_file);
	ret = system(cmd); // Extraction for tar
	if ( ret != 0)
	{
		fprintf(stderr," Extration Failed retrying with xvjf flag\n");
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"tar -xvjf patch.tar.bz2 >> %s",Firmware_history_file);
		ret = system(cmd); // Extraction for tar
	}
	if(ret != 0)
	{
		fprintf(stderr,"In %s file, patch.tar.bz2 extraction failed\n",FirmwarePatchFile);
		return -1;
	}			
	memset(FirmwarePath,0,sizeof(FirmwarePath));	
	sprintf(FirmwarePath,"%s/patch",ExtractPath);
	dp = opendir(FirmwarePath);
	if ( dp == NULL )
	{
		fprintf(stdout,"%s directory not found ",FirmwarePath);
		return -1;
	}		
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"chmod -R 777  %s/*",FirmwarePath);
	ret = system(cmd);
	if ( ret == 0 )
		fprintf(stdout,"Giving chmod -R 777 File Permission by default \n");


	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/start_fw.sh",FirmwarePath);
	ret = access(file,F_OK);
	if(ret == 0 )
	{
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"sh %s",file);
		ret =  system(cmd);
		fprintf(stdout, "Finished run %s script for run commands Before Applying Firmware patch work,return value of script %d",file,ret );
	}

	fprintf(stdout,"Copying Download files into / directory\n");
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"yes | cp -r %s/* /",FirmwarePath);

	ret =  system(cmd);
	if (ret == 0 )
		fprintf(stdout," Files Copied Successfully \n");	
	else
	{	
		fprintf(stdout," Files Copying Failed, return value %d\n",ret);
		return -1;	
	}
	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/end_fw.sh",FirmwarePath);
	ret = access(file,F_OK);
	if(ret == 0 )
	{
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"sh %s",file);
		ret =  system(cmd);
		fprintf(stdout, "Finished run %s script for run commands After Installed Firmware patch,return value of script %d",file,ret );
	}


	return ret;

}

void Update_Current_Date_with_Time(char *Date_time)
{
	struct tm *Today=NULL;
	struct timeval tv;

	gettimeofday (&tv,NULL);

	Today = localtime (&tv.tv_sec) ;

	sprintf(Date_time,"Date:%02d/%02d/%04d Time:%02d-%02d-%02d",Today->tm_mday,Today->tm_mon+1,Today->tm_year+1900,Today->tm_hour,Today->tm_min,Today->tm_sec);

	return;
}

