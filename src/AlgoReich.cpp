#include "AlgoReich.h"
#include "jfunc.h"

void AlgoReich::init()
{
  // printf("\n");
  // printf("ALGOOOREICH\n");

  switch (cyclePos)
  {
  // new moon
  case 0:
    seqMax = jrandom(3, 5 + 1);
    break;

  // waxing or waning crescent
  case 1:
  case 7:
    // seqMax=jrandom(6,8+1);
    // 3,5+1

    // assures A and B have different lengths
    // make sure to change both maps if you change
    if (ID == 1)
    {
      seqMax = jmap(continuity, 0, 100, 3, 8 + 1);
    }

    if (ID == 2)
    {
      int aMax = jmap(continuity, 0, 100, 3, 8 + 1);
      int bMax = 0;

      do
      {
        bMax = jrandom(3, 8 + 1);
      } while (aMax == bMax || (aMax == bMax * 2) || (bMax == aMax * 2));

      seqMax = bMax;
    }

    break;

  // first or last quarter
  case 2:
  case 6:
    // seqMax=(8,10+1);
    seqMax = jmap(continuity, 0, 100, 5, 9);
    seqMax *= ID;
    break;

    // waxing or waning gibbous
    // seqMax=4;
    // seqMax=6;
    // break;

  case 3:
  case 5:
    // srand(seed);
    // seqMax=6;
    // seqMax=7;
    seqMax = jmap(continuity, 0, 100, 4, 8);
    break;

  case 4:
    // seqMax=jrandom(20,40+1);
    seqMax = 8;
    break;
  }

  // seqMax=jrandom(4,8);

  // SETTING VARS FOR TESTING
  // myWeekday=
  // cyclePos=

  for (int i = 0; i < 32; i++)
  {
    // seq[i]=jrandom(2);
    if (jrandom(100) < seqDensity)
    {
      seq[i] = 1;
    }
  }

  bool empty = true;
  for (int i = 0; i < seqMax; i++)
  {
    if (seq[i] != 0)
    {
      empty = false;
      break;
    }
  }
  if (empty)
  {
    int rIndex = jrandom(0, seqMax);
    seq[rIndex] = 1;
  }
}

bool AlgoReich::shouldPlayGate()
{
  // CHECK MAX RESET
  if (seqInd > seqMax - 1)
  {
    seqInd = 0;
    mute[muteInd] = 1;
    muteInd++;

    if (muteInd > seqMax)
    {
      // reset
      muteInd = 0;
      for (int i = 0; i < seqMax; i++)
      {
        mute[i] = 0;
      }

      // RESET
      isResetLightOn = true;
    }
  }

  bool result = seq[seqInd] == 1 && mute[seqInd] == 1;
  seqInd++;
  return result;
}