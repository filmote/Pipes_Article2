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
