#include "grid.h"
#include "tetriminos.h"
#include "ui.h"
#include <Preferences.h>

#define TETRIS 4
#define SOFT_DROP_PERIOD 33 //ms
#define BUTTON_RETRIG_PERIOD 136 //ms

#define BUTTON_LEFT   25
#define BUTTON_RIGHT  27
#define BUTTON_DOWN   26
#define BUTTON_ROTATE_CW 14
#define BUTTON_ROTATE_CCW 33

//Flash memory - top scores
#define TOP_SCORES_INIT "topScoresInit"

#define TOP_SCORE1 "topScore1"
#define TOP_SCORE2 "topScore2"
#define TOP_SCORE3 "topScore3"

#define TOP_SCORE_NAME1 "topScoreName1"
#define TOP_SCORE_NAME2 "topScoreName2"
#define TOP_SCORE_NAME3 "topScoreName3"

#define TOP_SCORE_LEVEL1 "topScoreNameLevel1"
#define TOP_SCORE_LEVEL2 "topScoreNameLevel2"
#define TOP_SCORE_LEVEL3 "topScoreNameLevel3"

const char FLASH_DATA_NOT_INITIALIZED = 111;
const char FLASH_DATA_INITIALIZED = 123;

const long FLASH_TOP_SCORE1 = 696969;
const long FLASH_TOP_SCORE2 = 7500;
const long FLASH_TOP_SCORE3 = 5000;

#define EMPTY_PLAYER_NAME "      ";
#define MAX_NAME_LENGTH 6
const char* FLASH_TOP_SCORE_NAME1 = "HOWARD";
const char* FLASH_TOP_SCORE_NAME2 = "OTASAN";
const char* FLASH_TOP_SCORE_NAME3 = "LANCE";

const char FLASH_TOP_SCORE_LEVEL1 = 9;
const char FLASH_TOP_SCORE_LEVEL2 = 5;
const char FLASH_TOP_SCORE_LEVEL3 = 0;

//                   0lines, 1line, 2lines, 3lines, 4lines
int scoring_table[5] = { 0, 40, 100, 300, 1200 };
// score formula : 40 * (n + 1)   100 * (n + 1)   300 * (n + 1)   1200 * (n + 1), where n = level number

// Stores row numbers that were full.
byte clearedRows[4];

//order of columns for clearing effect (from the middle to sides)
byte clearedEffectColumnsOrder[GRID_COLUMNS] = { 4,5,3,6,2,7,1,8,0,9 };


//in milliseconds, level 30+ = 0.017s
const uint16_t game_fallSpeedsPerLevel[30] = {
799, //0
715, //1
632, //2
549, //3
466, //4
383, //5
300,
216,
133,
100,
83, //10
83,
83,
67,//13
67,
67,
50,//16
50,
50,
33,//19
33,
33,
33,
33,
33,
33,
33,
33,
33,
17
};

bool gameOver;
bool game_downPressed;
bool game_leftPressed;
bool game_rightPressed;
bool game_APressed;
bool game_BPressed;
int8_t game_softDropNumLines;
long lastTime;
long actualTime;
long game_leftPressStart;
long game_rightPressStart;

byte game_level = 6;
int game_linesCleared;
int game_actCleared;
String game_actPlayerName;
long game_score;
long game_topScore = 696969;
int game_music;
//Read from the Flash Memory
char flash_dataUntouched; 

long game_topScore1;
long game_topScore2;
long game_topScore3;

String game_topScoreName1;
String game_topScoreName2;
String game_topScoreName3;

byte game_topScoreLevel1;
byte game_topScoreLevel2;
byte game_topScoreLevel3;

Preferences preferences; //top scores table

