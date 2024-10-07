#include "AlgoBase.h"

class AlgoSparse : public AlgoBase
{
private:
	int seq[64] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int seqMax = 0;
	int seqInd = 0;

	int densityPercent = 10;

protected:
	bool shouldPlayGate();

public:
	void init();

	using AlgoBase::AlgoBase;
};
