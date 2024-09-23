#include "plugin.hpp"

struct Pet_rock : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		A_INPUT,
		B_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		A_OUTPUT,
		B_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		MOOD_LIGHT,
		MOON_1_LIGHT,
		MOON_2_LIGHT,
		MOON_3_LIGHT,
		MOON_4_LIGHT,
		MOON_5_LIGHT,
		MOON_6_LIGHT,
		MOON7_LIGHT,
		DAY_1_M_LIGHT,
		DAY_2_T_LIGHT,
		DAY_3_W_LIGHT,
		DAY_4_TH_LIGHT,
		DAY_5_F_LIGHT,
		DAY_6_SA_LIGHT,
		DAY_7_SU_LIGHT,
		LIGHTS_LEN
	};

	Pet_rock() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(A_INPUT, "");
		configInput(B_INPUT, "");
		configOutput(A_OUTPUT, "");
		configOutput(B_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct Pet_rockWidget : ModuleWidget {
	Pet_rockWidget(Pet_rock* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/pet-rock.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.25, 26.5)), module, Pet_rock::A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.619, 108.191)), module, Pet_rock::B_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.937, 26.5)), module, Pet_rock::A_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.642, 113.535)), module, Pet_rock::B_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(11.26, 14.499)), module, Pet_rock::MOOD_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(14.25, 54.122)), module, Pet_rock::MOON_1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 57.665)), module, Pet_rock::DAY_1_M_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(14.25, 59.91)), module, Pet_rock::MOON_2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 63.75)), module, Pet_rock::DAY_2_T_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(14.25, 65.448)), module, Pet_rock::MOON_3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 69.034)), module, Pet_rock::DAY_3_W_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(14.25, 71.374)), module, Pet_rock::MOON_4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 74.029)), module, Pet_rock::DAY_4_TH_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(14.25, 77.162)), module, Pet_rock::MOON_5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 79.267)), module, Pet_rock::DAY_5_F_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.75, 83.208)), module, Pet_rock::MOON_6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 84.593)), module, Pet_rock::DAY_6_SA_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(14.25, 89.302)), module, Pet_rock::MOON7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(1.25, 90.264)), module, Pet_rock::DAY_7_SU_LIGHT));
	}
};


Model* modelPet_rock = createModel<Pet_rock, Pet_rockWidget>("pet-rock");