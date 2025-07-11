#include "esp32logo.h"
#include "tetrislogo.h"
#include "SPI.h"
#include "TFT_eSPI.h"

// Stock font and GFXFF reference handle
#include <Wire.h>
#include <Adafruit_GFX.h>


// Define the TFT pins for ESP32
#define TFT_CS   5  // Chip Select pin for the TFT display
#define TFT_DC   2  // Data/Command pin for the TFT display
#define TFT_RST  4  // Reset pin for the TFT display

#define SCREEN_WIDTH        320 // TFT display width, in pixels for 2.4-inch display
#define SCREEN_HEIGHT       240 // TFT display height, in pixels for 2.4-inch display

 // Invoke custom library

const byte buttonPins[] = {14, 27, 26, 25};

#define GFXFF 1
#define FF18 &FreeMonoBold9pt7b
#define FF19 &FreeMonoBold12pt7b

#define TFT_MAX_WIDTH 240
#define TFT_MAX_HEIGHT 320
#define FONT_WIDTH 11 //px
#define FONT_WIDTH_BIG 14 //px

#define TILE_SIZE 14  //px

#define NORMAL_BORDER_WIDTH 2
#define GRID_BORDER_WIDTH 4
#define NORMAL_PADDING 2
#define EXTRA_PADDING 4

#define LINES_BOX_OFFSET_X 0
#define LINES_BOX_OFFSET_Y 0
#define LINES_BOX_TEXT_SIZE 16
#define LINES_BOX_HEIGHT (NORMAL_BORDER_WIDTH + NORMAL_PADDING + LINES_BOX_TEXT_SIZE + NORMAL_PADDING + NORMAL_BORDER_WIDTH)
#define LINES_BOX_TEXT_X (LINES_BOX_OFFSET_X + NORMAL_BORDER_WIDTH + NORMAL_PADDING)
#define LINES_BOX_TEXT_Y (LINES_BOX_OFFSET_Y + NORMAL_BORDER_WIDTH + NORMAL_PADDING)
#define LINES_CLEARED_X (NORMAL_BORDER_WIDTH + NORMAL_PADDING + 8 * FONT_WIDTH)
#define LINES_CLEARED_Y LINES_BOX_TEXT_Y
#define LINES_BOX_COLOR TFT_CYAN

#define GRID_BOX_OFFSET_X 0
#define GRID_BOX_OFFSET_Y (LINES_BOX_OFFSET_Y + LINES_BOX_HEIGHT + EXTRA_PADDING)
#define GRID_BOX_WIDTH (GRID_BORDER_WIDTH + NORMAL_PADDING + GRID_COLUMNS * TILE_SIZE + NORMAL_PADDING + GRID_BORDER_WIDTH)
#define LINES_BOX_WIDTH GRID_BOX_WIDTH
#define GRID_BOX_HEIGHT (GRID_BORDER_WIDTH + NORMAL_PADDING + GRID_ROWS * TILE_SIZE + NORMAL_PADDING + GRID_BORDER_WIDTH)

#define GRID_OFFSET_X (GRID_BOX_OFFSET_X + GRID_BORDER_WIDTH + NORMAL_PADDING)
#define GRID_OFFSET_Y (GRID_BOX_OFFSET_Y + GRID_BORDER_WIDTH + NORMAL_PADDING)
#define GRID_BOX_COLOR TFT_CYAN

#define LEVEL_BOX_OFFSET_X (GRID_BOX_WIDTH + EXTRA_PADDING)
#define LEVEL_BOX_OFFSET_Y (TFT_MAX_HEIGHT - LINES_BOX_HEIGHT)
#define LEVEL_BOX_WIDTH (NORMAL_BORDER_WIDTH + NORMAL_PADDING + 5 * FONT_WIDTH + NORMAL_PADDING + NORMAL_BORDER_WIDTH)
#define LEVEL_BOX_HEIGHT LINES_BOX_HEIGHT 
#define LEVEL_TEXT_X LEVEL_BOX_OFFSET_X
#define LEVEL_TEXT_Y LEVEL_BOX_OFFSET_Y - NORMAL_PADDING - LINES_BOX_TEXT_SIZE
#define LEVEL_NUMBER_X (LEVEL_BOX_OFFSET_X + 2 * FONT_WIDTH)
#define LEVEL_NUMBER_Y (LEVEL_BOX_OFFSET_Y + NORMAL_BORDER_WIDTH + NORMAL_PADDING)

