#include <Arduino.h>
#include <limits.h>
#include <Servo.h>
#include <NewPing.h>
#include <SerialCommand.h>
//#include <TimerOne.h>

#include "DetectedObject.h"
#include "FixedList.h"

#define CMD_HELLO ":HELLO"
#define CMD_ERROR ":ERROR"
#define CMD_SERVO ":SERVO"
#define CMD_OK    ":OK"
#define CMD_SONAR ":SONAR"
#define CMD_SCAN  ":SCAN"
#define CMD_RANGE ":RANGE"
#define CMD_OBJ ":OBJ"
#define ENDLINE "\r\n"

#define DISTANCEMAP_LEN 33 //todo calza
#define DETECTEDOBJECTS_LEN 17 //solo vamos a guardar en memoria 17.

Servo servo;
SerialCommand sCmd;
NewPing sonar(2,3);

unsigned int distanceMap[DISTANCEMAP_LEN];
FixedList<DetectedObject, DETECTEDOBJECTS_LEN> detectedObjects;


void unrecognized(const char *command)
{
  Serial.print(CMD_ERROR);
  Serial.print(ENDLINE);
}

void cmd_hello()
{
  Serial.print(CMD_HELLO);
  Serial.print(ENDLINE);
}

void cmd_sonar()
{
  Serial.print(CMD_SONAR);
  Serial.print(' ');
  Serial.print(sonar.ping_cm());
  Serial.print(ENDLINE);
}

void cmd_servo()
{
  char *arg = sCmd.next();
  if(arg != NULL)
  {
    int value = atoi(arg);
    servo.write(value);
  }

  Serial.print(CMD_SERVO);
  Serial.print(' ');
  Serial.print(servo.read());
  Serial.print(ENDLINE);
}

void cmd_scan()
{
  //escanear a la derecha
  for(int i=DISTANCEMAP_LEN/2;i<DISTANCEMAP_LEN;i++)
  {
    servo.write(15+i*5);
    delay(200);
    unsigned int ping = sonar.ping_cm();
    distanceMap[i] = (ping != 0)?ping:UINT_MAX;

  }
  //escanear a la izquierda, talvez sea a la derecha pero meh...
  for(int i=DISTANCEMAP_LEN-1;i>=0;i--)
  {
    servo.write(15+i*5);
    delay(200);
    unsigned int ping = sonar.ping_cm();
    distanceMap[i] = (ping != 0)?ping:UINT_MAX;
  }
  //volver al centro
  for(int i=0; i <DISTANCEMAP_LEN/2;i++)
  {
    servo.write(15+i*5);
    delay(200);
    unsigned int ping = sonar.ping_cm();
    distanceMap[i] = (ping != 0)?ping:UINT_MAX;
  }

  detectedObjects.clear();
  int start = 0, end = 0; //variables auxiliares.
  for(int i=0;i<DISTANCEMAP_LEN-1;i++)
  {
    if(distanceMap[i] == UINT_MAX) //si es 0, continuar.
    {
      start = i;
      continue;
    }
    if(abs(distanceMap[i] - distanceMap[i+1]) > 15) //mayor a 15?
    {
      end = i;
      DetectedObject obj(start,end);
      detectedObjects.add(obj);
      start = i+1;
    }
  }

  Serial.print(CMD_SCAN);
  Serial.print(' ');
  for(int i=0;i<DISTANCEMAP_LEN-1;i++)
  {
    Serial.print(distanceMap[i]);
    Serial.print(' ');
  }
  Serial.print(distanceMap[DISTANCEMAP_LEN-1]);
  Serial.print(ENDLINE);

  for(unsigned int i = 0; i < detectedObjects.lastIndex; i++)
  {
      DetectedObject &obj = detectedObjects.array[i];
      obj.centroide(distanceMap);
      Serial.print(CMD_OBJ);
      Serial.print(' ');
      Serial.print(obj.distance);
      Serial.print(' ');
      Serial.print(obj.angle);
      Serial.print(ENDLINE);
  }
}

void setup()
{
  Serial.begin(9600);
  servo.attach(9);

  sCmd.addCommand(CMD_HELLO, cmd_hello);
  sCmd.addCommand(CMD_SONAR, cmd_sonar);
  sCmd.addCommand(CMD_SERVO, cmd_servo);
  sCmd.addCommand(CMD_SCAN,  cmd_scan);
  sCmd.setDefaultHandler(unrecognized);

  //seteo todo a 0
  memset(&distanceMap, 0, sizeof(distanceMap));
}

void loop()
{
    sCmd.readSerial();
}
