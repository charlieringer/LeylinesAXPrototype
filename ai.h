#pragma once
#include <Axilya/AXMain.h>
#include "aistate.h"
using namespace AXGraphics;
using namespace std;

#ifndef AI_H
#define AI_H

class AI
{
    private:
       float eps;
       int numbSimulations;
       double exploreWeight;
       int maxRollout;
       int minpruneamount;
       float pruneworst;

       AIState*  selectBestMove(AIState* initialState);
       void rollout(AIState* rolloutStart);
       void removeRolloutChildren(AIState* rolloutStart);	
       void prune(AIState* state);

    public:
      AI();
      AIState* run(AIState* initialState);
};

#endif