#define NEXT_TEXT_X LEVEL_TEXT_X
#define NEXT_TEXT_Y LEVEL_TEXT_Y - 126
#define NEXT_BOX_OFFSET_X LEVEL_BOX_OFFSET_X
#define NEXT_BOX_OFFSET_Y LEVEL_BOX_OFFSET_Y - 126
#define NEXT_BOX_WIDTH (NORMAL_BORDER_WIDTH + NORMAL_PADDING + 5 * TILE_SIZE + NORMAL_PADDING + NORMAL_BORDER_WIDTH)
#define NEXT_BOX_HEIGHT (NORMAL_BORDER_WIDTH + NORMAL_PADDING + 4 * TILE_SIZE + NORMAL_BORDER_WIDTH + NORMAL_PADDING)
#define NEXT_X (NEXT_BOX_OFFSET_X + NORMAL_BORDER_WIDTH + NORMAL_PADDING) + 20
#define NEXT_Y (NEXT_BOX_OFFSET_Y + NORMAL_BORDER_WIDTH + NORMAL_PADDING) + 20

#define SCORE_BOX_OFFSET_X NEXT_TEXT_X
#define SCORE_BOX_OFFSET_Y 0
#define SCORE_BOX_WIDTH NEXT_BOX_WIDTH
#define SCORE_BOX_HEIGHT (2 * NORMAL_BORDER_WIDTH + 5 * NORMAL_PADDING + 6 * LINES_BOX_TEXT_SIZE)
#define SCORE_TOP_TEXT_X (SCORE_BOX_OFFSET_X + NORMAL_BORDER_WIDTH + NORMAL_PADDING)
#define SCORE_TOP_TEXT_Y (SCORE_BOX_OFFSET_Y + NORMAL_BORDER_WIDTH + LINES_BOX_TEXT_SIZE + NORMAL_PADDING)
#define SCORE_TOP_X (SCORE_BOX_OFFSET_X + NORMAL_BORDER_WIDTH + NORMAL_PADDING)
#define SCORE_TOP_Y (SCORE_TOP_TEXT_Y + LINES_BOX_TEXT_SIZE)
#define SCORE_ACT_TEXT_X SCORE_TOP_TEXT_X
#define SCORE_ACT_TEXT_Y (SCORE_TOP_TEXT_Y + 3* LINES_BOX_TEXT_SIZE + 2*NORMAL_PADDING)
#define SCORE_ACT_X SCORE_TOP_TEXT_X
#define SCORE_ACT_Y (SCORE_ACT_TEXT_Y + LINES_BOX_TEXT_SIZE)

#define TOP_SCORE_X 35
#define TOP_SCORE_Y 225

char result[7]; //for sprintf()

const uint16_t BLOCK_COLOURS[NUM_BLOCKS] = {
  0xF800,
  0xFEC0,
  0x0740,
  0x06BF,
  0x01FB,
  0xFB60,
  0xE81F
};

const uint16_t BLOCK_LEFT_COLOURS[NUM_BLOCKS] = {
  0xFDF6,
  0xFFF3,
  0xB7F4,
  0xB7BF,
  0x74BF,
  0xFF19,
  0xFE7F
};

const uint16_t BLOCK_RIGHT_COLOURS[NUM_BLOCKS] = {
  0xA004,
  0xFC40,
  0x0400,
  0x04B5,
  0x080A,
  0xFA40,
  0xB01B
};

/*
  0xF800, //RED
  0xE380, //YELLOW
  0x0461, //GREEN
  TFT_CYAN,
  0x481F, //DARK BLUE
  0x049F, //BLUE (ORANGE)
  0xF81B, //MAGENTA
  */

const uint16_t BLOCK_BORDER_COLOURS[NUM_BLOCKS] = {
  TFT_WHITE, //RED
  TFT_WHITE, //YELLOW
  TFT_WHITE, //GREEN
  TFT_WHITE, //CYAN
  TFT_WHITE, //DARK BLUE
  TFT_WHITE, //BLUE
  TFT_WHITE, //MAGENTA
};

/*
Yellow
255,218,0 - 0xFEC0
255,253,155 - 0xFFF3
255,138,0 - 0xFC40

Red
255,0,1 - 0xF800
255,190,181 - 0xFDF6
162,0,34 - 0xA004

Green
0,235,0 - 0x0740
179,255,167 - 0xB7F4
0,128,0 - 0x0400

Magenta
238,0,255 - 0xE81F
255,205,255 - 0xFE7F
180,0,221 - 0xB01B

Orange
255,109,0 - 0xFB60
255,226,204 - 0xFF19
255,75,0 - 0xFA40

Cyan
0,215,252 -  0x06BF
183,247,253 - 0xB7BF
0,149,174 -  0x04B5

Blue
0,63,221 - 0x01FB
118,150,255 - 0x74BF
10,0,86 - 0x080A
*/
typedef enum {
    START,
    RUNNING,
    GAMEOVER
} State;

