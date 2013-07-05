#include "soccer.h"
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
void CloseSoccer(void){
	SerialClose(port);
}
//-------------------------------------------------------
int *BallSensors(void){
  SerialWrite(port,"a\n");
  char *dataChar =  SerialRead(port,8);
  int *i = (int*)malloc(sizeof(int)*9);
  for(int x=0;x<9;x++) i[x] = (int)dataChar[x];
  return i;
}
//-------------------------------------------------------
void LED_R(bool status){
  if (status == 0)  SerialWrite(port,"c\n");
  else              SerialWrite(port,"d\n");
}
//-------------------------------------------------------
void SetMotors(int speed1,int speed2,int speed3,int speed4){
  char *bufferx = (char*)malloc(21*sizeof(char));
  sprintf(bufferx,"f%4dg%4dh%4di%4d\n",speed1,speed2,speed3,speed4);
  unsigned char p=0;
  do{
        if (bufferx[p] == ' ') bufferx[p] = '0';
  }
  while(bufferx[p++] != '\0');
  SerialWrite(port,bufferx);
}
//-------------------------------------------------------
void SetDirection(int angle,int speed){
  char *bufferx = (char*)malloc(9*sizeof(char));
  sprintf(bufferx,"b%3d%4d\n", angle, speed);
  SerialWrite(port,bufferx);
}
//-------------------------------------------------------
void Kick(void){
	SerialWrite(port,"e\n");
}
//-------------------------------------------------------
bool KickSensor(void){
	SerialWrite(port,"j\n");
        return SerialRead(port,1)[0];
}
//-------------------------------------------------------
int Compass(int mode){
	if (mode == 1){ 
		SerialWrite(port,"k\n");
        	return SerialRead(port,1)[0];
	}
	else{
		SerialWrite(port,"l\n");
		char *datas = SerialRead(port,2);
                return (datas[0]*256+datas[1]); 
	}
}
//-------------------------------------------------------
int *LineSensors(void){
return 0;
}
//-------------------------------------------------------
