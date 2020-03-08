#include <Arduino.h>
#line 1 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino"
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
#line 85 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino"
void setup();
#line 100 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino"
void loop();
#line 4 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
void play_InitGame();
#line 21 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
void play_NoSelection();
#line 64 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
void play_NodeSelected();
#line 382 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
void updatePipeWhenReversing(byte x, byte y);
#line 426 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
bool validMove(byte direction, Node selectedNode, int x, int y);
#line 508 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
void play_GameOver();
#line 4 "/Users/simonholmes/projects/Pipes_Article2/Render.ino"
void renderBoard();
#line 1 "/Users/simonholmes/projects/Pipes_Article2/Splash.ino"
void drawSplash();
#line 4 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
byte leftValue(byte val);
#line 14 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
byte rightValue(byte val);
#line 24 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void initBoard(byte puzzleNumber);
#line 66 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
bool nodeAlreadyPlayed(byte value);
#line 90 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void clearBoard(byte nodeValue);
#line 112 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
bool isNode(byte x, byte y);
#line 122 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
byte getNodeValue(byte x, byte y);
#line 132 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
bool isPipe(byte x, byte y);
#line 142 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
byte getPipeValue(byte x, byte y);
#line 152 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void setCellValue(byte x, byte y, byte pipeValue, byte nodeValue);
#line 162 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void clearSelection();
#line 174 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void clearHighlightAndSelection();
#line 188 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
bool isPuzzleComplete();
#line 213 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void toggleSoundSettings();
#line 236 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
void drawHorizontalDottedLine(int x1, int x2, int y);
#line 85 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino"
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




#line 1 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
/* ----------------------------------------------------------------------------
 *   Initialise the game based on the selected level and puzzle numbers.
 */
void play_InitGame() {

  arduboy.clear();
  initBoard(puzzle.index);
  gameState = STATE_NO_SELECTION;
  
}


/* ----------------------------------------------------------------------------
 *   Handle the game play before the user selects a node.
 *   
 *   The user can scroll to any node on the board using the directional arrows.
 *   Clicking the A button on a node selects it.  Clicking the B button returns
 *   the user to the level select (if currently on the first puzzle) or the 
 *   puzzle select screen (if they are currently working on a puzzle).
 */
void play_NoSelection() {

  if (arduboy.justPressed(LEFT_BUTTON) && player.highlightedNode.x > 0)                         { player.highlightedNode.x--; }
  if (arduboy.justPressed(RIGHT_BUTTON) && player.highlightedNode.x < puzzle.maximum.x - 1)     { player.highlightedNode.x++; }
  if (arduboy.justPressed(UP_BUTTON) && player.highlightedNode.y > 0)                           { player.highlightedNode.y--; }
  if (arduboy.justPressed(DOWN_BUTTON) && player.highlightedNode.y < puzzle.maximum.y - 1)      { player.highlightedNode.y++; }

  if (arduboy.justPressed(A_BUTTON) && isNode(player.highlightedNode.x, player.highlightedNode.y)) {

    if (nodeAlreadyPlayed(getNodeValue(player.highlightedNode.x, player.highlightedNode.y))) {

      clearBoard(getNodeValue(player.highlightedNode.x, player.highlightedNode.y));
      player.selectedNode.value = getNodeValue(player.highlightedNode.x, player.highlightedNode.y);
      player.selectedNode.x = player.highlightedNode.x;
      player.selectedNode.y = player.highlightedNode.y;
      gameState = STATE_NODE_SELECTED;

    }
    else {

      player.selectedNode.value = getNodeValue(player.highlightedNode.x, player.highlightedNode.y);
      player.selectedNode.x = player.highlightedNode.x;
      player.selectedNode.y = player.highlightedNode.y;
      gameState = STATE_NODE_SELECTED;

    }

  }
  
  renderBoard();
  
}


/* ----------------------------------------------------------------------------
 *   Handle the game play after the user selects a node.
 *   
 *   The user can only navigate through empty cells or onto the node that matches
 *   the one previously selected.  They can also back-track along the pipe they
 *   have already layed.
 *   
 *   Pressing the B button will clear the pipe that is currently being layed.
 */