typedef enum {
    LEFT,
    UP,
    RIGHT,
    DOWN
} Direction;

#define SNAKE_PIECE_SIZE     12 // Increase the size of the snake pieces
#define MAX_SANKE_LENGTH   165
#define MAP_SIZE_X          20 // Adjust the map size to fit the larger snake and centered game area
#define MAP_SIZE_Y          15 // Adjust the map size to fit the larger snake and centered game area
#define STARTING_SNAKE_SIZE  5
#define SNAKE_MOVE_DELAY    50

// Use hardware SPI
   // Invoke custom library
State gameState;

int8_t snake[MAX_SANKE_LENGTH][2];
uint8_t snake_length;
Direction dir;
Direction newDir;
int direction = 0;
int olddirection = 0;
int8_t fruit[2];

void setupGame();
void resetSnake();
void generateFruit();
void drawMap();
void drawScore();
void drawPressToStart();
void drawGameover();
int moveTime = 0;
int gamerunning = 0;


TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite spr = TFT_eSprite(&tft);

void ui_drawRectBorderWidth(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color, uint16_t width) {

  for(uint16_t i=0; i<width; i++) {
    tft.drawRect(x0+i, y0+i, w-2*i, h-2*i, color);
  }  
}

// x, y = position on grid
void ui_drawTile(byte x, byte y, byte blockId, bool clean, bool drawingNextBlock) {

//Coords for the tile square on screen
uint16_t x0;
uint16_t y0;

  x0 = GRID_OFFSET_X + (x * TILE_SIZE);
  y0 = GRID_OFFSET_Y + ((y-2) * TILE_SIZE); //y-2 = hidden top rows

  //Ensure I and O blocks are centered in the Next box
  if (drawingNextBlock){
    if (blockId == BLOCK_I) {
      x0 += TILE_SIZE/2;
      y0 += TILE_SIZE/2;
    }
    if (blockId == BLOCK_O) {
      x0 += TILE_SIZE/2;
    }
  }

  if (clean) {
    spr.fillSprite(TFT_BLACK);
    //tft.fillRect(x0, y0, TILE_SIZE, TILE_SIZE, TFT_BLACK);  
  } else {
    spr.fillRect(1, 1, TILE_SIZE-2, TILE_SIZE-2, BLOCK_COLOURS[blockId]);
    //tft.fillRect(x0+1, y0+1, TILE_SIZE-2, TILE_SIZE-2, BLOCK_COLOURS[blockId]);
    
    //shading
    spr.drawLine(0, 0, TILE_SIZE-1, 0, BLOCK_LEFT_COLOURS[blockId]);
    spr.drawLine(0, 0, 0, TILE_SIZE-1, BLOCK_LEFT_COLOURS[blockId]);
    spr.drawLine(0, 0, TILE_SIZE-1, 0, BLOCK_LEFT_COLOURS[blockId]);
    spr.drawLine(0, TILE_SIZE-1, TILE_SIZE-1, TILE_SIZE-1, BLOCK_RIGHT_COLOURS[blockId]);
    spr.drawLine(TILE_SIZE-1, 0, TILE_SIZE-1, TILE_SIZE-1, BLOCK_RIGHT_COLOURS[blockId]);
    
    /*
    tft.drawLine(x0, y0, x0+TILE_SIZE-1, y0, BLOCK_LEFT_COLOURS[blockId]);
    tft.drawLine(x0, y0, x0, y0+TILE_SIZE-1, BLOCK_LEFT_COLOURS[blockId]);
    tft.drawLine(x0, y0, x0+TILE_SIZE-1, y0, BLOCK_LEFT_COLOURS[blockId]);
    tft.drawLine(x0, y0+TILE_SIZE-1, x0+TILE_SIZE-1, y0+TILE_SIZE-1, BLOCK_RIGHT_COLOURS[blockId]);
    tft.drawLine(x0+TILE_SIZE-1, y0, x0+TILE_SIZE-1, y0+TILE_SIZE-1, BLOCK_RIGHT_COLOURS[blockId]);
    */

    if (blockId == BLOCK_T || blockId == BLOCK_L || blockId == BLOCK_S || blockId == BLOCK_I) {
      /*
      tft.drawPixel(x0+4, y0+4, TFT_WHITE);
      tft.drawPixel(x0+3, y0+4, TFT_WHITE);
      tft.drawPixel(x0+3, y0+5, TFT_WHITE);    
      */
      spr.drawPixel(4, 4, TFT_WHITE);
      spr.drawPixel(3, 4, TFT_WHITE);
      spr.drawPixel(3, 5, TFT_WHITE);
    } else {
      /*
      tft.drawPixel(x0+4, y0+4, TFT_BLACK);
      tft.drawPixel(x0+3, y0+4, TFT_BLACK);
      tft.drawPixel(x0+3, y0+5, TFT_BLACK);      
      */
      spr.drawPixel(4, 4, TFT_BLACK);
      spr.drawPixel(3, 4, TFT_BLACK);
      spr.drawPixel(3, 5, TFT_BLACK);      
    }

  }

  spr.pushSprite(x0, y0);
  
  random(NUM_BLOCKS);
}

