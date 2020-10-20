#include<header.h>

#define TRUE     1
#define FALSE    0

struct get_batsense{
	int batsense_val;
}__attribute__ ((packed));
struct get_batsense get_batterysense;

int fd;
struct get_batsense get_batterysense;
int get_node_readvalue(const char *node_name);

int gl11_iodev_open()
{
	fd=open("/dev/ioc",O_RDWR|O_NONBLOCK);
	if(fd==-1)
	{
		fprintf(stderr,"IODEVICE open failed\n");
		return -1;
	}
	return 0;
}



int gl11_iodev_close(void)
{
	return close(fd);
}


int gl11_buzz_off(void)
{
	int ret=0;

	ret=ioctl(fd,0xA8,FALSE);

	return ret;
}
int gl11_buzz_on(void)
{
	int ret=0;

	ret=ioctl(fd,0xA8,TRUE);

	return ret;
}


int gl11_GetBatteryInfo(int *AdapPresent,int *BattSt,int *BattVt)
{

	int Bat=0,adpt=0;

	adpt = get_node_readvalue("/sys/devices/platform/imx6q-ecspi.1/spi_master/spi1/spi1.1/bat1");
	if(Bat <0){
		fprintf(stderr,"LIB#Node Read Error\n");
		return -1;
	}

	*AdapPresent = (adpt >1000)?1:0;

	if(adpt > 1000){

		ioctl(fd,0xB4,&get_batterysense);

		*BattSt = get_batterysense.batsense_val;
	}
	else{
		*BattSt = 0;
	}

	Bat = get_node_readvalue("/sys/devices/platform/imx6q-ecspi.1/spi_master/spi1/spi1.1/bat2");
	if(Bat <0){
		fprintf(stderr,"LIB#Node Read Error\n");
		return -1;

	}
	if(Bat <200)
		*BattVt =0;
	else{
		*BattVt =(Bat/2);

	}
	return 0;
}


int get_node_readvalue(const char *node_name)
{
	int ssk=0, fd1=0;
	short int i=3, j=0;
	char bat[6]="";

	while(i >0)
	{
		if((fd1 = open(node_name, O_RDONLY)) <0){
			fprintf(stderr,"LIB#%s Node File Open Error\n",node_name);
			return -1;
		}
		if((j = read(fd1, bat, 5)) <0){
			fprintf(stderr,"LIB#%s Node File Read Error\n",node_name);
			return -1;
		}
		bat[j] = '\0';
		close(fd1);
		ssk += atoi(bat);
		i--;
	}
	return (ssk/3);
}