void play_NodeSelected() {

  if (arduboy.justPressed(LEFT_BUTTON)) {
    
    if (validMove(LEFT, player.selectedNode, player.highlightedNode.x - 1, player.highlightedNode.y)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case PIPE_HORIZONTAL_LR:
        case PIPE_CORNER_TR:
        case PIPE_CORNER_BR:
          updatePipeWhenReversing(player.highlightedNode.x - 1, player.highlightedNode.y);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case PIPE_HORIZONTAL_RL:
        case PIPE_CORNER_TL:
        case PIPE_CORNER_BL:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, PIPE_HORIZONTAL_RL, player.selectedNode.value); }
          break;

        case PIPE_CORNER_LT:
        case PIPE_CORNER_RT:
        case PIPE_VERTICAL_BT:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, PIPE_HORIZONTAL_RL, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_BL, player.selectedNode.value);
          break;

        case PIPE_CORNER_LB:
        case PIPE_CORNER_RB:
        case PIPE_VERTICAL_TB:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, PIPE_HORIZONTAL_RL, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_TL, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, PIPE_HORIZONTAL_RL, player.selectedNode.value);
          break;

      }
  
  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x - 1 == player.selectedNode.x && player.highlightedNode.y == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
          
      }
  
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x - 1, player.highlightedNode.y)  && getNodeValue(player.highlightedNode.x - 1, player.highlightedNode.y) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;
 

        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
                 
      }  
      
      player.highlightedNode.x--;
      
    }
    
  }

  if (arduboy.justPressed(RIGHT_BUTTON)) {

    if (validMove(RIGHT, player.selectedNode, player.highlightedNode.x + 1, player.highlightedNode.y)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case PIPE_HORIZONTAL_RL:
        case PIPE_CORNER_TL:
        case PIPE_CORNER_BL:
          updatePipeWhenReversing(player.highlightedNode.x + 1, player.highlightedNode.y);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case PIPE_HORIZONTAL_LR:
        case PIPE_CORNER_TR:
        case PIPE_CORNER_BR:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, PIPE_HORIZONTAL_LR, player.selectedNode.value); }
          break;

        case PIPE_CORNER_RT:
        case PIPE_CORNER_LT:
        case PIPE_VERTICAL_BT:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, PIPE_HORIZONTAL_LR, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_BR, player.selectedNode.value);
          break;

        case PIPE_CORNER_RB:
        case PIPE_CORNER_LB:
        case PIPE_VERTICAL_TB:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, PIPE_HORIZONTAL_LR, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_TR, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, PIPE_HORIZONTAL_LR, player.selectedNode.value);
          break;

      }
  
  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x + 1 == player.selectedNode.x && player.highlightedNode.y == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
          
      }
  
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x + 1, player.highlightedNode.y)  && getNodeValue(player.highlightedNode.x + 1, player.highlightedNode.y) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;
  

        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
                
      }
    
      player.highlightedNode.x++;
  
    }

  }

  if (arduboy.justPressed(UP_BUTTON)) {
    
    if (validMove(UP, player.selectedNode, player.highlightedNode.x, player.highlightedNode.y - 1)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case PIPE_VERTICAL_TB:
        case PIPE_CORNER_RB:
        case PIPE_CORNER_LB:
          updatePipeWhenReversing(player.highlightedNode.x, player.highlightedNode.y - 1);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case PIPE_VERTICAL_BT:
        case PIPE_CORNER_LT:
        case PIPE_CORNER_RT:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, PIPE_VERTICAL_BT, player.selectedNode.value); }
          break;

        case PIPE_CORNER_TR:
        case PIPE_CORNER_BR:
        case PIPE_HORIZONTAL_LR:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, PIPE_VERTICAL_BT, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_LT, player.selectedNode.value); 
          break;

        case PIPE_CORNER_TL:
        case PIPE_CORNER_BL:
        case PIPE_HORIZONTAL_RL:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, PIPE_VERTICAL_BT, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_RT, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, PIPE_VERTICAL_BT, player.selectedNode.value);
          break;

      }
  
  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x == player.selectedNode.x && player.highlightedNode.y - 1 == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
         
      }
  
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x, player.highlightedNode.y - 1)  && getNodeValue(player.highlightedNode.x, player.highlightedNode.y - 1) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;
 

        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
                 
      }  
    
      player.highlightedNode.y--;
    }

  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    
    if (validMove(DOWN, player.selectedNode, player.highlightedNode.x, player.highlightedNode.y + 1)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case PIPE_VERTICAL_BT:
        case PIPE_CORNER_RT:
        case PIPE_CORNER_LT:
          updatePipeWhenReversing(player.highlightedNode.x, player.highlightedNode.y + 1);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case PIPE_VERTICAL_TB:
        case PIPE_CORNER_LB:
        case PIPE_CORNER_RB:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, PIPE_VERTICAL_TB, player.selectedNode.value); }
          break;

        case PIPE_CORNER_TR:
        case PIPE_CORNER_BR:
        case PIPE_HORIZONTAL_LR:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, PIPE_VERTICAL_TB, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_LB, player.selectedNode.value);
          break;

        case PIPE_CORNER_TL:
        case PIPE_CORNER_BL:
        case PIPE_HORIZONTAL_RL:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, PIPE_VERTICAL_TB, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, PIPE_CORNER_RB, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, PIPE_VERTICAL_TB, player.selectedNode.value);
          break;
        
      }

  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x == player.selectedNode.x && player.highlightedNode.y + 1 == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
          
      }
   
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x, player.highlightedNode.y + 1)  && getNodeValue(player.highlightedNode.x, player.highlightedNode.y + 1) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;


        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
  
      }  
   
      player.highlightedNode.y++;

    }

  }

  if (arduboy.justPressed(B_BUTTON)) {

    clearBoard(player.selectedNode.value);
    clearSelection();
    gameState = STATE_NO_SELECTION;
      
  }


  renderBoard();
  
}


