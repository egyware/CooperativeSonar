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
#define DETECTEDOBJECTS_LEN 25 //solo vamos a guardar en memoria 17.

Servo servo;
SerialCommand sCmd;
NewPing sonar(2,3);

unsigned int distanceMap[DISTANCEMAP_LEN];
FixedList<DetectedObject, DETECTEDOBJECTS_LEN> detectedObjects;
int rangeA = 0, rangeB = DISTANCEMAP_LEN;

void unrecognized(const char *command)
{
  Serial.print(CMD_ERROR);
  Serial.print(ENDLINE);
}

void cmd_range()
{
    char *arg0 = sCmd.next();
    char *arg1 = sCmd.next();
    if(arg0 == NULL && arg1 == NULL)
    {
      rangeA = 15;
      rangeB = 180;
    }
    if(arg0 != NULL && arg1 != NULL)
    {
      rangeA = atoi(arg0);
      rangeB = atoi(arg1);
    }
    if(arg0 != NULL && arg1 == NULL)
    {
      int r = atoi(arg0);
      rangeA = max(15,r-15);
      rangeB = min(180,r+15);
    }
    if(rangeA > rangeB)
    {
      rangeA = 0;
      rangeB = DISTANCEMAP_LEN;
      Serial.print(CMD_ERROR);
      Serial.print(ENDLINE);
    }
    else
    {
      rangeA = (rangeA - 15)/5;
      rangeB = (rangeB - 15)/5;
      Serial.print(CMD_OK);
      Serial.print(ENDLINE);
    }
    servo.write(15+((rangeA+rangeB)/2)*5);
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
  int center = (rangeB+rangeA)/2;
  for(int i = center;i < rangeB; i++)
  {
    servo.write(15+i*5);
    //delay(200);
    unsigned int ping = NewPingConvert(sonar.ping_median(), US_ROUNDTRIP_CM);
    distanceMap[i] = (ping != 0)?ping:UINT_MAX;
  }
  //escanear a la izquierda, talvez sea a la derecha pero meh...
  for(int i = rangeB - 1;i >= rangeA;i--)
  {
    servo.write(15+i*5);
    //delay(200);
    unsigned int ping = NewPingConvert(sonar.ping_median(), US_ROUNDTRIP_CM);
    distanceMap[i] = (ping != 0)?ping:UINT_MAX;
  }
  //volver al centro
  for(int i = rangeA; i < center; i++)
  {
    servo.write(15+i*5);
    //delay(200);
    unsigned int ping = NewPingConvert(sonar.ping_median(), US_ROUNDTRIP_CM);
    distanceMap[i] = (ping != 0)?ping:UINT_MAX;
  }

  detectedObjects.clear();
  int start = rangeA, end = rangeB; //variables auxiliares.
  for(int i = rangeA;i < rangeB-1; i++)
  {
    if(distanceMap[i] == UINT_MAX) //si es 0, continuar.
    {
      //si ya hay una nube procesandose
      if(start < i)
      {
        end = i-1; //ignorar el valor actual, porque no detecto nada.
        DetectedObject obj(start,end);//añadir el nuevo objeto
        detectedObjects.add(obj);
      }
      start = i+1;//continuar
      continue;
    }
    if(abs(static_cast<int>(distanceMap[i] - distanceMap[i+1])) > 15) //mayor a 15?
    {
      end = i;
      DetectedObject obj(start,end);
      detectedObjects.add(obj);
      start = i+1;
    }
  }
  //el ultimo objeto
  if(end < rangeB-1)
  {
    DetectedObject obj(start, rangeB-1);
    detectedObjects.add(obj);
  }

  Serial.print(CMD_SCAN);
  Serial.print(' ');
  for(int i=rangeA;i<rangeB-1;i++)
  {
    Serial.print(distanceMap[i]);
    Serial.print(' ');
  }
  Serial.print(distanceMap[rangeB-1]);
  Serial.print(ENDLINE);

  for(unsigned int i = 0; i < detectedObjects.lastIndex; i++)
  {
      DetectedObject &obj = detectedObjects.array[i];
      obj.centroide(distanceMap);
      Serial.print(CMD_OBJ);
      Serial.print(' ');
      Serial.print(obj.distance); //distancia
      Serial.print(' ');
      Serial.print(obj.angle); //angulo
      Serial.print(' ');
      Serial.print(obj.endIndex - obj.startIndex); //ancho
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
  sCmd.addCommand(CMD_RANGE, cmd_range);
  sCmd.setDefaultHandler(unrecognized);

  //seteo todo a 0
  memset(&distanceMap, 0, sizeof(distanceMap));
}

void loop()
{
    sCmd.readSerial();
}
