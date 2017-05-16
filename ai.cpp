#include "ai.h"

AIState*  AI::run(AIState* initialState)
{
    //Make the intial children
    initialState->generateChildren ();
    //Start a count
    int count = 0;
    //Whilst time allows
    while(count < numbSimulations)
    {
        //Increment the count
        count++;
        //Start at the inital state
        AIState* bestNode = initialState;
        //And loop through it's child
        while(bestNode->children.size() > 0)
        {
            //Set the scores as a base line
            double bestScore = -1;
            int bestIndex = -1;
            //Loop thorugh all of the children
            for(int i = 0; i < bestNode->children.size(); i++)
            {
                //win score is basically just wins/games unless no games have been played, then it is 1
                double wins = bestNode->children[i]->wins;
                double games = bestNode->children[i]->totGames;
                double score = (games > 0) ? wins / games : 1.0;

                //UBT (Upper Confidence Bound 1 applied to trees) function balances explore vs exploit.
                //Because we want to change things the constant is configurable.
                double exploreRating = exploreWeight*sqrt((2*log(initialState->totGames + 1) / (games + 0.1)));
                //Total score is win score + explore socre
                double totalScore = score+exploreRating;
                //If the score is better update
                if (!(totalScore > bestScore)) continue;
                bestScore = totalScore;
                bestIndex = i;
            }
            //Set the best child for the next iteration
            bestNode = bestNode->children[bestIndex];
        }
        //Finally roll out this node.
        rollout(bestNode);
    }

    //Onces all the simulations have taken place we select the best move...
    int mostGames = -1;
    int bestMove = -1;
    //Loop through all children
    for(int i = 0; i < initialState->children.size(); i++)
    {
        //Find the one that was played the most (this is the best move as we are selecting the robust child)
        int games = initialState->children[i]->totGames;
        if(games >= mostGames)
        {
            mostGames = games;
            bestMove = i;
        }
    }
    //Set that child to the next move
    return initialState->children[bestMove];
}

//Rollout function (plays random moves till it hits a termination)
void AI::rollout(AIState* rolloutStart)
{
    //If the rollout start is a terminal state
    int rolloutStartResult = rolloutStart->getWinner();
    if (rolloutStartResult >= 0)
    {
        //Add a win is it is a win, or a loss is a loss or otherwise a draw
        if(rolloutStartResult == rolloutStart->playerIndex) rolloutStart->addScore(1);
        else if(rolloutStartResult == (rolloutStart->playerIndex+1)%2) rolloutStart->addScore(0);
        return;
    }
    bool terminalStateFound = false;
    //Get the children
    vector<AIState*> children = rolloutStart->generateChildren();

    int loopCount = 0;
    while(!terminalStateFound)
    {
        //Loop through till a terminal state is found
        loopCount++;
        //If max roll out is hit or no childern were generated
        if (loopCount >= maxRollout || children.size() == 0) {
            //Record a draw
            rolloutStart->addScore (0.5);
            break;
        }
        //Get a random child index
        int index = rand() % children.size();
        //and see if that node is terminal
        int endResult = children[index]->getWinner ();
        if(endResult >= 0)
        {
            terminalStateFound = true;
            if (endResult == 2) rolloutStart->addScore(0.5);
            //If it is a win add a win
            else if(endResult == rolloutStart->playerIndex) rolloutStart->addScore(1);
            //Else add a loss
            else rolloutStart->addScore(0);
        } else {
            //Otherwise select that nodes as the childern and continue
            children = children [index]->generateChildren();
        }
    }
    // //Reset the children as these are not 'real' children but just ones for the roll out.
    // foreach( AIState child in rolloutStart.children)
    // {
    //     child.children = new List<AIState>();
    // }
}