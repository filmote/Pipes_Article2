# 1 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino"
/* ----------------------------------------------------------------------------
     Pipes  by  Simon Holmes, 2017
   ----------------------------------------------------------------------------
*/
# 6 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino" 2
# 7 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino" 2
# 8 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino" 2
# 38 "/Users/simonholmes/projects/Pipes_Article2/Pipes_Article2.ino"
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
  byte index; // Puzzle number.
  Node maximum; // Used to store the dimensions of the puzzle based
                                          // on the puzzle level, eg. PUZZLE_5X5 has a maximum
                                          // x and y value of 5.
  byte board[9][9]; // Actual board details - supports maximum of 9 x 9.
}
puzzle;

const byte* nodes[] = {0, node_1, node_2, node_3, node_4, node_5, node_6, node_7, node_8, node_9, node_10 };
const byte* pipes[] = {pipe_nothing, pipe_horizontal, pipe_horizontal, pipe_vertical, pipe_vertical, pipe_corner_TL, pipe_corner_TL,
                       pipe_corner_TR, pipe_corner_TR, pipe_corner_BL, pipe_corner_BL, pipe_corner_BR, pipe_corner_BR};

byte frame = 0;
byte gameState = 0;


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

    case 0:
      drawSplash();
      break;

    case 1:
      play_InitGame();
      break;

    case 2:
      play_NoSelection();
      break;

    case 3:
      play_NodeSelected();
      break;

    case 4:
      play_GameOver();
      break;

  }

  arduboy.display();

}
# 1 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
/* ----------------------------------------------------------------------------
 *   Initialise the game based on the selected level and puzzle numbers.
 */
