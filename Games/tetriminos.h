#define TILES_PER_BLOCK   4
#define NUM_BLOCK_ROTATIONS 4
#define NUM_BLOCK_OFFSETS 8 // 4 x [X,Y]
#define NUM_BLOCKS        7 // T J Z O S L I

#define BLOCK_T 0
#define BLOCK_J 1
#define BLOCK_Z 2
#define BLOCK_O 3
#define BLOCK_S 4
#define BLOCK_L 5
#define BLOCK_I 6

//Center X,Y of a spawned block
const int8_t SPAWN_ROW    = 2;
const int8_t SPAWN_COLUMN = 5;


/* Block Ids 
T - 0
J - 1
Z - 2
O - 3
S - 4
L - 5
I - 6
*/

// 8 Bytes X,Y * 4 rotations * 7 block types = 224 Bytes
const int8_t tetriminos[NUM_BLOCK_OFFSETS * NUM_BLOCKS * 4] = {
  
  //X1,Y1,X2,Y2,X3,Y3,X4,Y4
  //Each X,Y is an offset from 0,0 which is a center of 5x5 matrix each block type fits in.
  
  -1, 0, 0, 0, 1, 0, 0, -1,  // 00: T up
  0, -1, 0, 0, 1, 0, 0, 1,  // 01: T right
  -1, 0, 0, 0, 1, 0, 0, 1,  // 02: T down (spawn)
  0, -1, -1, 0, 0, 0, 0, 1, // 03: T left

  0, -1, 0, 0, -1, 1, 0, 1, // 04: J left
  -1, -1, -1, 0, 0, 0, 1, 0,// 05: J up
  0, -1, 1, -1, 0, 0, 0, 1, // 06: J right
  -1, 0, 0, 0, 1, 0, 1, 1,  // 07: J down (spawn)

  -1, 0, 0, 0, 0, 1, 1, 1, // 08: Z horizontal (spawn)
  1, -1, 0, 0, 1, 0, 0, 1, // 09: Z vertical
  -1, 0, 0, 0, 0, 1, 1, 1, // 0A: Z horizontal
  1, -1, 0, 0, 1, 0, 0, 1, // 0B: Z vertical

  -1, 0, 0, 0, -1, 1, 0, 1, // 0C: O (spawn)
  -1, 0, 0, 0, -1, 1, 0, 1, // 0D: O
  -1, 0, 0, 0, -1, 1, 0, 1, // 0E: O
  -1, 0, 0, 0, -1, 1, 0, 1, // 0F: O

  0, 0, 1, 0, -1, 1, 0, 1, // 10: S horizontal (spawn)
  0, -1, 0, 0, 1, 0, 1, 1, // 11: S vertical
  0, 0, 1, 0, -1, 1, 0, 1, // 12: S horizontal
  0, -1, 0, 0, 1, 0, 1, 1, // 13: S vertical

  0, -1, 0, 0, 0, 1, 1, 1, // 14: L right
  -1, 0, 0, 0, 1, 0, -1, 1, // 15: L down (spawn)
  -1, -1, 0, -1, 0, 0, 0, 1, // 16: L left
  1, -1, -1, 0, 0, 0, 1, 0, // 17 L up

  0, -2, 0, -1, 0, 0, 0, 1, // 18: I vertical
  -2, 0, -1, 0, 0, 0, 1, 0, // 19: I horizontal (spawn)
  0, -2, 0, -1, 0, 0, 0, 1, // 1A: I vertical
  -2, 0, -1, 0, 0, 0, 1, 0 // 1B: I horizontal
};

//Which tetrimino to use for each block Id when spawned
const int8_t spawnRotationIds[NUM_BLOCKS] = {  
	2, // 02: T down (spawn)
  3, // 07: J down (spawn)
  0, // 08: Z horizontal (spawn)
  0, // 0C: O (spawn)
  0, // 10: S horizontal (spawn)
  1, // 15: L down (spawn)
  1  // 19: I horizontal (spawn)
};

byte currentBlockId;
byte lastBlockId;
byte nextBlockId;
byte rotationId; // 0-3

byte gameSpawnCount;

// Actual position of a moving block
int8_t blockX; 
int8_t blockY;
int8_t softDropStartY;

void tetriminos_init() {  
  gameSpawnCount = 0;
  randomSeed(analogRead(A0));  
}

// DEBUG
/*
void tetriminos_dumpBlockInfos() {

  Serial.print("lastBlockId=");
  Serial.print(lastBlockId);
  Serial.print(",currentBlockId=");
  Serial.print(currentBlockId);
  Serial.print(",nextBlockId=");
  Serial.println(nextBlockId);
  
}

void tetriminos_dumpBlockPosition() {

  Serial.print("blockX=");
  Serial.print(blockX);
  Serial.print(",blockY=");
  Serial.println(blockY);
 
}
*/

//Get a next block id different from the last value
byte tetriminos_getNextBlockId(byte lastVal) {
  
  long rand;
  byte val;
  
  do {
    rand = random(NUM_BLOCKS);
    val = byte(rand);
  } while (val == lastVal);
    
  return val;
  
}