// clean = cleaning up after a block moved
void ui_drawBlock(bool clean) {

  // single tile coords
  byte x;
  byte y;
  byte tmpOffset;
  
  tmpOffset = tetriminos_getCurrentBlockOffset();

  //Get relative positions for each tile
  for (byte i=0; i<TILES_PER_BLOCK; i++) { 
    
    //Get grid position
    x = blockX + tetriminos[tmpOffset+2*i];
    y = blockY + tetriminos[tmpOffset+2*i+1];
    //Serial.print("x="); Serial.print(x); Serial.print(",y="); Serial.println(y);

    // Do not draw tiles in hidden rows
    if (y >= 2) {
      ui_drawTile(x, y, currentBlockId, clean, false);
    }
  }
}

void ui_drawNextBlock(bool clean) {
  
  byte nBlockX = 13;
  byte nBlockY = 13;
  
  // single tile coords
  byte x;
  byte y;
  byte tmpOffset;

  tmpOffset = tetriminos_getNextBlockOffset();

  for (byte i=0; i<TILES_PER_BLOCK; i++) {
        
    //Get grid position
    x = nBlockX + tetriminos[tmpOffset+2*i];
    y = nBlockY + tetriminos[tmpOffset+2*i+1];

    ui_drawTile(x, y, nextBlockId, clean, true);
    
  }
  
}

void ui_clearGrid() {
  tft.fillRect(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_COLUMNS*TILE_SIZE, GRID_ROWS*TILE_SIZE, TFT_BLACK);  
}

void ui_drawLinesCount(int cleared) {

  sprintf(result, "%03d", cleared);
  tft.fillRect(LINES_CLEARED_X, LINES_CLEARED_Y, 3*FONT_WIDTH, LINES_BOX_TEXT_SIZE, TFT_BLACK);
  tft.drawString(result, LINES_CLEARED_X, LINES_CLEARED_Y, GFXFF);
  
}

void ui_drawLevel(int level) {

  sprintf(result, "%02d", level);
  tft.fillRect(LEVEL_NUMBER_X, LEVEL_NUMBER_Y, 2*FONT_WIDTH, LINES_BOX_TEXT_SIZE, TFT_BLACK);
  tft.drawString(result, LEVEL_NUMBER_X, LEVEL_NUMBER_Y, GFXFF);  
}

