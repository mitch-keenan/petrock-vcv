#include "AlgoPlain.h"
#include "jfunc.h"

void AlgoPlain::init()
{
    // note I'm leaving in some older random value ranges to see what I experimented with in the past

    // Randomly populate rhythm
    // this may seems weird/cheesy, but as I note in the manual - I decided to focus on one and only one translated elements for the moon cycle, which is the length relationship of A and B - I found through practice that the random population of steps actually produces great results, the key is how many steps you use, and the relationship between the 2 step lengths. The interesting difference is comparing for example a pair of 8 step rhythms, vs a 7 step rhythm, and a 15 step rhythm being played against each other - this is the "meta movement" of the rhythmic flavor, in every algo/mood

    for (int i = 0; i < 64; i++)
    {
        seq[i] = jrandom(2);
    }

    switch (cyclePos)
    {
    // new moon
    case 0:
        seqMax = jrandom(5, 6 + 1);
        break;

    // waxing or waning crescent
    case 1:
    case 7:
        // seqMax=jrandom(7,9+1);
        seqMax = jrandom(4, 16 + 1);
        break;

    // first or last quarter
    case 2:
    case 6:
        // seqMax=jrandom(10,12+1);
        seqMax = jmap(continuity, 0, 100, 6, 12 + 1);
        seqMax *= ID; // B is Double A
        break;

    // waxing or waning gibbous
    case 3:
        seqMax = 12;
    case 5:

        // srand(seed);
        // seqMax=jrandom(13,15+1);
        seqMax = 16;
        break;

    // full moon
    case 4:
        // seqMax=jrandom(20,40+1);
        seqMax = 16;
        break;
    }

    // seqMax=jrandom(4,24);
}

bool AlgoPlain::shouldPlayGate()
{
    bool result = seq[seqInd] == 1;
    seqInd++;
    if (seqInd > seqMax - 1)
    {
        // RESET
        seqInd = 0;
        isResetLightOn = true;
    }
    return result;
}