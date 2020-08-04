#include<header.h>
char *Standard_Apps_path="/mnt/sysuser/Software-Upgrade/Applications_Downloads/";
char *Application_history_file="/etc/vision/RHMS/Apps/Installed_Apps_patches_history";
int Install_Application_patch(char *ApplicationPatchFile)
{
	char Patch_info_file[460];
	char md5sum[64];
	char file[460];
	char cmd[500];
	int ret =0;
	char ExtractPath[430];
	char ApplicationPath[440];
	char RemoveExtractPath[440];
	DIR *dp=NULL;
	char ApplicationType[128];
	char ApplicationName[128];
	float Version;
	memset(ApplicationType,0,sizeof(ApplicationType));
	memset(ApplicationName,0,sizeof(ApplicationName));

	ret = Get_Tokens_of_ApplicationPatchfile(ApplicationPatchFile,ApplicationType,ApplicationName,&Version);
	printf(" ApplicationType = %s ApplicationName = %s Version= %.1f\n" ,ApplicationType,ApplicationName,Version);

	if( ret  != 0 )
		return -1;

	fprintf(stdout,"Application Installing ...\n");
	memset(ExtractPath,0,sizeof(ExtractPath));

	sprintf(ExtractPath,"%s/%s/%s/Extract",Standard_Apps_path,ApplicationType,ApplicationName);
	sprintf(RemoveExtractPath,"rm -rf %s",ExtractPath);	

	if ( access(ApplicationPatchFile,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",ApplicationPatchFile);
		//Delete_From_installation(ApplicationPatchFile,FIRMWARE);
		return -1;
	}


	if( access(ExtractPath,F_OK) == 0 )
		system (RemoveExtractPath); // Removing previous files  

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p %s",ExtractPath);
	system(cmd); // Extract Patch in Particular Folder 

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p /etc/vision/RHMS/Apps/%s/%s",ApplicationType,ApplicationName);
	system(cmd); // Extract Patch in Particular Folder 

	memset(Patch_info_file,0,sizeof(Patch_info_file));

	sprintf(Patch_info_file,"/etc/vision/RHMS/Apps/%s/%s/AppUpdated.info",ApplicationType,ApplicationName);


	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"unzip %s -d %s  >> %s",ApplicationPatchFile,ExtractPath,Application_history_file);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 

	if(ret != 0)
	{
		fprintf(stderr,"Unzip failed %s\n",ApplicationPatchFile);	
		return -1;
	}
	chdir(ExtractPath);// Changing directory to firmware
	ret = system("md5sum -c app.md5"); // Md5sum verification

	if( ret != 0)
	{
		system (RemoveExtractPath); // Removing previous files  
		fprintf(stderr,"app.md5 Checksum failed %s\n",ApplicationPatchFile);
		return -1;
	}
	memset(md5sum,0,sizeof(md5sum));	
	get_md5sum("app.md5",md5sum);

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"tar -xvf app.tar.bz2 >> %s",Application_history_file);
	ret = system(cmd); // Extraction for tar
	if ( ret != 0)
	{
		fprintf(stderr," Extration Failed retrying with xvjf flag\n");
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"tar -xvjf app.tar.bz2 >> %s",Application_history_file);
		ret = system(cmd); // Extraction for tar
	}
	if(ret != 0)
	{
		fprintf(stderr,"In %s file, app.tar.bz2 extraction failed\n",ApplicationPatchFile);
		return -1;
	}			
	memset(ApplicationPath,0,sizeof(ApplicationPath));	
	sprintf(ApplicationPath,"%s/app",ExtractPath);
	dp = opendir(ApplicationPath);
	if ( dp == NULL )
	{
		fprintf(stdout,"%s directory not found ",ApplicationPath);
		return -1;
	}	
	else closedir(dp);	


	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/NonCritical",ApplicationPath);

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
	sprintf(cmd,"chmod -R 777  %s/*",ApplicationPath);
	ret = system(cmd);
	if ( ret == 0 )
		fprintf(stdout,"Giving chmod -R 777 File Permission by default \n");


	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/start_app.sh",ApplicationPath);
	ret = access(file,F_OK);
	if(ret == 0 )
	{
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"sh %s",file);
		ret =  system(cmd);
		fprintf(stdout, "Finished run %s script for run commands Before Applying Application patch work,return value of script %d",file,ret );
		sync();
	}

	fprintf(stdout,"Copying Download files into / directory\n");
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"yes | cp -r %s/* /",ApplicationPath);

	ret =  system(cmd);
	if (ret == 0 )
		fprintf(stdout," Files Copied Successfully \n");	
	else
	{	
		fprintf(stdout," Files Copying Failed, return value %d\n",ret);
		return -1;	
	}
	memset(file,0,sizeof(file));
	sprintf(file,"%s/tmp/end_app.sh",ApplicationPath);
	ret = access(file,F_OK);
	if(ret == 0 )
	{
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"sh %s",file);
		ret =  system(cmd);
		fprintf(stdout, "Finished run %s script for run commands After Installed Application patch,return value of script %d",file,ret );
		sync();
	}
	Update_Application_patch_info_File(Patch_info_file,ApplicationType,ApplicationName,Version,md5sum);
	system (RemoveExtractPath); // Removing previous files  
	return 0;
}
