/* ----------------------------------------------------------------------------
     Pipes  by  Simon Holmes, 2017
   ----------------------------------------------------------------------------
*/
#include <Arduboy2.h>
#include "Images.h"
#include "Puzzles.h"

#define NOTHING                      0
#define PIPE_HORIZONTAL_LR           1
#define PIPE_HORIZONTAL_RL           2
#define PIPE_VERTICAL_TB             3
#define PIPE_VERTICAL_BT             4
#define PIPE_CORNER_TL               5
#define PIPE_CORNER_LT               6
#define PIPE_CORNER_TR               7
#define PIPE_CORNER_RT               8
#define PIPE_CORNER_BL               9
#define PIPE_CORNER_LB               10
#define PIPE_CORNER_BR               11
#define PIPE_CORNER_RB               12
#define NODE                         15

#define GRID_WIDTH                   11
#define GRID_HEIGHT                  11

#define UP                           0
#define DOWN                         1
#define LEFT                         2
#define RIGHT                        3

#define STATE_INTRO                  0
#define STATE_INIT_GAME              1
#define STATE_NO_SELECTION           2
#define STATE_NODE_SELECTED          3
#define STATE_GAME_OVER              4

Arduboy2 arduboy;
Sprites sprites;


/* ----------------------------------------------------------------------------
 *   Basic structure to hold a node value and coordinates ..
 */
struct Node {
  byte x;
  byte y;
  byte value;
};


/* ----------------------------------------------------------------------------
 *  Player structure to record the highlighted and selected node ..
 */
struct Player {
  Node highlightedNode;
  Node selectedNode;
}
player;


/* ----------------------------------------------------------------------------
 *   Puzzle structure.
 */
struct Puzzle {
  byte index;                             // Puzzle number.
  Node maximum;                           // Used to store the dimensions of the puzzle based
                                          // on the puzzle level, eg. PUZZLE_5X5 has a maximum
                                          // x and y value of 5.
  byte board[9][9];                       // Actual board details - supports maximum of 9 x 9.
}
puzzle;

const byte* nodes[] = {0, node_1, node_2, node_3, node_4, node_5, node_6, node_7, node_8, node_9, node_10 };
const byte* pipes[] = {pipe_nothing, pipe_horizontal, pipe_horizontal, pipe_vertical, pipe_vertical, pipe_corner_TL, pipe_corner_TL,
                       pipe_corner_TR, pipe_corner_TR, pipe_corner_BL, pipe_corner_BL, pipe_corner_BR, pipe_corner_BR};
                       
byte frame = 0;
byte gameState = STATE_INTRO;


/* ----------------------------------------------------------------------------
 *     Initialise the Arduboy and get ready ..
 */
void setup() {

  arduboy.begin();
  arduboy.clear();

  puzzle.index = 0;
  puzzle.maximum.x = 5;
  puzzle.maximum.y = 5;

}


/* ----------------------------------------------------------------------------
 *   Play pipes !
 */
void loop() {

  arduboy.pollButtons();

  switch (gameState) {

    case STATE_INTRO:
      drawSplash();
      break;

    case STATE_INIT_GAME:
      play_InitGame();
      break;

    case STATE_NO_SELECTION:
      play_NoSelection();
      break;

    case STATE_NODE_SELECTED:
      play_NodeSelected();
      break;

    case STATE_GAME_OVER:
      play_GameOver();
      break;

  }

  arduboy.display();

}



