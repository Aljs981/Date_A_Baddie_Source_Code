#ifndef YOURDATE_H
#define YOURDATE_H

#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "bn_sprite_item.h"
#include "bn_sprite_items_chinitasmiling.h"
#include "bn_sprite_items_chinitacreepy.h"
#include "bn_sprite_items_chinitacrying.h"
#include "bn_sprite_items_chinitalaughing.h"
#include "bn_sprite_items_chinitablush.h"
#include "bn_sprite_items_chinitashocked.h"
#include "bn_sprite_items_chinitaangry.h"
#include "bn_sprite_items_mestizaangry.h"
#include "bn_sprite_items_mestizablush.h"
#include "bn_sprite_items_mestizacreepy.h"
#include "bn_sprite_items_mestizacrying.h"
#include "bn_sprite_items_mestizalaughing.h"
#include "bn_sprite_items_mestizashocked.h"
#include "bn_sprite_items_mestizasmiling.h"
#include "bn_sprite_items_morenaangry.h"
#include "bn_sprite_items_morenablush.h"
#include "bn_sprite_items_morenacreepy.h"
#include "bn_sprite_items_morenacrying.h"
#include "bn_sprite_items_morenalaughing.h"
#include "bn_sprite_items_morenashocked.h"
#include "bn_sprite_items_morenasmiling.h"
#include "bn_string.h"
#include "bn_log.h"
#include "player.h"
#include "dialogue.h"
#include "bn_sprite_text_generator.h"



class YourDate {
protected:
    // bn::vector<Dialogue, 32> dialogues;
    int lovepoints;
    bn::optional<bn::sprite_ptr> currentSprite;

    // FIX: "most vexing parse" — Type name(args); inside a class body
    // is parsed as a member FUNCTION declaration, not a constructed field.
    // Just declare it here; construct it in the member-init list below,
    // since bn::sprite_text_generator has no default constructor.
    bn::sprite_text_generator currentTextGen;

    bn::vector<bn::sprite_ptr, 32> currentDialogueSprite;
    int currentDialogueIndex = 0;   // FIX: start at 0, drop the dead -1 sentinel branch
    bn::string<10> mood;
    int dialogueCount;
    Player player;
    Dialogues dialogues;

public:

    // FIX: default constructor was missing entirely — needed so Scene's
    // `YourDate yourdate;` member (bare, no args) still compiles.
    YourDate() :
        currentTextGen(common::variable_8x16_sprite_font)
    {
        lovepoints = 20;
        mood = "happy";
    }

    YourDate(Player p) :
        currentTextGen(common::variable_8x16_sprite_font)
    {
        lovepoints = 20;
        mood = "happy";
        this->player = p;
       
    }

    // FIX: destructor had setup code in it (lovepoints/mood/dialogueCount
    // reset) — that belongs in a constructor, not a destructor. Destructor
    // should just clean up; nothing manual to clean up here yet.
    virtual ~YourDate() {}

    virtual int getLovePoints() {
        return this->lovepoints;
    }

    virtual void addLovePoints(int amount) {
        if(getLovePoints() > 100) {
            lovepoints = 100;
            return;
        }
        lovepoints += amount;
        BN_LOG("Love Points Added: ", amount);
    }

    virtual void decreaseLovePoints(int amount) {
        if(getLovePoints() <= 0) {
            BN_LOG("GAME OVER");
            return;
        }
        lovepoints -= amount;
    }
  
    virtual void setMood(bn::string<10> chosenMood){
       this->mood = chosenMood;
    }

    virtual Dialogues& getDialogues(){
        return this->dialogues;
    }

    // virtual void addDialogue(bn::sprite_item spriteReaction,Message message) {
    //     ++dialogueCount;
    //     dialogues.push_back({
    //         dialogueCount,
    //         spriteReaction,
    //         message
    //     });
    // }

    // virtual void removeDialogue(int id) {
    //     for(int i = 0; i < dialogues.size(); i++) {
    //         const Dialogue& currentDialogue = dialogues[i];   // FIX: was copying by value every iteration
    //         if(currentDialogue.id == id) {
    //             dialogues.erase(dialogues.begin() + i);
    //             return;
    //         }
    //     }
    // }

    // virtual void viewAllDialogues() {
    //     for(const Dialogue& currentDialogue : dialogues) {   // FIX: reference instead of copy
    //         // logging left commented out as in your original
    //     }
    // }

    // FIX: added bounds checking so this can never read past the end of
    // `dialogues` — that was an out-of-bounds read waiting to happen once
    // you call this more times than dialogues exist.
    // FIX: return by const reference instead of by value — avoids copying
    // the sprite_item + string(32) payload on every single call.
    // virtual const Dialogue& nextDialogue(){
    //     if(currentDialogueIndex >= dialogues.size()){
    //         currentDialogueIndex = 0;   // clamp to last valid line
    //     }

    //     const Dialogue& current = dialogues[currentDialogueIndex];
    //     ++currentDialogueIndex;
    //     return current;
    // }

    // FIX: return by const reference — was copying the whole vector
    // (all 32 Dialogue slots' worth of strings/sprite_items) on every call.
    // virtual const bn::vector<Dialogue, 32>& getAllDialogues() {
    //     return this->dialogues;
    // }

    virtual bn::string<10> getMood() {
        return this->mood;
    }

    // FIX: create_sprite(x, y) only — it was being called with 4 args,
    // including passing spritetoDraw as an argument to its own method,
    // and passing currentDialogueSprite (a vector, meant for TEXT sprites,
    // not portrait sprites) where it didn't belong.
    // virtual void drawSprite(bn::sprite_item spritetoDraw){
    //     currentSprite.reset();
    //     currentSprite = spritetoDraw.create_sprite(0, 0);

    //     currentSprite->set_scale(2);
    // }

    // FIX: was an empty stub. This is where currentDialogueSprite (the
    // vector meant for generated text sprites) actually gets used, via
    // currentTextGen — same pattern as textgen2.generate_top_left(...)
    // in main.cpp.
    // virtual void drawDialogue(bn::string<32> dialoguetoDraw){
    //     currentDialogueSprite.clear();
    //     currentTextGen.generate(0, 0, dialoguetoDraw, currentDialogueSprite);
    // }

};


class Chinita : public YourDate {
public:
    Chinita(Player p) : YourDate(p) {  mood = "happy";}
};


class Mestiza : public YourDate {
public:
    Mestiza(Player p) : YourDate(p) { mood = "happy";}
};


class Morena : public YourDate {
public:
    Morena(Player p) : YourDate(p) { mood = "happy";}
};


#endif