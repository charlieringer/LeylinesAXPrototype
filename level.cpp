#include "level.h"

Level::Level(string levelfile)
{
    AXXML levelData(levelfile);

    width = levelData.child("Level").attribute("Width").as_int();
    numbPlayerWiz = levelData.child("Level").attribute("PlayerNumbWiz").as_int();
    numbAIWiz = levelData.child("Level").attribute("AINumbWiz").as_int();

    //Parse the special board postions
    AXXMLnode boardPositions = levelData.child("Level").child("SpecialBoardPositions");
    for (AXXMLnode_iterator it = boardPositions.begin(); it != boardPositions.end(); ++it)
    {
        specialBoardPositions[it->attribute("id").as_int()] = it->attribute("value").as_int();
    }   

    //Parse the player deck
    AXXMLnode playerDeck = levelData.child("Level").child("PlayerDeckDist");
    for (AXXMLnode_iterator it = playerDeck.begin(); it != playerDeck.end(); ++it)
    {
        for (AXXMLattribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            playerDeckDistribution.push_back(ait->as_int());
        }
    }  

    //Parse the ai deck
    AXXMLnode aiDeck = levelData.child("Level").child("PlayerDeckDist");
    for (AXXMLnode_iterator it = aiDeck.begin(); it != aiDeck.end(); ++it)
    {
        for (AXXMLattribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            aiDeckDistribution.push_back(ait->as_int());
        }
    }   

    //Loads the fonts
    smallFont = new AXFont("data/Arial.ttf", 20);
    bigFont = new AXFont("data/Arial.ttf", 40);
    
    //And sets the text
    playerScoreText.setText("Player Score: " + to_string(playerScore), smallFont);
    aiScoreText.setText("AI Score: " + to_string(aiScore), smallFont);
    aiThinking.setText("AI THINKING...", smallFont);

    //Sets the X and Y offsets for the board.
    if(width%2 == 0)
    {
        xOffset = 520-(81*(width/2))+40;
        yOffset = 280-(81*(width/2))+40;

    } else {
        xOffset = 520-(81*(width/2));
        yOffset = 280-(81*(width/2));
    }

    makeBoard(); 
    makeAIDeck();
    makePlayerDeck();
    makeAIHand();
    makePlayerHand();
    //TODO: Load and make spells
    tileHScore = getTileHScore();
}

void Level::makeAIDeck()
{
    //Get the deck distribution and set it as the deck
    aiDeck = aiDeckDistribution;
    //and then shuffle it
    random_shuffle(aiDeck.begin(), aiDeck.end());
}

void Level::makePlayerDeck()
{
    //Get the deck distribution and set it as the deck
    playerDeck = playerDeckDistribution;
    //and then shuffle it
    random_shuffle(playerDeck.begin(), playerDeck.end());
}

void Level::makeAIHand()
{
    //You get 4 number tiles
    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile();
        tile->setType(getNextAITile());
        aiHand.push_back(tile);
    }

    //And 3 wizards
    for(int i = 0; i < numbAIWiz; i++)
    {            
        Tile* tile = new Tile();
        tile->setType(AIWIZ);
        tile->setDraggable(true);
        aiHand.push_back(tile);
    }

}

void Level::makePlayerHand()
{
    //You get 4 number tiles
    for(int i = 0; i < 4; i++)
    { 
        Tile* tile = new Tile(10, 118+(i*81), 80, 80);
        tile->setType(getNextPlayerTile());
        tile->setDraggable(true);
        hand.push_back(tile);
    }

    //And 3 wizards
    for(int i = 0; i < numbPlayerWiz; i++)
    {            
        Tile* tile = new Tile(10+81, 118+(i*81), 80, 80);
        tile->setType(YOURWIZ);
        tile->setDraggable(true);
        hand.push_back(tile);
    }
}

int Level::getNextPlayerTile()
{
    //If the deck is empty remake it
    if(playerDeck.size() == 0) makePlayerDeck();
    //Get the last element
    int returnValue = playerDeck.back();
    //Remove this tile
    playerDeck.pop_back();
    //And return it
    return returnValue;
}

int Level::getNextAITile()
{
    //If the deck is empty remake it
    if(aiDeck.size() == 0) makeAIDeck();
    //Get the last element
    int returnValue = aiDeck.back();
    //Remove this tile
    aiDeck.pop_back();
    //And return it
    return returnValue;
}

