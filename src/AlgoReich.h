#include "AlgoBase.h"

class AlgoReich : public AlgoBase
{
private:
  int seq[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int mute[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  int muteInd = 0;
  int seqMax;
  int seqInd = 0;

  int seqDensity = 50;

protected:
  bool shouldPlayGate();

public:
  void init();
  using AlgoBase::AlgoBase;
};
