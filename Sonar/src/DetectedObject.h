#ifndef __DETECTED_OBJECT__H__
#define __DETECTED_OBJECT__H__

/**
  * Estructura que guarda los centroides, de un objeto detectado
  */
struct DetectedObject
{
  int distance;
  short int angle;
  int startIndex;
  int endIndex;

  DetectedObject()
  {
    startIndex = endIndex = 0;
  }

  DetectedObject(int start, int end)
  {
     startIndex = start;
     endIndex   = end;
  }

  void centroide(unsigned int *distanceMap)
  {
    distance = 0;
    angle = (endIndex - startIndex + 1)/2 * 5;
    for(int index = startIndex; index <= endIndex; index++)
    {
        distance = distance + distanceMap[index];
    }
    distance = distance / ( endIndex - startIndex + 1);
  }

};

#endif
