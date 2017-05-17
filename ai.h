#pragma once
#include <Axilya/AXMain.h>
#include "main.h"
#include "aistate.h"
using namespace AXGraphics;
using namespace std;

class AI
{
	private:
		float eps = 0.25;
	    int numbSimulations = 500;
        double exploreWeight = 1.45;
        int maxRollout = 8;
        AIState*  selectBestMove(AIState* initialState);
        void rollout(AIState* rolloutStart);
        void removeRolloutChildren(AIState* rolloutStart);	

	public:
		AIState* run(AIState* initialState);
};

