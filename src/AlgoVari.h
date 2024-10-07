#include "AlgoBase.h"

class AlgoVari : public AlgoBase
{
private:
    int seqA[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int seqB[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int curSeq[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool bPlaying = false;

    int seqMax = 0;
    int seqInd = 0;

    int maxRepeats = 0;
    int repeats = 0;

protected:
    bool shouldPlayGate();

public:
    void init();

    using AlgoBase::AlgoBase;
};
