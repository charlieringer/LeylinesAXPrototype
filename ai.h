#pragma once
#include <Axilya/AXMain.h>
#include "aistate.h"
using namespace AXGraphics;
using namespace std;

#ifndef AI_H
#define AI_H

//Class
class AI
{
    private:
       float eps = 0.0; // Epsilon (for E-Greedy rollout)
       int thinkingTime = 0; // The amount of time the agent has to think
       float exploreWeight = 0; //How much to weight exploring new moves
       int maxRollout = 0; //How long the rollout is
       int minpruneamount = 0; //At what point does the AI stop pruning 
       float pruneworst = 0; //How % of moves to prune

       AIState*  selectBestMove(AIState* initialState); //Finds the best move (at the end of MCTS)
       void rollout(AIState* rolloutStart); //Simulates a game
       void removeRolloutChildren(AIState* rolloutStart);	//Removes the children which were part of the rollout
       void prune(AIState* state); //Removes child nodes from the supplided node (based on the pruneworst value)

    public:
      AI(); //Constructor
      AIState* run(AIState* initialState); //Takes an initial state, runs the MCTS and then returns the best child move.
};

#endif