void game_initTopScores() {
  
  preferences.begin("tetris", false);

  flash_dataUntouched = preferences.getChar(TOP_SCORES_INIT, FLASH_DATA_NOT_INITIALIZED);

  if (flash_dataUntouched == FLASH_DATA_NOT_INITIALIZED) {

    //Serial.println("Top Scores unitialized, setting up.");
    
    preferences.putChar(TOP_SCORES_INIT, FLASH_DATA_INITIALIZED);
    
    preferences.putLong(TOP_SCORE1, FLASH_TOP_SCORE1);
    preferences.putLong(TOP_SCORE2, FLASH_TOP_SCORE2);
    preferences.putLong(TOP_SCORE3, FLASH_TOP_SCORE3);

    preferences.putString(TOP_SCORE_NAME1, FLASH_TOP_SCORE_NAME1);
    preferences.putString(TOP_SCORE_NAME2, FLASH_TOP_SCORE_NAME2);
    preferences.putString(TOP_SCORE_NAME3, FLASH_TOP_SCORE_NAME3);

    preferences.putChar(TOP_SCORE_LEVEL1, FLASH_TOP_SCORE_LEVEL1); 
    preferences.putChar(TOP_SCORE_LEVEL2, FLASH_TOP_SCORE_LEVEL2); 
    preferences.putChar(TOP_SCORE_LEVEL3, FLASH_TOP_SCORE_LEVEL3);

    game_topScore1 = FLASH_TOP_SCORE1;
    game_topScore2 = FLASH_TOP_SCORE2;
    game_topScore3 = FLASH_TOP_SCORE3;

    game_topScoreName1 = FLASH_TOP_SCORE_NAME1;
    game_topScoreName2 = FLASH_TOP_SCORE_NAME2;
    game_topScoreName3 = FLASH_TOP_SCORE_NAME3;

    game_topScoreLevel1 = FLASH_TOP_SCORE_LEVEL1;
    game_topScoreLevel2 = FLASH_TOP_SCORE_LEVEL2;
    game_topScoreLevel3 = FLASH_TOP_SCORE_LEVEL3;
    
  } else {

    //Serial.println("Top Scores initialized, reading from flash.");

    game_topScore1 = preferences.getLong(TOP_SCORE1, FLASH_TOP_SCORE1);
    game_topScore2 = preferences.getLong(TOP_SCORE2, FLASH_TOP_SCORE2);
    game_topScore3 = preferences.getLong(TOP_SCORE3, FLASH_TOP_SCORE3);

    game_topScoreName1 = preferences.getString(TOP_SCORE_NAME1, FLASH_TOP_SCORE_NAME1);
    game_topScoreName2 = preferences.getString(TOP_SCORE_NAME2, FLASH_TOP_SCORE_NAME2);
    game_topScoreName3 = preferences.getString(TOP_SCORE_NAME3, FLASH_TOP_SCORE_NAME3);

    game_topScoreLevel1 = preferences.getChar(TOP_SCORE_LEVEL1, FLASH_TOP_SCORE_LEVEL1); 
    game_topScoreLevel2 = preferences.getChar(TOP_SCORE_LEVEL2, FLASH_TOP_SCORE_LEVEL2); 
    game_topScoreLevel3 = preferences.getChar(TOP_SCORE_LEVEL3, FLASH_TOP_SCORE_LEVEL3);     
  }

  preferences.end();

}

void game_init() {

  //Uncomment to reset high scores
  //preferences.begin("tetris", false);
  //preferences.clear();
  //preferences.end();
  
  game_initTopScores();

  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ROTATE_CW, INPUT_PULLUP);
  pinMode(BUTTON_ROTATE_CCW, INPUT_PULLUP);

  gameOver = false;

  game_actPlayerName = EMPTY_PLAYER_NAME;
  game_score = 0;  
  game_topScore = game_topScore1;
  game_linesCleared = 0;  
  game_downPressed = false;
  game_leftPressed = false;
  game_rightPressed = false;
  game_APressed = false;
  game_BPressed = false;
  
  grid_clear();
  
  tetriminos_init();      
}

void game_waitForAnyKey() {

  bool pressed = false;

    while (1) {

      if (digitalRead(BUTTON_LEFT) == LOW) {
        return;
      }
      if (digitalRead(BUTTON_RIGHT) == LOW) {
        return;
      }
      if (digitalRead(BUTTON_DOWN) == LOW) {
        return;
      }
      if (digitalRead(BUTTON_ROTATE_CW) == LOW) {
        return;
      }
      if (digitalRead(BUTTON_ROTATE_CCW) == LOW) {
        return;
      }

      delay(20);     
    }
}

