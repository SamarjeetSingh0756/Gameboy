#define GRID_ROWS 20
#define GRID_HIDDEN_ROWS 2
#define GRID_ALL_ROWS (GRID_ROWS + GRID_HIDDEN_ROWS) //20 visible + 2 hidden top rows
#define GRID_COLUMNS 10

const int8_t GRID_CELL_EMPTY = -1;

int8_t grid[GRID_ROWS][GRID_COLUMNS];

void grid_clear() {
	for (int8_t i=0; i<GRID_ALL_ROWS; i++) {
		for (int8_t j=0; j<GRID_COLUMNS; j++) { 
			grid[i][j] = GRID_CELL_EMPTY;
		}
	}
}

bool grid_isEmpty(byte x, byte y) {
  
  if (grid[x][y] != GRID_CELL_EMPTY) {
    return false;
  } else {
    return true;  
  }
   
}

//Return true if there is any block tile in the first 5 rows
bool grid_isTileInTop5Rows() {
  for (int8_t i=GRID_HIDDEN_ROWS+4; i>=GRID_HIDDEN_ROWS; i--) {
    for (int8_t j=0; j<GRID_COLUMNS; j++) {
      if (grid[i][j] != GRID_CELL_EMPTY) {
        return true;
      }
    }
  }
  return false;
}

bool grid_isRowFull(byte r) {

  //Serial.print("grid_isRowFull r=");
  //Serial.println(r);

  for (int8_t c=0; c<GRID_COLUMNS; c++) {
    if (grid[r+GRID_HIDDEN_ROWS][c] == GRID_CELL_EMPTY)
      return false;
  }
  //Serial.println("row full");   
  return true;  
}

bool grid_isRowEmpty(byte r) {

  //Serial.print("grid_isRowEmpty r=");
  //Serial.println(r);

  for (int8_t c=0; c<GRID_COLUMNS; c++) {
    if (grid[r+GRID_HIDDEN_ROWS][c] != GRID_CELL_EMPTY)
      return false;
  }

  //Serial.println("row empty");   
  return true;  
}

void grid_clearRow(byte r) {

  //Serial.println("grid_clearRow1");   
  
  for (int8_t c=0; c<GRID_COLUMNS; c++) {
    grid[r+GRID_HIDDEN_ROWS][c] = GRID_CELL_EMPTY;
  }

  //Serial.println("grid_clearRow2");   
}

void grid_shiftRowsRown(byte r, byte numRows) {

  //Serial.println("grid_shiftRowsRown1");
  
  for (byte c=0; c<GRID_COLUMNS; c++)
  {
      grid[r+GRID_HIDDEN_ROWS+numRows][c] = grid[r+GRID_HIDDEN_ROWS][c];
      grid[r+GRID_HIDDEN_ROWS][c] = GRID_CELL_EMPTY;
  }

  //Serial.println("grid_shiftRowsRown2");
}

//DEBUG
/*
void dumpGrid() {

  for (int8_t i=0; i<GRID_ALL_ROWS; i++) {    
    for (int8_t j=0; j<GRID_COLUMNS; j++) {
      if (grid[i][j] == GRID_CELL_EMPTY)
        Serial.print(".");
      else 
        Serial.print(grid[i][j]);
    }
    Serial.println();    
  }  
}
*/
