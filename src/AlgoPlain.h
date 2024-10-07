#include "AlgoBase.h"

#include "jfunc.h"
class AlgoPlain : public AlgoBase
{
private:
	int seq[64] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int seqMax = 0;
	int seqInd = 0;

protected:
	bool shouldPlayGate();

public:
	void init();

	using AlgoBase::AlgoBase;
};