void game_waitForNoKey() {

  byte holding;

    while (1) {

      holding = false;
      
      if (digitalRead(BUTTON_LEFT) == LOW) {
        holding = true;
      }

      if (digitalRead(BUTTON_RIGHT) == LOW) {
        holding = true;
      }

      if (digitalRead(BUTTON_DOWN) == LOW) {
        holding = true;
      }

      if (digitalRead(BUTTON_ROTATE_CW) == LOW) {
        holding = true;
      }

      if (digitalRead(BUTTON_ROTATE_CCW) == LOW) {
        holding = true;
      }

      if (!holding) {
        return;
      }
      
      delay(20);     
      
    }
}

void game_intro() {

  ui_drawIntroScreen();

  game_waitForAnyKey();

  game_waitForNoKey();



}

bool game_isGameOver() {  
  if (!tetriminos_blockFits()) {
    gameOver = true;    
    return true;
  } else {
    return false;
  }
}

byte game_checkFullRows() {

  byte cleared = 0;

  //dumpGrid();
  
  for (int8_t r=GRID_ROWS-1; r>=0;  r--) {
    if (grid_isRowFull(r)) {      
      grid_clearRow(r);
      clearedRows[cleared] = r+GRID_HIDDEN_ROWS;
      cleared++;  
    } else if (cleared > 0) {
      grid_shiftRowsRown(r, cleared);   
    }
  }
  
  //dumpGrid();
  return cleared;
  
}

void game_reDrawGrid() {

  int8_t gridValue;
  
  ui_clearGrid();

  //Serial.println("game_reDrawGrid1");
  
  for (byte r=0; r<GRID_ROWS; r++) {
    for (byte c=0; c<GRID_COLUMNS; c++) {
      gridValue = grid[r+GRID_HIDDEN_ROWS][c];
      if (gridValue != GRID_CELL_EMPTY) {
        ui_drawTile(c, r+GRID_HIDDEN_ROWS, gridValue, false, false);
      }
    }
  }
  //Serial.println("game_reDrawGrid2");    
}

void game_drawClearedLinesEffect() {

  byte rowNr;

  //Serial.print("game_actCleared=");
  //Serial.println(game_actCleared);
  //Serial.println("clearedRows:");
  for (byte i=0; i<game_actCleared; i++) {
    rowNr = clearedRows[i];
    //Serial.println(rowNr);
  }
    
  for (byte c=0; c<GRID_COLUMNS; c++) {
    for (byte j=0; j<game_actCleared; j++) {
      ui_drawTile(clearedEffectColumnsOrder[c], clearedRows[j], GRID_CELL_EMPTY, true, false);      
    }
    delay(15);
  }

  if (game_actCleared == TETRIS) {
    ui_celebrateTetris();
  }
}

void game_updateScore() {
  
  if (game_downPressed) {
    game_softDropNumLines = blockY - softDropStartY;
    if (game_softDropNumLines > 0) {
      game_score += game_softDropNumLines;
    }
    //Serial.print("softDrop lines = ");
    //Serial.println(game_softDropNumLines);
  }  

  game_actCleared = game_checkFullRows();

//  Serial.print("game_actCleared=");
//  Serial.println(game_actCleared);  
  
  if (game_actCleared > 0) {

    if (game_actCleared == TETRIS) {
    } else {
    }
   
    game_linesCleared += game_actCleared;
    game_score += (game_level+1) * scoring_table[game_actCleared];

    if (game_linesCleared % 10 == 0) {
      game_level++;
    }
    
    game_drawClearedLinesEffect();
    game_reDrawGrid();    
  } else {
  }  

}

//Check if we need to make the music faster based on the stack state
//game_music == 1/2/3 (normal speed)
//game_music == 4/5/6 (fast - in case there's any block tile in [X,0] to [X,4]
void game_adjustMusic() {

  bool filledEnough;


  filledEnough = grid_isTileInTop5Rows();
  //Serial.print("filledEnough=");

}

void game_btn_checkLeft() {

long actTime;

  if (game_downPressed)
    return;
  
  if (!digitalRead(BUTTON_LEFT) == LOW) {
    game_leftPressed = false;
    return;
  }
  
  delay(5);
  
  if (!digitalRead(BUTTON_LEFT) == LOW) {
    game_leftPressed = false;
    return;
  }

  if (!game_leftPressed) {
    game_leftPressStart = millis();
  } else {
    actTime = millis();
    if (actTime - game_leftPressStart < BUTTON_RETRIG_PERIOD) {
      return;
    } else {
      game_leftPressStart = actTime;
    }
  }
 
  game_leftPressed = true;
           
  if (tetriminos_canBlockMoveLeft()) {
    //Serial.println("can move left");
    ui_drawBlock(true);
    blockX--;           
    ui_drawBlock(false);    
  } else {
    //Serial.println("cannot move left");
  }
  
}