void ui_drawScore(long score) {

  sprintf(result, "%06ld", score);
  
  tft.fillRect(SCORE_ACT_X, SCORE_ACT_Y, 6*FONT_WIDTH, LINES_BOX_TEXT_SIZE, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(result, SCORE_ACT_X, SCORE_ACT_Y, GFXFF);    
}

void ui_drawTopScore(long topScore) {

  sprintf(result, "%06ld", 690911);
  tft.fillRect(SCORE_TOP_X, SCORE_TOP_Y, 6*FONT_WIDTH, LINES_BOX_TEXT_SIZE, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(result, SCORE_TOP_X, SCORE_TOP_Y, GFXFF);    
}

void ui_drawLevelSelectRectangle(byte x, byte y, bool clean) {
  
  int middle = (TFT_MAX_WIDTH-12*FONT_WIDTH)/2;
  int shift = 15;
  
      
  if (clean)
    tft.fillRect(middle+x-24, 59+y-shift, 34, 18, TFT_BLACK);
  else
    tft.fillRect(middle+x-24, 59+y-shift, 34, 18, TFT_RED);
    
}

void ui_drawIntroScreen() {
  
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);

  tft.pushImage(10, 10, 220, 49, tetrislogo);
  
  tft.pushImage(70, 80, 100, 36, esp32logo);

  tft.setFreeFont(FF18);
  tft.setTextColor(TFT_BLACK);
  
  //tft.drawString("Original concept by", 15, 140, GFXFF);  
  //tft.drawString("Alexey Pazhitnov", 35, 160, GFXFF);

  tft.drawString("Project Submitted By", 5, 160, GFXFF);  
  tft.drawString("Samarjeet Singh", 35, 185, GFXFF);
  tft.drawString(" Naman Dhingra", 36, 206, GFXFF);

  tft.drawString("Press any button", 34, 280, GFXFF);  
    
}

void ui_drawPlayground() {

  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FF18); // Select the font
  
  // Lines Box
  ui_drawRectBorderWidth(LINES_BOX_OFFSET_X, LINES_BOX_OFFSET_Y, LINES_BOX_WIDTH, LINES_BOX_HEIGHT, LINES_BOX_COLOR, NORMAL_BORDER_WIDTH);  
  tft.drawString("LINES - ", LINES_BOX_TEXT_X, LINES_BOX_TEXT_Y, GFXFF);

  // Grid Box
  ui_drawRectBorderWidth(GRID_BOX_OFFSET_X, GRID_BOX_OFFSET_Y, GRID_BOX_WIDTH, GRID_BOX_HEIGHT, GRID_BOX_COLOR, GRID_BORDER_WIDTH);

  // Score Box
  ui_drawRectBorderWidth(SCORE_BOX_OFFSET_X, SCORE_BOX_OFFSET_Y, SCORE_BOX_WIDTH, SCORE_BOX_HEIGHT, GRID_BOX_COLOR, NORMAL_BORDER_WIDTH);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("TOP", SCORE_TOP_TEXT_X, SCORE_TOP_TEXT_Y, GFXFF);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString("SCORE", SCORE_ACT_TEXT_X, SCORE_ACT_TEXT_Y, GFXFF);

  // Next Box
  ui_drawRectBorderWidth(NEXT_BOX_OFFSET_X, NEXT_BOX_OFFSET_Y, NEXT_BOX_WIDTH, NEXT_BOX_HEIGHT, GRID_BOX_COLOR, NORMAL_BORDER_WIDTH);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("NEXT", NEXT_TEXT_X, NEXT_TEXT_Y, GFXFF);

  // Level Box
  ui_drawRectBorderWidth(LEVEL_BOX_OFFSET_X, LEVEL_BOX_OFFSET_Y, LEVEL_BOX_WIDTH, LEVEL_BOX_HEIGHT, GRID_BOX_COLOR, NORMAL_BORDER_WIDTH);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("LEVEL", LEVEL_TEXT_X, LEVEL_TEXT_Y, GFXFF);
    
}

void ui_drawGameOverCurtain() {

  for (byte r=0; r<GRID_ROWS; r++) {
    tft.fillRect(GRID_OFFSET_X, GRID_OFFSET_Y+r*TILE_SIZE, GRID_COLUMNS*TILE_SIZE,  2, TFT_BLACK);
    tft.fillRect(GRID_OFFSET_X, GRID_OFFSET_Y+r*TILE_SIZE+2, GRID_COLUMNS*TILE_SIZE,  3, TFT_RED);
    tft.fillRect(GRID_OFFSET_X, GRID_OFFSET_Y+r*TILE_SIZE+5, GRID_COLUMNS*TILE_SIZE,  5, TFT_WHITE);
    tft.fillRect(GRID_OFFSET_X, GRID_OFFSET_Y+r*TILE_SIZE+10, GRID_COLUMNS*TILE_SIZE, 3, TFT_MAGENTA);
    tft.drawLine(GRID_OFFSET_X, GRID_OFFSET_Y+r*TILE_SIZE+13, GRID_OFFSET_X+GRID_COLUMNS*TILE_SIZE, GRID_OFFSET_Y+r*TILE_SIZE+13, TFT_BLACK);    
    delay(50);
  }    
}

void ui_celebrateTetris() {
  ui_drawRectBorderWidth(GRID_BOX_OFFSET_X, GRID_BOX_OFFSET_Y, GRID_BOX_WIDTH, GRID_BOX_HEIGHT, TFT_WHITE, GRID_BORDER_WIDTH);
  delay(50);
  ui_drawRectBorderWidth(GRID_BOX_OFFSET_X, GRID_BOX_OFFSET_Y, GRID_BOX_WIDTH, GRID_BOX_HEIGHT, GRID_BOX_COLOR, GRID_BORDER_WIDTH);
  delay(50);
  ui_drawRectBorderWidth(GRID_BOX_OFFSET_X, GRID_BOX_OFFSET_Y, GRID_BOX_WIDTH, GRID_BOX_HEIGHT, TFT_WHITE, GRID_BORDER_WIDTH);
  delay(50);
  ui_drawRectBorderWidth(GRID_BOX_OFFSET_X, GRID_BOX_OFFSET_Y, GRID_BOX_WIDTH, GRID_BOX_HEIGHT, GRID_BOX_COLOR, GRID_BORDER_WIDTH);  
  delay(50);
}

