//
//  Leylines protoype - Charlie Ringer - Copyright © 2017 cring002.github.io. All rights reserved.
//  
//  Axilya used with an MIT License - github.com/vexparadox/axilya
//
#include <Axilya/AXMain.h>
#include "level.h"

using namespace std;

Level* currentLevel;

void setup()
{
    //Set the random seed
    srand (time(NULL));
    //Load all of the tile textures
    Tile::loadTextures();
    //Create a new level
    currentLevel = new Level();
    //And set it up
    currentLevel->setup();
}

void update(){
    //Update the current level
    currentLevel->update();
}

void draw(){
    //Draw the current level
    currentLevel->draw();
}

int main(int argc, char *argv[])
{
    //Make a new AX Window
    if(!AXWindow::init(860, 640, "Leyline Prototype", AX_DEFAULT, update, draw)){
        AXLog::log("Window failed to initialise", "Quitting", AX_LOG_ERROR);
        return -1;
    }
    //Set up the data I need to
    setup();
    //return the window run, this is where the update and draw will be called
    return AXWindow::run();
}
