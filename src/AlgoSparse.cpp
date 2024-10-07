#include "AlgoSparse.h"
#include "jfunc.h"

void AlgoSparse::init()
{

	switch (cyclePos)
	{
	// new moon
	case 0:
		seqMax = jrandom(10, 20);
		break;

	// waxing or waning crescent
	case 1:
	case 7:
		seqMax = jrandom(15, 30 + 1);
		break;

	// first or last quarter
	case 2:
		if (ID == 1)
		{
			seqMax = 32;
		}

		if (ID == 2)
		{
			seqMax = 64;
		}
	case 6:

		if (ID == 1)
		{
			seqMax = 24;
		}

		if (ID == 2)
		{
			seqMax = 48;
		}
		// seqMax=(20,32+1);
		break;

	// waxing or waning gibbous
	case 3:
		seqMax = 32;
		break;
	case 5:
		seqMax = 24;
		break;
	// full moon
	case 4:
		// seqMax=jrandom(20,40+1);
		seqMax = 64;
		break;
	}

	// printf("sparse week\n");
	// printf("%d\n", myWeekday);

	// densityPercent = map(myWeekday,1,7,5,15);
	densityPercent = 10;

	// percents should prob be changed per cycle
	// 10 works well for waxing gibbous

	// printf("Density %\n");
	// printf("%d\n", densityPercent);

	// seqMax = jrandom(20,31);
	// printf("Seq Max\n");
	// printf("%d\n", seqMax);
	// seqMax=32;
	// seqMax should def sometimes be set at 32 or 16, either
	// randomly or in response to time vars
	int seedStepInd = jrandom(0, seqMax);
	printf("%d\n", seedStepInd);

	seq[seedStepInd] = 1;

	for (int i = 0; i < seqMax; i++)
	{
		if (jrandom(100) < densityPercent)
		{
			// printf("step marked\n");
			// printf("%d\n", i);
			// printf("%d\n", jrandom(100));
			seq[i] = 1;
		}
	}

	// printf("Seq %d: \n", ID);
	// printf("/ / / / / / / /\n", ID);

	// for (int i = 0; i < 32; i++)
	// {
	// 	printf("%d ", seq[i]);

	// 	if ((i + 1) % 8 == 0)
	// 	{
	// 		printf("\n");
	// 	}
	// }

	// printf("\n / / / / / / / /\n", ID);
	// printf("\n", ID);
}

bool AlgoSparse::shouldPlayGate()
{
	// check for reset
	if (seqInd > seqMax - 1)
	{
		seqInd = 0;
		isResetLightOn = true;
	}

	bool result = seq[seqInd] == 1;
	seqInd++;
	return result;
}