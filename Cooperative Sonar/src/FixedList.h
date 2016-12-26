#ifndef __FIXED_LIST_H__
#define __FIXED_LIST_H__

/**
  * Lista de tamaño fijo definido por un template.
  * Util para guardar una cantidad limitada de objetos.
  */
template <typename T, unsigned int N>
struct FixedList
{
    T array[N];
    unsigned int lastIndex;

    FixedList()
    {
      lastIndex = 0;
    }

    bool add(T &t)
    {
       if(lastIndex >= N) return false;
       array[lastIndex++] = t;
       return true;
    }

    void clear()
    {
      lastIndex = 0;
    }
};

#endif
