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
int main(void){
	InitSoccer("/dev/ttyAMA0",B115200);
//	pthread_t cameraThread; 
//        pthread_create( &cameraThread, NULL, &camera, NULL);
	while(1){
	int *i = BallSensors();
	int min = 255;
	int index = -1;
	for (int x=0;x<8;x++){	
		i[x]-=60;
		printf("%d ",i[x]);
		if (i[x] < min){
			min = i[x];
			index = i[x];
		}
	}
	unsigned int sum = i[0]+i[1]+i[2]+i[3]+i[4]+i[5]+i[6]+i[7];
	unsigned int vaha = (unsigned char)i[1]+(unsigned char)i[2]*200+(unsigned char)i[3]*400+(unsigned char)i[4]*600+(unsigned char)i[5]*800+
			    (unsigned char)i[6]*1000+(unsigned char)i[7]*1200;
	unsigned int vysl = 0;
	if (sum!= 0)	vysl = vaha/sum;
	printf("sum=%d vaha=%d vysl=%d\n",sum,vaha,vysl);
	usleep(50000);
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
