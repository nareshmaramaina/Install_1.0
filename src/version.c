#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *Standard_Apps_path="/mnt/sysuser/Software-Upgrade/Applications_Downloads/";
char *Standard_Firmwares_path="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/";

int main()
{
	char FirmwareType[128];
	char Filename[128];
	float Version;
	memset(FirmwareType,0,sizeof(FirmwareType));
	memset(Filename,0,sizeof(Filename));
	char FirmwarePatchFile[128]="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/testing/firmware-.zip\n";

	Get_Tokens_of_FirmwarePatchfile(FirmwarePatchFile,FirmwareType,Filename,&Version);
	printf(" FirmwarType = %s Filename = %s Version= %.1f\n" ,FirmwareType,Filename,Version);

}

//int Get_Token_of_FirmwarePatchfile(char *FirmwarePatchFile)
int Get_Tokens_of_FirmwarePatchfile(char *file,char *FirmwareType,char *Filename,float *Version)
{
	int i;
	char  *ptr=NULL;
	char FirmwarePatchFile[128];
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
			{
				strcpy(FirmwareType,ptr);
				fprintf(stdout," FirmwareType  =%s, ptr = %s \n",FirmwareType,ptr);
			}
			else if ( i == 4 )
			{
				strcpy(Filename,ptr);
				break;	
			}
		}
	}

	*Version = atof(Filename+9);	
	if ( strlen(FirmwareType) == 0 || strlen(Filename) == 0 ||  *Version <= 0  )
	{
		fprintf(stderr,"Token Error %s \n",file);
		return -1;
	}
	return 0;
}

