#ifndef UTILITY_H
#define UTILITY_H
#include "bn_string.h"
#include "bn_log.h"
#include "bn_sprite_item.h"
#include "bn_array.h"
#include "common_fixed_8x8_sprite_font.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_items_cursor.h"
#include "bn_optional.h"
#include "bn_sprite_items_dialoguebox.h"

 class GameTimer{
    public:
        bn::string<10> state;
        int currentSecond;
        int currentMinute;
        int currentHour;
      
      GameTimer(){
         this->currentSecond = 0;
         this->currentMinute = 0;
         this->currentHour = 0;
         state = "STOPPED";
      }

      GameTimer(int hour, int minute, int second){
        this->currentHour = hour;
        this->currentMinute = minute;
        this->currentSecond = second;
        this->state = "INIT";
      }

      void stop(){
        this->currentSecond = 0;
         this->currentMinute = 0;
         this->currentHour = 0;
         this->state = "STOPPED";
      }

      void set(int hour, int minute, int second) {

            if(hour < 0 || hour >= 24) {
                BN_LOG("INVALID HOUR");
                return;
            }

            if(minute < 0 || minute >= 60) {
                BN_LOG("INVALID MINUTE");
                return;
            }

            if(second < 0 || second >= 60) {
                BN_LOG("INVALID SECOND");
                return;
            }

            this->currentHour = hour;
            this->currentMinute = minute;
            this->currentSecond = second;
            this->state = "INIT";
       }

     void tick(){
    if(this->state == "INIT"){
        this->state = "COUNTING";
    } else if(this->state == "COUNTING"){

        if(this->currentHour <= 0 && 
           this->currentMinute <= 0 && 
           this->currentSecond <= 0){
            stop();
            return;
        }
          
        --this->currentSecond; 
          
        if(this->currentSecond < 0){
            this->currentSecond = 59;
            --this->currentMinute;
            
            if(this->currentMinute < 0){
                this->currentMinute = 59;
                --this->currentHour;
            }
        }
    } else{
        BN_LOG("CANNOT TICK MUST SET TIMER FIRST");
    }
}

 };

 class ErrorMessageMaker{
    private:
        bn::sprite_text_generator errortextgen{common::fixed_8x8_sprite_font};
        bn::vector<bn::sprite_ptr,64> errortextsprites;
        bn::optional<bn::sprite_ptr> dialogueBoxSprite;
        bn::sprite_item dialogueBoxItem;

        static constexpr int CHAR_WIDTH = 8;   // fixed_8x8 font
        static constexpr int CHAR_HEIGHT = 8;
        static constexpr int MAX_CHARS_PER_LINE = 26; // ~208px, adjust base sa screen mo
        static constexpr int PADDING_X = 8;
        static constexpr int PADDING_Y = 6;

    public:
        // NEW: sprite_item kailangan i-init sa member init list
        // (most vexing parse kung ilalagay sa body bilang plain declaration)
        ErrorMessageMaker() : dialogueBoxItem(bn::sprite_items::dialoguebox){

        }

        void createErrorMessage(bn::string<64> message){
            clearErrorMessage();

            // NEW: hatiin ang message sa mga lines kung sobra sa MAX_CHARS_PER_LINE
            bn::vector<bn::string<32>, 8> lines;
            bn::string<32> currentLine = "";

            for(int i = 0; i < message.size(); ++i){

                currentLine.push_back(message[i]);

                bool atLimit = currentLine.size() >= MAX_CHARS_PER_LINE;
                bool atEnd = (i == message.size() - 1);

                if(atLimit || atEnd){
                    lines.push_back(currentLine);
                    currentLine.clear();
                }
            }

            // NEW: hanapin yung pinakamahabang linya para malaman yung
            // kailangang width ng box (hindi yung buong message.size())
            int longestLineChars = 0;
            for(const bn::string<32>& line : lines){
                if(line.size() > longestLineChars){
                    longestLineChars = line.size();
                }
            }

            int textBlockWidth = longestLineChars * CHAR_WIDTH;
            int textBlockHeight = lines.size() * CHAR_HEIGHT;

            int startX = -(textBlockWidth / 2);
            int startY = -(textBlockHeight / 2);

            // box muna bago text, para nasa likod ang box sa creation order
            dialogueBoxSprite = dialogueBoxItem.create_sprite(0, 0);

            bn::size baseDimensions = dialogueBoxSprite->dimensions();

            int desiredWidth = textBlockWidth + (PADDING_X * 2);
            int desiredHeight = textBlockHeight + (PADDING_Y * 2);

            bn::fixed scaleX = bn::fixed(desiredWidth) / baseDimensions.width();
            bn::fixed scaleY = bn::fixed(desiredHeight) / baseDimensions.height();

            dialogueBoxSprite->set_scale(scaleX, scaleY);
            dialogueBoxSprite->set_position(0, 0);

            // ngayon text, per-line, nakasentro
            for(int i = 0; i < lines.size(); ++i){
                int lineY = startY + (i * CHAR_HEIGHT);
                errortextgen.generate(startX, lineY, lines[i], errortextsprites);
            }
        }

        void clearErrorMessage(){
            errortextsprites.clear();
            dialogueBoxSprite.reset();
        }

        
};


