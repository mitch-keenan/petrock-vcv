#pragma once
#include <rack.hpp>

class AlgoBase
{
protected:
	int ID;
	rack::engine::Input *in;
	rack::engine::Output *out;
	rack::engine::Light *resetLight;
	rack::engine::Light *statusLight;
	int continuity;
	int weekday;
	int cyclePos;

	float resetLightPhaseAccumulator = 0.f;

	bool isResetLightOn = false;
	float resetDur = 0.2f;
	float gateThreshold = 0.1f;

	float gateHighVoltage = 8.f;

	// since algos are essential gate skippers, this tracks whether the currently
	// active gate has been handled or not
	bool isGateHandled = false;
	bool isGatePlaying = false;

protected:
	/**
	 * shouldPlayGate is called automatically, once per incoming gate.
	 * If the gate should be played, return true, otherwise return false.
	 */
	virtual bool shouldPlayGate()
	{
		return false;
	};

	void updateLights(const rack::Module::ProcessArgs &args)
	{
		if (isResetLightOn)
		{
			resetLightPhaseAccumulator += args.sampleTime;
			if (resetLightPhaseAccumulator > resetDur)
			{
				resetLightPhaseAccumulator = 0.f;
				isResetLightOn = false;
			};
		}

		resetLight->setBrightness(isResetLightOn ? 1.f : 0.f);
		statusLight->setBrightness(isGatePlaying && !isResetLightOn ? 1.f : 0.f);
	}

	void updateOutput(const rack::Module::ProcessArgs &args)
	{
		out->setVoltage(isGatePlaying ? gateHighVoltage : 0.f);
	}

public:
	// Input ID (A=1, B=2), input ptr, output ptr, resetLight ptr, weekday, cycle, seed
	AlgoBase(
			int tempID,
			rack::engine::Input *tempIn,
			rack::engine::Output *tempOut,
			rack::engine::Light *tempResetLight,
			rack::engine::Light *tempStatusLight,
			int tempWeekday,
			int tempCycle,
			int tempContinuity)
	{
		// ID - 1 is A and 2 is B, using integers makes it easier to do a bit of math stuff sometimes
		ID = tempID;
		in = tempIn;
		out = tempOut;
		resetLight = tempResetLight;
		statusLight = tempStatusLight;

		weekday = tempWeekday;
		cyclePos = tempCycle;
		continuity = tempContinuity;
	};

	virtual void init() {};
	void tick(const rack::Module::ProcessArgs &args)
	{
		float inRead = in->getVoltage();
		if (inRead > gateThreshold)
		{
			if (!isGateHandled)
			{
				isGatePlaying = shouldPlayGate();
				isGateHandled = true;
			}
		}
		else
		{
			isGateHandled = false;
			isGatePlaying = false;
		}
		updateOutput(args);
		updateLights(args);
	};
};