void game_btn_checkRight() {

  long actTime;

  if (game_downPressed)
    return;
  
  if (!digitalRead(BUTTON_RIGHT) == LOW) {
    game_rightPressed = false;
    return;
  }
  
  delay(5);
  
  if (!digitalRead(BUTTON_RIGHT) == LOW) {
    game_rightPressed = false;
    return;
  }

  if (!game_rightPressed) {
    game_rightPressStart = millis();
  } else {
    actTime = millis();
    if (actTime - game_rightPressStart < BUTTON_RETRIG_PERIOD) {
      return;
    } else {
      game_rightPressStart = actTime;
    }
  }  

  game_rightPressed = true;
  
  if (tetriminos_canBlockMoveRight()) {
    //Serial.println("can move right");
    ui_drawBlock(true);
    blockX++;    
    ui_drawBlock(false);    
  } else {
    //Serial.println("cannot move right");
  }
  
}

void game_btn_checkRotateCW() {

  if (game_downPressed)
    return;
  
  if (!digitalRead(BUTTON_ROTATE_CW) == LOW) {
    game_BPressed = false;
    return;
  }
  
  delay(5);
  
  if (!digitalRead(BUTTON_ROTATE_CW) == LOW) {
    game_BPressed = false;
    return;      
  }

  if (!game_BPressed) {
    if (tetriminos_canBlockRotateCW()) {
      //Serial.println("canRotateCW");
      ui_drawBlock(true);

      if (rotationId < NUM_BLOCK_ROTATIONS-1) {
        rotationId++;
      } else {
        rotationId = 0;
      }
      
      //rotationId = (rotationId < NUM_BLOCK_ROTATIONS-1) ? rotationId++ : 0;

      ui_drawBlock(false);    
    } else {
      //Serial.println("CW cannotRotate");
    }

    game_BPressed = true;
  }
  
}

void game_btn_checkRotateCCW() {

  if (game_downPressed)
    return;
  
  if (!digitalRead(BUTTON_ROTATE_CCW) == LOW) {
    game_APressed = false;
    return;
  }
  
  delay(5);
  
  if (!digitalRead(BUTTON_ROTATE_CCW) == LOW) {
    game_APressed = false;
    return;      
  }

  if (!game_APressed) {    
    if (tetriminos_canBlockRotateCCW()) {      
      //Serial.println("canRotateCCW");
      ui_drawBlock(true);

      if (rotationId > 0) {
        rotationId--;
      } else {
        rotationId = NUM_BLOCK_ROTATIONS-1;
      }
      
   //   rotationId = (rotationId > 0) ? rotationId-- : NUM_BLOCK_ROTATIONS-1;
                    
      ui_drawBlock(false);    
    } else {
      //Serial.println("CCW cannotRotate");
    }

    game_APressed = true;
  }
  
}

void game_btn_checkDown() {

  if (game_leftPressed || game_rightPressed)
    return;
  
  if (!digitalRead(BUTTON_DOWN) == LOW) {
    game_downPressed = false;
    return;
  }
  
  delay(5);
  
  if (!digitalRead(BUTTON_DOWN) == LOW){
    game_downPressed = false;
    return;
  }

  if (!game_downPressed) {
    softDropStartY = blockY;
  }
  
  game_downPressed = true;  
  //Serial.print("softDropStartY = ");
  //Serial.println(softDropStartY);
      
}