void ui_drawLevelNumber(int level) {
  
  byte x;
  byte y;
  int middle = (TFT_MAX_WIDTH-12*FONT_WIDTH)/2;     
  
  x = middle-13;
  y = 46;

  if (level >= 5) {
    y = y + 21;
    x = x + (level-5)*37;    
  } else {
    x = x + (level*37);    
  }
  
  sprintf(result, "%1d", level);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(FF18);
  tft.drawString(result, x, y, GFXFF);    
    
}

void ui_drawLevelPlus10(bool clean) {
  if (clean) {
    tft.fillRect(196, 16, 3*FONT_WIDTH, LINES_BOX_TEXT_SIZE, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_WHITE);
    tft.drawString("+10", 196, 16, GFXFF);
  }
}

void ui_drawMusicSelectionPointer(byte index, bool clean) {
  
  int middle = (TFT_MAX_WIDTH-12*FONT_WIDTH)/2;

  if (clean) {
    tft.setTextColor(TFT_BLACK);
  } else {
    tft.setTextColor(TFT_WHITE);
  }
  
  tft.drawString("*", middle, 131+index*20, GFXFF);
    
}

void ui_drawMusicSelection() {

  int middle = (TFT_MAX_WIDTH-12*FONT_WIDTH)/2;
  int shift = 15;  
  
  // Select Level
  tft.setTextColor(TFT_WHITE);
  tft.drawString("SELECT MUSIC", middle, 115-shift, GFXFF);
  ui_drawRectBorderWidth(middle-5, 115-5-shift, 12*FONT_WIDTH+10, FONT_WIDTH+15, TFT_GREEN, 3);  

  tft.setTextColor(TFT_GREEN);
  tft.drawString("OFF", middle+20, 145-shift, GFXFF);
  tft.drawString("MUSIC 1", middle+20, 165-shift, GFXFF);
  tft.drawString("MUSIC 2", middle+20, 185-shift, GFXFF);
  tft.drawString("MUSIC 3", middle+20, 205-shift, GFXFF);
  
}

void ui_drawLevelSelectionScreen() {

  byte i;
  byte x;
  byte y;
  int middle = (TFT_MAX_WIDTH-12*FONT_WIDTH)/2;
  int shift = 15;
    
  tft.fillScreen(TFT_BLACK);

  // Screen Border
  ui_drawRectBorderWidth(0, 0, TFT_MAX_WIDTH-1, TFT_MAX_HEIGHT-1, TFT_WHITE, 5);
  tft.setFreeFont(FF18);

  // Select Level
  tft.setTextColor(TFT_WHITE);
  tft.drawString("SELECT LEVEL", middle, 31-shift, GFXFF);
  ui_drawRectBorderWidth(middle-5, 30-5-shift, 12*FONT_WIDTH+10, FONT_WIDTH+15, TFT_RED, 3);

  for (i=0; i<10; i++) {
    ui_drawLevelNumber(i);
  }

  x = middle+10;
  y = 60-shift;
  for (i=0; i<4; i++) {
    tft.drawLine(x, y, x, y+41, TFT_WHITE);
    x = x + 37;
  }
  

  ui_drawRectBorderWidth(middle-28, 66-10-shift, 12*FONT_WIDTH_BIG+20, 46, TFT_WHITE, 2);
  tft.drawLine(middle-28, 63, middle+11*FONT_WIDTH_BIG+5, 63, TFT_WHITE);


  ui_drawMusicSelection();
   
}

void ui_drawEnterNameScreen() {
  tft.fillScreen(TFT_BLACK);

  // Screen Border
  ui_drawRectBorderWidth(0, 0, TFT_MAX_WIDTH-1, TFT_MAX_HEIGHT-1, TFT_GREEN, 5);

  tft.setFreeFont(FF19);
  
  tft.setTextColor(TFT_CYAN);
  tft.drawString("CONGRATULATION!", 15, 30, GFXFF);  

  tft.setTextColor(TFT_YELLOW);
  tft.drawString("Your score made", 15, 75, GFXFF);  
  tft.drawString("it to TOP 3!", 40, 100, GFXFF);  

  tft.setTextColor(TFT_GREEN);
  tft.drawString("Enter your name:", 10, 160, GFXFF);  
  
}

void ui_drawEnterNameCharacter(byte scorePosition, byte charPosition, char selectedChar, bool clean) {

  int x;
  int y;
  x = TOP_SCORE_X + FONT_WIDTH*charPosition;
  y = TOP_SCORE_Y + 25 + 20*(scorePosition-1);

  if (clean) {
    tft.fillRect(x, y-2, FONT_WIDTH, 19, TFT_BLACK);         
  } else {
    tft.fillRect(x, y-2, FONT_WIDTH, 19, TFT_RED); 
  }  
  tft.drawChar(selectedChar, x, y+11, GFXFF);  
}