void play_InitGame() {

  arduboy.clear();
  initBoard(puzzle.index);
  gameState = 2;

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

  if (arduboy.justPressed(
# 23 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 23 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         5
# 23 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 23 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Left button value for functions requiring a bitmask */) && player.highlightedNode.x > 0) { player.highlightedNode.x--; }
  if (arduboy.justPressed(
# 24 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 24 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         6
# 24 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 24 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Right button value for functions requiring a bitmask */) && player.highlightedNode.x < puzzle.maximum.x - 1) { player.highlightedNode.x++; }
  if (arduboy.justPressed(
# 25 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 25 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         7
# 25 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 25 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Up button value for functions requiring a bitmask */) && player.highlightedNode.y > 0) { player.highlightedNode.y--; }
  if (arduboy.justPressed(
# 26 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 26 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         4
# 26 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 26 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Down button value for functions requiring a bitmask */) && player.highlightedNode.y < puzzle.maximum.y - 1) { player.highlightedNode.y++; }

  if (arduboy.justPressed(
# 28 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 28 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         3
# 28 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 28 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The A button value for functions requiring a bitmask */) && isNode(player.highlightedNode.x, player.highlightedNode.y)) {

    if (nodeAlreadyPlayed(getNodeValue(player.highlightedNode.x, player.highlightedNode.y))) {

      clearBoard(getNodeValue(player.highlightedNode.x, player.highlightedNode.y));
      player.selectedNode.value = getNodeValue(player.highlightedNode.x, player.highlightedNode.y);
      player.selectedNode.x = player.highlightedNode.x;
      player.selectedNode.y = player.highlightedNode.y;
      gameState = 3;

    }
    else {

      player.selectedNode.value = getNodeValue(player.highlightedNode.x, player.highlightedNode.y);
      player.selectedNode.x = player.highlightedNode.x;
      player.selectedNode.y = player.highlightedNode.y;
      gameState = 3;

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

  if (arduboy.justPressed(
# 66 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 66 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         5
# 66 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 66 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Left button value for functions requiring a bitmask */)) {

    if (validMove(2, player.selectedNode, player.highlightedNode.x - 1, player.highlightedNode.y)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 1:
        case 7:
        case 11:
          updatePipeWhenReversing(player.highlightedNode.x - 1, player.highlightedNode.y);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 0, 0);
          break;

        case 2:
        case 5:
        case 9:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, 2, player.selectedNode.value); }
          break;

        case 6:
        case 8:
        case 4:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, 2, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 9, player.selectedNode.value);
          break;

        case 10:
        case 12:
        case 3:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, 2, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 5, player.selectedNode.value);
          break;

        case 15:
          setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, 2, player.selectedNode.value);
          break;

      }


      // Have we selected the original node?  If so clear the board of this pipe ..

      if (player.highlightedNode.x - 1 == player.selectedNode.x && player.highlightedNode.y == player.selectedNode.y) {

        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = 2;

      }


      // Have we selected a pair node?  

      if (isNode(player.highlightedNode.x - 1, player.highlightedNode.y) && getNodeValue(player.highlightedNode.x - 1, player.highlightedNode.y) == player.selectedNode.value) {

        clearSelection();
        gameState = 2;


        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = 4;

        }

      }

      player.highlightedNode.x--;

    }

  }

  if (arduboy.justPressed(
# 141 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 141 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         6
# 141 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 141 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Right button value for functions requiring a bitmask */)) {

    if (validMove(3, player.selectedNode, player.highlightedNode.x + 1, player.highlightedNode.y)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 2:
        case 5:
        case 9:
          updatePipeWhenReversing(player.highlightedNode.x + 1, player.highlightedNode.y);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 0, 0);
          break;

        case 1:
        case 7:
        case 11:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, 1, player.selectedNode.value); }
          break;

        case 8:
        case 6:
        case 4:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, 1, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 11, player.selectedNode.value);
          break;

        case 12:
        case 10:
        case 3:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, 1, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 7, player.selectedNode.value);
          break;

        case 15:
          setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, 1, player.selectedNode.value);
          break;

      }


      // Have we selected the original node?  If so clear the board of this pipe ..

      if (player.highlightedNode.x + 1 == player.selectedNode.x && player.highlightedNode.y == player.selectedNode.y) {

        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = 2;

      }


      // Have we selected a pair node?  

      if (isNode(player.highlightedNode.x + 1, player.highlightedNode.y) && getNodeValue(player.highlightedNode.x + 1, player.highlightedNode.y) == player.selectedNode.value) {

        clearSelection();
        gameState = 2;


        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = 4;

        }

      }

      player.highlightedNode.x++;

    }

  }

  if (arduboy.justPressed(
# 216 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 216 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         7
# 216 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 216 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Up button value for functions requiring a bitmask */)) {

    if (validMove(0, player.selectedNode, player.highlightedNode.x, player.highlightedNode.y - 1)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 3:
        case 12:
        case 10:
          updatePipeWhenReversing(player.highlightedNode.x, player.highlightedNode.y - 1);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 0, 0);
          break;

        case 4:
        case 6:
        case 8:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, 4, player.selectedNode.value); }
          break;

        case 7:
        case 11:
        case 1:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, 4, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 6, player.selectedNode.value);
          break;

        case 5:
        case 9:
        case 2:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, 4, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 8, player.selectedNode.value);
          break;

        case 15:
          setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, 4, player.selectedNode.value);
          break;

      }


      // Have we selected the original node?  If so clear the board of this pipe ..

      if (player.highlightedNode.x == player.selectedNode.x && player.highlightedNode.y - 1 == player.selectedNode.y) {

        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = 2;

      }


      // Have we selected a pair node?  

      if (isNode(player.highlightedNode.x, player.highlightedNode.y - 1) && getNodeValue(player.highlightedNode.x, player.highlightedNode.y - 1) == player.selectedNode.value) {

        clearSelection();
        gameState = 2;


        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = 4;

        }

      }

      player.highlightedNode.y--;
    }

  }

  if (arduboy.justPressed(
# 290 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 290 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         4
# 290 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 290 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The Down button value for functions requiring a bitmask */)) {

    if (validMove(1, player.selectedNode, player.highlightedNode.x, player.highlightedNode.y + 1)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 4:
        case 8:
        case 6:
          updatePipeWhenReversing(player.highlightedNode.x, player.highlightedNode.y + 1);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 0, 0);
          break;

        case 3:
        case 10:
        case 12:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, 3, player.selectedNode.value); }
          break;

        case 7:
        case 11:
        case 1:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, 3, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 10, player.selectedNode.value);
          break;

        case 5:
        case 9:
        case 2:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, 3, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, 12, player.selectedNode.value);
          break;

        case 15:
          setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, 3, player.selectedNode.value);
          break;

      }


      // Have we selected the original node?  If so clear the board of this pipe ..

      if (player.highlightedNode.x == player.selectedNode.x && player.highlightedNode.y + 1 == player.selectedNode.y) {

        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = 2;

      }


      // Have we selected a pair node?  

      if (isNode(player.highlightedNode.x, player.highlightedNode.y + 1) && getNodeValue(player.highlightedNode.x, player.highlightedNode.y + 1) == player.selectedNode.value) {

        clearSelection();
        gameState = 2;


        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = 4;

        }

      }

      player.highlightedNode.y++;

    }

  }

  if (arduboy.justPressed(
# 365 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         (1 << (
# 365 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         2
# 365 "/Users/simonholmes/projects/Pipes_Article2/Play.ino" 3
                         )) 
