#include<header.h>
int  Install_Apps(int Total_Installations)
{
	int ret;
	int PatchCount,Deleted,Installed;

	char ApplicationPatchFile[340];


	for( PatchCount = 1,Deleted = 0,Installed=0; PatchCount <= Total_Installations; PatchCount++)
	{	
		memset(ApplicationPatchFile,0,sizeof(ApplicationPatchFile));

		ret = Get_PatchFileName_for_installation(ApplicationPatchFile,APPLICATION,PatchCount-Deleted);

		if ( ret != 0 )
		{
			fprintf(stdout,"Application Installations Not Found\n");
			break;
		}
		else
		{	
			ret =  Install_Application_patch(ApplicationPatchFile); // -1 Failed Patch, 0 Success Instaled, 1 Battery Required
			if ( ret != 1 )
			{
				if ( ret == 0 )	
				{
					Installed++;
					fprintf(stdout,"Application Installed Successfully, Installed Count  = %d\n",Installed);
				}
				else 
				{
					fprintf(stdout,"Failed To Apply Application Patch No=%d, App = %s\n",PatchCount,ApplicationPatchFile); 
					system("cat /vision/DeviceManagement/logos/AppInstallationFailed.png > /dev/fb0");
					sleep(1);
				}
				Delete_From_installation(ApplicationPatchFile,APPLICATION);
				Deleted++;
			}
			else
				fprintf(stdout,"Failed To Install Application, Due to Battery must and Should Require for this patch installation, Application Patch No=%d, App = %s\n",PatchCount,ApplicationPatchFile);

		}
	}
	return Installed;
}
