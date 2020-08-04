#include<header.h>

int  Install_Firmware()
{
	int ret;

	char FirmwarePatchFile[340];

	memset(FirmwarePatchFile,0,sizeof(FirmwarePatchFile));

	ret = Get_PatchFileName_for_installation(FirmwarePatchFile,FIRMWARE,0);

	if ( ret != 0 )
		fprintf(stdout,"Firmware Installations Not Found\n");

	else
	{	
		ret =  Install_Firmware_patch(FirmwarePatchFile);
		if ( ret != 1 )
			Delete_From_installation(FirmwarePatchFile,FIRMWARE);

	}
	return ret;
}