# 365 "/Users/simonholmes/projects/Pipes_Article2/Play.ino"
                         /**< The B button value for functions requiring a bitmask */)) {

    clearBoard(player.selectedNode.value);
    clearSelection();
    gameState = 2;

  }


  renderBoard();

}


/* ----------------------------------------------------------------------------
 *   Fix the pipe graphics when reversing back on yourself.
 */
void updatePipeWhenReversing(byte x, byte y) {

  switch (getPipeValue(x, y)) {

    case 7:
    case 5:
    case 3:
      setCellValue(x, y, 3, player.selectedNode.value);
      break;

    case 11:
    case 9:
    case 4:
      setCellValue(x, y, 4, player.selectedNode.value);
      break;

    case 6:
    case 10:
    case 1:
      setCellValue(x, y, 1, player.selectedNode.value);
      break;

    case 8:
    case 12:
    case 2:
      setCellValue(x, y, 2, player.selectedNode.value);
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
      (!isNode(x,y) && getPipeValue(x,y) == 0) ||
      (isNode(x,y) && getNodeValue(x,y) == selectedNode.value && (x != selectedNode.x || y != selectedNode.y))
     ) return true;


  // Is the pipe turning back on itself?

  switch (direction) {

    case (0):

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 3:
        case 12:
        case 10:
          return true;

      }

      break;

    case (1):

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 4:
        case 6:
        case 8:
          return true;

      }

      break;

    case (2):

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 7:
        case 11:
        case 1:
          return true;

      }

      break;

    case (3):

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case 5:
        case 9:
        case 2:
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
  gameState = 0;

}
# 1 "/Users/simonholmes/projects/Pipes_Article2/Render.ino"
/* ----------------------------------------------------------------------------
 *   Render the board.
 */
