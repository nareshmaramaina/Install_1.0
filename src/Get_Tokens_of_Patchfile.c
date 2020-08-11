#include<header.h>
extern char *Standard_Firmwares_path;
extern char *Standard_Apps_path;

int Get_Tokens_of_FirmwarePatchfile(char *file,char *FirmwareName,float *Version)
{
	char VersionBuff[128];
	int i;
	char  *ptr=NULL;
	char FirmwarePatchFile[128];
	memset(VersionBuff,0,sizeof(VersionBuff));
	memset(FirmwarePatchFile,0,sizeof(FirmwarePatchFile));
	strcpy(FirmwarePatchFile,file);
	if ( strstr(FirmwarePatchFile,Standard_Firmwares_path) == NULL )
	{
		fprintf(stdout,"standard path not found Firmware Path %s\n",Standard_Firmwares_path);
		return -1;
	}

	ptr = strtok(FirmwarePatchFile, "/");
	if(ptr!=NULL)
	{

		for( i=0; ( ( ptr = strtok(NULL, "/") ) != NULL );i++)
		{
			if( i == 3)
				strcpy(FirmwareName,ptr);
			else if ( i == 4 )
			{
				strcpy(VersionBuff,ptr);
				break;	
			}
		}
	}
	else 
	{
		fprintf(stdout,"Tokens not found with / path\n");
		return -1;
	}

	*Version = atof(VersionBuff+9);	
	if ( strlen(FirmwareName) == 0 ||  *Version <= 0  )
	{
		fprintf(stderr,"Token Error %s \n",file);
		return -1;
	}
	return 0;
}


/*int main()
  {
  char ApplicationType[128];
  char ApplicationName[128];
  float Version;
  memset(ApplicationType,0,sizeof(ApplicationType));
  memset(ApplicationName,0,sizeof(ApplicationName));
  char ApplicationPatchFile[128]="/mnt/sysuser/Software-Upgrade/Applications_Downloads/BANK/ICICI/app-1.3.zip";

  Get_Tokens_of_ApplicationPatchfile(ApplicationPatchFile,ApplicationType,ApplicationName,&Version);
  printf(" FirmwarType = %s ApplicationName = %s Version= %.1f\n" ,ApplicationType,ApplicationName,Version);

  } */

//int Get_Token_of_ApplicationPatchfile(char *ApplicationPatchFile)
int Get_Tokens_of_ApplicationPatchfile(char *file,char *ApplicationType,char *ApplicationName,float *Version)
{
	int i;
	char  *ptr=NULL;
	char VersionBuff[24];
	char ApplicationPatchFile[128];
	memset(ApplicationPatchFile,0,sizeof(ApplicationPatchFile));
	strcpy(ApplicationPatchFile,file);
	if ( strstr(ApplicationPatchFile,Standard_Apps_path) == NULL )
	{
		fprintf(stdout,"standard path not found Application Path %s\n",Standard_Apps_path);
		return -1;
	}

	ptr = strtok(ApplicationPatchFile, "/");
	if(ptr!=NULL)
	{

		for( i=0; ( ( ptr = strtok(NULL, "/") ) != NULL );i++)
		{
			if( i == 3)
				strcpy(ApplicationType,ptr);
			else if ( i == 4 )
				strcpy(ApplicationName,ptr);
			else if ( i == 5 )
			{
				strcpy(VersionBuff,ptr);
				break;	
			}
		}
	}
	else 
	{
		fprintf(stdout,"Tokens not found with / path\n");
		return -1;
	}

	*Version = atof(VersionBuff+4);	
	if ( strlen(ApplicationType) == 0 || strlen(ApplicationName) == 0 ||  *Version <= 0  )
	{
		fprintf(stderr,"Token Error %s \n",file);
		return -1;
	}
	return 0;
}

