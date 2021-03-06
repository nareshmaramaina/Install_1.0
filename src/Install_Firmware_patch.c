#include<header.h>
char *Standard_Firmwares_path="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/";
char *Firmware_history_file="/etc/vision/RHMS/Firmware/Installed_Firmware_patches_history";
int Install_Firmware_patch(char *FirmwarePatchFile)
{
	char file[460];
	char cmd[500];
	char md5sum[64];
	int ret =0;
	char ExtractPath[430];
	char FirmwarePath[440];
	char RemoveExtractPath[440];
	char FirmwareName[128];
	float Version;
	char ProjectName[128];
	DIR *dp=NULL;

	memset(ProjectName,0,sizeof(ProjectName));
	memset(FirmwareName,0,sizeof(FirmwareName));
	ret = Get_Tokens_of_FirmwarePatchfile(FirmwarePatchFile,FirmwareName,&Version,ProjectName);

	printf(" FirmwareName = %s  Version= %.1f\n" ,FirmwareName,Version);
	if( ret  != 0 )
		return -1;

	if ( access(FirmwarePatchFile,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",FirmwarePatchFile);
		return -1;
	}

	fprintf(stdout,"Firmware Installing ...\n");
	memset(ExtractPath,0,sizeof(ExtractPath));

	sprintf(ExtractPath,"%s/%s/%s/Extract",Standard_Firmwares_path,ProjectName,FirmwareName);
	sprintf(RemoveExtractPath,"rm -rf %s",ExtractPath);	


	if( access(ExtractPath,F_OK) == 0 )
		system (RemoveExtractPath); // Removing previous files  

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p %s",ExtractPath);
	system(cmd); // Extract Patch in Particular Folder 

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p /etc/vision/RHMS/Firmware/%s/%s",ProjectName,FirmwareName);
	system(cmd); 


	sprintf(cmd,"unzip %s -d %s  >> %s",FirmwarePatchFile,ExtractPath,Firmware_history_file);

	printf("Extracting %s\n",cmd);  
	system("cat /vision/DeviceManagement/logos/InstallingFirmware.png > /dev/fb0");

	ret = system(cmd); // Unzipping 

	if(ret != 0)
	{
		system (RemoveExtractPath); // Removing previous files  
		fprintf(stderr,"Unzip failed %s\n",FirmwarePatchFile);	
		return -1;
	}
	sleep(1);
	chdir(ExtractPath);// Changing directory to firmware
	ret = system("md5sum -c patch.md5"); // Md5sum verification

	if( ret != 0)
	{
		fprintf(stderr,"patch.md5 Checksum failed %s\n",FirmwarePatchFile);
		return -1;
	}
	memset(md5sum,0,sizeof(md5sum));	
	get_md5sum("patch.md5",md5sum);

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
	else closedir(dp);	


	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/NonCritical",FirmwarePath);

	ret = access(file,F_OK);
	if ( ret != 0 )
	{
		ret = Check_Battery_and_Minumum_Charge();

		if ( ret != 0 )
			return 1;
	}
	else
		fprintf(stdout,"Non Critical Mode Enabled\n");



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
		fprintf(stdout, "Finished run %s script for run commands Before Applying Firmware patch work,return value of script = %d\n",file,ret );
		sync();
	}

	memset(file,0,sizeof(file));
	sprintf(file,"%s/boot_files/",FirmwarePath);
	dp = opendir(file);
	if ( dp != NULL )
	{
		closedir(dp);
		fprintf(stdout,"%s Boot Directory found, Searching for Boot Images\n",file);

		ret  = system("grep Hardware /proc/cpuinfo  |grep MX25 -q");
		if ( ret == 0 )
		{
			fprintf(stdout,"This is Imx25 device\n");
			if ( Update_imx25_BootImages(file) == 0 )
				fprintf(stdout,"Boot Image Patch Applied Successfully\n");
			else 
			{
				fprintf(stdout,"Update_imx25_BootImages updation Failed\n");
				return -1;
			}
		}

		else 
		{
			fprintf(stdout,"This is Imx6 device\n");
			if ( 	Update_BootImages(file) == 0 )
				fprintf(stdout," Boot Image Patch Applied Successfully\n");
			else 
			{
				fprintf(stdout," Update_BootImages updation Failed\n");
				return -1;
			}
		}
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
		fprintf(stdout, "Finished run %s script for run commands After Installed Firmware patch,return value of script = %d\n",file,ret );
		sync();
	}

	Update_Firmware_patch_info_File(FirmwareName,Version,md5sum,ProjectName);
	system (RemoveExtractPath); // Removing previous files  

	return 0;

}
