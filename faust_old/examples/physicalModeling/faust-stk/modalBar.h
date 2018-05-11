//Modal datas for modalBar.dsp
//©Romain Michon (rmichon@ccrma.stanford.edu), 2011
//licence: STK-4.3

#include <stdio.h>
#include <stdlib.h>

float loadPreset(int preset, int index0, int index1){
	static float presets[9][4][4] = { 
		{{1.0, 3.99, 10.65, -2443},		// Marimba
			{0.9996, 0.9994, 0.9994, 0.999},
			{0.04, 0.01, 0.01, 0.008},
			{0.429688, 0.445312, 0.093750}},
		{{1.0, 2.01, 3.9, 14.37}, 		// Vibraphone
			{0.99995, 0.99991, 0.99992, 0.9999},	
			{0.025, 0.015, 0.015, 0.015 },
			{0.390625,0.570312,0.078125}},
		{{1.0, 4.08, 6.669, -3725.0},		// Agogo 
			{0.999, 0.999, 0.999, 0.999},	
			{0.06, 0.05, 0.03, 0.02},
			{0.609375,0.359375,0.140625}},
		{{1.0, 2.777, 7.378, 15.377},		// Wood1
			{0.996, 0.994, 0.994, 0.99},	
			{0.04, 0.01, 0.01, 0.008},
			{0.460938,0.375000,0.046875}},
		{{1.0, 2.777, 7.378, 15.377},		// Reso
			{0.99996, 0.99994, 0.99994, 0.9999},	
			{0.02, 0.005, 0.005, 0.004},
			{0.453125,0.250000,0.101562}},
		{{1.0, 1.777, 2.378, 3.377},		// Wood2
			{0.996, 0.994, 0.994, 0.99},	
			{0.04, 0.01, 0.01, 0.008},
			{0.312500,0.445312,0.109375}},
		{{1.0, 1.004, 1.013, 2.377},		// Beats
			{0.9999, 0.9999, 0.9999, 0.999},	
			{0.02, 0.005, 0.005, 0.004},
			{0.398438,0.296875,0.070312}},
		{{1.0, 4.0, -1320.0, -3960.0},		// 2Fix
			{0.9996, 0.999, 0.9994, 0.999},	
			{0.04, 0.01, 0.01, 0.008},
			{0.453125,0.453125,0.070312}},
		{{1.0, 1.217, 1.475, 1.729},		// Clump
			{0.999, 0.999, 0.999, 0.999},	
			{0.03, 0.03, 0.03, 0.03 },
			{0.390625,0.570312,0.078125}},
	};
	return presets[preset][index0][index1];
};