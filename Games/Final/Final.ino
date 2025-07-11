
//#include <Wire.h>
//#include <Adafruit_GFX.h>
#include "game.h"
int gametype =0;
int update_button = 22;
//#include "Snake_game.h"
void setup() {
  pinMode(update_button, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("test");
  ui_init();
    
  game_start();
}

void switch_game(){
  if (gametype==0){
    gametype=1;
    intialize_snake();
  }
  else{
    ESP.restart();
  }
}


void checkforupdate(){
  if (digitalRead(update_button) == LOW) {
    switch_game();
    delay(500);
  }

}


void loop() {
  if (gametype==0){
  checkforupdate();
  game_btn_checkLeft();
  game_btn_checkRight();
  game_btn_checkDown();
  game_btn_checkRotateCW();
  game_btn_checkRotateCCW();
  actualTime = millis();
  if (actualTime - lastTime >= game_getFallSpeed()) {
 
    lastTime = actualTime;
    if (tetriminos_canBlockMoveDown()) {
      ui_drawBlock(true);
      blockY++;
      ui_drawBlock(false);

    } else {

      tetriminos_placeBlock();                   
      game_updateScore(); 
      game_adjustMusic();
      ui_drawNextBlock(true);
      tetriminos_spawnBlock();      
      game_showStats();    
      ui_drawBlock(false);
      ui_drawNextBlock(false);
      
      if (game_isGameOver()) {
        delay(500);
        ui_drawGameOverCurtain();
        delay(1000);
        game_evaluateFinalScore();
        game_start();       
      }
    }
  }
  }
  else{
    checkforupdate();
    if (gamerunning == 0) {
        if (buttonPress()) {
            tft.fillScreen(TFT_BLACK);
            gamerunning++;
            setupGame(); // Move setupGame() call here
        }
    } else if (gamerunning == 1) {
        moveTime++;
        readDirection();
        // Only update when necessary
        if (moveTime >= SNAKE_MOVE_DELAY) {
            if (moveSnake()) {
                gameState = GAMEOVER;
                gamerunning++;
                drawGameover();
            } else {
                checkFruit();
                drawScore();
                drawMap();

                delay(50);
            }
            moveTime = 0;
        }
        // Redraw map and score

    }
  }
}
