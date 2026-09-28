// dialogue.h
#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_string.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_common_fixed_8x8_font.h"
#include "bn_sprite_items_dialoguebox.h"
#include "bn_sprite_items_arrow.h"
#include "bn_sprite_items_button.h"
#include "bn_array.h"


struct Message{
    bn::string<32> positiveMessage;
    bn::string<32> negativeMessage;
};


// struct Dialogue
// {
//     int id;
//     bn::sprite_item reaction;
//     Message message;
// };

struct ChoiceResponse{
    bn::sprite_item actorImage;
    bn::string<64> responseMessage;
};

struct Dialogue{
    bn::sprite_item speakerImage;
    bn::string<15> speakerName;
    bn::string<64> message;
    bn::vector<bn::string<12>,3> choices;
    bn::vector<ChoiceResponse,3> choiceResponses;
};

class Dialogues{
    private:
    //   bn::vector<bn::string<64>,65> messageList;
    //   bn::string<15> speakerName;
    //   bn::string<64> currentMessage;
    
      bn::sprite_text_generator dialoguegen{common::fixed_8x8_sprite_font}; 
      bn::sprite_text_generator choicegen{common::fixed_8x8_sprite_font};
      bn::optional<bn::sprite_ptr> dialoguebox;
      bn::optional<bn::sprite_ptr> arrow;
      bn::vector<bn::sprite_ptr,50> choicesSprite;
      bn::vector<bn::sprite_ptr,64> currentMessageSprite;
      bn::optional<bn::sprite_ptr> currentSpeakerImage;

      bn::vector<Dialogue,120> dialogueList;
    //   bn::vector<bn::sprite_item,35> speakerImages;
      int currentDialogueIndex;
      int currentChoiceIndex;

      int globalChoiceX;
      bool dialogueIsRunning = false;
      bool spritesDisplayed = false;
    

     
    public:
        Dialogues(){
            currentDialogueIndex = -1;
            currentChoiceIndex = -1;
            globalChoiceX = 0;
        }
        
        void addMessage(
            bn::sprite_item speakerImage,
            bn::string<15> speakerName,
            bn::string<64> message,
            bn::vector<bn::string<12>,3> choices, //pagpipipiliaan mo
            bn::vector<ChoiceResponse,3> choiceResponses //possible responses
        ){
            dialogueList.push_back({speakerImage,speakerName,message,choices,choiceResponses});
        }

        void showMessage(){
                if(spritesDisplayed){
                    BN_LOG("CANNOT SHOW DIALOGUE! ITS ALREADY CREATED");
                    showResponse();
                    spritesDisplayed = false;
                    dialogueIsRunning = false;
                    return;
                }
            clearAllSprites();
            nextMessage();
            createDialogueBox();
             dialogueIsRunning = true;
             showChoices();
            dialoguegen.generate(-57,41,dialogueList[currentDialogueIndex].message,currentMessageSprite);
            currentSpeakerImage = dialogueList[currentDialogueIndex].speakerImage.create_sprite(0,2);
            spritesDisplayed = true;
        }

        void createDialogueBox(){
             dialoguebox = bn::sprite_items::dialoguebox.create_sprite(-1,49);
            dialoguebox->set_horizontal_scale(2);
        }

        void showResponse(){
            clearAllSprites();
            createDialogueBox();
            dialoguegen.generate(-57,41,dialogueList[currentDialogueIndex].choiceResponses[currentChoiceIndex].responseMessage,currentMessageSprite);
            currentSpeakerImage = dialogueList[currentDialogueIndex].choiceResponses[currentChoiceIndex].actorImage.create_sprite(0,2);
        }

        void nextMessage(){
            currentDialogueIndex++;
            if(currentDialogueIndex >= dialogueList.size()){
                currentDialogueIndex = dialogueList.size() - 1;
                BN_LOG("DIALOGUS: REACHED CURRENT DIALOGUE INDEX LIMIT");
            }
        }

        void previousMessage(){
            currentDialogueIndex--;
            if(currentDialogueIndex < 0){
                currentDialogueIndex = 0;
                BN_LOG("DIALOGUS: CURRENT DIALOGUE INDEX CANNOT BE -1");
            }
        }
        
        bn::vector<bn::string<12>,3>& getCurrentChoices(){
            return this->dialogueList[currentDialogueIndex].choices;
        }

        ChoiceResponse getCurrentResponse(){
            return this->dialogueList[currentDialogueIndex].choiceResponses[currentChoiceIndex];
        }

       

        void showChoices(){
            arrow.reset();
            currentChoiceIndex = 0;

            if(dialogueIsRunning){
                globalChoiceX = 31;
                int currentY = 21;
    

                 for(int i = 0; i < getCurrentChoices().size(); i++){
                        if(i == 0){
                            choicegen.generate(globalChoiceX,currentY,getCurrentChoices()[i],choicesSprite);
                            continue;
                        }
                        
                        double prevChoiceY = choicesSprite[i - 1].y().to_double();

                        choicegen.generate(globalChoiceX,prevChoiceY - 10,getCurrentChoices()[i],choicesSprite);

                     
                 }
                 createArrow();
            }
        }

        void createArrow(){
             int currentChoiceY = choicesSprite[currentChoiceIndex].y().to_double();
             if(dialogueIsRunning){
                arrow = bn::sprite_items::arrow.create_sprite(globalChoiceX - 10,currentChoiceY);
             }
            
        }

        void upArrow(){
            ++currentChoiceIndex;

             if(currentChoiceIndex >= dialogueList[currentDialogueIndex].choices.size()){
                --currentChoiceIndex;
             }
             
          
             double currentChoiceY = choicesSprite[currentChoiceIndex].y().to_double();
           arrow->set_position(globalChoiceX - 10,currentChoiceY);
            arrow->put_above();
            BN_LOG("CURRENT CHOICE: ",dialogueList[currentDialogueIndex].choices[currentChoiceIndex]);
        }

        void downArrow(){
            --currentChoiceIndex;

            if(currentChoiceIndex < 0){
                ++currentChoiceIndex;
             }
          
             int currentChoiceY = choicesSprite[currentChoiceIndex].y().to_double();
           arrow->set_position(globalChoiceX - 10,currentChoiceY);
             arrow->put_above();
              BN_LOG("CURRENT CHOICE: ",dialogueList[currentDialogueIndex].choices[currentChoiceIndex]);
        }

        void clearAllSprites(){
            currentMessageSprite.clear();
            choicesSprite.clear();
            dialoguebox.reset();
            currentSpeakerImage.reset();
            arrow.reset();
            dialogueIsRunning = false;
            spritesDisplayed = false;
        }

        void selectChoice(){
            
        }

        bool isDialogueRunning(){
            return this->dialogueIsRunning;
        }
        // void showMessage(bn::sprite_item speakerImage,
        //  bn::string message,
        // ){
            
        // }
     
       
};


#endif