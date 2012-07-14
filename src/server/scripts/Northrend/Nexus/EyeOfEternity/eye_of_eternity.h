#ifndef DEF_EYE_OF_ETERNITY_H
#define DEF_EYE_OF_ETERNITY_H

 
enum
{
    MAX_ENCOUNTER                = 1,
 
    TYPE_MALYGOS                 = 2,
    TYPE_OUTRO_CHECK             = 4,  // Because this encounter is really hard and outro takes about 90s and malygos dies at its end, I add some check against server crash
    TYPE_DESTROY_PLATFORM        = 5,
    TYPE_VORTEX                  = 6,
    TYPE_PLAYER_HOVER            = 7,
    DATA_DROP_PLAYERS            = 8,
    TYPE_SET_PLAYER_TO_CHECK     = 9,
    TYPE_CHECK_PLAYER_FLYING     = 10,
    
    NPC_WYRMREST_SKYTALON        = 30161, // Dragon Vehicle in Third Phase
    NPC_MALYGOS                  = 28859,
};
#endif
