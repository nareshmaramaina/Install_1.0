#include <sys/file.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include<sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <termios.h>
#include <net/if.h>
#include<unistd.h>
#include<termios.h>
#include<errno.h>
#include <netdb.h> 
#include <resolv.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <dirent.h>
#include <dlfcn.h>

#define FIRMWARE 1
#define APPLICATION 2




int get_Machineid(char *machineid);
int get_macid(char *macid);
int get_device_serialnumber(char *serialnum);  // Updating Device serial number in RHMS health structures
int get_RHMS_Server_details(void);
void Wait_for_internet(void);
int Check_internet_by_hostname(char *host);
int Check_Battery_and_Minumum_Charge(void); 
int Install_Application_patch(char *ApplicationPatchFile);
int Install_Firmware_patch(char *FirmwarePatchFile);
int  Install_Firmware();
int  Install_Apps();
int Delete_From_installation(char *patch_file,int type);
int Get_Tokens_of_FirmwarePatchfile(char *file,char *FirmwareName,float *Version);
int Get_Tokens_of_ApplicationPatchfile(char *file,char *ApplicationType,char *ApplicationName,float *Version);
int Get_Total_Downloaded_Updates(int type);
void Update_Current_Date_with_Time(char *Date_time);
int Update_BootImages(char *BootImagesPath);
int Get_PatchFileName_for_installation(char *PatchFileName,int type,int Patch_Number);
int Delete_patch(int Total_PatchCount,char *patch_file,int type);
int Update_BootImages(char *BootImagesPath);
void Update_Firmware_patch_info_File(char *FirmwareName,float Version,char *md5sum);
void Update_Application_patch_info_File(char *app_patch_info_file,char *ApplicationType,char *ApplicationName,float Version,char *md5sum);
void get_md5sum(char *file,char  *md5sum);
void Update_Current_Date_with_Time(char *Date_time);
void Write_Current_Version(int Version);