void game_levelAndMusicSelection() {

    game_music = 0;   // OFF

    bool downPressed = false;        
    int i = 25;
    byte xSkip = 37;
    byte ySkip = 21;
    byte x = 0;
    byte y = 0;

    ui_drawLevelSelectRectangle(x, y, false);
    ui_drawLevelNumber(i);
    ui_drawMusicSelectionPointer(game_music, false);
    
    while(1) {   

      if (digitalRead(BUTTON_DOWN) == LOW) {
        
        if (downPressed == false) {
          downPressed = true;
          ui_drawLevelPlus10(false);
          delay(200);
        } else {
          downPressed = false;
          ui_drawLevelPlus10(true);
          delay(200);
        }
      } 
     
      if (digitalRead(BUTTON_LEFT) == LOW) {

          ui_drawLevelSelectRectangle(x, y, true);
          ui_drawLevelNumber(i);
          
          if (i == 0) {
              i = 9;
              x = 4 * xSkip;
              y = ySkip;
          }
          else if (i == 5) {
              i = 4;
              y = 0;
              x = 4 * xSkip;
          } else if ((i >= 1 && i <= 4) || (i > 5 && i <= 9)) {
              i--;
              x = x - xSkip;
          }

        ui_drawLevelSelectRectangle(x, y, false);
        ui_drawLevelNumber(i);          
        delay(100);          
      }
      if (digitalRead(BUTTON_RIGHT) == LOW) {

        ui_drawLevelSelectRectangle(x, y, true);
        ui_drawLevelNumber(i);
          
          if (i == 4) {
              i = 5;
              x = 0;
              y = ySkip;
          }
          else if (i == 9) {
              i = 0;
              y = 0;
              x = 0;
          } else if ((i >= 0 && i < 4) || (i >= 5 && i < 9)) {
              i++;
              x = x + xSkip;
          }

        ui_drawLevelSelectRectangle(x, y, false);
        ui_drawLevelNumber(i);
        delay(100);        
      }

      
      
      if (digitalRead(BUTTON_ROTATE_CW) == LOW) {        
        game_level = 25;
        if (downPressed) {
          game_level += 10;
        }
        return;
      }
  
      if (digitalRead(BUTTON_ROTATE_CCW) == LOW) {
         ui_drawMusicSelectionPointer(game_music, true);
          game_music++;
          if (game_music == 4) {
            game_music = 0;
          }
          //mp3.playFolder(MP3_SD_CARD_FOLDER_NR, game_music);
          ui_drawMusicSelectionPointer(game_music, false);
          delay(500);     
     } 
  }
}

void game_showTopScores() {
  ui_drawTopScores(
    game_topScoreName1, game_topScore1, game_topScoreLevel1,
    game_topScoreName2, game_topScore2, game_topScoreLevel2,
    game_topScoreName3, game_topScore3, game_topScoreLevel3);  
}

uint16_t game_getFallSpeed() {

  uint16_t tmp;
  
  if (game_level <= 29) {
    tmp = game_fallSpeedsPerLevel[game_level];
  } else {
    tmp = game_fallSpeedsPerLevel[29];
  }

  if (game_downPressed) {    
    return SOFT_DROP_PERIOD;
  } else {
    return tmp;
  }  
}

void game_showStats() {
  ui_drawScore(game_score);
  ui_drawLinesCount(game_linesCleared);
  ui_drawLevel(game_level);
}

void game_showTopScore() {
  ui_drawTopScore(game_topScore);
}

void game_dumpTopScores() {
  Serial.print("1 ");  Serial.print(game_topScore1); Serial.print(" ");  Serial.print(game_topScoreName1); Serial.print(" "); Serial.println(game_topScoreLevel1);
  Serial.print("2 ");  Serial.print(game_topScore2); Serial.print(" ");  Serial.print(game_topScoreName2); Serial.print(" "); Serial.println(game_topScoreLevel2);
  Serial.print("3 ");  Serial.print(game_topScore3); Serial.print(" ");  Serial.print(game_topScoreName3); Serial.print(" "); Serial.println(game_topScoreLevel3);
}

