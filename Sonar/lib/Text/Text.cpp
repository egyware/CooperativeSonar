#include "Text.h"

int toInt(char *string)
{
  if(string[0] == '\0') return 0;//si es vacio, es 0 :P aunque deberia estar malo
  int value = string[0] - '0';
  for(int i = 1; string[i] != '\0'; i++)
  {
    value = value*10 + string[i] - '0';
  }
  return value;
}

bool startWith(char *prefix, char *string)
{
  int i = 0;
  for( ; prefix[i] != '\0' && string[i] != '\0'; i++)
  {
    if(prefix[i] != string[i]) return false;
  }

  if(prefix[i] != '\0') return false;

  return true;
}
