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
	int max = 0;
	int index = -1;
	for (int x=0;x<8;x+=1){	
		i[x]=150-i[x];
		if(i[x]<0)i[x] =0;
		printf("%d ",i[x]);
		if (i[x] > max){
			max = i[x];
			index = x;
		}
	}
	int index_next=0;
	int index_last=0;
	if (index != -1){
	if (index == 7){
		index_next = 0;
		index_last = 6;
	}
	else if(index == 0){
		index_next = 1;
		index_last = 7;
	}
	else{
		index_next=index+1;
		index_last=index-1;
	}
	}
	unsigned int sum = (i[index_last]+i[index]+i[index_next]);
	unsigned int vysl;
	int x=-1;
	if (sum !=0){
		vysl = (i[index_last]*2000+i[index]*4000+i[index_next]*6000)/sum;
		if(vysl > 4250)		x = 2;
		else if (vysl >	3850)	x = 1;
		else			x = 0;
	}
	if (x == 2)	index = index*2+2;
	else if(x==1)	index = index*2+1;
	else if(x==0)	index = index*2;
	if(index == 0) index = 16;
	printf("index=%d vysl=%d sum=%d\n",index,vysl,sum);
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