void game_enterPlayerName(byte scorePosition) {

  const char START_CHAR = 'A';
  const char ASCII_MAX = '_';
  const char ASCII_MIN = ' ';
  char currentChar = START_CHAR;
  byte index = 0;
  

  ui_drawEnterNameScreen();
  game_showTopScores();

  ui_drawEnterNameCharacter(scorePosition, index, currentChar, false);

  while (index < MAX_NAME_LENGTH) {

    if (digitalRead(BUTTON_LEFT) == LOW) {
      
       if (currentChar == ASCII_MIN) {
          currentChar = ASCII_MAX;
       } else {
          currentChar--;
       }

       ui_drawEnterNameCharacter(scorePosition, index, currentChar, false);
       delay(100);          
       
    } else if (digitalRead(BUTTON_RIGHT) == LOW) {
  
       if (currentChar == ASCII_MAX) {
          currentChar = ASCII_MIN;
       } else {
          currentChar++;
       }


       ui_drawEnterNameCharacter(scorePosition, index, currentChar, false);
       delay(100);          
       
    } else if (digitalRead(BUTTON_ROTATE_CW) == LOW) {    //confirm character
       game_actPlayerName[index] = currentChar;
       ui_drawEnterNameCharacter(scorePosition, index, currentChar, true);
       index++;       
       if (index < MAX_NAME_LENGTH) {
         ui_drawEnterNameCharacter(scorePosition, index, currentChar, false);       
       }
       delay(500);                            
    } else if (digitalRead(BUTTON_ROTATE_CCW) == LOW) {   //backspace
      if (index > 0) {
        game_actPlayerName[index] = ' ';
        currentChar = ' ';
        ui_drawEnterNameCharacter(scorePosition, index, currentChar, true);
        index--;
        currentChar = game_actPlayerName[index];
        ui_drawEnterNameCharacter(scorePosition, index, currentChar, false);
        delay(200);
      }
    }

    delay(20);
    
  };


  //Serial.println(game_actPlayerName);
  
  if (scorePosition == 1) {
    game_topScoreName1 = game_actPlayerName;
  } else if (scorePosition == 2) {
    game_topScoreName2 = game_actPlayerName;
  } else if (scorePosition == 3) {
    game_topScoreName3 = game_actPlayerName;
  }

  //game_dumpTopScores();
}

void game_storeScoresToFlash() {
  
  preferences.begin("tetris", false);

  preferences.putLong(TOP_SCORE1, game_topScore1);
  preferences.putLong(TOP_SCORE2, game_topScore2);
  preferences.putLong(TOP_SCORE3, game_topScore3);

  preferences.putString(TOP_SCORE_NAME1, game_topScoreName1);
  preferences.putString(TOP_SCORE_NAME2, game_topScoreName2);
  preferences.putString(TOP_SCORE_NAME3, game_topScoreName3);

  preferences.putChar(TOP_SCORE_LEVEL1, game_topScoreLevel1); 
  preferences.putChar(TOP_SCORE_LEVEL2, game_topScoreLevel2); 
  preferences.putChar(TOP_SCORE_LEVEL3, game_topScoreLevel3);

  preferences.end();
  
}

void game_evaluateFinalScore() {
  
  bool updateScores = false;
  byte scorePosition;

  if (game_score > game_topScore1) {

    updateScores = true;
    scorePosition = 1;
    
    game_topScore3 = game_topScore2;
    game_topScoreName3 = game_topScoreName2;
    game_topScoreLevel3 = game_topScoreLevel2;

    game_topScore2 = game_topScore1;
    game_topScoreName2 = game_topScoreName1;
    game_topScoreLevel2 = game_topScoreLevel1;

    game_topScore1 = game_score;
    game_topScoreName1 = game_actPlayerName;
    game_topScoreLevel1 = game_level;            
    
  } else if (game_score > game_topScore2) {

    updateScores = true;
    scorePosition = 2;
    
    game_topScore3 = game_topScore2;
    game_topScoreName3 = game_topScoreName2;
    game_topScoreLevel3 = game_topScoreLevel2;

    game_topScore2 = game_score;
    game_topScoreName2 = game_actPlayerName;
    game_topScoreLevel2 = game_level;
    
  } else if (game_score > game_topScore3) {

    updateScores = true;
    scorePosition = 3;
    
    game_topScore3 = game_score;
    game_topScoreName3 = game_actPlayerName;
    game_topScoreLevel3 = game_level;                
   
  }

  if (updateScores) {
    //mp3.playFolder(1, MP3_SONG_TOP_SCORE);
    game_enterPlayerName(scorePosition);
    game_storeScoresToFlash();
  }
  delay(5000);        
}

void game_start() {
  
  game_init();
  //mp3.playFolder(MP3_SD_CARD_FOLDER_NR, MP3_SONG_INTRO);

  game_intro();  

  game_showTopScores();

  ui_drawPlayground();
  
  game_showTopScore();  
    
  tetriminos_spawnBlock();

  game_showStats();  



  ui_drawBlock(false);

  ui_drawNextBlock(false);

  lastTime = millis();
}