void Level::makeBoard()
{
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < width; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(EMPTY);
            tile->setDraggable(false);
            backBoard.push_back(tile);
        }
    }

    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < width; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(EMPTY);
            tile->setDraggable(false);
            board.push_back(tile);
        }
    }
    for(auto const &element : specialBoardPositions) {
        if(element.second == RANDOM)
        {
            board[element.first]->setType(getTileFromDistribution());
        } else {
            board[element.first]->setType(element.second);
        }
        numbPiecesPlayed++;
    }
    maxNumberOfPieces = width*width;
}

int Level::getTileFromDistribution()
{
    //Gets a random tile value from the distribution
    int randIndex = rand()%playerDeckDistribution.size();
    return playerDeckDistribution[randIndex];
}

int Level::getTileHScore()
{
    int total = 0;
    for(int i = 0; i < playerDeckDistribution.size(); i++)
    {
        total += playerDeckDistribution[i];
    }

    for(int i = 0; i < aiDeckDistribution.size(); i++)
    {
        total += aiDeckDistribution[i];
    }

    return (int)total/(playerDeckDistribution.size()+aiDeckDistribution.size());
}

void Level::update()
{
    if(numbPiecesPlayed < maxNumberOfPieces)
    {
        if(playersTurn) checkUserInput();
        else runAI();
    } else {
        if(playerScore > aiScore) gameOver.setText("You Won! Click to replay.", bigFont);
        else  if (aiScore > playerScore) gameOver.setText("You Lost! Click to replay.", bigFont);
        else gameOver.setText("You Drew! Click to replay.", bigFont);
        waitForReset(); 
    }
}

void Level::draw(){
    //Set the background colour
    setBackground(100,150,75,0);
    playerScoreText.display(10,10);
    aiScoreText.display(10,50);
    for(int i = 0; i < backBoard.size(); i++)backBoard[i]->display();
    for(int i = 0; i < board.size(); i++)board[i]->display();
    for(int i = 0; i < hand.size(); i++) hand[i]->display();

    if(!playersTurn)
    {
        fill(AXColour(255,255,255,128));
        drawRectCenter(400, 300, 300, 100);
        aiThinking.displayCentered(400, 300);
    }
    if(numbPiecesPlayed == maxNumberOfPieces)
    {
        fill(AXColour(255,255,255,128));
        drawRectCenter(400, 300, 500, 200);
        gameOver.displayCentered(400, 300);
    }
}

void Level::checkUserInput()
{
    int mX = AXInput::mouseX;
    int mY = AXInput::mouseY;

    if(!AXInput::getValue("MB1") && tileSelected)
    {
        handleDroppedTile();
    } else if(tileSelected)
    {
        selectedTile->setXCenteredOn(mX);
        selectedTile->setYCenteredOn(mY);
        return;
    } else if(AXInput::getValue("MB1"))
    {
        for(int i = 0; i < hand.size(); i++)
        {
            if(hand[i]->containsPoint(mX,mY))
            {
                selectedTile = hand[i];
                tileSelected = true;
                selectedTileSX = selectedTile->getX();
                selectedTileSY = selectedTile->getY();
            }
        }
    }
}

void Level::handleDroppedTile()
{
    int mX = AXInput::mouseX;
    int mY = AXInput::mouseY;
    tileSelected = false;
    bool tilePlaced = false;
    for(int i = 0; i < board.size(); i++)
    {
        if(board[i]->containsPoint(mX,mY) && board[i]->getType() == EMPTY)
        {
            board[i]->setType(selectedTile->getType());
            hand.erase(
                remove_if( hand.begin(), hand.end(),
                    [this](const Tile* tile){ return tile == selectedTile;}
                ),hand.end());
            
            if(selectedTile->getType() < YOURWIZ)
            {
                Tile* tile = new Tile(selectedTileSX, selectedTileSY, 80, 80);
                tile->setType(getNextPlayerTile());
                tile->setDraggable(true);
                hand.push_back(tile);
            }
            tilePlaced = true;
            playersTurn = false;
            calculateGameScore();
            numbPiecesPlayed++;
            break;
        }
    }
    if(!tilePlaced)
    {
        selectedTile->setX(selectedTileSX);
        selectedTile->setY(selectedTileSY);
    }
    selectedTile = nullptr;
}

