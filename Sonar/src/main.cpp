#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
//#include <Timer.h>

#include "Text.h"

#define CMD_HELLO ":HELLO"
#define CMD_ERROR ":ERROR"
#define CMD_SERVO ":SERVO"
#define CMD_OK    ":OK"
#define CMD_SONAR ":SONAR"
#define CMD_SCAN  ":SCAN"
#define CMD_SCANFIRST ":SCAN+FIRST"
#define CMD_RANGE ":RANGE"
#define CMD_OBJ ":OBJ"
#define ENDLINE "\r\n"

#define MAX_LEN 128
#define DISTANCEMAP_LEN 33 //todo calza

Servo servo;
NewPing sonar(2,3);
int distanceMap[33];
int distanceAux;

char commandLine[MAX_LEN];
byte index = 0;



void setup()
{
  Serial.begin(9600);
  servo.attach(9);
}

void loop()
{
    //va guardando los bytes entrantes en el buffer
    if(Serial.available())
    {
      //protección ante desborde
      if(index >= MAX_LEN)
      {
        index = 0;
      }
      int incomingByte = Serial.read();
      commandLine[index++] = incomingByte;
      commandLine[index] = 0;
    }
    //verificamos si termino de enviar datos por ascii verificando si termina con \r\n
    if(index > 2 && '\r' == commandLine[index-2] && '\n' == commandLine[index-1])
    {
      bool error = true;
      commandLine[index-2] = '\0'; //solo lo acortamos para hacer mejor el parsing
      //verificar si empieza con :
      if(':' == commandLine[0])
      {
        //para saber si funciona el coso.
        if(startWith(CMD_HELLO, commandLine))
        {
          Serial.print(CMD_HELLO);
          Serial.print(ENDLINE);
          error = false;
        }
        else
        if(startWith(CMD_SONAR, commandLine))
        {
          //pregunta el estado del servo
          if('?' == commandLine[6])
          {
            Serial.print(CMD_SONAR);
            Serial.print('=');
            Serial.print(sonar.ping_cm());
            Serial.print(ENDLINE);
            error = false;
          }
        }
        else
        if(startWith(CMD_SERVO, commandLine))
        {
          //pregunta el estado del servo
          if('?' == commandLine[6])
          {
            Serial.print(CMD_SERVO);
            Serial.print('=');
            Serial.print(servo.read());
            Serial.print(ENDLINE);
            error = false;
          }
          else
          //cambia el estado del servo
          if('=' == commandLine[6])
          {
            int value = toInt(&commandLine[7]);
            servo.write(value);
            Serial.print(CMD_OK);
            Serial.print(ENDLINE);
            error = false;
          }
        }
        else
        if(startWith(CMD_SCANFIRST, commandLine))
        {
          if(servo.read() <= 90)
          {
            for(int i=0;i<DISTANCEMAP_LEN-1;i++)
            {
              servo.write(15+i*5);
              delay(200);
              distanceMap[i] = sonar.ping_cm();
            }
          }
          else
          {
            for(int i=DISTANCEMAP_LEN-1;i>0;i--)
            {
              servo.write(15+i*5);
              delay(200);
              distanceMap[i] = sonar.ping_cm();
            }
          }

          Serial.print(CMD_SCAN);
          Serial.print('=');
          for(int i=0;i<DISTANCEMAP_LEN-2;i++)
          {
            Serial.print(distanceMap[i]);
            Serial.print(';');
          }
          Serial.print(distanceMap[DISTANCEMAP_LEN-1]);
          Serial.print(ENDLINE);
          error = false;
        }
        else
        if(startWith(CMD_SCAN, commandLine))
        {
          if(servo.read() <= 90)
          {
            for(int i=0;i<DISTANCEMAP_LEN-1;i++)
            {
              servo.write(15+i*5);
              delay(200);
              distanceAux = sonar.ping_cm();
              if(abs(distanceMap[i] - distanceAux) > 0)
              {
                  Serial.print(CMD_OBJ);
                  Serial.print('=');
                  Serial.print(i);
                  Serial.print(';');
                  Serial.print(distanceAux);
                  Serial.print(';');
                  Serial.print(distanceMap[i]);
                  Serial.print(ENDLINE);
              }
              distanceMap[i] = distanceAux;
            }
          }
          else
          {
            for(int i=DISTANCEMAP_LEN-1;i>0;i--)
            {
              servo.write(15+i*5);
              delay(200);
              distanceAux = sonar.ping_cm();
              if(abs(distanceMap[i] - distanceAux) > 0)
              {
                  Serial.print(CMD_OBJ);
                  Serial.print('=');
                  Serial.print(i);
                  Serial.print(';');
                  Serial.print(distanceAux);
                  Serial.print(';');
                  Serial.print(distanceMap[i]);
                  Serial.print(ENDLINE);
              }
              distanceMap[i] = distanceAux;
            }
          }

          Serial.print(CMD_SCAN);
          Serial.print('=');
          for(int i=0;i<DISTANCEMAP_LEN-2;i++)
          {
            Serial.print(distanceMap[i]);
            Serial.print(';');
          }
          Serial.print(distanceMap[DISTANCEMAP_LEN-1]);
          Serial.print(ENDLINE);
          error = false;
        }
      }

      if(error)
      {
        Serial.print(CMD_ERROR);
        Serial.print(ENDLINE);
      }
      //reseteamos el buffer
      index = 0;
    }
}