class KeyboardMaker{
    public:
     bn::string<9> keys[29] = {
        "A", "B", "C", "D", "E", "F", "G",
        "H", "I", "J", "K", "L", "M", "N",
        "O", "P", "Q", "R", "S", "T", "U",
        "V", "W", "X", "Y", "Z", "OK","DELETE",
        "ERASE"
    };
     bn::sprite_text_generator textgen{common::fixed_8x8_sprite_font};
     int currentKeyIndex = 0;
     bn::vector<bn::sprite_ptr,64> textSprites;
     bn::optional<bn::sprite_ptr> cursor;
     bn::sprite_text_generator textgen2{common::fixed_8x8_sprite_font};
     bn::vector<bn::sprite_ptr,64> textSprites2;
     bn::vector<int,64> letterWidths;
     bool keyboardExists;
     int typedTextX = -101;
     int typedTextY = 43;

     // NEW: position ng bawat KEY (hindi sprite), indexed by key index 0-28
     int keyPositionX[29];
     int keyPositionY[29];
     int maxCharacters;
     ErrorMessageMaker errormaker;

      KeyboardMaker(){
        keyboardExists = false;
        maxCharacters = 8;
    }

      void goRight(){
         if(keyboardExists){
            ++currentKeyIndex;
            if(currentKeyIndex > (29 - 1)){
                currentKeyIndex = currentKeyIndex - 1;
            }
            bn::string<7> currentkeytext = bn::to_string<7>(keys[currentKeyIndex]);
            BN_LOG("KEY GOES TO RIGHT");
            BN_LOG("CURRENT KEY: " , currentkeytext);
           drawCursorBasedOnSelected();
         }
      }

      void goLeft(){
         if(keyboardExists){
             --currentKeyIndex;
            if(currentKeyIndex < 0){
                currentKeyIndex = 0;
            }

            bn::string<7> currentkeytext = bn::to_string<7>(keys[currentKeyIndex]);
            BN_LOG("KEY GOES TO LEFT");
            BN_LOG("CURRENT KEY: ", currentkeytext);
           drawCursorBasedOnSelected();
         }
      }

      void drawKeyboard(){
         cursor.reset();
         textSprites.clear();
          int startingX = -106;
          int currentX = startingX;
          int currentY = -65;
          int nearendscreenwidth = 120 - 10;
           for(int i = 0; i < 29; i++){
                 if(keys[i] == "OK"){
                    currentY += 10;
                    currentX = startingX;
                 } else if(keys[i] == "DELETE" || keys[i] == "ERASE"){
                    currentY += 10;
                    currentX = startingX;
                 }

                 if(currentX >= nearendscreenwidth){
                    currentY += 10;
                    currentX = startingX;
                 }

              // NEW: i-record muna yung totoong position ng key na ito
              // BAGO pa man sumingit yung susunod na letrang idadagdag sa textSprites
              keyPositionX[i] = currentX;
              keyPositionY[i] = currentY;

              textgen.generate(currentX,currentY,keys[i],textSprites);

                 if(currentKeyIndex == i){
                     // NEW: gamitin diretso yung currentX/currentY, hindi na
                     // textSprites[i] — kasi hindi tugma yung sprite index sa
                     // key index pagdating sa multi-letter keys (OK/DELETE/ERASE)
                      cursor = bn::sprite_items::cursor.create_sprite(currentX, currentY + 5);
                  }
              currentX += 20;
           }
         keyboardExists = true;
      }

