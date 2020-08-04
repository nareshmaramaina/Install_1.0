#include<header.h>
char *Standard_Firmwares_path="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/";
char *Firmware_history_file="/etc/vision/RHMS/Firmware/Installed_patches_history";
int Install_Firmware_patch(char *FirmwarePatchFile)
{
	char file[360];
	char cmd[400];
	char md5sum[64];
	int ret =0;
	char ExtractPath[330];
	char FirmwarePath[340];
	char RemoveExtractPath[340];
	char FirmwareName[128];
	float Version;
	DIR *dp=NULL;

	memset(FirmwareName,0,sizeof(FirmwareName));
	ret = Get_Tokens_of_FirmwarePatchfile(FirmwarePatchFile,FirmwareName,&Version);

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

	sprintf(ExtractPath,"%s/%s/Extract",Standard_Firmwares_path,FirmwareName);
	sprintf(RemoveExtractPath,"rm -rf %s",ExtractPath);	



	system("/vision/lcd_bkl &");

	system("cat /vision/apply_patch > /dev/fb0");

	if( access(ExtractPath,F_OK) == 0 )
		system (RemoveExtractPath); // Removing previous files  

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p %s",ExtractPath);
	system(cmd); // Extract Patch in Particular Folder 

	system("mkdir -p /etc/vision/RHMS/Firmware/");

	sprintf(cmd,"unzip %s -d %s  >> %s",FirmwarePatchFile,ExtractPath,Firmware_history_file);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 
	
	if(ret != 0)
	{
		system (RemoveExtractPath); // Removing previous files  
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
	sprintf(file,"%s/boot_files/",FirmwarePath);
	dp = opendir(file);
	if ( dp != NULL )
	{
		closedir(dp);
		fprintf(stdout,"%s Boot Directory found, Searching for Boot Images",file);
		Update_BootImages(file);
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"rm -rf %s",file);
		system(cmd);
	}


	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/start_fw.sh",FirmwarePath);
	ret = access(file,F_OK);
	if(ret == 0 )
	{
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"sh %s",file);
		ret =  system(cmd);
		fprintf(stdout, "Finished run %s script for run commands Before Applying Firmware patch work,return value of script %d",file,ret );
		sync();
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
		sync();
	}

	Update_Firmware_patch_info_File(FirmwareName,Version,md5sum);
	system (RemoveExtractPath); // Removing previous files  
	return 0;

}
