/* 
 *  joystick Led Button
 *  Author       : sana2dang ( fly9p ) - sana2dang@naver.com / sana2dang@gmail.com
 *  Creation Date: 2017 - 03 - 28
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
  *
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
#define pinX  19	// GPIO 19 X
#define pinAr  21	// GPIO 20 A(R)
#define pinAg  20	// GPIO 21 A(G)
#define pinAb  26	// GPIO 26 A(B)


#define AbtnShotPressTime 700	
#define AbtnLongPressTime 1500
#define stayTime 10000



int RGBbtnPress	= 0;	// RGBbtnPress on/off flag
int chargeShot	= 0;	// chargeShot cnt
long sTime	= 0;	// RGBbtnPress Start Time
int chargeCnt 	= 0;	// Charge Count
int k = 0;		// Led Cycle Count


long sStopTime = 0;	// stop time
int rotationFlag = 0;	// rotation
int rotationCnt = 0;

void funAllLightOn(mode)
{
	if( mode == 0 )		// ALL LED ON
	{
		digitalWrite(pinAr,1);
		digitalWrite(pinAg,1);
		digitalWrite(pinAb,1);
		digitalWrite(pinB,1);
		digitalWrite(pinX,1);
		digitalWrite(pinY,1);
		digitalWrite(pinL,1);
		digitalWrite(pinR,1);
	}
	if( mode == 1 )		// ALL LED ON without A btn
	{
		digitalWrite(pinB,1);
		digitalWrite(pinX,1);
		digitalWrite(pinY,1);
		digitalWrite(pinL,1);
		digitalWrite(pinR,1);
	}
}

void funAllLightOff(mode)
{
	if( mode == 0 )		// ALL LED OFF
	{
		digitalWrite(pinAr,0);
		digitalWrite(pinAg,0);
		digitalWrite(pinAb,0);
		digitalWrite(pinB,0);
		digitalWrite(pinX,0);
		digitalWrite(pinY,0);
		digitalWrite(pinL,0);
		digitalWrite(pinR,0);
	}
	if( mode == 1 )		// ALL LED OFF without A btn
	{
		digitalWrite(pinB,0);
		digitalWrite(pinX,0);
		digitalWrite(pinY,0);
		digitalWrite(pinL,0);
		digitalWrite(pinR,0);
	}

}


void funRainbowLed()
{	
	if( rotationFlag == 1)	// stay button led
	{
		switch( rotationCnt )
		{
			case 1:
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);
				rotationCnt ++;
			break;
			case 0: 
				digitalWrite(pinB,1);
				rotationCnt ++;
			break;
			case 2: 
				digitalWrite(pinR,1);
				rotationCnt ++;
			break;
			case 3: 
				digitalWrite(pinL,1);
				rotationCnt ++;
			break;
			case 5: 
				digitalWrite(pinY,1);
				rotationCnt = 0;
			break;
			case 4: 
				digitalWrite(pinX,1);
				rotationCnt++;
			break;	
			default: 
				funAllLightOff(0);	
		}
		delay(500);		// 0.5sec 
		funAllLightOff(0);		
	
	}
	else
	{
		rotationCnt = 0;	
	}

	// A Btn Event
	if( RGBbtnPress == 1 && chargeShot == 0 )
	{
		switch( chargeCnt )
		{
			case 0: 
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);				
			break;
			case 1:
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);
			break;
			case 2: 
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);
			break;			
			case 3: 
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);			
			break;
			case 4: 
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,0);
			break;
			case 5: 
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
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

		if( ( millis() - sTime ) < AbtnShotPressTime )		// RGBbtnPress keep check1
		{					
			chargeCnt = 0;
		}
		else
		{
			
			if( ( millis() - sTime )  > AbtnLongPressTime )	// RGBbtnPress keep check2
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
	if( RGBbtnPress == 0 )
	{	
		if( chargeShot == 1 )
		{
			for(k=0; k<2; k++)
			{
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,0);
				funAllLightOn(1);
				delay(40);

				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);
				funAllLightOff(1);
				delay(60);
	
				digitalWrite(pinAr,0);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);				
				funAllLightOn(1);
				delay(40);

				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,0);			
				funAllLightOff(1);
				delay(60);

				digitalWrite(pinAr,1);
				digitalWrite(pinAg,0);
				digitalWrite(pinAb,1);
				funAllLightOn(1);
				delay(40);

				digitalWrite(pinAr,0);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);			
				funAllLightOff(1);
				delay(60);
		
				digitalWrite(pinAr,1);
				digitalWrite(pinAg,1);
				digitalWrite(pinAb,1);			
				funAllLightOn(1);
				delay(40);
				funAllLightOff(0);
			}
			funAllLightOn(0);
			delay(400);
			funAllLightOff(0);
		}		
		
		chargeCnt	= 0;		// init value
		chargeShot	= 0;		// init value
	}

}

// LED Btn Thread
void* ledBtnThread(void *arg)
{
	int chargeCnt = 0;	
	int k = 0;
	
	while(1)
	{			
		funRainbowLed();
		usleep(1000);
		
		if( ( millis() - sStopTime ) > stayTime )	// rotation check
		{
			rotationFlag = 1;
		}
		
	}
	
}





int main (int argc, char **argv)
{
	int fd;
	unsigned char axes = 2;
	int version = 0x000800;
	unsigned char buttons = 2;
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

	
	
	// Start led btn	
	if (argc == 2 ) 
	{

		int *axis;
		int *button;
		int i;
		struct js_event js;
		


		res = pthread_create(&a_thread, NULL, ledBtnThread, (void*)NULL);
			if( res )
		printf("thread create error!\n");

		if( wiringPiSetupGpio() == -1 )
			return 0;
		
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
					//printf("%2d:%s ", i, button[i] ? "on " : "off");
					
					sStopTime = millis();		// rotation time Start
					rotationFlag = 0;		// stop rotation

					if( i== 1 && button[i] == 1 )
					{
						if( RGBbtnPress  != 1 )
						{
							RGBbtnPress  = 1;							
							sTime = millis();
						}
					}
					if( i==1 && button[i] == 0 )
					{						
						RGBbtnPress  = 0;							
						sTime = millis();
						digitalWrite(pinAr,0);		// RGB Led Off
						digitalWrite(pinAg,0);		// RGB Led Off
						digitalWrite(pinAb,0);		// RGB Led Off		
					}

					
					// A 버튼
					if( i==0 && button[i] == 1 )
						digitalWrite(pinB,1);					
					if( i==0 && button[i] == 0 && chargeShot == 0 )
						digitalWrite(pinB,0);

					// Y 버튼
					if( i==3 && button[i] == 1 )
						digitalWrite(pinX,1);
					if( i==3 && button[i] == 0  && chargeShot == 0 )
						digitalWrite(pinX,0);

					// X 버튼
					if( i==2 && button[i] == 1 )
						digitalWrite(pinY,1);
					if( i==2 && button[i] == 0 && chargeShot == 0 )
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

					// Select 버튼( Coin )
					if( i==6 && button[i] == 1 )
					{
						//digitalWrite(pinAg,1);
						funAllLightOn(0);
						delay(40);
						//digitalWrite(pinAg,0);
						funAllLightOff(0);
						delay(40);
						//digitalWrite(pinAg,1);
						funAllLightOn(0);
						delay(40);
						//digitalWrite(pinAg,0);
						funAllLightOff(0);
						delay(40);
						//digitalWrite(pinAg,1);
						funAllLightOn(0);
						delay(40);
						//digitalWrite(pinAg,0);
						funAllLightOff(0);
					}				
				}
			}

			fflush(stdout);
		}
	}

	printf("ledbtn: unknown joystick: %s\n", argv[1]);
	return -1;
}