      void drawCursorBasedOnSelected(){
        if(keyboardExists){
            // NEW: gamitin din yung recorded keyPositionX/Y sa halip na
            // textSprites[currentKeyIndex] — parehong pinagmulan ng bug dati
            cursor->set_x(keyPositionX[currentKeyIndex] - 5);
            cursor->set_y(keyPositionY[currentKeyIndex] - 5);
            cursor->set_scale(1.2);
            cursor->put_above();
        }
        
      }
      
      void clearAll(){
        cursor.reset();
        textSprites.clear();
        textSprites2.clear();
        errormaker.clearErrorMessage();
        currentKeyIndex = 0;
      }

    //    bn::string<10> drawSelected(){
    //     errormaker.clearErrorMessage();
    //     bn::string<10> selectedChar = "";
    //     int nearEndScreenWidth = 120 - 10;

    //      if(keyboardExists  && textSprites2.size() <= maxCharacters){
    //         if(typedTextX >= nearEndScreenWidth){
    //             typedTextX = -101;
    //         }

    //          if(keys[currentKeyIndex] == "DELETE"){
    //              deleteLastLetter();
    //             return "";
    //          }

    //          if(keys[currentKeyIndex] == "ERASE"){
    //              clearTypedText();
    //             return "";
    //          }

    //         textgen2.generate(typedTextX, typedTextY, keys[currentKeyIndex], textSprites2);

    //         int keyWidth = (keys[currentKeyIndex].size() * 8) + 4;

    //         letterWidths.push_back(keyWidth);
    //         typedTextX += keyWidth;

    //         selectedChar = keys[currentKeyIndex];
    //      } else{
    //         errormaker.createErrorMessage("keyboard not exist or exceed keys!");
    //      }
    //       return selectedChar;
    //   }

    bn::string<10> drawSelected(){
        // 1. Always clear the old error message first
        errormaker.clearErrorMessage();
        bn::string<10> selectedChar = "";
        int nearEndScreenWidth = 120 - 10;

        // 2. Check if keyboard exists
        if(!keyboardExists) {
            errormaker.createErrorMessage("keyboard not exist!");
            return selectedChar;
        }

        // 3. ALWAYS allow the user to delete or erase, regardless of character limit
        if(keys[currentKeyIndex] == "DELETE"){
            deleteLastLetter();
            return "";
        }

        if(keys[currentKeyIndex] == "ERASE"){
            clearTypedText();
            return "";
        }

        // 4. Check the limit ONLY when trying to type a new character
        if(textSprites2.size() <= maxCharacters){
            if(typedTextX >= nearEndScreenWidth){
                typedTextX = -101;
            }

            textgen2.generate(typedTextX, typedTextY, keys[currentKeyIndex], textSprites2);

            int keyWidth = (keys[currentKeyIndex].size() * 8) + 4;

            letterWidths.push_back(keyWidth);
            typedTextX += keyWidth;

            selectedChar = keys[currentKeyIndex];
        } else {
            // Now, this only triggers if they try to type a standard key while full
            errormaker.createErrorMessage("exceed keys!");
        }
        
        return selectedChar;
    }


      void deleteLastLetter(){
          if(!textSprites2.empty()){
            textSprites2.pop_back();

            int removedWidth = letterWidths.back();
            letterWidths.pop_back();

            typedTextX -= removedWidth;
          }
      }

      void clearTypedText(){
        textSprites2.clear();
        letterWidths.clear();
        typedTextX = -101;
      }

      bn::string<10> getCurrentKey(){
        return keys[currentKeyIndex];
      }

  };

  

//   class KeyboardMaker{
//     public:
//      bn::string<9> keys[29] = {
//         "A", "B", "C", "D", "E", "F", "G",
//         "H", "I", "J", "K", "L", "M", "N",
//         "O", "P", "Q", "R", "S", "T", "U",
//         "V", "W", "X", "Y", "Z", "OK","DELETE",
//         "ERASE"
//     };
//      bn::sprite_text_generator textgen{common::fixed_8x8_sprite_font};
//      int currentKeyIndex = 0;
//      bn::vector<bn::sprite_ptr,64> textSprites;
//      bn::optional<bn::sprite_ptr> cursor;
//      bn::sprite_text_generator textgen2{common::fixed_8x8_sprite_font};
//      bn::vector<bn::sprite_ptr,64> textSprites2;
//      bool keyboardExists;
//       int typedTextX = -101;         // FIX: sariling tracked X, top-left space, hindi na kinukuha mula sa .x()
//      int typedTextY = 43;  
//     //  int lastDrawnSelectedLetterIndex = -1;
      