/* ----------------------------------------------------------------------------
 *   Fix the pipe graphics when reversing back on yourself.
 */
void updatePipeWhenReversing(byte x, byte y) {

  switch (getPipeValue(x, y)) {

    case PIPE_CORNER_TR:           
    case PIPE_CORNER_TL:                   
    case PIPE_VERTICAL_TB:                 
      setCellValue(x, y, PIPE_VERTICAL_TB, player.selectedNode.value);
      break;

    case PIPE_CORNER_BR:          
    case PIPE_CORNER_BL:          
    case PIPE_VERTICAL_BT:        
      setCellValue(x, y, PIPE_VERTICAL_BT, player.selectedNode.value);
      break;

    case PIPE_CORNER_LT:
    case PIPE_CORNER_LB:
    case PIPE_HORIZONTAL_LR:
      setCellValue(x, y, PIPE_HORIZONTAL_LR, player.selectedNode.value);
      break;
           
    case PIPE_CORNER_RT:                   
    case PIPE_CORNER_RB: 
    case PIPE_HORIZONTAL_RL:        
      setCellValue(x, y, PIPE_HORIZONTAL_RL, player.selectedNode.value);
      break;
 
  }
 
}


/* ----------------------------------------------------------------------------
 *   Is the nominated move valid?
 *   
 *   The user can only navigate through empty cells or onto the node that matches
 *   the one previously selected.  They can also back-track along the pipe they
 *   have already layed.
 *   
 *   direction:     The button the user clicked, eg. UP, DOWN, LEFT and Right.
 *   selectedNode:  The node previously selected.
 *   x and y:       Coordinates to test.  Passed as char to allow negatives.
 */
bool validMove(byte direction, Node selectedNode, int x, int y) {

  
  // Off the grid!

  if (x < 0 || x >= puzzle.maximum.x || y < 0 || y >= puzzle.maximum.y) return false;

  
  // Is it a clear cell or the matching node?
  
  if (
      (!isNode(x,y) && getPipeValue(x,y) == NOTHING) ||
      (isNode(x,y) && getNodeValue(x,y) == selectedNode.value && (x != selectedNode.x || y != selectedNode.y))
     ) return true;
  

  // Is the pipe turning back on itself?

  switch (direction) {

    case (UP):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case PIPE_VERTICAL_TB:
        case PIPE_CORNER_RB:
        case PIPE_CORNER_LB:
          return true;

      }

      break;
      
    case (DOWN):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {
          
        case PIPE_VERTICAL_BT:
        case PIPE_CORNER_LT:
        case PIPE_CORNER_RT:
          return true;

      }

      break;

    case (LEFT):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {
   
        case PIPE_CORNER_TR:
        case PIPE_CORNER_BR:
        case PIPE_HORIZONTAL_LR:
          return true;

      }

      break;

    case (RIGHT):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case PIPE_CORNER_TL:
        case PIPE_CORNER_BL:
        case PIPE_HORIZONTAL_RL:
          return true;

      }

      break;

  }
  
  return false;
  
}


/* ----------------------------------------------------------------------------
 *   Place holder for the Game Over function.
 */
void play_GameOver() {

  delay(10000);
  gameState = STATE_INTRO;
  
}


#line 1 "/Users/simonholmes/projects/Pipes_Article2/Render.ino"
/* ----------------------------------------------------------------------------
 *   Render the board.
 */
