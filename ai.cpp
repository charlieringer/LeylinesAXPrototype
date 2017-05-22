#include "ai.h"

AI::AI()
{
    //TODO: Parse from file
    //Set up the variables (this will be read from file eventually)
    eps = 0.25;
    thinkingTime = 3000;
    exploreWeight = 1.45;
    maxRollout = 8;
    minpruneamount = 60;
    pruneworst = 0.33;
}

AIState*  AI::run(AIState* initialState)
{
    vector<int> aihand = initialState->getAIHand();
    //Make the intial children
    initialState->generateChildren();
    //Start a count
    int count = 0;
    //Whilst time allows
    AXTimer timer(true);
    float startTime = timer.elapsedTime();

    while(timer.elapsedTime() < startTime+thinkingTime)
    { 
        count++; //Increment the count
        //Start at the inital state
        AIState* bestNode = initialState;
        //And loop through it's child
        while(bestNode->getNumbChildren() > 0)
        {
            //Set the scores as a base line
            float bestScore = -1;
            int bestIndex = -1;
            //If this node is not yet pruned do so
            if(!bestNode->isPruned()) prune(bestNode);
            //Loop thorugh all of the children
            int numChildren = bestNode->getNumbChildren();
            for(int i = 0; i < numChildren; i++)
            {
                vector<AIState* > currentChildren = bestNode->getChildren();
                //win score is basically just wins/games unless no games have been played, then it is 1
                float wins = currentChildren[i]->getWins();
                float games = currentChildren[i]->getTotalGames();
                float score = (games > 0) ? wins / games : 1.0;

                //UBT (Upper Confidence Bound 1 applied to trees) function balances explore vs exploit.
                //Because we want to change things the constant is configurable.
                float exploreRating = exploreWeight*sqrt((2*log(initialState->getTotalGames() + 1) / (games + 0.1)));
                //Total score is win score + explore socre
                float totalScore = score+exploreRating;
                //If the score is better update
                if (!(totalScore > bestScore)) continue;
                bestScore = totalScore;
                bestIndex = i;
            }
            //Set the best child for the next iteration
            bestNode = bestNode->getNthChild(bestIndex);
        }
        //Finally roll out this node.
        rollout(bestNode);
    }
    //Once done select the best move and return it
    return selectBestMove(initialState);
}

AIState* AI::selectBestMove(AIState* initialState)
{
    //Onces all the simulations have taken place we select the best move...
    int mostGames = -1;
    int bestMove = -1;
    //Loop through all children
    for(int i = 0; i < initialState->getNumbChildren(); i++)
    {
        //Find the one that was played the most (this is the best move as we are selecting the robust child)
        int games = initialState->getChildren()[i]->getTotalGames();
        if(games >= mostGames)
        {
            mostGames = games;
            bestMove = i;
        }
    }
    //Set that child to the next move
    return initialState->getChildren()[bestMove];
}

//Rollout function (plays random moves till it hits a termination)
void AI::rollout(AIState* rolloutStart)
{
    //If the rollout start is a terminal state
    int rolloutStartResult = rolloutStart->getWinner();
    if (rolloutStartResult >= 0)
    {
        rolloutStart->addScore(rolloutStart->getHueristicScore());
        return;
    }
    bool terminalStateFound = false;
    AIState* latest = rolloutStart;
    //Get the children
    vector<AIState*> children = rolloutStart->generateChildren();

    int loopCount = 0;
    while(!terminalStateFound)
    {
        //Loop through till a terminal state is found
        loopCount++;
        //If max roll out is hit or no childern were generated
        if (loopCount >= maxRollout || children.size() == 0) {
            rolloutStart->addScore (latest->getHueristicScore());
            break;
        }

        int index;
        float p = rand()/RAND_MAX;

        if(p < eps)
        {
            int bestScore = -INFINITY;
            int bestIndex = 0;
            for(int i = 0; i < children.size(); i ++)
            {
                if(children[i]->getHueristicScore() > bestScore)
                {
                    bestIndex = i;
                    bestScore = children[i]->getHueristicScore();
                }
            }
            index = bestIndex;
        } else {
            //Get a random child index
            index = rand() % children.size();
        }
        //and see if that node is terminal
        int endResult = children[index]->getWinner ();
        if(endResult >= 0)
        {
            terminalStateFound = true;
            rolloutStart->addScore(latest->getHueristicScore());
        } else {
            latest = children [index];
            //Otherwise select that nodes as the childern and continue
            children = children [index]->generateChildren();

        }
    }
    rolloutStart->removeChildren();
}

void AI::prune(AIState* state)
{
    state->setPruned(true);
    int totNumbChildren = state->getNumbChildren();
    if(totNumbChildren < minpruneamount|| pruneworst == 0)
    {
        return;
    }
    state->removeWorstNChildren(floor(totNumbChildren*pruneworst));
}