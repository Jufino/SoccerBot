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
void gpio_open(int port, int DDR)
{
	FILE *f;
	f = fopen("/sys/class/gpio/export", "w");
	fprintf(f, "%d\n", port);
	fclose(f);

	char file[128];
	sprintf(file, "/sys/class/gpio/gpio%d/direction", port);
	f = fopen(file, "w");
	if (DDR == 0)	fprintf(f, "in\n");
	else		fprintf(f, "out\n");
	fclose(f);
}

void gpio_close(int port)
{
	FILE *f;
	f = fopen("/sys/class/gpio/unexport", "w");
	fprintf(f, "%d\n", port);
	fclose(f);
}

int gpio_read(int port)
{
	FILE *f;

	char file[128];
	sprintf(file, "/sys/class/gpio/gpio%d/value", port);
	f = fopen(file, "r");

	int i;
	fscanf(f, "%d", &i);
	fclose(f);
	return i;

}
void gpio_write(int port, int value){
	FILE *f;

	char file[128];
	sprintf(file, "/sys/class/gpio/gpio%d/value", port);
	f = fopen(file, "w");

	if (value == 0)	fprintf(f, "0\n");
	else		fprintf(f, "1\n");

	fclose(f);
}
//-------------------------------------------------------
int port;
void InitSoccer(const char *port_name,speed_t speed){
  port = SerialOpen(port_name,speed);
  gpio_open(17,1);
  gpio_open(18,1);
}
//-------------------------------------------------------
void CloseSoccer(void){
	SerialClose(port);
	gpio_close(17);
	gpio_close(18);
}
//-------------------------------------------------------
int *BallSensors(void){
  SerialWrite(port,"a\n");
  char *dataChar =  SerialRead(port,8);
  int *i = (int*)malloc(sizeof(int)*9);
  for(int x=0;x<9;x++) i[x] = (int)dataChar[x];
  return i;
}

void BallSensorsCalb(signed char *positionBall,unsigned int *distanceBall){
        int Sens[8];
	for (int x=0;x<5;x++){
		int *i = BallSensors();	
		if (x==0) for(int y=0;y<8;y++)	Sens[y] = i[y];
		else	  for(int y=0;y<8;y++)  Sens[y] += i[y];
		usleep(1000);
	}
	int max = 0;
        signed char index = -1;
        for (int x=0;x<8;x++){
                Sens[x]=150-(Sens[x]/5);
                if(Sens[x]<0)Sens[x] =0;
		if (Sens[x] > max){
                        max = Sens[x];
                        index = x;
                }
        }
        signed char index_next=0;
        signed char index_last=0;
	unsigned sum = -1;
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
		sum = (Sens[index_last]+Sens[index]+Sens[index_next]);
        }
	else	*positionBall = 0;
	unsigned int vysl= 0;
        signed char x = -1;
        if ((sum !=0) & (index != -1)){
                vysl = (Sens[index_last]*2000+Sens[index]*4000+Sens[index_next]*6000)/sum;
                if(vysl > 4250)         x = 2;
                else if (vysl > 3850)   x = 1;
                else                    x = 0;
        }
        if (x == 2)     *positionBall = index*2+2;
        else if(x==1)   *positionBall = index*2+1;
        else if(x==0)   *positionBall = index*2;
	else 		*positionBall = 0;
        if((index == 0) & (x == 0))  *positionBall = 16;
	*distanceBall = sum;
}
//-------------------------------------------------------
void LED(bool status){
  if (status == 0)  SerialWrite(port,"c\n");
  else              SerialWrite(port,"d\n");
}
void CompassCalb(void){
	SerialWrite(port,"n\n");
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
void SetDirection(int angle,int speed,int calb){
	if (calb < 0){
		switch(angle){
			case 0:   SetMotors(-speed,-speed,speed+calb,speed+calb);	break;
			case 45:  SetMotors(calb,-speed,calb,speed+calb);  			break;
			case 90:  SetMotors(speed+calb,-speed,-speed,speed+calb); 	break;
        		case 135: SetMotors(speed+calb,calb,-speed,calb);  			break;
        		case 180: SetMotors(speed+calb,speed+calb,-speed,-speed); 	break;
        		case 225: SetMotors(calb,speed+calb,calb,-speed);          		break;
        		case 270: SetMotors(-speed,speed+calb,speed+calb,-speed); 	break;
			case 315: SetMotors(-speed,calb,speed+calb,calb);			break;
			default:  SetMotors(0,0,0,0);					break;
		}
	}
	else{
               switch(angle){
                        case 0:   SetMotors(-speed+calb,-speed+calb,speed,speed); 	break;
                        case 45:  SetMotors(calb,-speed+calb,calb,speed);          		break;
                        case 90:  SetMotors(speed,-speed+calb,-speed+calb,speed); 	break;
                        case 135: SetMotors(speed,calb,-speed+calb,calb);          		break;	
                        case 180: SetMotors(speed,speed,-speed+calb,-speed+calb); 	break;
                        case 225: SetMotors(calb,speed,calb,-speed+calb);          		break;
                        case 270: SetMotors(-speed+calb,speed,speed,-speed+calb); 	break;
                        case 315: SetMotors(-speed+calb,calb,speed,calb);          		break;
                        default:  SetMotors(0,0,0,0);                   		break;
                }
	}
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
void CompassRaw(int *Sensor1,int *Sensor2){
	SerialWrite(port,"m\n");
        char *datas = SerialRead(port,4);
        *Sensor1 = (datas[0]*256+datas[1]);
	*Sensor2 = (datas[2]*256+datas[3]);
}
//-------------------------------------------------------
int *LineSensors(void){
return 0;
}
//-------------------------------------------------------
void LEDR(bool status){
	if (status == true)	gpio_write(17,0);
	else			gpio_write(17,1);
}
void LEDG(bool status){
        if (status == true)     gpio_write(18,0);
        else                    gpio_write(18,1);
}
