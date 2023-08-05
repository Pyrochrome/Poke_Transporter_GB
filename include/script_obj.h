#ifndef SCRIPT_OBJ_H
#define SCRIPT_OBJ_H

#include <string>
#include "pokemon_party.h"

#define CHECK_GAME 1
#define START_LINK 2
#define ERROR_TIMEOUT_ONE 3
#define ERROR_DISCONNECT 4
#define ERROR_COM_ENDED 5
#define ERROR_TIMEOUT_TWO 6
#define ERROR_COLOSSEUM 7
#define IMPORT_POKEMON 8

class script_obj
{
public:
    script_obj();
    script_obj(std::string nText, int nNext_index);
    script_obj(std::string nText, int nNext_index, int nCond_index);
    script_obj(std::string nText, int nNext_index, int nCond_index, int nFalse_index);

    std::string get_text();
    int get_true_index();
    int get_false_index();
    int get_cond_id();

private:
    std::string text;
    int next_index;
    int conditional_index;
    int next_false_index;
};

#endif