#pragma once
#include <Axilya/AXMain.h>
#include "main.h"
#include "aistate.h"
using namespace AXGraphics;
using namespace std;

class AI
{
	private:
	    int numbSimulations = 100;
        double exploreWeight = 1.45;
        int maxRollout = 5;

	public:
		AIState* run(AIState* initialState);
		void rollout(AIState* rolloutStart);	
};