void Level::calculateGameScore()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;

    for(int i = 0; i < board.size(); i++)
    {
        if(board[i]->getType() == YOURWIZ)
        {
            int rowStart = i-(i%width);
            int colStart = i%width;

            for(int j = 0; j < width; j++)
            {
                int current = i-j;
                if(current < rowStart) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestPlayerScore+=board[current]->getType();
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+j;
                if(current >= rowStart+width) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestPlayerScore+=board[current]->getType();
            }

            for(int j = 0; j < width; j++)
            {
                int current = i-(j*width);
                if(current < colStart) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestPlayerScore+=board[current]->getType();
                
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+(j*width);
                if(current > colStart+(width*width)-width) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestPlayerScore+=board[current]->getType();  
            }

        } else if (board[i]->getType() == AIWIZ)
        {
            int rowStart = i-(i%width);
            int colStart = i%width;

            for(int j = 0; j < width; j++)
            {
                int current = i-j;
                if(current < rowStart) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestAIScore+=board[current]->getType();
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+j;
                if(current >= rowStart+width) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestAIScore+=board[current]->getType();
            }

            for(int j = 0; j < width; j++)
            {
                int current = i-(j*width);
                if(current < colStart) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestAIScore+=board[current]->getType();
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+(j*width);
                if(current > colStart+(width*width)-width) break;
                if(board[current]->getType() == ROCK) break;
                if(board[current]->getType() < NONNUMERICTILES) latestAIScore+=board[current]->getType();  
            }
        }
    }
    playerScore = latestPlayerScore;
    aiScore = latestAIScore;

    playerScoreText.setText("Player Score: " + to_string(playerScore), smallFont);
    aiScoreText.setText("AI Score: " + to_string(aiScore), smallFont);
}

void Level::runAI()
{
    if(numbPiecesPlayed == maxNumberOfPieces) return;
    vector<int> aiHandForState;
    vector<int> playerHandForState;
    vector<int> boardForState;

    for(int i = 0; i < aiHand.size(); i++) aiHandForState.push_back(aiHand[i]->getType());
    for(int i = 0; i < hand.size();   i++) playerHandForState.push_back(UNKNOWN);
    for(int i = 0; i < board.size();  i++) boardForState.push_back(board[i]->getType());

    AIState* currentState = new AIState(1, NULL, boardForState, playerHandForState, aiHandForState, numbPiecesPlayed, tileHScore, width);
    AIState* newState = ai.run(currentState);

    for(int i = 0; i < aiHand.size(); i++) delete aiHand[i];
    for(int i = 0; i < board.size();  i++) delete board[i];
    aiHand.clear();
    board.clear();
    unpackState(*newState);
    delete currentState;
}

void Level::unpackState(AIState& newState)
{
    for(int i = 0; i < newState.getAIHand().size(); i++) 
    {
        Tile* tile = new Tile();
        if(newState.getAIHand()[i] != UNKNOWN) tile->setType(newState.getAIHand()[i]);
        else tile->setType(getNextAITile());
        tile->setDraggable(false);
        aiHand.push_back(tile);
    }

    int latestWizX = 10;
    int latestTileX = 10;

    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < width; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(newState.getBoard()[i*width+j]);
            tile->setDraggable(false);
            board.push_back(tile);
        }
    }
    numbPiecesPlayed++;
    calculateGameScore();
    playersTurn = true;
}


void Level::waitForReset()
{
    if(AXInput::getValue("MB1"))
    {
        for(int i = 0; i < aiHand.size(); i++) delete aiHand[i];
        for(int i = 0; i < hand.size(); i++) delete hand[i];
        for(int i = 0; i < board.size();  i++) delete board[i];
        numbPiecesPlayed = 1;
        aiHand.clear();
        hand.clear();
        board.clear();

        playersTurn = true;
        playerScore = 0;
        aiScore = 0;

        playerScoreText.setText("Player Score: " + to_string(playerScore), smallFont);
        aiScoreText.setText("AI Score: " + to_string(aiScore), smallFont);

        makeBoard();
        makeAIDeck();
        makePlayerDeck();
        makeAIHand();
        makePlayerHand();
    }
}