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
        int minpruneamount = 5;
        float pruneworst = 0.25;
        AIState*  selectBestMove(AIState* initialState);
        void rollout(AIState* rolloutStart);
        void removeRolloutChildren(AIState* rolloutStart);	
        void prune(AIState* state);

	public:
		AIState* run(AIState* initialState);
};

