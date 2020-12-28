CC= arm-none-linux-gnueabi-gcc -Wall 
bin_strip=arm-none-linux-gnueabi-strip

CFLAGS=-Wall  -I ../include  
libs=-L ../lib/ -lgl11pos  
Installer_FirmwareAndApps:main.c Install_Application_patch.c  Install_Apps.c  Install_Firmware.c  Install_Firmware_patch.c Check_Battery_and_Minumum_Charge.c Get_Tokens_of_Patchfile.c Delete_From_installation.c  Get_PatchFileName_for_installation.c Update_BootImages.c Update_patch_info_File.c Write_Current_Version.c Update_IMX25_BootImages.c 


	$(CC)  $?  $(libs)  $(CFLAGS) -o $@
	$(bin_strip) $@ 
clean:
	rm Installer_FirmwareAndApps 
		
