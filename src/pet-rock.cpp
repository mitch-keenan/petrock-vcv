#include "stdlib.h"

#include <rack.hpp>

#include "jfunc.h"
#include "plugin.hpp"
#include "Clock.h"
#include "PhaseData.h"
#include "AlgoPlain.h"
#include "AlgoReich.h"
#include "AlgoSparse.h"
#include "AlgoVari.h"

// Create a new light size for the moon light
template <typename TBase = GrayModuleLightWidget>
struct MoonLight : TBase
{
	MoonLight()
	{
		this->box.size = mm2px(math::Vec(12, 12));
	}
};

struct PetRock : Module
{
	enum ParamId
	{
		PARAMS_LEN
	};
	enum InputId
	{
		A_INPUT,
		B_INPUT,
		INPUTS_LEN
	};
	enum OutputId
	{
		A_OUTPUT,
		B_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId
	{
		ENUMS(MOOD_LIGHT_RGB, 3),
		MOON_0_LIGHT,
		MOON_1_LIGHT,
		MOON_2_LIGHT,
		MOON_3_LIGHT,
		MOON_4_LIGHT,
		MOON_5_LIGHT,
		MOON_6_LIGHT,
		MOON_7_LIGHT,
		DAY_1_M_LIGHT,
		DAY_2_T_LIGHT,
		DAY_3_W_LIGHT,
		DAY_4_TH_LIGHT,
		DAY_5_F_LIGHT,
		DAY_6_SA_LIGHT,
		DAY_7_SU_LIGHT,
		RESET_A_LIGHT,
		STATUS_A_LIGHT,
		RESET_B_LIGHT,
		STATUS_B_LIGHT,
		LIGHTS_LEN
	};

	// Utilities Classes
	Clock *myClock;
	PhaseData myPhaseData;

	// Algo Classes
	AlgoBase *algo1;
	AlgoBase *algo2;

	// set to -1 to disable
	int testVarCycle = -1;
	int testVarEpoch = -1;
	int testVarWeekday = -1;
	bool testVarStandalone = false;
	bool testDayOffSet = false;

	bool testVarRandom = false;
	int testVarSeed = -1;

	// TIME VARS
	int curEpoch = 0;
	int seedOffset = 1;

	int today = 6;
	int curMonth = 12;
	int curYear = 22;
	int curWeekday = 3;
	int cycle = 0;
	long todaySeed = 0;

	// midnight vars
	long msTillMidnight = 0;
	int midnightAnimCounter = 500;
	int midnightAnimDelay = 500;

	// Basic phase lookup
	void phaseCheck(long days)
	{
		INFO("pet_rock - corresponding value in array: %d \n", myPhaseData.phaseDataArray[days]);
		cycle = myPhaseData.phaseDataArray[days];
	}

	void clockSetup()
	{
		myClock = new Clock;
		myClock->readClock();

		curWeekday = myClock->weekday;

		if (testVarWeekday != -1)
		{
			curWeekday = testVarWeekday;
		}

		INFO("pet_rock - weekday: %d \n", myClock->weekday);

		today = myClock->day;
		curMonth = myClock->month;
		curYear = myClock->year;

		// print the time
		INFO("pet_rock - %d : %d : %d \n", myClock->hour, myClock->minute, myClock->second);
		INFO("pet_rock - date: %d / %d, %d \n", curMonth, today, curYear);

		// adding 20 seconds as a buffer/precuation -
		// if somehow the pico ms since boot is off from the rtc time, will create an annoying infinite boot bug

		// How many milliseconds until midnight? hours converted to ms + minutes converted to ms + seconds converted to ms
		msTillMidnight = ((23 - (myClock->hour)) * 60 * 60 * 1000) + ((59 - (myClock->minute)) * 60 * 1000) + ((59 - (myClock->second)) * 1000);
		msTillMidnight = msTillMidnight + 20000;

		// use as test value`
		// msTillMidnight = 1000*5;

		struct tm tm = {0};
		time_t time1, time2;
		struct tm date1 = {0}, date2 = {0};

		// set the two dates
		date1.tm_year = 20 + 100; // year - 1900
		date1.tm_mon = 1 - 1;			// month (0-11)
		date1.tm_mday = 1;				// day of the month
		time1 = mktime(&date1);

		date2.tm_year = curYear;
		date2.tm_mon = curMonth - 1;
		date2.tm_mday = today;
		time2 = mktime(&date2);

		// get the difference in seconds
		long diff = difftime(time2, time1);

		// convert to days
		long days = (int)(diff / (60 * 60 * 24));

		// time is measured in days since jan 1 2020, since that is the first value in the lookup table
		INFO("pet_rock - days since jan 1 2020: %d ", days);

		// run the lookup table check and set
		phaseCheck(days);

		if (testVarCycle > -1)
		{
			cycle = testVarCycle;
		}

		INFO("pet_rock - cycle: %d \n", cycle);
		int cyclesPassed = 0;

		// Goes through all the moon data, when it sees a new moon counts it as one cycle passed
		for (int i = 0; i < days + 1; i++)
		{
			if (myPhaseData.phaseDataArray[i] == 0)
			{
				cyclesPassed = cyclesPassed + 1;
			}
		}

		INFO("pet_rock - cycles passed %d \n", cyclesPassed);

		// change to +3 (the +3 is just an offset so that it started on the "first" algo on launch day
		curEpoch = (cyclesPassed + 3) % 4;
		if (testVarEpoch > -1)
		{
			curEpoch = testVarEpoch;
		}

		INFO("pet_rock - epoch: %d", curEpoch);
	}

