/* 
 *  joystick Led Button
 *  Author       : sana2dang ( fly9p ) - sana2dang@naver.com
 *  Creation Date: Mar 8, 2017
 *  Cafe         : http://cafe.naver.com/raspigamer
 *  Thanks to    : zzeromin, smyani, GreatKStar, KimPanda, StarNDevil, angel
 * 
 * - Reference -
 * wiringPi
 * http://wiringpi.com/the-gpio-utility/
 * apt-get install wiringPi
 * 
 * jstest
 * https://github.com/flosse/linuxconsole/blob/master/utils/jstest.c
 * - complie -
 * sudo gcc ledBtn.c -o ledBtn -lwiringPi -lpthread
 * 
 */

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <pthread.h>

#include <linux/joystick.h>

#define NAME_LENGTH 128

#define pinL  6		// GPIO 6  L
#define pinR  12	// GPIO 12 R
#define pinY  13	// GPIO 13 Y
#define pinB  16	// GPIO 16 B
#define pinX  19		// GPIO 19 X
#define pinAr  20	// GPIO 20 A(R)
#define pinAg  21	// GPIO 21 A(G)
#define pinAb  26	// GPIO 26 A(B)


int AbtnPress	= 0;	// AbtnPress
int chargeShot	= 0;	// hargeShot
time_t sTime	= 0;	// AbtnPress Start Time
time_t eTime	= 0;	// AbtnPress End Time
int chargeCnt 	= 0;	// Charge Count
int k = 0;		// Led Cycle Count


time_t sStopTime = 0;
time_t eStopTime = 0;
int lotationFlag = 0;
int lotationCnt = 0;


void funLotationLed()
{

}

void funAllLightOn()
{
	digitalWrite(pinB,1);
	digitalWrite(pinX,1);
	digitalWrite(pinY,1);
	digitalWrite(pinL,1);
	digitalWrite(pinR,1);
}

void funAllLightOff()
{
	digitalWrite(pinB,0);
	digitalWrite(pinX,0);
	digitalWrite(pinY,0);
	digitalWrite(pinL,0);
	digitalWrite(pinR,0);

}




void funRainbowLed()
{

	if( lotationFlag == 1)
	{
		switch( lotationCnt )
		{
			case 0:
				digitalWrite(pinAb,1);
				lotationCnt ++;
			break;
			case 1: 
				digitalWrite(pinB,1);
				lotationCnt ++;
			break;
			case 2: 
				digitalWrite(pinR,1);
				lotationCnt ++;
			break;
			case 3: 
				digitalWrite(pinL,1);
				lotationCnt ++;
			break;
			case 4: 
				digitalWrite(pinY,1);
				lotationCnt ++;
			break;
			case 5: 
				digitalWrite(pinX,1);
				lotationCnt ++;
			break;	
			default: 
				lotationCnt = 0;	
		}
		delay(500);		// 0.5sec 
		funAllLightOff();
		
	
	}
	else
	{
		lotationCnt = 0;	
	}



	// A Btn Event
	if( AbtnPress == 1 && chargeShot == 0 )
	{
		switch( chargeCnt )
		{
			case 0:
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,0);
			break;
			case 1: 
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);
			break;
			case 2: 
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);				
			break;
			case 3: 
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);			
			break;
			case 4: 
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);
			break;
			case 5: 
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);				
			break;
			case 6: 
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);				
			break;
			default: 
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,0);				
				chargeCnt = 0;
			break;
		}
		delay(50);
		
		eTime = clock();		// AbtnPress End Time Init		
		
		printf("%lf\n", (double)(eTime - sTime )/CLOCKS_PER_SEC );
		if( (double)(eTime - sTime )/CLOCKS_PER_SEC  < 0.0015 )	// AbtnPress End - Start Time < about 1sec	
		{						
			chargeCnt = 0;
		}else
		{
			
			if( (double)(eTime - sTime )/CLOCKS_PER_SEC  >= 0.0030 )	// AbtnPress End - Start Time >=  2sec							
			{				
				chargeShot = 1;
				digitalWrite(pinAr,0);	// RGB Led Off
				digitalWrite(pinAg,0);	// RGB Led Off
				digitalWrite(pinAb,0);	// RGB Led Off
			}
			else
			{
				chargeCnt++;
			}
		}
	}
	if( AbtnPress == 0 )
	{	
		if( chargeShot == 1 )
		{
			for(k=0; k<2; k++)
			{
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,0);
				funAllLightOn();
				delay(30);

				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);
				funAllLightOff();
				delay(30);
	
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);				
				funAllLightOn();
				delay(30);

				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);			
				funAllLightOff();
				delay(30);

				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);
				funAllLightOn();
				delay(30);

				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);			
				funAllLightOff();
				delay(30);
		
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);			
				funAllLightOn();
				delay(30);
				funAllLightOff();
			}
			funAllLightOn();
			delay(400);
			funAllLightOff();


	
		}
		digitalWrite(pinAr,0);		// RGB Led Off
		digitalWrite(pinAg,0);		// RGB Led Off
		digitalWrite(pinAb,0);		// RGB Led Off
		
		
		chargeCnt	= 0;
		chargeShot	= 0;
	}

}

