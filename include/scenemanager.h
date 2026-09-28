#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "bn_core.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_donmac.h"
#include "bn_regular_bg_items_karinderya.h"
#include "bn_regular_bg_items_fishballan.h"
#include "bn_sprite_item.h"
#include "bn_string.h"
#include "player.h"
#include "yourdate.h"
#include "bn_optional.h"
#include "bn_vector.h"
#include "bn_array.h"
#include "fixed_32x64_sprite_font.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_titlescreen.h"
#include "bn_affine_bg_item.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_items_dateselectionchinita.h"
#include "bn_affine_bg_items_dateselectionmestiza.h"
#include "bn_affine_bg_items_dateselectionmorena.h"
#include "bn_music_items.h"
#include "utility.h"
#include "bn_music.h"

    struct Setting{
      bn::regular_bg_item backgroundImage;
      bn::string<16> backgroundName;
    };
    

    class Scene{
      private:
        YourDate yourdate;
        bn::optional<bn::regular_bg_ptr> _chosenbgsprite;
        bn::optional<bn::music_item> currentMusic;
        
      
      public:
        Scene(){
          
        }
        
      Scene(bn::string<64> playername, bn::string<64> chosendate){
          Player newPlayer = Player(playername);

          // Message templates - max 3 words bawat linya, ALL CAPS
          bn::array<bn::string<32>,50> messageTexts = {
              "HELLO CUTIE, ", "MISS KITA, ", "ANG GANDA MO, ", "GUSTO KITA, ",
              "KUMAIN KA NA, ", "IKAW LANG, ", "TYPE KITA, ", "ANG SAYA MO, ",
              "MAHAL KITA, ", "ANG CUTE MO, ", "SABIK AKO SAYO, ", "KILIG AKO SAYO, ",
              "HALIKA DITO, ", "IKAW BA YUN, ", "ANG BAIT MO, ", "TARA NA, ",
              "ANG TAGAL MO, ", "TYPE MO BA, ", "ANG INIT MO, ", "IKAW PA RIN, ",
              "ANG LAMIG NGAYON, ", "GUTOM KA NA, ", "ANG CUTE TALAGA, ", "MISS NA MISS, ",
              "IKAW NA LANG, ", "ANG GANDA TALAGA, ", "SALAMAT SAYO, ", "ANG SWEET MO, ",
              "WAG MALUNGKOT, ", "BAKIT UMIIYAK KA, ", "IKAW ANG SPECIAL, ", "ANG SAYA NGAYON, ",
              "ANONG BALITA MO, ", "SINO KA BA, ", "ANONG GINAGAWA MO, ", "SAAN TAYO PUNTA, ",
              "KAILAN TAYO ULIT, ", "ANG NGITI MO, ", "ALAM MO BA, ", "IKAW SA PANAGINIP, ",
              "GRABE KA TALAGA, ", "BAKIT KA NANDITO, ", "CUTE KAPAG GALIT, ", "KAYA MO BA, ",
              "TOTOO BA TAYO, ", "ANG ESPESYAL MO, ", "PUWEDE BA KITA, ", "ILANG TAON KA, ",
              "ANONG PABORITO MO, ", "SALAMAT SA PAGSAMA, "
          };

          // 10 sets ng choices, 1 salita bawat choice, ALL CAPS
          bn::array<bn::array<bn::string<12>,3>,10> choiceSets = {{
              {"GOOD","BAD","CREEPY"},
              {"FLIRTY","RUDE","WEIRD"},
              {"SWEET","HURTFUL","CONFUSED"},
              {"COMPLIMENT","DENY","CREEPY"},
              {"ROMANTIC","HURTFUL","SURPRISED"},
              {"THANKFUL","ANNOYED","CREEPY"},
              {"HONEST","JOKE","SHY"},
              {"AGREE","REFUSE","TEASE"},
              {"CARING","COLD","NERVOUS"},
              {"EXCITED","BORED","CURIOUS"}
          }};

          // Response TEXT na nakatapat bawat index sa choiceSets - dapat magkatugma ang meaning
          bn::array<bn::array<bn::string<20>,3>,10> responseTexts = {{
              {"HAHAHAHAH","BAKIT GANYAN","ANO BA YAN"},           // GOOD, BAD, CREEPY
              {"NAKS SALAMAT","BASTOS KA PALA","HA ANO SABI MO"},   // FLIRTY, RUDE, WEIRD
              {"ANG SWEET MO","BAKIT MO SINABI","HA HINDI GETS"},   // SWEET, HURTFUL, CONFUSED
              {"SALAMAT DIYAN","HINDI TOTOO YAN","OKAY LANG YAN"},  // COMPLIMENT, DENY, CREEPY
              {"MAHAL KITA DIN","BAKIT GANYAN","GRABE BIGLA NAMAN"},// ROMANTIC, HURTFUL, SURPRISED
              {"WALANG ANUMAN","SORRY NA NGA","INTERESTING NAMAN"}, // THANKFUL, ANNOYED, CREEPY
              {"SALAMAT SA HONESTY","HAHA NGA NAMAN","BAKIT KA MAHIYA"}, // HONEST, JOKE, SHY
              {"SIGE TARA NA","SAYANG NAMAN YUN","HOY HUWAG NGA"},  // AGREE, REFUSE, TEASE
              {"SALAMAT SA ALALA","OKAY LANG YAN","WAG KA MAG ALALA"}, // CARING, COLD, NERVOUS
              {"TARA GALING DUN","SORRY KUNG GANUN","ALAMIN MO NA LANG"} // EXCITED, BORED, CURIOUS
          }};

          // Reaction index bawat response: 0=smiling 1=laughing 2=blush 3=shocked 4=crying 5=angry 6=creepy
          bn::array<bn::array<int,3>,10> responseReactionIndex = {{
              {1,5,3}, {2,5,3}, {2,4,3}, {0,5,6}, {2,4,3},
              {0,5,6}, {0,1,2}, {0,4,1}, {2,5,3}, {1,4,6}
          }};

           if(chosendate == "chinita"){
              yourdate = Chinita(newPlayer);
               changeBackground(1);

               bn::array<bn::sprite_item,7> chinitaReactions = {
                   bn::sprite_items::chinitasmiling,
                   bn::sprite_items::chinitalaughing,
                   bn::sprite_items::chinitablush,
                   bn::sprite_items::chinitashocked,
                   bn::sprite_items::chinitacrying,
                   bn::sprite_items::chinitaangry,
                   bn::sprite_items::chinitacreepy
               };

               for(int i = 0; i < 50; ++i){
                   int setIndex = i % 10;
                   bn::array<bn::string<12>,3>& currentChoiceSet = choiceSets[setIndex];
                   bn::array<bn::string<20>,3>& currentResponseTexts = responseTexts[setIndex];
                   bn::array<int,3>& currentReactionIdx = responseReactionIndex[setIndex];

                   bn::vector<bn::string<12>,3> choices;
                   choices.push_back(currentChoiceSet[0]);
                   choices.push_back(currentChoiceSet[1]);
                   choices.push_back(currentChoiceSet[2]);

                   // Index 0 ng choiceresponses ay tumutugma sa index 0 ng choices, atbp.
                   ChoiceResponse choiceresponse1(chinitaReactions[currentReactionIdx[0]], currentResponseTexts[0]);
                   ChoiceResponse choiceresponse2(chinitaReactions[currentReactionIdx[1]], currentResponseTexts[1]);
                   ChoiceResponse choiceresponse3(chinitaReactions[currentReactionIdx[2]], currentResponseTexts[2]);

                   bn::vector<ChoiceResponse,3> choiceresponses;
                   choiceresponses.push_back(choiceresponse1);
                   choiceresponses.push_back(choiceresponse2);
                   choiceresponses.push_back(choiceresponse3);

                   yourdate.getDialogues().addMessage(chinitaReactions[i % 7],
                       "CHINITA", messageTexts[i] + playername, choices, choiceresponses);
               }

           } else if(chosendate == "mestiza"){
              yourdate = Mestiza(newPlayer);
               changeBackground(2);

               bn::array<bn::sprite_item,7> mestizaReactions = {
                   bn::sprite_items::mestizasmiling,
                   bn::sprite_items::mestizalaughing,
                   bn::sprite_items::mestizablush,
                   bn::sprite_items::mestizashocked,
                   bn::sprite_items::mestizacrying,
                   bn::sprite_items::mestizaangry,
                   bn::sprite_items::mestizacreepy
               };

               for(int i = 0; i < 50; ++i){
                   int setIndex = i % 10;
                   bn::array<bn::string<12>,3>& currentChoiceSet = choiceSets[setIndex];
                   bn::array<bn::string<20>,3>& currentResponseTexts = responseTexts[setIndex];
                   bn::array<int,3>& currentReactionIdx = responseReactionIndex[setIndex];

                   bn::vector<bn::string<12>,3> choices;
                   choices.push_back(currentChoiceSet[0]);
                   choices.push_back(currentChoiceSet[1]);
                   choices.push_back(currentChoiceSet[2]);

                   ChoiceResponse choiceresponse1(mestizaReactions[currentReactionIdx[0]], currentResponseTexts[0]);
                   ChoiceResponse choiceresponse2(mestizaReactions[currentReactionIdx[1]], currentResponseTexts[1]);
                   ChoiceResponse choiceresponse3(mestizaReactions[currentReactionIdx[2]], currentResponseTexts[2]);

                   bn::vector<ChoiceResponse,3> choiceresponses;
                   choiceresponses.push_back(choiceresponse1);
                   choiceresponses.push_back(choiceresponse2);
                   choiceresponses.push_back(choiceresponse3);

                   yourdate.getDialogues().addMessage(mestizaReactions[i % 7],
                       "MESTIZA", messageTexts[i] + playername, choices, choiceresponses);
               }

           } else if(chosendate == "morena"){
               yourdate = Morena(newPlayer);
                changeBackground(3);

               bn::array<bn::sprite_item,7> morenaReactions = {
                   bn::sprite_items::morenasmiling,
                   bn::sprite_items::morenalaughing,
                   bn::sprite_items::morenablush,
                   bn::sprite_items::morenashocked,
                   bn::sprite_items::morenacrying,
                   bn::sprite_items::morenaangry,
                   bn::sprite_items::morenacreepy
               };

               for(int i = 0; i < 50; ++i){
                   int setIndex = i % 10;
                   bn::array<bn::string<12>,3>& currentChoiceSet = choiceSets[setIndex];
                   bn::array<bn::string<20>,3>& currentResponseTexts = responseTexts[setIndex];
                   bn::array<int,3>& currentReactionIdx = responseReactionIndex[setIndex];

                   bn::vector<bn::string<12>,3> choices;
                   choices.push_back(currentChoiceSet[0]);
                   choices.push_back(currentChoiceSet[1]);
                   choices.push_back(currentChoiceSet[2]);

                   ChoiceResponse choiceresponse1(morenaReactions[currentReactionIdx[0]], currentResponseTexts[0]);
                   ChoiceResponse choiceresponse2(morenaReactions[currentReactionIdx[1]], currentResponseTexts[1]);
                   ChoiceResponse choiceresponse3(morenaReactions[currentReactionIdx[2]], currentResponseTexts[2]);

                   bn::vector<ChoiceResponse,3> choiceresponses;
                   choiceresponses.push_back(choiceresponse1);
                   choiceresponses.push_back(choiceresponse2);
                   choiceresponses.push_back(choiceresponse3);

                   yourdate.getDialogues().addMessage(morenaReactions[i % 7],
                       "MORENA", messageTexts[i] + playername, choices, choiceresponses);
               }
           }
           playMusic();
        }

        void playMusic(){
          currentMusic = bn::music_items::ifheartfallsdown;
          currentMusic->play(0.5);
        }

        void stopMusic(){
           bn::music::stop();
        }
    
       void changeBackground(int backgroundNumber){
      
        bn::optional<bn::regular_bg_item> chosenBackground = bn::nullopt;
          switch(backgroundNumber){
            case 1:
                chosenBackground = bn::regular_bg_items::fishballan;
              break;
            case 2:
                  chosenBackground = bn::regular_bg_items::donmac;
              break;
            case 3:
                  chosenBackground = bn::regular_bg_items::karinderya;
              break;
            default:
             // BN_LOG("TEST");
              break;
          }
         
            if(chosenBackground){
              _chosenbgsprite = chosenBackground->create_bg(0, 0);
            }
           
         
       }

       void updateScene(){
           yourdate.getDialogues().showMessage();
        //  Dialogue currentdialogue = yourdate.nextDialogue();
          // yourdate.drawSprite(currentdialogue.reaction);
           if(yourdate.getMood() == "happy"){
                // yourdate.drawDialogue(currentdialogue.message.positiveMessage);
           } else if(yourdate.getMood() == "angry"){
              //  yourdate.drawDialogue(currentdialogue.message.negativeMessage);
           }
        
          
       }

       void choiceUp(){
           if(!yourdate.getDialogues().isDialogueRunning()){
             BN_LOG("CANNOT SWITCH CHOICE TO UP! DIALOGUE IS NOT RUNNING YET");
             return;
           }
          yourdate.getDialogues().upArrow();
       }

       void choiceDown(){
         if(!yourdate.getDialogues().isDialogueRunning()){
             BN_LOG("CANNOT SWITCH CHOICE TO DOWN! DIALOGUE IS NOT RUNNING YET");
             return;
           }
          yourdate.getDialogues().downArrow();
       }
       
       YourDate& getYourDate(){
         return this->yourdate;
       }
         
    };


    class MainmenuScene{
       private:
         int currentSelectedIndex;
         bn::optional<bn::music_item> currentMusic;
         bn::optional<bn::regular_bg_ptr> currentBackgroundSprite; 
         bn::vector<bn::sprite_ptr,10> optionSprite;
         bn::vector<bn::string<10>,3> options;
         bn::sprite_text_generator mainmenutextgen{fixed_32x64_sprite_font};
         
         
       public:
       /*: gamestate(gamestate), scenemanager(scenemanager)*/
         MainmenuScene(){
            options.push_back("New Game");
            options.push_back("Exit");
            currentSelectedIndex = 0;
               playMusic();
               drawBackground();
         }
       
       void switchOption(){
           ++currentSelectedIndex;
            if(currentSelectedIndex > 1){
               currentSelectedIndex = 0;
            }
            drawOption();
       }
       
       void drawOption(){
          optionSprite.clear();
          mainmenutextgen.generate(-103,16,getCurrentSelectedOption(),optionSprite);
       }

       void drawBackground(){
          currentBackgroundSprite = bn::regular_bg_items::titlescreen.create_bg(0,0);
       }
         
       void playMusic(){
          currentMusic = bn::music_items::maintheme;
          currentMusic->play(0.5);
       }

       void stopMusic(){
         bn::music::stop();
          currentMusic.reset();
       }
       bn::string<10>& getCurrentSelectedOption(){
            return options[this->currentSelectedIndex];
       }
     

       bool selectOption(){
         if(getCurrentSelectedOption() == "New Game"){
            this->optionSprite.clear();
            this->currentBackgroundSprite.reset();
            this->currentMusic.reset();
         }
          return true;
      }
      
    };
   
    class SelectCharacterScene{
   private:
      bn::optional<bn::affine_bg_ptr> currentCharacterSprite;
      bn::vector<Character,3> dateSelections;
      int currentCharacterIndex;
      int currentCharacterX;
      int currentCharacterY;
      bool isMoving;
      bool isSwitching;
      bn::string<32> sceneType; 
      KeyboardMaker keyboard;
      bn::optional<bn::music_item> currentMusic;
      
   public:
     SelectCharacterScene(){
      
     }
     
     SelectCharacterScene(bool executeScene){
         if(executeScene){
           keyboard.drawKeyboard();
           playMusic();
           dateSelections.push_back({bn::affine_bg_items::dateselectionchinita,"chinita"});
           dateSelections.push_back({bn::affine_bg_items::dateselectionmestiza,"mestiza"});
           dateSelections.push_back({bn::affine_bg_items::dateselectionmorena,"morena"});
           sceneType = "playernameselection";
           currentCharacterIndex = 0;
            currentCharacterX = 120;
            currentCharacterY = 0;
            isMoving = false;
            isSwitching = false;
         }
     }

     void showDatableCharacter(){
        if(currentCharacterIndex > 2){      
           currentCharacterIndex = 0;
        }

        if(currentCharacterIndex < 0){
           currentCharacterIndex = 0;
        }

       if(sceneType == "dateselection" && !isMoving && !isSwitching){   // <-- fix: idinagdag ang !isSwitching
           currentCharacterSprite = dateSelections[currentCharacterIndex].characterSprite.create_bg(currentCharacterX,currentCharacterY);
           currentCharacterSprite->set_wrapping_enabled(false); 
           isMoving = true;
        }
     }

     void nextDatableCharacter(){
        if(sceneType == "dateselection" && !isMoving && !isSwitching){
           isSwitching = true;
        }
     }

     bool& checkIfMoving(){
        return this->isMoving;
     }

     bool& checkIfSwitching(){
        return this->isSwitching;
     }

     void switchCharacter(){
        this->isSwitching = true;
     }
     void setSceneType(bn::string<32> sceneType){
       this->sceneType = sceneType;
     }

     void moveCharacterLeft(){
        if(isMoving && !isSwitching){
            if(currentCharacterX > 0){
                --currentCharacterX;
              currentCharacterSprite->set_x(currentCharacterX);
            } else{
              currentCharacterX = 0;
              currentCharacterSprite->set_x(currentCharacterX);
              isMoving = false;
            }
        }

     }
      
     void resetAll(){
        currentCharacterSprite.reset();
        isMoving = false;
        isSwitching = false;
     }

     bn::string<34> selectCharacter(){
        resetAll();
        return dateSelections[currentCharacterIndex].characterName;
     }

     void rightCharacter(){
       ++currentCharacterIndex;
     }
    
     void leftCharacter(){
       --currentCharacterIndex;
     }
     void switchingCharacter(){
        if(isSwitching && !isMoving){
           if(currentCharacterX > -120){
              --currentCharacterX;
              currentCharacterSprite->set_x(currentCharacterX);
           } else{
               currentCharacterSprite.reset();
               currentCharacterX = 120;
               isSwitching = false;         // <-- inilipat: i-set false BAGO tawagin showDatableCharacter,
               showDatableCharacter();      //     para agad gumawa ng bagong sprite sa parehong frame
           }
        }
     }
     
     void playMusic(){
       currentMusic = bn::music_items::thequestion;
       currentMusic->play(0.5);
     }

     void stopMusic(){
       bn::music::stop();
       currentMusic.reset();
     }
    
     bn::string<32>& getSceneType(){
        return this->sceneType;
     }
     KeyboardMaker& getKeyboard(){
       return this->keyboard;
     }
};
  

#endif