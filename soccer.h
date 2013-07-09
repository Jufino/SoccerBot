#ifndef _SOCCER_H
#define _SOCCER_H
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "libserial.h"
//----------------------------------------------------------
void InitSoccer(const char *port_name,speed_t speed);
//----------------------------------------------------------
void CloseSoccer(void);
//----------------------------------------------------------
int *BallSensors(void);
//----------------------------------------------------------
void BallSensorsCalb(signed char *positionBall,unsigned int *distanceBall);
//----------------------------------------------------------
void LED(bool status);
void LEDR(bool status);
void LEDG(bool status);
//----------------------------------------------------------
void SetMotors(int speed1,int speed2,int speed3,int speed4);
//----------------------------------------------------------
void SetDirection(int angle,int speed,int calb);
//----------------------------------------------------------
void Kick(void);
//----------------------------------------------------------
bool KickSensor(void);
//----------------------------------------------------------
int Compass(int mode);
void CompassRaw(int *Sensor1,int *Sensor2);
void CompassCalb(void);
//----------------------------------------------------------
int *LineSensors(void);
//----------------------------------------------------------
void tik(void);
//----------------------------------------------------------
void tok(void);
//----------------------------------------------------------
