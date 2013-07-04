#include "blaze.h"
//--------------------------------------------------------
char *int2char(char *buffer,int i,bool newline){ 
  char *bufferx = (char*)malloc(strlen(buffer)*sizeof(char)+5);
  if      (buffer != NULL && newline == false)	sprintf(bufferx,"%s%4d",buffer,i);
  else if (buffer == NULL && newline == false)  	sprintf(bufferx,"%4d",i);
  else if (buffer != NULL && newline == true)    sprintf(bufferx,"%s%4d\n",buffer,i);
  else						sprintf(bufferx,"%4d\n",i);
  unsigned char p=0;
  do{
        if (bufferx[p] == ' ') bufferx[p] = '0';
  }
  while(bufferx[p++] != '\0');
  return bufferx;
}

//-------------------------------------------------------
timeval startTime,endTime;
void tik(void){
	gettimeofday(&startTime, NULL);
}
void tok(void){
	long seconds, useconds;
	double duration;
        gettimeofday(&endTime, NULL);
        seconds  = endTime.tv_sec  - startTime.tv_sec;
        useconds = endTime.tv_usec - startTime.tv_usec;
        duration = seconds + useconds/1000000.0;
        printf("%5.6f seconds\n", duration);
}
//-------------------------------------------------------
int port;
void InitSoccer(const char *port_name,speed_t speed){
  port = SerialOpen(port_name,speed);
}
int *BallSensors(void){
  SerialWrite(port,"a\n");
  char *datas =  SerialRead(port,8);
  return datas;
}
void LED_R(bool status){
  if (status == 0)  SerialWrite(port,"c\n");
  else              SerialWrite(port,"d\n");
}
void SetMotors(int speed1,int speed2,int speed3,int speed4){

}
void SetDirection(int angle,int speed){

}
void Kick(void){
	SerialWrite(port,"e\n");
}
bool KickSensor(void){

}
int Compass(int mode){

}
int *LineSensors(void)}

}
