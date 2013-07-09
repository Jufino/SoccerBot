#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "soccer.h"
#include "gafuso.h"
#define vykresli_img 1
bool statCam = true;
using namespace std;
//-----------------------------------------------------------
int kbhit(void);
void* camera(void *arg);
int last_index=150;
int last_distance=500;
int Sens1null=0;
int Sens2null=0;
int main(void){
	InitSoccer("/dev/ttyAMA0",B115200);
//	pthread_t cameraThread; 
//        pthread_create( &cameraThread, NULL, &camera, NULL);
	CompassRaw(&Sens1null,&Sens2null);
	CompassCalb();
	while(1){
	int Sensor1;
	int Sensor2;
	CompassRaw(&Sensor1,&Sensor2);
	printf("Sens1:%d Sens2:%d Compass:%d kicksens:%d \n",Sensor1,Sensor2,Compass(1),KickSensor());
/*	signed char index=0;
	unsigned int distance=0;
	float hodnota = (float)Compass(1)*1.412;
	int calb=0;
	if(hodnota > 5) 	calb = 5;
	else if(hodnota > -5)	calb = 0;
	else 			calb = -5;
	if(hodnota < 20 || hodnota>340){
	BallSensorsCalb(&index,&distance);
	if (((distance > 200) & (index%2==1))|((distance > 190) & (index%2==0))){
                        switch(index){
                                case 1: SetDirection(0,200,calb);    break;
                                case 2: SetDirection(90,200,calb);  break;
                                case 3: SetDirection(135,200,calb);  break;
                                case 4: SetDirection(180,200,calb);    break;
                                case 5: SetDirection(180,200,calb);  break;
                                case 6: SetDirection(225,200,calb);  break;
                                case 7: SetDirection(225,200,calb);    break;
                                case 8: SetDirection(270,200,calb);  break;
                                case 9: SetDirection(270,200,calb);  break;
                                case 10: SetDirection(90,200,calb);    break;
                                case 11: SetDirection(135,200,calb);  break;
                                case 12: SetDirection(135,200,calb);  break;
                                case 13: SetDirection(180,200,calb);    break;
                                case 14: SetDirection(180,200,calb);  break;
                                case 15: SetDirection(225,200,calb);  break;
                                case 16: SetDirection(270,200,calb);  break;
                                default: SetDirection(0,0,0);     break;
                        }
         }
         else{
                        switch(index){
                                case 1: SetDirection(0,200,calb);    break;
                                case 2: SetDirection(90,200,calb);  break;
                                case 3: SetDirection(45,200,calb);  break;
                                case 4: SetDirection(90,200,calb);    break;
                                case 5: SetDirection(90,200,calb);  break;
                                case 6: SetDirection(135,200,calb);  break;
                                case 7: SetDirection(135,200,calb);    break;
                                case 8: SetDirection(180,200,calb);  break;
                                case 9: SetDirection(180,200,calb);  break;
                                case 10: SetDirection(180,200,calb);    break;
                                case 11: SetDirection(225,200,calb);  break;
                                case 12: SetDirection(225,200,calb);  break;
                                case 13: SetDirection(270,200,calb);    break;
                                case 14: SetDirection(270,200,calb);  break;
                                case 15: SetDirection(315,200,calb);  break;
                                case 16: SetDirection(270,200,calb);  break;
                                default: SetDirection(0,0,0);     break;
			}
		}
	}
else if(hodnota > 20 &&hodnota < 180)	SetMotors(-130,-130,-130,-130);
else					SetMotors(130,130,130,130);
//	printf("hodnota=%f\n",hodnota);
//	usleep(5000);
	}

	SetDirection(0,200,5);*/
/*
	LEDR(false);
	LEDG(true);
	usleep(200000);
        LEDR(true);
	LEDG(false);
        usleep(200000);
*/
	}
	return 0;
}
//-----------------------------------------
float uhol(int x1,int y1,int x2,int y2){
	return (atan(((float)y1-(float)y2)/((float)x1-(float)x2))/3.14)*180;
}
void* camera(void *arg){
	CvCapture* camera = cvCaptureFromCAM(0);
	cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_WIDTH,320);
	cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_HEIGHT, 240);
	IplImage  *img = cvQueryFrame(camera);
	int clientsock;
	if (vykresli_img == 1)	clientsock = GafusoCreate(1212);
	while(statCam){
		img = cvQueryFrame(camera);
		if(vykresli_img){
			GafusoRecv(clientsock);       
			if(strcmp(GafusoLoad('f'), "img") == 0)	GafusoSendImg(clientsock,img,50);
		}
	}
	return NULL;
}
//-----------------------------------------
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}