	void algoSetup()
	{
		INFO("pet_rock - Reached algo setup \n");
		INFO("pet_rock - \n");
		INFO("pet_rock - ----------\n");

		int myContinuity = jrandom(0, 100);

		switch (curEpoch)
		{
		case 1:
			INFO("pet_rock - CURRENT: Reich Algo\n");
			algo1 = new AlgoReich(1, &inputs[A_INPUT], &outputs[A_OUTPUT], &lights[RESET_A_LIGHT], &lights[STATUS_A_LIGHT], curWeekday, cycle, myContinuity);
			algo2 = new AlgoReich(2, &inputs[B_INPUT], &outputs[B_OUTPUT], &lights[RESET_B_LIGHT], &lights[STATUS_B_LIGHT], curWeekday, cycle, myContinuity);
			break;
		case 2:
			INFO("pet_rock - CURRENT: Sparse Algo\n");
			algo1 = new AlgoSparse(1, &inputs[A_INPUT], &outputs[A_OUTPUT], &lights[RESET_A_LIGHT], &lights[STATUS_A_LIGHT], curWeekday, cycle, myContinuity);
			algo2 = new AlgoSparse(2, &inputs[B_INPUT], &outputs[B_OUTPUT], &lights[RESET_B_LIGHT], &lights[STATUS_B_LIGHT], curWeekday, cycle, myContinuity);
			break;
		case 3:
			INFO("pet_rock - Current: Algo Vari \n");
			algo1 = new AlgoVari(1, &inputs[A_INPUT], &outputs[A_OUTPUT], &lights[RESET_A_LIGHT], &lights[STATUS_A_LIGHT], curWeekday, cycle, myContinuity);
			algo2 = new AlgoVari(2, &inputs[B_INPUT], &outputs[B_OUTPUT], &lights[RESET_B_LIGHT], &lights[STATUS_B_LIGHT], curWeekday, cycle, myContinuity);
			break;
		case 0: /* intentional fallthrough */
		default:
			INFO("pet_rock - CURRENT: Plain Algo\n");
			// continuity is the name of a variable that A and B object share, so that they can calculate random variables that will be the same for both of them, or related to the other objects value (ie, a random value, but B's is always half of A's)
			algo1 = new AlgoPlain(1, &inputs[A_INPUT], &outputs[A_OUTPUT], &lights[RESET_A_LIGHT], &lights[STATUS_A_LIGHT], curWeekday, cycle, myContinuity);
			algo2 = new AlgoPlain(2, &inputs[B_INPUT], &outputs[B_OUTPUT], &lights[RESET_B_LIGHT], &lights[STATUS_B_LIGHT], curWeekday, cycle, myContinuity);
			break;
		}

		algo1->init();
		// if full moon, use the same exact random seed for both A and B
		if (cycle == 4)
		{
			srand(todaySeed);
		}
		algo2->init();
	}

	void lightSetup()
	{
		setLights(0, curWeekday, cycle, curEpoch);
	}