void ui_drawTopScores(String n1, long s1, byte l1,
                      String n2, long s2, byte l2,
                      String n3, long s3, byte l3) {

  int shift = 25;
  

  // Top Scores box
  ui_drawRectBorderWidth(7, 195+shift, TFT_MAX_WIDTH-15, 92, TFT_YELLOW, 2);
  tft.drawLine(11, 218+shift, TFT_MAX_WIDTH-15, 218+shift, TFT_YELLOW);
  tft.drawLine(11, 219+shift, TFT_MAX_WIDTH-15, 219+shift, TFT_YELLOW);

  // Top Scores
  tft.setFreeFont(FF18);  
  tft.setTextColor(TFT_WHITE);
  tft.drawString("NAME", 40, 200+shift, GFXFF);
  tft.drawString("SCORE", 120, 200+shift, GFXFF);
  tft.drawString("LV", 200, 200+shift, GFXFF);
  
  tft.drawString("1", 15, 225+shift, GFXFF);
  tft.drawString("2", 15, 245+shift, GFXFF);
  tft.drawString("3", 15, 265+shift, GFXFF);

  tft.drawString(n1, TOP_SCORE_X, TOP_SCORE_Y+shift, GFXFF);
  tft.drawString(n2, TOP_SCORE_X, TOP_SCORE_Y+20+shift, GFXFF);
  tft.drawString(n3, TOP_SCORE_X, TOP_SCORE_Y+40+shift, GFXFF);

  sprintf(result, "%06d", s1);
  tft.drawString(result, 120, 225+shift, GFXFF);
  sprintf(result, "%06d", s2);
  tft.drawString(result, 120, 245+shift, GFXFF);
  sprintf(result, "%06d", s3);
  tft.drawString(result, 120, 265+shift, GFXFF);

  sprintf(result, "%02d", l1);
  tft.drawString(result, 200, 225+shift, GFXFF);
  sprintf(result, "%02d", l2);
  tft.drawString(result, 200, 245+shift, GFXFF);
  sprintf(result, "%02d", l3);
  tft.drawString(result, 200, 265+shift, GFXFF);    
  
}

void ui_init() {
  
  spr.createSprite(TILE_SIZE, TILE_SIZE);
  spr.setColorDepth(8);
  
  // Setup the LCD
  tft.init();
  tft.setRotation(0);
  
}

void after_init(){
  spr.createSprite(TILE_SIZE, TILE_SIZE);
  spr.setColorDepth(8);
  tft.setRotation(0); 
}

void setupGame() {
    gameState = START;
    dir = RIGHT;
    newDir = RIGHT;
    resetSnake();
    generateFruit();
    //tft.fillScreen(TFT_BLACK);
    drawMap();
    drawScore();
    // drawPressToStart();
}

void resetSnake() {
    snake_length = STARTING_SNAKE_SIZE;
    for (int i = 0; i < snake_length; i++) {
        snake[i][0] = MAP_SIZE_X / 2 - i;
        snake[i][1] = MAP_SIZE_Y / 2;
    }
}

void generateFruit() {
    bool b = false;
    do {
        b = false;
        fruit[0] = random(0, MAP_SIZE_X);
        fruit[1] = random(0, MAP_SIZE_Y);
        for (int i = 0; i < snake_length; i++) {
            if (fruit[0] == snake[i][0] && fruit[1] == snake[i][1]) {
                b = true;
                continue;
            }
        }
    } while (b);
}

void drawPressToStart() {
    tft.setTextSize(.5); // Increase text size
    tft.setTextColor(TFT_WHITE);

    // Calculate the position to center the text horizontally and vertically
    int16_t x = (SCREEN_WIDTH - tft.textWidth("Press A Button To Start The Game!"));
    int16_t y = (SCREEN_HEIGHT - tft.fontHeight(2)) / 2;

    tft.setCursor(x, y + 2);
    tft.println("         Press any button to\n       Start the game!");
}

bool collisionCheck(int8_t x, int8_t y) {
    for (int i = 1; i < snake_length; i++) {
        if (x == snake[i][0] && y == snake[i][1]) return true;
    }
    if (x < 0 || y < 0 || x >= MAP_SIZE_X || y >= MAP_SIZE_Y) return true;
    return false;
}

