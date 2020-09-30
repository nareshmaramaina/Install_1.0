#include<header.h>
#include<gl11.h>

int gl11_battery_status()
{
	int AdapPresent=0,BattSt=0,BattVt=0,ret=0;
	int batVolt ;

	float stripBatVolt;

	gl11_iodev_open();

	ret=gl11_GetBatteryInfo(&AdapPresent,&BattSt,&BattVt);

	batVolt =  BattVt;

	stripBatVolt = ((8.4)*batVolt)/1024;

	fprintf(stdout,"Adapterstatus=%d batterystatus=%d batteryvoltage=%d Voltage=%f V\n",AdapPresent,BattSt,BattVt,stripBatVolt);

	if(ret<0)
		fprintf(stdout,"Fail to get the battery status\n");
	else
		fprintf(stdout,"getting the battery status success\n");

	sleep(2);

	gl11_iodev_close();
	return BattVt;
}
int Check_Battery_and_Minumum_Charge(void) 
{
	int BattVt;
	BattVt = gl11_battery_status();
	if(BattVt > 400  )
	{
		printf("\nBattery Present\n");
		if(BattVt<870)
		{
			system("cat /vision/DeviceManagement/logos/LowBattery.png > /dev/fb0");

			sleep(1);

			printf("Low Battery Please Charge Your Battery\n");

			gl11_iodev_open();

			gl11_buzz_on();

			sleep(2);

			gl11_buzz_off();

			gl11_iodev_close();
			return -1;
		}
		else return 0;
	}
	else if(BattVt <= 0)
	{
		system("cat /vision/DeviceManagement/logos/InsertBattery.png > /dev/fb0");
			sleep(1);
		printf("\nBattery Not Present\n");
		return -1;
	}

	return 0;
}

