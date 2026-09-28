#ifndef PLAYER_H
#define PLAYER_H
#include "bn_string.h"
#include "bn_affine_bg_item.h"


struct Character{
    bn::affine_bg_item characterSprite;
    bn::string<10> characterName;
 };
 
 
class Player{
    private:
        bn::string<16> playername;
        int rizz;
    
    public:
        Player(){
             this->playername = "John";
            this->rizz = 100;
        }
        Player(bn::string<64> playername){
            this->playername = playername;
            this->rizz = 100;
        }

        bn::string<16>& getPlayerName(){
            return playername;
        }
        
        void setPlayerName(bn::string<16> playername){
            this->playername = playername;
        }

        void setRizz(int rizz){
            this->rizz = rizz;
        }
        
        int getRizz(){
            return rizz;
        }

        
};

#endif