	void reset()
	{
		clockSetup();
		lightSetup();
		todaySeed = today + curMonth + curYear + seedOffset;
		srand(testVarRandom ? testVarSeed : todaySeed);
		algoSetup();
	}

	void onReset(const ResetEvent &e) override
	{
		Module::onReset(e);
		reset();
	}

	PetRock()
	{
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(A_INPUT, "A Clock");
		configInput(B_INPUT, "B Clock");
		configOutput(A_OUTPUT, "A");
		configOutput(B_OUTPUT, "B");

		reset();
		INFO("pet_rock_init\n");
	}

	void setLights(int mode, int dow, int cycle, int epoch)
	{
		INFO("pet_rock - setLights mode: %d, dow: %d, cycle: %d, epoch: %d\n", mode, dow, cycle, epoch);

		float rVal = 0.f;
		float gVal = 0.f;
		float bVal = 0.f;

		if (mode == 0)
		{
			for (int i = 0; i <= 6; i++)
			{
				lights[DAY_1_M_LIGHT + i].setBrightness(0.f);
			}
			lights[DAY_1_M_LIGHT + dow - 1].setBrightness(1.f);

			for (int i = 0; i < 8; i++)
			{
				lights[MOON_0_LIGHT + i].setBrightness(0.f);
			}
			lights[MOON_0_LIGHT + cycle].setBrightness(1.f);

			switch (epoch)
			{
			case 0:
				rVal = 250.f;
				gVal = 70.f;
				bVal = 70.f;
				INFO("pet_rock - red");
				break;

			case 1:
				rVal = 67.f;
				gVal = 160.f;
				bVal = 255.f;
				INFO("pet_rock - blue");
				break;

			case 2:
				rVal = 255.f;
				gVal = 255.f;
				bVal = 100.f;
				INFO("pet_rock - yellow");
				break;

			case 3:
				rVal = 50.f;
				gVal = 255.f;
				bVal = 95.f;
				INFO("pet_rock - green");
				break;
			}
		}

		float rValF = rVal / 255.f;
		float gValF = gVal / 255.f;
		float bValF = bVal / 255.f;

		INFO("pet_rock - vals: R: %f, G: %f, B: %f\n", rValF, gValF, bValF);

		lights[MOOD_LIGHT_RGB].setBrightness(rValF);
		lights[MOOD_LIGHT_RGB + 1].setBrightness(gValF);
		lights[MOOD_LIGHT_RGB + 2].setBrightness(bValF);
	}

