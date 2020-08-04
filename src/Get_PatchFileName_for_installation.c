#include<header.h>
extern char *Install_Applications_file;
extern char *Install_Firmwares_file;
extern char *Standard_Apps_path;
extern char *Standard_Firmwares_path;
/*int main()
  {
  char PatchFileName[340];
  memset(PatchFileName,0,sizeof(PatchFileName));
  Get_PatchFileName_for_installation(PatchFileName,2,6);
  return 0;
  }*/
int Get_PatchFileName_for_installation(char *PatchFileName,int type,int Patch_Number)
{
	FILE *fp=NULL;
	char *line=NULL;
	size_t len=0;
	int Found=0,count=0;
	char filename[128];
	char path[128];

	memset(filename,0,sizeof(memset));
	memset(path,0,sizeof(path));

	if ( type == FIRMWARE )
	{
		strcpy(path,Standard_Firmwares_path);
		strcpy(filename,Install_Firmwares_file);
	}
	else if ( type == APPLICATION )
	{
		strcpy(path,Standard_Apps_path);
		strcpy(filename,Install_Applications_file);
	}
	else
	{
		fprintf(stdout,"Unknown type Requested\n");
		return -1;
	}

	fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		fprintf(stdout," %s file not found\n",filename);
		return -1;
	}

	while( (getline(&line, &len, fp) > 0) )
	{
		if ( strstr(line,path) != NULL )
		{
			if ( type == APPLICATION )
			{
				count++;
				if( count == Patch_Number)
					fprintf(stdout,"Application Patch Number %d Found\n",Patch_Number);
				else continue;
			}

			if ( line[strlen(line)-1] == '\n' )
				line[strlen(line)-1] = '\0';

			strcpy(PatchFileName,line);
			Found=1;
			break;
		}
	}
	free(line);
	line=NULL;
	fclose(fp);

	if ( Found == 1 )
	{

		fprintf(stdout," %s patch file found for installation , type %d \n",PatchFileName,type);
		return 0;
	}
	else 
	{
		fprintf(stdout,"No Patch files Found for installation\n");
	}

	return -1;
}