void renderBoard() {

  arduboy.clear();


  // Draw placed pipes ..

  for (int y = 0; y < puzzle.maximum.y; y++) {

    for (int x = 0; x < puzzle.maximum.x; x++) {

      if (isPipe(x,y)) {

        sprites.drawExternalMask(x * 11, y * 11, pipes[getPipeValue(x, y)], pipe_mask, frame, frame);

      }

    }

  }



   // Draw nodes ..

  for (int y = 0; y < puzzle.maximum.y; y++) {

    for (int x = 0; x < puzzle.maximum.x; x++) {

      if (isNode(x, y)) {

        sprites.drawOverwrite(x * 11 + 2, y * 11 + 2, nodes[getNodeValue(x,y)], frame);

      }

    }

  }


  // Draw grid marks ..

  for (int y = 0; y <= puzzle.maximum.y; y++) {

    for (int x = 0; x <= puzzle.maximum.x; x++) {

      arduboy.drawPixel(x * 11, y * 11, 1 /**< Color value for a lit pixel for draw functions. */);

    }

  }


  // Draw selected cell ..

  arduboy.drawRect(player.highlightedNode.x * 11, player.highlightedNode.y * 11, 11 + 1, 11 + 1, 1 /**< Color value for a lit pixel for draw functions. */);

}
# 1 "/Users/simonholmes/projects/Pipes_Article2/Splash.ino"
void drawSplash() {

  arduboy.clear();

  sprites.drawOverwrite(112, 0, logo_straight_TB_noflange, frame);
  sprites.drawOverwrite(112, 16, logo_elbow_TL, frame);
  sprites.drawOverwrite(96, 16, logo_elbow_TR, frame);
  sprites.drawOverwrite(96, 0, logo_elbow_LB, frame);
  sprites.drawOverwrite(80, 0, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(64, 0, logo_straight_LR, frame);
  sprites.drawOverwrite(48, 0, logo_elbow_RB, frame);
  sprites.drawOverwrite(48, 16, logo_straight_TB_noflange, frame);
  sprites.drawOverwrite(48, 32, logo_elbow_TR, frame);
  sprites.drawOverwrite(64, 32, logo_elbow_LB, frame);
  sprites.drawOverwrite(64, 48, logo_straight_TB_noflange, frame);

  sprites.drawOverwrite(0, 0, logo_straight_TB_noflange, frame);
  sprites.drawOverwrite(0, 16, logo_straight_TB, frame);
  sprites.drawOverwrite(0, 32, logo_elbow_TR, frame);
  sprites.drawOverwrite(16, 32, logo_straight_LR, frame);
  sprites.drawOverwrite(32, 32, logo_elbow_TL, frame);
  sprites.drawOverwrite(32, 16, logo_elbow_RB, frame);
  sprites.drawOverwrite(48, 16, logo_cross_underlap_hflange, frame);
  sprites.drawOverwrite(64, 16, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(80, 16, logo_elbow_LB, frame);
  sprites.drawOverwrite(80, 32, logo_elbow_TR, frame);
  sprites.drawOverwrite(96, 32, logo_straight_LR, frame);
  sprites.drawOverwrite(112, 32, logo_straight_LR_noflange, frame);

  sprites.drawOverwrite(0, 0, logo_cross_underlap, frame);
  sprites.drawOverwrite(16, 0, logo_elbow_LB, frame);
  sprites.drawOverwrite(16, 16, logo_straight_TB, frame);
  sprites.drawOverwrite(16, 32, logo_cross_overlap_hflange, frame);
  sprites.drawOverwrite(16, 48, logo_elbow_TR, frame);
  sprites.drawOverwrite(32, 48, logo_straight_LR, frame);
  sprites.drawOverwrite(48, 48, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(64, 48, logo_cross_underlap_hflange, frame);
  sprites.drawOverwrite(80, 48, logo_straight_LR_noflange, frame);
  sprites.drawOverwrite(96, 48, logo_elbow_LB, frame);

  arduboy.display();
  arduboy.delayShort(2000);

  arduboy.fillRect(19, 18, 92, 30, 0 /**< Color value for an unlit pixel for draw functions. */);
  drawHorizontalDottedLine(20, 110, 19);
  drawHorizontalDottedLine(20, 110, 46);
  sprites.drawOverwrite(23, 21, logo, frame);
  arduboy.display();

  arduboy.delayShort(2000);

  gameState = 1;

}
# 1 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
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

    byteRead = 
# 32 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino" 3
              (__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(
# 32 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
              &puzzles_5x5[i]
# 32 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino" 3
              )); uint8_t __result; __asm__ __volatile__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; }))
# 32 "/Users/simonholmes/projects/Pipes_Article2/Utils.ino"
                                            ;


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

        puzzle.board[y][x] = 0;

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
    arduboy.drawPixel(x3, y, 1 /**< Color value for a lit pixel for draw functions. */);
  }

}
