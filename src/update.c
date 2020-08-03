#include<header.h>

int Update_app()
{
	int ret;

	if ( access("/mnt/sysuser/App-Upgrade/Extract/app",F_OK) != 0 )
	{
		fprintf(stderr,"/mnt/sysuser/App-Upgrade/Extract/app/ Patch Not Found \n");
		remove("/mnt/sysuser/App-Upgrade/download_complete");
		return -1;
	}

	system("chmod -R 777  /mnt/sysuser/App-Upgrade/Extract/app/*");

	ret=access("/mnt/sysuser/App-Upgrade/Extract/app/tmp/start_fw.sh",F_OK);
	if(ret == 0 )
		system("sh /mnt/sysuser/App-Upgrade/Extract/app/tmp/start_fw.sh");


	ret = system("yes | cp -r /mnt/sysuser/App-Upgrade/Extract/app/*   /");
	if( ret != 0 )
	{
		fprintf(stdout," Apply Patch Failed, ret= %d yes | cp -r /mnt/sysuser/App-Upgrade/Extract/app/*   / command failed\n",ret);
		system("rm -rf /mnt/sysuser/App-Upgrade/Extract/app/");
		remove("/mnt/sysuser/App-Upgrade/download_complete");
		return -1;
	}

	else 	Update_App_release();	

	ret=access("/tmp/end_fw.sh",F_OK);
	if(ret == 0 )
		system("sh /tmp/end_fw.sh");

	system("rm -rf /mnt/sysuser/App-Upgrade/Extract/");
	remove("/mnt/sysuser/App-Upgrade/download_complete");
	sleep(2);
	return 0;
}
int Update_App_release()
{

	char date[24];
	char md5sum[48];
	float version=0.0;

	memset(date,0,sizeof(date));	
	memset(md5sum,0,sizeof(md5sum));	

	FILE *fp=NULL;
	fp = fopen("/etc/app_release","w");
	if ( fp == NULL)
	{
		fprintf(stderr,"/etc/app_release file not found \n");
		return -1;
	}

	get_time(date);

	get_version(&version);

	get_md5sum(md5sum);

	fprintf(fp,"APP_VERSION=%.1f\nAPP_DATE=%s\nAPP_MD5SUM=%s\n",version,date,md5sum);

	fclose(fp);
	fp = fopen("/etc/.RHMS_Patches.info","a");
	if ( fp == NULL)
	{
		fprintf(stderr,"/etc/.RHMS_Patches.info file not found \n");
		return -1;
	}
	fprintf(fp,"\nAPP_VERSION=%.1f\nAPP_DATE=%s\nAPP_MD5SUM=%s\n*********************************\n\n",version,date,md5sum);

	fclose(fp);

	fprintf(stdout,"Application Patch has Updated Successfully with %.1f version\n",version);

	return 0;
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