void renderBoard() { 

  arduboy.clear();


  // Draw placed pipes ..
  
  for (int y = 0; y < puzzle.maximum.y; y++) {
      
    for (int x = 0; x < puzzle.maximum.x; x++) {
      
      if (isPipe(x,y)) {
        
        sprites.drawExternalMask(x * GRID_WIDTH, y * GRID_HEIGHT, pipes[getPipeValue(x, y)], pipe_mask, frame, frame);

      }
        
    }
      
  }



   // Draw nodes ..
  
  for (int y = 0; y < puzzle.maximum.y; y++) {
      
    for (int x = 0; x < puzzle.maximum.x; x++) {
      
      if (isNode(x, y)) {
        
        sprites.drawOverwrite(x * GRID_WIDTH + 2, y * GRID_HEIGHT + 2, nodes[getNodeValue(x,y)], frame);
        
      }
        
    }
      
  }

  
  // Draw grid marks ..

  for (int y = 0; y <= puzzle.maximum.y; y++) {
  
    for (int x = 0; x <= puzzle.maximum.x; x++) {
      
      arduboy.drawPixel(x * GRID_WIDTH, y * GRID_HEIGHT, WHITE);
      
    }

  }

  
  // Draw selected cell ..
  
  arduboy.drawRect(player.highlightedNode.x * GRID_WIDTH, player.highlightedNode.y * GRID_HEIGHT, GRID_WIDTH + 1, GRID_HEIGHT + 1, WHITE);
    
}