	void process(const ProcessArgs &args) override
	{
		algo1->tick(args);
		if (cycle != 0)
		{
			algo2->tick(args);
		}
	}
};

struct PetRockWidget : ModuleWidget
{
	PetRockWidget(PetRock *module)
	{
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/pet-rock.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.25, 26.5)), module, PetRock::A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.619, 108.191)), module, PetRock::B_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.937, 26.5)), module, PetRock::A_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.642, 113.535)), module, PetRock::B_OUTPUT));

		addChild(createLightCentered<MoonLight<RedGreenBlueLight>>(mm2px(Vec(11.26, 14.499)), module, PetRock::MOOD_LIGHT_RGB));

		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 57.665)), module, PetRock::DAY_1_M_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 63.75)), module, PetRock::DAY_2_T_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 69.034)), module, PetRock::DAY_3_W_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 74.029)), module, PetRock::DAY_4_TH_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 79.267)), module, PetRock::DAY_5_F_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 84.593)), module, PetRock::DAY_6_SA_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(1.25, 90.264)), module, PetRock::DAY_7_SU_LIGHT));

		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.35, 50.25)), module, PetRock::MOON_0_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.25, 54.122)), module, PetRock::MOON_1_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.25, 59.91)), module, PetRock::MOON_2_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.25, 65.448)), module, PetRock::MOON_3_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.25, 71.374)), module, PetRock::MOON_4_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.25, 77.162)), module, PetRock::MOON_5_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(13.75, 83.208)), module, PetRock::MOON_6_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(14.25, 89.302)), module, PetRock::MOON_7_LIGHT));

		addChild(createLightCentered<MediumSimpleLight<BlueLight>>(mm2px(Vec(17.765, 38.062)), module, PetRock::RESET_A_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(17.765, 38.062)), module, PetRock::STATUS_A_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<BlueLight>>(mm2px(Vec(18.668, 107.745)), module, PetRock::RESET_B_LIGHT));
		addChild(createLightCentered<MediumSimpleLight<WhiteLight>>(mm2px(Vec(18.668, 107.745)), module, PetRock::STATUS_B_LIGHT));
	}

	void appendContextMenu(Menu *menu) override
	{
		PetRock *module = getModule<PetRock>();

#ifdef PET_ROCK_DEV_MODE
		menu->addChild(new MenuSeparator);
		menu->addChild(createMenuLabel("Dev settings"));
		menu->addChild(createSubmenuItem("Force Algorithm (Mood)", "",
																		 [=](Menu *menu)
																		 {
																			 menu->addChild(createMenuItem("Plain (red)", module->testVarEpoch == 0 ? "(current)" : "", [=]()
																																		 { module->testVarEpoch = 0; module->reset(); }));
																			 menu->addChild(createMenuItem("Reich (blue)", module->testVarEpoch == 1 ? "(current)" : "", [=]()
																																		 { module->testVarEpoch = 1; module->reset(); }));
																			 menu->addChild(createMenuItem("Sparse (yellow)", module->testVarEpoch == 2 ? "(current)" : "", [=]()
																																		 { module->testVarEpoch = 2; module->reset(); }));
																			 menu->addChild(createMenuItem("Vari (green)", module->testVarEpoch == 3 ? "(current)" : "", [=]()
																																		 { module->testVarEpoch = 3; module->reset(); }));
																			 menu->addChild(createMenuItem("Use date", module->testVarEpoch == -1 ? "(current)" : "", [=]()
																																		 { module->testVarEpoch = -1; module->reset(); }));
																		 }));

		menu->addChild(createSubmenuItem("Force Day of Week", "",
																		 [=](Menu *menu)
																		 {
																			 menu->addChild(createMenuItem("Monday", module->testVarWeekday == 1 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 1; module->reset(); }));
																			 menu->addChild(createMenuItem("Tuesday", module->testVarWeekday == 2 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 2; module->reset(); }));
																			 menu->addChild(createMenuItem("Wednesday", module->testVarWeekday == 3 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 3; module->reset(); }));
																			 menu->addChild(createMenuItem("Thursday", module->testVarWeekday == 4 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 4; module->reset(); }));
																			 menu->addChild(createMenuItem("Friday", module->testVarWeekday == 5 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 5; module->reset(); }));
																			 menu->addChild(createMenuItem("Saturday", module->testVarWeekday == 6 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 6; module->reset(); }));
																			 menu->addChild(createMenuItem("Sunday", module->testVarWeekday == 7 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = 7; module->reset(); }));
																			 menu->addChild(createMenuItem("Use date", module->testVarWeekday == -1 ? "(current)" : "", [=]()
																																		 { module->testVarWeekday = -1; module->reset(); }));
																		 }));

		menu->addChild(createSubmenuItem("Force Moon Phase", "",
																		 [=](Menu *menu)
																		 {
																			 menu->addChild(createMenuItem("New Moon", module->testVarCycle == 0 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 0; module->reset(); }));
																			 menu->addChild(createMenuItem("Waxing Crescent", module->testVarCycle == 1 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 1; module->reset(); }));
																			 menu->addChild(createMenuItem("First Quarter", module->testVarCycle == 2 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 2; module->reset(); }));
																			 menu->addChild(createMenuItem("Waxing Gibbous", module->testVarCycle == 3 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 3; module->reset(); }));
																			 menu->addChild(createMenuItem("Full Moon", module->testVarCycle == 4 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 4; module->reset(); }));
																			 menu->addChild(createMenuItem("Waning Gibbous", module->testVarCycle == 5 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 5; module->reset(); }));
																			 menu->addChild(createMenuItem("Last Quarter", module->testVarCycle == 6 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 6; module->reset(); }));
																			 menu->addChild(createMenuItem("Waning Crescent", module->testVarCycle == 7 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = 7; module->reset(); }));
																			 menu->addChild(createMenuItem("Use date", module->testVarCycle == -1 ? "(current)" : "", [=]()
																																		 { module->testVarCycle = -1; module->reset(); }));
																		 }));
#endif
	}
};

Model *modelPetRock = createModel<PetRock, PetRockWidget>("pet-rock");