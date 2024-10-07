#include "stdlib.h"
#include "jfunc.h"

long jrandom(long howbig)
{
  if (howbig == 0)
  {
    return 0;
  }
  return arc4random() % howbig;
}

long jrandom(long howsmall, long howbig)
{
  if (howsmall >= howbig)
  {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return jrandom(diff) + howsmall;
}

long jmap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
