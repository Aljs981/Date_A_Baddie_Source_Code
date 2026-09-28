#include "bn_core.h"
#include "bn_sprites.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "common_variable_8x16_sprite_font.h"
#include "common_fixed_8x8_sprite_font.h"
#include "bn_vector.h"
#include "bn_string.h"
#include "bn_music_items.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_donmac.h"
#include "bn_keypad.h"
#include "bn_sprite_items_dialoguebox.h"
#include "player.h"
#include "bn_regular_bg_items_karinderya.h"
#include "scenemanager.h"
#include "gamemanager.h"


int main(){
   bn::core::init(); //panginitialize ng mga variables, objects bago maggameloop 
     bn::string<12> playerName; //name toh ng player na nakaglobal

     GameState gamestate = GameState::TITLE_SCREEN; //ito yung current scene state

     //ito naman mga class bawat scene may sarili silang object. makikita mo mga 
     // class na ginawa ko sa include folder
     MainmenuScene mainmen; 
     SelectCharacterScene selectcharacterscene;
     Scene datescene;
    

     mainmen.drawOption();
    int currentConvoIndex = -1;
     while(true){ //gameloop
             
        switch(gamestate){
            case GameState::TITLE_SCREEN:
                if(bn::keypad::right_pressed()){
                      mainmen.switchOption();
                } else if(bn::keypad::a_pressed() 
                         && mainmen.getCurrentSelectedOption() == "New Game"){

                     if(mainmen.selectOption()){ 
                         mainmen.stopMusic(); 
                         selectcharacterscene = SelectCharacterScene(true);
                         gamestate = GameState::CHARACTERSELECTION_SCREEN;
                     }
                }
                break;
            case GameState::CHARACTERSELECTION_SCREEN:
                  
                 if(selectcharacterscene.getSceneType() == "playernameselection"){
                          //KEYBOARD INPUT PLAYER NAME PROCESSING
                        if(bn::keypad::left_pressed()){
                            selectcharacterscene.getKeyboard().goLeft();
                        } else if(bn::keypad::right_pressed()){
                            selectcharacterscene.getKeyboard().goRight();
                        } else if(bn::keypad::a_pressed()){

                            if(selectcharacterscene.getKeyboard().getCurrentKey() == "OK"){
                                selectcharacterscene.getKeyboard().clearAll();
                                selectcharacterscene.stopMusic();
                                selectcharacterscene.setSceneType("dateselection");
                            } else{
                                bn::string<10> text = selectcharacterscene.getKeyboard().drawSelected();
                                playerName += text;
                            }
                            
                        }

                    } else{
                            if(bn::keypad::left_pressed()){
                                 selectcharacterscene.leftCharacter();
                                if(!selectcharacterscene.checkIfSwitching() && 
                                    !selectcharacterscene.checkIfMoving()){
                                    selectcharacterscene.switchCharacter();
                                }
                            } else if(bn::keypad::right_pressed()){
                                 selectcharacterscene.rightCharacter();
                                 if(!selectcharacterscene.checkIfSwitching() && !selectcharacterscene.checkIfMoving()){
                                    selectcharacterscene.switchCharacter();
                                }
                            } else if(bn::keypad::a_pressed()){
                                 datescene = Scene(playerName,selectcharacterscene.selectCharacter());
                                 gamestate = GameState::MAIN_GAME;
                                 break;
                            } else{

                            }
                            selectcharacterscene.switchingCharacter();
                            selectcharacterscene.showDatableCharacter();
                            if(selectcharacterscene.checkIfMoving()){
                                selectcharacterscene.moveCharacterLeft();
                            }
                            
                    }
                    
                break;
            case GameState::GAMEOVER_SCREEN:
                break;
            case GameState::MAIN_GAME:
            
                 if(bn::keypad::a_pressed()){
                     datescene.updateScene();
                 } else if(bn::keypad::up_pressed()){
                    datescene.choiceUp();
                 } else if(bn::keypad::down_pressed()){
                      datescene.choiceDown();
                 }
                break;
            default:
                BN_LOG("STATE HAS A PROBLEM");
                break;
        }
         
        bn::core::update(); //naguupdate kada frame ng mga changes
     }
}