#line 1 "/Users/simonholmes/projects/Pipes_Article2/Splash.ino"
void drawSplash() { 
  
  arduboy.clear(); 
    
  sprites.drawOverwrite(112,  0, logo_straight_TB_noflange, frame);
  sprites.drawOverwrite(112, 16, logo_elbow_TL, frame);
  sprites.drawOverwrite(96,  16, logo_elbow_TR, frame);
  sprites.drawOverwrite(96,   0, logo_elbow_LB, frame);
  sprites.drawOverwrite(80,   0, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(64,   0, logo_straight_LR, frame);
  sprites.drawOverwrite(48,   0, logo_elbow_RB, frame);
  sprites.drawOverwrite(48,  16, logo_straight_TB_noflange, frame);
  sprites.drawOverwrite(48,  32, logo_elbow_TR, frame);
  sprites.drawOverwrite(64,  32, logo_elbow_LB, frame);
  sprites.drawOverwrite(64,  48, logo_straight_TB_noflange, frame);

  sprites.drawOverwrite(0,    0, logo_straight_TB_noflange, frame);
  sprites.drawOverwrite(0,   16, logo_straight_TB, frame);
  sprites.drawOverwrite(0,   32, logo_elbow_TR, frame);
  sprites.drawOverwrite(16,  32, logo_straight_LR, frame);
  sprites.drawOverwrite(32,  32, logo_elbow_TL, frame);
  sprites.drawOverwrite(32,  16, logo_elbow_RB, frame);
  sprites.drawOverwrite(48,  16, logo_cross_underlap_hflange, frame);
  sprites.drawOverwrite(64,  16, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(80,  16, logo_elbow_LB, frame);
  sprites.drawOverwrite(80,  32, logo_elbow_TR, frame);
  sprites.drawOverwrite(96,  32, logo_straight_LR, frame);
  sprites.drawOverwrite(112, 32, logo_straight_LR_noflange, frame);

  sprites.drawOverwrite(0,    0, logo_cross_underlap, frame);
  sprites.drawOverwrite(16,   0, logo_elbow_LB, frame);
  sprites.drawOverwrite(16,  16, logo_straight_TB, frame);
  sprites.drawOverwrite(16,  32, logo_cross_overlap_hflange, frame);
  sprites.drawOverwrite(16,  48, logo_elbow_TR, frame);
  sprites.drawOverwrite(32,  48, logo_straight_LR, frame);
  sprites.drawOverwrite(48,  48, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(64,  48, logo_cross_underlap_hflange, frame);
  sprites.drawOverwrite(80,  48, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(96,  48, logo_elbow_LB, frame);

  arduboy.display();
  arduboy.delayShort(2000);

  arduboy.fillRect(19, 18, 92, 30, BLACK);
  drawHorizontalDottedLine(20, 110, 19);
  drawHorizontalDottedLine(20, 110, 46);
  sprites.drawOverwrite(23, 21, logo, frame);
  arduboy.display();

  arduboy.delayShort(2000);

  gameState = STATE_INIT_GAME;
  
}


#line 1 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
/* ----------------------------------------------------------------------------
 *  Return the upper 4 bits of a byte.
 */
byte leftValue(byte val) {

  return val >> 4; 
      
}


/* ----------------------------------------------------------------------------
 *  Return the lower 4 bits of a byte.
 */
byte rightValue(byte val) {

  return val & 0x0F; 
      
}


/* ----------------------------------------------------------------------------
 *  Initialise the board.
 */
void initBoard(byte puzzleNumber) {

  byte x = 0;
  byte y = 0;
  byte byteRead = 0;

  for (int i = (puzzleNumber * 15); i < (puzzleNumber + 1) * 15; i++) {

    byteRead = pgm_read_byte(&puzzles_5x5[i]);

  
    // Load up the left hand value ..

    puzzle.board[y][x] = 0;
    if (leftValue(byteRead) > 0) {
      puzzle.board[y][x] = 0xF0 | leftValue(byteRead);
    }
    x++;

  
    // Are we still in the confines of the board?

    if (x <= puzzle.maximum.x) {         
      puzzle.board[y][x] = 0;
      if (rightValue(byteRead) > 0) {
        puzzle.board[y][x] = 0xF0 | rightValue(byteRead);
      }
    }

      
    x++;
      
    if (x >= puzzle.maximum.x) { y++; x = 0; }
        
  }

}


/* ----------------------------------------------------------------------------
 *  Has the node already been played?
 */
bool nodeAlreadyPlayed(byte value) { 

  for (byte y = 0; y < puzzle.maximum.y; y++) {
    
    for (byte x = 0; x < puzzle.maximum.x; x++) {
      
      if (getNodeValue(x, y) == value && !isNode(x, y)) {
        
        return true;
        
      }
      
    }
      
  }
    
  return false;
    
}


/* ----------------------------------------------------------------------------
 *  Clear the board of pipes for a certain node value.
 */
void clearBoard(byte nodeValue) {

  for (byte y = 0; y < puzzle.maximum.y; y++) {
    
    for (byte x = 0; x < puzzle.maximum.x; x++) {
      
      if (getNodeValue(x, y) == nodeValue && !isNode(x, y)) {
        
        puzzle.board[y][x] = NOTHING;
        
      }
      
    }
    
  }
  
}


/* ----------------------------------------------------------------------------
 *  Is the position nominated a node?
 */
bool isNode(byte x, byte y) {

  return (puzzle.board[y][x] & 0xF0) == 0xF0;
  
}


/* ----------------------------------------------------------------------------
 *  Get the node value for the position.
 */
byte getNodeValue(byte x, byte y) {
  
  return (puzzle.board[y][x] & 0x0F);

}


/* ----------------------------------------------------------------------------
 *  Is the position nominated a pipe?
 */
bool isPipe(byte x, byte y) {

  return (puzzle.board[y][x] & 0xF0) > 0x00 && (puzzle.board[y][x] & 0xF0) != 0xF0;
  
}


/* ----------------------------------------------------------------------------
 *  Get the pipe value for the position.
 */
byte getPipeValue(byte x, byte y) {
  
  return (puzzle.board[y][x] & 0xF0) >> 4;

}


/* ----------------------------------------------------------------------------
 *  Set the pipe value for the position.
 */
void setCellValue(byte x, byte y, byte pipeValue, byte nodeValue) {
  
  puzzle.board[y][x] = (pipeValue << 4) | nodeValue;
  
}


/* ----------------------------------------------------------------------------
 *  Clear the player's selection.
 */
void clearSelection() {

  player.selectedNode.value = 0;
  player.selectedNode.x = 0;
  player.selectedNode.y = 0;

}


/* ----------------------------------------------------------------------------
 *  Clear the player's highlight and selection.
 */
void clearHighlightAndSelection() {

  player.selectedNode.value = 0;
  player.selectedNode.x = 0;
  player.selectedNode.y = 0;
  player.highlightedNode.x = 0;
  player.highlightedNode.y = 0;

}


/* ----------------------------------------------------------------------------
 *  Is the puzzle complete?  All cells should have a 'node' value.
 */
bool isPuzzleComplete() {

  for (byte y = 0; y < puzzle.maximum.y; y++) {
    
    for (byte x = 0; x < puzzle.maximum.x; x++) {
      
      if (getNodeValue(x, y) == 0) {
        
        return false;
        
      }
      
    }
    
  }

  return true;
  
}



/* ----------------------------------------------------------------------------
 *  Toggle the sound setting and commit to the EEPROM.
 */
void toggleSoundSettings() {

  if (arduboy.audio.enabled()) {
  
    arduboy.audio.off(); 
    arduboy.audio.saveOnOff();
  
  }
  else {
  
    arduboy.audio.on(); 
    arduboy.audio.saveOnOff();
  
  }
    
}


/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 *  
 *  So much nicer than a solid line!
 */
void drawHorizontalDottedLine(int x1, int x2, int y) {

  for (int x3 = x1; x3 <= x2; x3+=2) {
    arduboy.drawPixel(x3, y, WHITE);
  }
  
}


