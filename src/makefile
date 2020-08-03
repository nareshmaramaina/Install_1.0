CC= arm-none-linux-gnueabi-gcc -Wall 
bin_strip=arm-none-linux-gnueabi-strip

CFLAGS=-Wall  -I ../include  
libs=-L ../lib/ -lgl11pos  -lpcsclite
Common_bat_check:main.c  update.c Apply_Application_Patch.c  Apply_Firmware_patch.c get_updates_info.c get_POS_Application_version.c  get_POS_Firmware_version.c update_script.c Check_Battery_and_Minumum_Charge.c


	$(CC)  $?  $(libs)  $(CFLAGS) -o $@
	$(bin_strip) $@ 
	cp Common_bat_check /u01/Common_apps
clean:
	rm Common_bat_check
		
