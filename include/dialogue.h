#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <tonc.h>

// Dialogue
#define DIA_OPEN 0
#define DIA_E4 1
#define DIA_MG_FRLGE 2
#define DIA_MG_RS 3
#define DIA_LETS_START 4
#define DIA_START 5
#define DIA_TRANS_GOOD 6
#define DIA_NEW_DEX 7
#define DIA_NO_NEW_DEX 8
#define DIA_SEND_FRIEND_KANTO 9
#define DIA_SEND_FRIEND_HOENN_RS 10
#define DIA_SEND_FRIEND_HOENN_E 11
#define DIA_THANK 12
#define DIA_GET_MON 13
#define DIA_MG_OTHER_EVENT 14
#define DIA_PKMN_TO_COLLECT 15
#define DIA_NO_VALID_PKMN 16
#define DIA_ASK_QUEST 17
#define DIA_WHAT_GAME_TRANS 18
#define DIA_WHAT_LANG_TRANS 19
#define DIA_NO_GB_ROM 20
#define DIA_IN_BOX 21
#define DIA_MYTHIC_CONVERT 22
#define DIA_CANCEL 23
#define DIA_SOME_INVALID_PKMN 24
#define DIA_MENU_BACK 25
#define DIA_IS_MISSINGNO 26
#define DIA_ERROR_COLOSSEUM 27
#define DIA_ERROR_COM_ENDED 28
#define DIA_ERROR_DISCONNECT 29
#define DIA_ERROR_TIME_ONE 30
#define DIA_ERROR_TIME_TWO 31
#define DIA_WHAT_LANG_EVENT 32
#define DIA_WHAT_GAME_EVENT 33
#define DIA_K_DEX_NOT_FULL 34
#define DIA_J_DEX_NOT_FULL 35

#define DIA_SIZE 36
#define DIA_END DIA_SIZE

extern const byte *dialogue[DIA_SIZE];

#endif