void tetriminos_spawnBlock() {  
    
  long rand;
  byte val;
  
  if (gameSpawnCount == 0) {
  
    rand = random(NUM_BLOCKS);
    val = byte(rand);

//  currentBlockId = BLOCK_I;
//  lastBlockId = BLOCK_I;
//  nextBlockId = BLOCK_I;
    currentBlockId = val;
    lastBlockId = val;
    nextBlockId = tetriminos_getNextBlockId(currentBlockId);
    
  } else {

  //currentBlockId = BLOCK_I;
  //lastBlockId = BLOCK_I;
  //nextBlockId = BLOCK_I;    
    lastBlockId = currentBlockId;
    currentBlockId = nextBlockId;
    nextBlockId = tetriminos_getNextBlockId(currentBlockId);    
  }

  gameSpawnCount++;  

  blockX = SPAWN_COLUMN;
  blockY = SPAWN_ROW;
  rotationId = spawnRotationIds[currentBlockId];

  //tetriminos_dumpBlockInfos();
    
}

//For a given blockId, get the offset to the tetriminos table for a spawned block
byte tetriminos_getCurrentBlockOffset() {

  //Calculate row for the spawned tetrimino in positions array 
  byte tmpOffset = 4 * NUM_BLOCK_OFFSETS * currentBlockId; //first row for the blockId  
  tmpOffset += (NUM_BLOCK_OFFSETS * rotationId); //get the rotation for the block
  return tmpOffset; 
}

byte tetriminos_getNextBlockOffset() {

  byte rotationId = spawnRotationIds[nextBlockId];

  //Calculate row for next tetrimino in positions array 
  byte tmpOffset = 4 * NUM_BLOCK_OFFSETS * nextBlockId; //first row for the blockId    
  tmpOffset += (NUM_BLOCK_OFFSETS * rotationId); //get the rotation for the block
  return tmpOffset; 
}

//Lock the block onto the grid
void tetriminos_placeBlock() {
  
  int8_t tmpx;
  int8_t tmpy;

  byte tmpOffset;

  tmpOffset = tetriminos_getCurrentBlockOffset();
  
  for (byte i=0; i<TILES_PER_BLOCK; i++) {

    //Get grid position
    tmpx = blockX + tetriminos[tmpOffset+2*i];
    tmpy = blockY + tetriminos[tmpOffset+2*i+1];    

    grid[tmpy][tmpx] = currentBlockId;
    
  }
}

// Checks that a given block at blockX, blockY in a given orientation do not overlap with 
// another locked block
bool tetriminos_blockFits() {

  int8_t tmpx;
  int8_t tmpy;

  byte tmpOffset;

  tmpOffset = tetriminos_getCurrentBlockOffset();
  
  for (byte i=0; i<TILES_PER_BLOCK; i++) {

    //Get grid position
    tmpx = blockX + tetriminos[tmpOffset+2*i];
    tmpy = blockY + tetriminos[tmpOffset+2*i+1];    

    // tile out of bounds
    if (tmpx < 0 || tmpx > GRID_COLUMNS-1 || tmpy > GRID_ALL_ROWS-1) {
      return false;
    }

    // tile not empty
    if (!grid_isEmpty(tmpy, tmpx)) {
      return false;
    }
    
  }

  return true;
}

bool tetriminos_canBlockMoveDown() {
    
  blockY++;

  if (!tetriminos_blockFits()) {
    blockY--;     //revert
    return false;
  }

  blockY--;
  return true;
    
}

bool tetriminos_canBlockMoveLeft() {
    
  blockX--;

  if (!tetriminos_blockFits()) {
    blockX++;     //revert
    return false;
  }

  blockX++;
  return true;
    
}

bool tetriminos_canBlockMoveRight() {
    
  blockX++;

  if (!tetriminos_blockFits()) {
    blockX--;     //revert
    return false;
  }

  blockX--;
  return true;
    
}

bool tetriminos_canBlockRotateCW() {
  
  byte tmpRotationId = rotationId;

  if (rotationId < NUM_BLOCK_ROTATIONS-1) {
    rotationId++;
  } else {
    rotationId = 0;
  }

  //for some reason this does not work
  //rotationId = (rotationId < NUM_BLOCK_ROTATIONS-1) ? rotationId++ : 0;
  
  if (!tetriminos_blockFits()) {
    rotationId = tmpRotationId;     //revert
    return false;
  }

  rotationId = tmpRotationId;
  return true;  
}

bool tetriminos_canBlockRotateCCW() {
  
  byte tmpRotationId = rotationId;

  if (rotationId > 0) {
    rotationId--;
  } else {
    rotationId = NUM_BLOCK_ROTATIONS-1;
  }
  
  //rotationId = (rotationId > 0) ? rotationId-- : NUM_BLOCK_ROTATIONS-1;
  
  if (!tetriminos_blockFits()) {
    rotationId = tmpRotationId;     //revert
    return false;
  }

  rotationId = tmpRotationId;
  return true;  
}