//       KeyboardMaker(){
//         keyboardExists = false;
//     }

//       void goRight(){
//          if(keyboardExists){
//             ++currentKeyIndex;
//             if(currentKeyIndex > (29 - 1)){
//                 currentKeyIndex = currentKeyIndex - 1;
//             }
//             bn::string<7> currentkeytext = bn::to_string<7>(keys[currentKeyIndex]);
//             BN_LOG("KEY GOES TO RIGHT");
//             BN_LOG("CURRENT KEY: " , currentkeytext);
//            drawCursorBasedOnSelected();
//          }
          
//       }

//       void goLeft(){
//          if(keyboardExists){
//              --currentKeyIndex;
//             if(currentKeyIndex < 0){
//                 currentKeyIndex = 0;
//             }
         
//             bn::string<7> currentkeytext = bn::to_string<7>(keys[currentKeyIndex]);
//             BN_LOG("KEY GOES TO LEFT");
//             BN_LOG("CURRENT KEY: ", currentkeytext);
//            drawCursorBasedOnSelected();
//          }
        
//       }

      
//       void drawKeyboard(){
//          cursor.reset();
//          textSprites.clear();
//           int currentX = 10;
//           int currentY = 10;
//           int nearendscreenwidth = 240 - 10;
//            for(int i = 0; i < 29; i++){
//                  if(keys[i] == "OK"){
//                     currentX += 5;
//                  } else if(keys[i] == "DELETE" || keys[i] == "ERASE"){
//                     currentX += 25;
//                  } 

//                  if(currentX >= nearendscreenwidth){
//                     currentY += 10;
//                     currentX = 10;
//                  }
           
                 
                
//               textgen.generate_top_left(currentX,currentY,keys[i],textSprites);
//                  if(currentKeyIndex == i){
//                      int currentSpriteX = textSprites[i].x().integer();
//                      int currentSpriteY = textSprites[i].y().integer();
                     
//                       cursor = bn::sprite_items::cursor.create_sprite(currentSpriteX,currentSpriteY + 5);
                        
                    
//                   }
//               currentX += 20;

//            }
//          keyboardExists = true;
//       }

//       void drawCursorBasedOnSelected(){
//         if(keyboardExists){
//             cursor->set_x(textSprites[currentKeyIndex].x().integer() - 5);
//             cursor->set_y(textSprites[currentKeyIndex].y().integer() - 5);
//             cursor->set_scale(1.2);
//             cursor->put_above();
//         }
//       }
//        bn::string<10> drawSelected(){
//         bn::string<10> selectedChar = "";
//         int nearEndScreenWidth = 120 - 10;

//          if(keyboardExists){
//             if(typedTextX >= nearEndScreenWidth){
//                 typedTextX = -101; // wrap pabalik
//             }

//              if(keys[currentKeyIndex] == "DELETE"){
//                  deleteLastLetter();
//                 return "";
//              }

             

//             // FIX: generate_top_left LANG ang gamit, mula simula hanggang huli —
//             // walang paghahalo ng generate() vs generate_top_left(), walang
//             // umaasang readback mula .x() na iba't ibang coordinate space
//             textgen2.generate(typedTextX, typedTextY, keys[currentKeyIndex], textSprites2);

//             int keyWidth = (keys[currentKeyIndex].size() * 8) + 4;
//             typedTextX += keyWidth;

//             selectedChar = keys[currentKeyIndex];
//          } 
//           return selectedChar;
//       }
    

//       void deleteLastLetter(){
//           if(!textSprites2.empty()){
//             // typedTextX -= (lastSpriteWidth - 2);
//             textSprites2.pop_back(); 
//             int lastSpriteIndex = textSprites2.size() - 1;
//              int lastSpriteWidth = textSprites2[lastSpriteIndex].dimensions().width();
            
//             // typedTextX = textSprites2[lastSpriteIndex].x().integer() + (lastSpriteWidth + 4);
//              typedTextX = textSprites2[lastSpriteIndex].x().integer() + 1;
//           }
         
//       }
//       void resetTypedText(){
//         textSprites2.clear();
//         typedTextX = 10;
//       }


//   };



#endif