void drawMap() {
    int offsetMapX = (SCREEN_WIDTH - SNAKE_PIECE_SIZE * MAP_SIZE_X) / 2; // Center the game area horizontally
    int offsetMapY = (SCREEN_HEIGHT - SNAKE_PIECE_SIZE * MAP_SIZE_Y) / 2; // Center the game area vertically

    // Clear the game area
    tft.fillRect(offsetMapX - 2, offsetMapY - 2, SNAKE_PIECE_SIZE * MAP_SIZE_X + 4, SNAKE_PIECE_SIZE * MAP_SIZE_Y + 4, TFT_BLACK);

    // Draw the fruit
    tft.drawRect(fruit[0] * SNAKE_PIECE_SIZE + offsetMapX, fruit[1] * SNAKE_PIECE_SIZE + offsetMapY, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, TFT_RED);

    // Draw the border
    //tft.drawRect(offsetMapX - 2, offsetMapY - 2, SNAKE_PIECE_SIZE * MAP_SIZE_X + 4, SNAKE_PIECE_SIZE * MAP_SIZE_Y + 4, TFT_WHITE);

    // Draw the snake
    for (int i = 0; i < snake_length; i++) {
        tft.fillRect(snake[i][0] * SNAKE_PIECE_SIZE + offsetMapX, snake[i][1] * SNAKE_PIECE_SIZE + offsetMapY, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, TFT_GREEN);
    }
}

void drawScore() {
    // Define the score area dimensions
    const int16_t scoreAreaX = 5;
    const int16_t scoreAreaY = SCREEN_HEIGHT - 30;
    const int16_t scoreAreaWidth = tft.textWidth("Score: 999"); // Fixed the error here
    const int16_t scoreAreaHeight = tft.fontHeight(2) + 4; // Fixed the error here

    // Clear the score area
    tft.fillRect(scoreAreaX, scoreAreaY, scoreAreaWidth, scoreAreaHeight, TFT_BLACK);

    tft.setTextSize(.5); // Increase text size
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(scoreAreaX, scoreAreaY + 1); // Position the score text within the score area
    tft.print(F("\nScore:"));
    tft.println(snake_length - STARTING_SNAKE_SIZE);
}

void drawGameover() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1.5); // Increase text size
    tft.setTextColor(TFT_WHITE);
    tft.setCursor((SCREEN_WIDTH - tft.textWidth("GAMEOVER")) / 2, SCREEN_HEIGHT / 2 - 20); // Center the text horizontally and vertically
    tft.println("GAMEOVER");
    tft.setTextSize(.5);
    tft.println("   Press RST to continue!");
}

bool buttonPress() {
    for (byte i = 0; i < 4; i++) {
        byte buttonPin = buttonPins[i];
        if (digitalRead(buttonPin) == LOW) {
            return true;
        }
    }
    return false;
}

void readDirection() {
    for (byte i = 0; i < 4; i++) {
        byte buttonPin = buttonPins[i]; 
        if (digitalRead(buttonPin) == LOW) {
            Direction newDirection = (Direction)i;
            if (newDirection != dir && (newDirection == LEFT || newDirection == RIGHT || newDirection == UP || newDirection == DOWN)) {
                newDir = newDirection;
                dir = newDirection;
                return;
            }
        }
    }
}

bool moveSnake() {
    int8_t newX = snake[0][0];
    int8_t newY = snake[0][1];

    if (newDir == LEFT) {
        newX--;
    } else if (newDir == UP) {
        newY--;
    } else if (newDir == RIGHT) {
        newX++;
    } else if (newDir == DOWN) {
        newY++;
    }

    // Handle wrap-around
    if (newX < 0) newX = MAP_SIZE_X - 1;
    if (newX >= MAP_SIZE_X) newX = 0;
    if (newY < 0) newY = MAP_SIZE_Y - 1;
    if (newY >= MAP_SIZE_Y) newY = 0;

    if (collisionCheck(newX, newY))
        return true;

    // Shift the snake body
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i][0] = snake[i - 1][0];
        snake[i][1] = snake[i - 1][1];
    }

    // Update the head position
    snake[0][0] = newX;
    snake[0][1] = newY;

    // Clear the last segment if the snake didn't grow
    if (snake_length < MAX_SANKE_LENGTH) {
        tft.fillRect(snake[snake_length][0] * SNAKE_PIECE_SIZE + (SCREEN_WIDTH - SNAKE_PIECE_SIZE * MAP_SIZE_X) / 2,
                     snake[snake_length][1] * SNAKE_PIECE_SIZE + (SCREEN_HEIGHT - SNAKE_PIECE_SIZE * MAP_SIZE_Y) / 2,
                     SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, TFT_BLACK);
    }

    return false;
}

void checkFruit() {
    if (fruit[0] == snake[0][0] && fruit[1] == snake[0][1]) {
        if (snake_length + 1 <= MAX_SANKE_LENGTH)
            snake_length++;
        generateFruit();
    }
}


void intialize_snake(){
    tft.setRotation(1);  // Set the rotation of the display if needed
    tft.fillScreen(TFT_BLACK);

    for (byte i = 0; i < 4; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
    randomSeed(analogRead(A0));
    //setupGame();
    drawPressToStart();
}