// LED Btn Thread
void* ledBtnThread(void *arg)
{
	int chargeCnt = 0;	
	int k = 0;

	sStopTime = clock();		// lotation time Start
	while(1)
	{			
		funRainbowLed();
		usleep(1000);
		
		eStopTime = clock();

		// 0.1은 약 4sec
		if( (double)(eStopTime - sStopTime)/CLOCKS_PER_SEC  > 0.3 ) // 12sec no action
		{
			lotationFlag = 1;
		}
		else
		{
			if( lotationFlag == 1 )
				sStopTime = clock();
			lotationFlag = 0;
		}

	}
}





int main (int argc, char **argv)
{
	int fd;
	unsigned char axes = 2;
	unsigned char buttons = 2;
	int version = 0x000800;
	char name[NAME_LENGTH] = "Unknown";

	// thread value
	int res;
	pthread_t a_thread;
	void* thread_result;


	if (argc != 2) 
	{
		puts("");
		puts("Usage: ledbtn <#joystick>");
		puts("");
		puts("pin number( GPIO ) : LED(6 12 13 16 19) RGB(20 21 26)");		
		puts("");
		puts("ex) ./ledbtn /dev/input/js0");
		puts("");
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("ledbtn");
		exit(1);
	}

	ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGAXES, &axes);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);

	printf("joystick (%s) has %d axes and %d buttons.\n",
		name, axes, buttons );
	printf("Testing ... (Ctrl+C to exit)\n");

	/*
	* Start led btn
	*/
	if (argc == 2 ) 
	{

		int *axis;
		int *button;
		int i;
		struct js_event js;
		


		res = pthread_create(&a_thread, NULL, ledBtnThread, (void*)NULL);
			if( res )
		printf("thread create error!\n");


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( wiringPiSetupGpio() == -1 )
			return 0;
		/*
		printf("GPIO pin number\n A:%d\tB:%d\tX:%d\tY:%d\tL:%d\tR:%d\n", 
			atoi(argv[2]),
			atoi(argv[3]),
			atoi(argv[4]),
			atoi(argv[5]),
			atoi(argv[6]),
			atoi(argv[7])
			);
		*/
		pinMode(pinAr, OUTPUT);	
		pinMode(pinAg, OUTPUT);	
		pinMode(pinAb, OUTPUT);	
		pinMode(pinB, OUTPUT);	
		pinMode(pinX, OUTPUT);	
		pinMode(pinY, OUTPUT);	
		pinMode(pinL, OUTPUT);	
		pinMode(pinR, OUTPUT);	

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		axis = calloc(axes, sizeof(int));
		button = calloc(buttons, sizeof(char));
		
		while (1) {
			
			if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) 
			{
				perror("\nledbtn: error reading");
				exit (1);
			}					

			switch(js.type & ~JS_EVENT_INIT) 
			{
				case JS_EVENT_BUTTON:
					button[js.number] = js.value;
				break;
				case JS_EVENT_AXIS:
					axis[js.number] = js.value;
				break;
			}
			
			printf("\r");			
			/*
			if (axes) {
				printf("Axes: ");
				for (i = 0; i < axes; i++)
					printf("%2d:%6d ", i, axis[i]);
			}
			*/

			if (buttons) { 
				for (i = 0; i < buttons; i++)
				{
					printf("%2d:%s ", i, button[i] ? "on " : "off");
					
					sStopTime = clock();
					
					if( i==0 && button[i] == 1 )
					{
						if( AbtnPress  != 1 )
						{
							AbtnPress  = 1;							
							sTime = clock();
						}
					}
					if( i==0 && button[i] == 0 )
					{						
						AbtnPress  = 0;							
						sTime = clock();
					}

					
					// B 버튼
					if( i==1 && button[i] == 1 )
						digitalWrite(pinB,1);					
					if( i==1 && button[i] == 0 && chargeShot == 0 )
						digitalWrite(pinB,0);

					// X 버튼
					if( i==2 && button[i] == 1 )
						digitalWrite(pinX,1);
					if( i==2 && button[i] == 0  && chargeShot == 0 )
						digitalWrite(pinX,0);

					// Y 버튼
					if( i==3 && button[i] == 1 )
						digitalWrite(pinY,1);
					if( i==3 && button[i] == 0 && chargeShot == 0 )
						digitalWrite(pinY,0);

					// L 버튼
					if( i==4 && button[i] == 1 )
						digitalWrite(pinL,1);
					if( i==4 && button[i] == 0 && chargeShot == 0 )
						digitalWrite(pinL,0);

					// R 버튼
					if( i==5 && button[i] == 1 )
						digitalWrite(pinR,1);
					if( i==5 && button[i] == 0 && chargeShot == 0 )
						digitalWrite(pinR,0);
				
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
				}
			}

			fflush(stdout);
		}
	}

	printf("ledbtn: unknown joystick: %s\n", argv[1]);
	return -1;
}