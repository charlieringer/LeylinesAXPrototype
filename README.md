Prototype of Leylines in Axilya
---------------------------

Build & Run
-----------
- First install Axilya (see it's Github page)
- To build (via command line) naviagate to dir and then enter "make".
- To run (via command line) naviagate to dir and then enter "run".
- To build then run (via command line) naviagate to dir and then enter "make run".

How to play
-----------
- You are the blue wizards
- The AI is the red wizards
- Each turn choose to either place a wizard or a numbered tile
- If you place a numbered tile you will draw a new one
- Use the mouse to play tiles by clicking and dragging them
- Each wizard is worth points equal to the total value in the row and column it is in
- Your total score is the score for all Wizards.
- The game end when the board is full, most points wins

The AI
------
- The AI uses MCTS with short rollouts for speed 
- It then uses the score at the end of the rollout as a scoring hueristic (the sigmoid 0f the difference in scores)
- It also used an epsilon-greedy rollout policy to quide it to strong choices faster

Axilya
------
- This game is made using Axylia
- Check it out here: https://github.com/vexparadox/Axilya

Todo
----
- Spells
