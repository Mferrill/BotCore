/* SPGM Memeber of Trinitycore Server: http://immortalones.game-server.cc Repo: http://code.google.com/p/spgm-trinity/
*  This is a script I combined from Buff Npc and professions, I have redesigned it and added to it. To come: Enchanting, Guildhouse, TeleNpc2, Bots so this will be a all in one NPC with options in the config file and Database.
*/

/* ScriptData
SDName: AllInOne
SD%Complete: 100
SDComment: Buff Master Profession SkillUp Trainers
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "../../shared/Configuration/Config.h"
#ifndef _TRINITY_CORE_CONFIG
# define _TRINITY_CORE_CONFIG  "worldserver.conf"
#endif _TRINITY_CORE_CONFIG

bool GossipHello_AllInOne(Player* pPlayer, Creature* pCreature)
{
	Config TScriptConfig;
	if (!TScriptConfig.SetSource(_TRINITY_CORE_CONFIG,true))
		sLog.outError("TScript: Unable to open configuration file");

	if (TScriptConfig.GetBoolDefault("AllInOne.OnlyGMs", false)) // If AllInOne.OnlyGMs is enabled in trinitycore.conf
		if (pPlayer->GetSession()->GetSecurity() == SEC_PLAYER)
		{
			pCreature->MonsterWhisper("Sorry, I am set only for Game Masters.", pPlayer->GetGUID());
			return true;
		}

	bool EnableBuffs = TScriptConfig.GetBoolDefault("AllInOne.EnableBuffs", true);
	bool EnableProfessionsSkillUp = TScriptConfig.GetBoolDefault("AllInOne.EnableProfessionsSkillUp", true);
	bool EnableTrainers = TScriptConfig.GetBoolDefault("AllInOne.EnableTrainers", true);
	bool EnableBots = TScriptConfig.GetBoolDefault("AllInOne.EnableBots", true);
	bool EnablePlayerTools = TScriptConfig.GetBoolDefault("AllInOne.EnablePlayerTools", true);
	bool EnableRemoveResSickness = TScriptConfig.GetBoolDefault("AllInOne.EnableRemoveResSickness", true);
	bool EnableMorph = TScriptConfig.GetBoolDefault("AllInOne.EnableMorph", true);

	// Main Menu for Alliance
    if (pPlayer->GetTeam() == ALLIANCE)
	{
	// Buffs
	if (EnableBuffs)
		pPlayer->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		pPlayer->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Bots
	if (EnableBots)
		pPlayer->ADD_GOSSIP_ITEM(3, "Bots ->"  				  , GOSSIP_SENDER_MAIN, 600);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		pPlayer->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
		// Check config if "Morph" is enabled or not
	if (EnableMorph)
		pPlayer->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}
	else // Main Menu for Horde
	{
	// Buffs
	if (EnableBuffs)
		pPlayer->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		pPlayer->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		pPlayer->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
	// Check config if "Morph" is enabled or not
	if (EnableMorph)
		pPlayer->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}

	// Check config file if "Remove res sickness" option is enabled or not
	if (EnableRemoveResSickness)
		pPlayer->ADD_GOSSIP_ITEM(9, "Remove Resurrect Sickness" , GOSSIP_SENDER_MAIN, 7000);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

return true;
}

void SendDefaultMenu_AllInOne(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

// Not allow in combat
if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
	return;
}

Config TScriptConfig;
if (!TScriptConfig.SetSource(_TRINITY_CORE_CONFIG,true))
		sLog.outError("TScript: Unable to open configuration file");
	bool EnableBuffs = TScriptConfig.GetBoolDefault("AllInOne.EnableBuffs", true);
	bool EnableProfessionsSkillUp = TScriptConfig.GetBoolDefault("AllInOne.EnableProfessionsSkillUp", true);
	bool EnableSmallBuff = TScriptConfig.GetBoolDefault("AllInOne.EnableSmallBuff", true);
	bool EnableGreatBuff = TScriptConfig.GetBoolDefault("AllInOne.EnableGreatBuff", true);
	bool EnableGMBuff = TScriptConfig.GetBoolDefault("AllInOne.EnableGMBuff", true);
	bool EnableTrainers = TScriptConfig.GetBoolDefault("AllInOne.EnableTrainers", true);
	bool EnableClassTrainers = TScriptConfig.GetBoolDefault("AllInOne.EnableClassTrainers", true);
	bool EnableProfessionsTrainers = TScriptConfig.GetBoolDefault("AllInOne.EnableProfessionsTrainers", true);
	bool EnablePlayerTools = TScriptConfig.GetBoolDefault("AllInOne.EnablePlayerTools", true);
	bool EnableRemoveResSickness = TScriptConfig.GetBoolDefault("AllInOne.EnableRemoveResSickness", true);
	bool EnableGivemeGold = TScriptConfig.GetBoolDefault("AllInOne.EnableGivemeGold", true);
	bool EnableProfessions = TScriptConfig.GetBoolDefault("AllInOne.EnableProfessions", true);
	bool EnableSecondarySkills = TScriptConfig.GetBoolDefault("AllInOne.EnableSecondarySkills", true);
	bool EnableMorph = TScriptConfig.GetBoolDefault("AllInOne.EnableMorph", true);

//Mony Check
if (pPlayer->GetMoney() < (TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("You don't have enough money.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

case 100: // Buffs
	if (EnableSmallBuff)
		pPlayer->ADD_GOSSIP_ITEM(2, "Small Buff ->"   			               , GOSSIP_SENDER_MAIN, 1000);
	if (EnableGreatBuff)
		pPlayer->ADD_GOSSIP_ITEM(2, "Great Buff ->"                           , GOSSIP_SENDER_MAIN, 2000);
	if (EnableGMBuff)
		pPlayer->ADD_GOSSIP_ITEM(2, "GM Buff ->"    			               , GOSSIP_SENDER_MAIN, 3100);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 200: //Professions SkillUp
	// Check config if "EnableProfessions" is enabled or not
	if (EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM(3, "Professions ->"       				   , GOSSIP_SENDER_MAIN, 3000);
	// Check config if "EnableSecondarySkills" is enabled or not
	if (EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM(3, "Secondary Skills ->"  				   , GOSSIP_SENDER_MAIN, 6000);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 300: //Trainers
	// Check config if "Class Trainers" is enabled or not
	if (EnableClassTrainers)
		pPlayer->ADD_GOSSIP_ITEM(3, "Class Trainers ->"       			       , GOSSIP_SENDER_MAIN, 400);
	// Check config if "Profession Trainers" is enabled or not
	if (EnableProfessionsTrainers)
		pPlayer->ADD_GOSSIP_ITEM(3, "Professions Trainers ->" 				   , GOSSIP_SENDER_MAIN, 500);
	// Professions Skillup
	if (EnableProfessionsSkillUp)
		pPlayer->ADD_GOSSIP_ITEM(3, "Professions SkillUp ->"  				   , GOSSIP_SENDER_MAIN, 200);
		pPlayer->ADD_GOSSIP_ITEM(3, "Mount Trainer"       					   , GOSSIP_SENDER_MAIN, 634);
		pPlayer->ADD_GOSSIP_ITEM(3, "Stable Master"  				           , GOSSIP_SENDER_MAIN, 612);
		pPlayer->ADD_GOSSIP_ITEM(3, "Weapon Master"  				           , GOSSIP_SENDER_MAIN, 613);
		pPlayer->ADD_GOSSIP_ITEM(3, "Max Weapon Skill"    				       , GOSSIP_SENDER_MAIN, 4026);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 700: //Morph
		pPlayer->ADD_GOSSIP_ITEM(4, "Famous ->"   		    			       , GOSSIP_SENDER_MAIN, 710);
		pPlayer->ADD_GOSSIP_ITEM(4, "Funny ->" 							   , GOSSIP_SENDER_MAIN, 750);
		pPlayer->ADD_GOSSIP_ITEM(4, "Make me Bigger ->" 					   , GOSSIP_SENDER_MAIN, 771);
		pPlayer->ADD_GOSSIP_ITEM(4, "Make me Smaller ->" 					   , GOSSIP_SENDER_MAIN, 772);
		pPlayer->ADD_GOSSIP_ITEM(4, "DeMorph ->" 							   , GOSSIP_SENDER_MAIN, 799);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 710: //Morph Famous
		pPlayer->ADD_GOSSIP_ITEM(4, "Algalon ->"  		    			       , GOSSIP_SENDER_MAIN, 711);
		pPlayer->ADD_GOSSIP_ITEM(4, "Doomwalker ->" 						   , GOSSIP_SENDER_MAIN, 712);
		pPlayer->ADD_GOSSIP_ITEM(4, "Doomlord kazzak ->" 					   , GOSSIP_SENDER_MAIN, 713);
		pPlayer->ADD_GOSSIP_ITEM(4, "Onyxia ->" 							   , GOSSIP_SENDER_MAIN, 714);
		pPlayer->ADD_GOSSIP_ITEM(4, "Thaddius ->" 							   , GOSSIP_SENDER_MAIN, 715);
		pPlayer->ADD_GOSSIP_ITEM(4, "Gluth ->" 							   , GOSSIP_SENDER_MAIN, 716);
		pPlayer->ADD_GOSSIP_ITEM(4, "Maexxna ->" 							   , GOSSIP_SENDER_MAIN, 717);
		pPlayer->ADD_GOSSIP_ITEM(4, "Sapphiron->" 							   , GOSSIP_SENDER_MAIN, 718);
		pPlayer->ADD_GOSSIP_ITEM(4, "Noth The Plaguebringer->"				   , GOSSIP_SENDER_MAIN, 719);
		pPlayer->ADD_GOSSIP_ITEM(4, "Kel'Thuzad ->" 						   , GOSSIP_SENDER_MAIN, 720);
		pPlayer->ADD_GOSSIP_ITEM(4, "Kael'thas Sunstrider ->" 				   , GOSSIP_SENDER_MAIN, 721);
		pPlayer->ADD_GOSSIP_ITEM(4, "C'thun ->" 							   , GOSSIP_SENDER_MAIN, 722);
		pPlayer->ADD_GOSSIP_ITEM(4, "Thrall ->" 							   , GOSSIP_SENDER_MAIN, 723);
		pPlayer->ADD_GOSSIP_ITEM(4, "DeMorph ->" 							   , GOSSIP_SENDER_MAIN, 799);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 700);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 750: //Morph Funny
		pPlayer->ADD_GOSSIP_ITEM(4, "Gerky ->"   	 		   			       , GOSSIP_SENDER_MAIN, 751);
		pPlayer->ADD_GOSSIP_ITEM(4, "Fire elemental ->" 					   , GOSSIP_SENDER_MAIN, 752);
		pPlayer->ADD_GOSSIP_ITEM(4, "Pandaren Monk ->" 					   , GOSSIP_SENDER_MAIN, 753);
		pPlayer->ADD_GOSSIP_ITEM(4, "Lil' K.T ->" 					 		   , GOSSIP_SENDER_MAIN, 754);
		pPlayer->ADD_GOSSIP_ITEM(4, "The Weapon you're Wearing ->"	 	       , GOSSIP_SENDER_MAIN, 755);
		pPlayer->ADD_GOSSIP_ITEM(4, "Red Bubbles ->" 					       , GOSSIP_SENDER_MAIN, 756);
		pPlayer->ADD_GOSSIP_ITEM(4, "A Shark ->"    		   			       , GOSSIP_SENDER_MAIN, 757);
		pPlayer->ADD_GOSSIP_ITEM(4, "A Fel cannon ->"   	   			       , GOSSIP_SENDER_MAIN, 758);
		pPlayer->ADD_GOSSIP_ITEM(4, "Human Female  ->" 	   			       , GOSSIP_SENDER_MAIN, 759);
		pPlayer->ADD_GOSSIP_ITEM(4, "A Fat Human ->" 		   			       , GOSSIP_SENDER_MAIN, 760);
		pPlayer->ADD_GOSSIP_ITEM(4, "A Rabbit ->" 	 		   			       , GOSSIP_SENDER_MAIN, 761);
		pPlayer->ADD_GOSSIP_ITEM(4, "A Cool looking Bug ->"   			       , GOSSIP_SENDER_MAIN, 762);
		pPlayer->ADD_GOSSIP_ITEM(4, "Eye Of C'thun ->"   	   			       , GOSSIP_SENDER_MAIN, 763);
		pPlayer->ADD_GOSSIP_ITEM(4, "DeMorph ->" 							   , GOSSIP_SENDER_MAIN, 799);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 700);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 1000: //Small  Buff
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Mark of the Wild"                , GOSSIP_SENDER_MAIN, 1001);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Thorns"                          , GOSSIP_SENDER_MAIN, 1005);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Amplify Magic"                   , GOSSIP_SENDER_MAIN, 1010);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Arcane Intellect"                , GOSSIP_SENDER_MAIN, 1015);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Dalaran Intellect"               , GOSSIP_SENDER_MAIN, 1020);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Dampen Magic"                    , GOSSIP_SENDER_MAIN, 1025);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Blessing of Kings"               , GOSSIP_SENDER_MAIN, 1030);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Blessing of Might"               , GOSSIP_SENDER_MAIN, 1035);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Blessing of Wisdom"              , GOSSIP_SENDER_MAIN, 1040);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Divine Spirit"                   , GOSSIP_SENDER_MAIN, 1045);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Power Word: Fortitude"           , GOSSIP_SENDER_MAIN, 1050);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Shadow Protection"               , GOSSIP_SENDER_MAIN, 1055);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 100);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000: //Great Buff
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Gift of the Wild"                , GOSSIP_SENDER_MAIN, 2001);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Arcane Brilliance"               , GOSSIP_SENDER_MAIN, 2005);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Dalaran Brilliance"              , GOSSIP_SENDER_MAIN, 2010);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Kings"       , GOSSIP_SENDER_MAIN, 2015);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Might"       , GOSSIP_SENDER_MAIN, 2020);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Sanctuary"   , GOSSIP_SENDER_MAIN, 2025);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Wisdom"      , GOSSIP_SENDER_MAIN, 2030);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Prayer of Fortitude"             , GOSSIP_SENDER_MAIN, 2035);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Prayer of Shadow Protection"     , GOSSIP_SENDER_MAIN, 2040);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Prayer of Spirit"                , GOSSIP_SENDER_MAIN, 2045);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 100);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3100: //GM  Buff
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Agamaggan's Agility"             , GOSSIP_SENDER_MAIN, 3001);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Agamaggan's Strength"            , GOSSIP_SENDER_MAIN, 3005);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Armor Magic"                     , GOSSIP_SENDER_MAIN, 3010);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Armor Penetration"               , GOSSIP_SENDER_MAIN, 3015);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Increased Stamina"               , GOSSIP_SENDER_MAIN, 3020);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Razorhide"                       , GOSSIP_SENDER_MAIN, 3025);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Rising Spirit"                   , GOSSIP_SENDER_MAIN, 3030);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Spirit of the Wind"              , GOSSIP_SENDER_MAIN, 3035);
		pPlayer->ADD_GOSSIP_ITEM(2, "Buff me Wisdom of Agamaggan"             , GOSSIP_SENDER_MAIN, 3040);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 100);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 4000: //Player Tools
	// Check config if "Give me Gold" is enabled or not
	if (EnableGivemeGold)
		pPlayer->ADD_GOSSIP_ITEM(8, "Give me 5000 Gold"                       , GOSSIP_SENDER_MAIN, 4001);
		pPlayer->ADD_GOSSIP_ITEM(8, "Give me Soul Shard"                     , GOSSIP_SENDER_MAIN, 4005);
		pPlayer->ADD_GOSSIP_ITEM(8, "Heal me Please"                          , GOSSIP_SENDER_MAIN, 4010);
		pPlayer->ADD_GOSSIP_ITEM(8, "Heal me and party members Please"        , GOSSIP_SENDER_MAIN, 4015);
		pPlayer->ADD_GOSSIP_ITEM(8, "Conjure Level 80 Refreshment"            , GOSSIP_SENDER_MAIN, 4020);
		pPlayer->ADD_GOSSIP_ITEM(8, "Conjure Mana Gem Level 77"               , GOSSIP_SENDER_MAIN, 4025);
		pPlayer->ADD_GOSSIP_ITEM(8, "Drunk"   						           , GOSSIP_SENDER_MAIN, 4027);
		pPlayer->ADD_GOSSIP_ITEM(8, "Sober"   						           , GOSSIP_SENDER_MAIN, 4028);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 400: //Class Trainers
		pPlayer->ADD_GOSSIP_ITEM(3, "Death Knight"               			   , GOSSIP_SENDER_MAIN, 601);
		pPlayer->ADD_GOSSIP_ITEM(3, "Druid"                        		   , GOSSIP_SENDER_MAIN, 602);
		pPlayer->ADD_GOSSIP_ITEM(3, "Hunter"                 				   , GOSSIP_SENDER_MAIN, 603);
		pPlayer->ADD_GOSSIP_ITEM(3, "Mage"                 				   , GOSSIP_SENDER_MAIN, 604);
		pPlayer->ADD_GOSSIP_ITEM(3, "Paladin"              				   , GOSSIP_SENDER_MAIN, 605);
		pPlayer->ADD_GOSSIP_ITEM(3, "Priest"    	         			       , GOSSIP_SENDER_MAIN, 606);
		pPlayer->ADD_GOSSIP_ITEM(3, "Rogue"               	 				   , GOSSIP_SENDER_MAIN, 607);
		pPlayer->ADD_GOSSIP_ITEM(3, "Shaman"            					   , GOSSIP_SENDER_MAIN, 608);
		pPlayer->ADD_GOSSIP_ITEM(3, "Warlock"         						   , GOSSIP_SENDER_MAIN, 609);
		pPlayer->ADD_GOSSIP_ITEM(3, "Warrior"          				       , GOSSIP_SENDER_MAIN, 610);
		pPlayer->ADD_GOSSIP_ITEM(3, "Hunter Pet"    					       , GOSSIP_SENDER_MAIN, 611);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 300);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 500: //Profession Trainers
		pPlayer->ADD_GOSSIP_ITEM(3, "Alchemy"      				           , GOSSIP_SENDER_MAIN, 620);
		pPlayer->ADD_GOSSIP_ITEM(3, "Blacksmithing"                           , GOSSIP_SENDER_MAIN, 621);
		pPlayer->ADD_GOSSIP_ITEM(3, "Enchanter"         			           , GOSSIP_SENDER_MAIN, 622);
		pPlayer->ADD_GOSSIP_ITEM(3, "Engineering"           				   , GOSSIP_SENDER_MAIN, 623);
		pPlayer->ADD_GOSSIP_ITEM(3, "Herbalism"             				   , GOSSIP_SENDER_MAIN, 624);
		pPlayer->ADD_GOSSIP_ITEM(3, "Inscription"             			       , GOSSIP_SENDER_MAIN, 625);
		pPlayer->ADD_GOSSIP_ITEM(3, "Jewelcrafting"            			   , GOSSIP_SENDER_MAIN, 626);
		pPlayer->ADD_GOSSIP_ITEM(3, "Leatherworking"            			   , GOSSIP_SENDER_MAIN, 627);
		pPlayer->ADD_GOSSIP_ITEM(3, "Mining"             					   , GOSSIP_SENDER_MAIN, 628);
		pPlayer->ADD_GOSSIP_ITEM(3, "Skinning"                 			   , GOSSIP_SENDER_MAIN, 629);
		pPlayer->ADD_GOSSIP_ITEM(3, "Tailoring"           					   , GOSSIP_SENDER_MAIN, 630);
		pPlayer->ADD_GOSSIP_ITEM(3, "Cooking"               				   , GOSSIP_SENDER_MAIN, 631);
		pPlayer->ADD_GOSSIP_ITEM(3, "First Aid"             				   , GOSSIP_SENDER_MAIN, 632);
		pPlayer->ADD_GOSSIP_ITEM(3, "Fishing"               				   , GOSSIP_SENDER_MAIN, 633);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 300);
		pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000: //Profession
            pPlayer->ADD_GOSSIP_ITEM(3, "Alchemy."              , GOSSIP_SENDER_MAIN, 1101);
            pPlayer->ADD_GOSSIP_ITEM(3, "Blacksmithing."        , GOSSIP_SENDER_MAIN, 1102);
            pPlayer->ADD_GOSSIP_ITEM(3, "Enchanting."           , GOSSIP_SENDER_MAIN, 1103);
            pPlayer->ADD_GOSSIP_ITEM(3, "Engineering."          , GOSSIP_SENDER_MAIN, 1104);
            pPlayer->ADD_GOSSIP_ITEM(3, "Herbalism."            , GOSSIP_SENDER_MAIN, 1105);
            pPlayer->ADD_GOSSIP_ITEM(3, "Inscription."          , GOSSIP_SENDER_MAIN, 1106);
            pPlayer->ADD_GOSSIP_ITEM(3, "Jewelcrafting."        , GOSSIP_SENDER_MAIN, 1107);
            pPlayer->ADD_GOSSIP_ITEM(3, "Leatherworking."       , GOSSIP_SENDER_MAIN, 1108);
            pPlayer->ADD_GOSSIP_ITEM(3, "Mining."               , GOSSIP_SENDER_MAIN, 1109);
            pPlayer->ADD_GOSSIP_ITEM(3, "Skinning."             , GOSSIP_SENDER_MAIN, 1110);
            pPlayer->ADD_GOSSIP_ITEM(3, "Tailoring."            , GOSSIP_SENDER_MAIN, 1111);
			pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"               , GOSSIP_SENDER_MAIN, 200);
            pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 6000: //Secondary Skills
            pPlayer->ADD_GOSSIP_ITEM(3, "Cooking."              , GOSSIP_SENDER_MAIN, 2101);
            pPlayer->ADD_GOSSIP_ITEM(3, "First Aid."            , GOSSIP_SENDER_MAIN, 2102);
            pPlayer->ADD_GOSSIP_ITEM(3, "Fishing."              , GOSSIP_SENDER_MAIN, 2103);
          //pPlayer->ADD_GOSSIP_ITEM(3, "Riding."               , GOSSIP_SENDER_MAIN, 2104);
			pPlayer->ADD_GOSSIP_ITEM(7, "<- Back"               , GOSSIP_SENDER_MAIN, 200);
            pPlayer->ADD_GOSSIP_ITEM(7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 5005: //Back To Main Menu
	// Main Menu for Alliance
    if (pPlayer->GetTeam() == ALLIANCE)
	{
	// Buffs
	if (EnableBuffs)
		pPlayer->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		pPlayer->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		pPlayer->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
			// Check config if "Morph" is enabled or not
	if (EnableMorph)
		pPlayer->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}
	else // Main Menu for Horde
	{
	// Buffs
	if (EnableBuffs)
		pPlayer->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		pPlayer->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		pPlayer->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
			// Check config if "Morph" is enabled or not
	if (EnableMorph)
		pPlayer->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}

	// Check config file if "Remove res sickness" option is enabled or not
	if (EnableRemoveResSickness)
		pPlayer->ADD_GOSSIP_ITEM(9, "Remove Resurrect Sickness" , GOSSIP_SENDER_MAIN, 7000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

//////////////////////////////////////////////////Small Buff///////////////////////////////////////////////////////////////

case 1001: // Buff me Mark of the Wild
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48469,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1005: // Buff me Thorns
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,53307,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1010: // Buff me Amplify Magic
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,43017,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1015: // Buff me Arcane Intellect
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,42995,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1020: // Buff me Dalaran Intellect
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,61024,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1025: // Buff me Dampen Magic
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,43015,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1030: // Buff me Blessing of Kings
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,20217,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1035: // Buff me Blessing of Might
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48932,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1040: // Buff me Blessing of Wisdom
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48936,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1045: // Buff me Divine Spirit
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48073,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1050: // Buff me Power Word: Fortitude
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48161,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 1055: // Buff me Shadow Protection
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48169,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

//////////////////////////////////////////////////Great Buff///////////////////////////////////////////////////////////////

case 2001: // Buff me Gift of the Wild
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48470,true);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2005: // Buff me Arcane Brilliance
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,43002,true);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2010: // Buff me Dalaran Brilliance
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,61316,true);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2015: // Buff me Greater Blessing of Kings
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,25898,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2020: // Buff me Greater Blessing of Might
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48934,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2025: // Buff me Greater Blessing of Sanctuary
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,25899,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2030: // Buff me Greater Blessing of Wisdom
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48938,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2035: // Buff me Prayer of Fortitude
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48162,true);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2040: // Buff me Prayer of Shadow Protection
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48170,true);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 2045: // Buff me Prayer of Spirit
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48074,true);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

//////////////////////////////////////////////////GM Buff///////////////////////////////////////////////////////////////

case 3001: // Buff me Agamaggan's Agility
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,17013,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3005: // Buff me Agamaggan's Strength
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,16612,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3010: // Buff me Armor Magic
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,58453,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3015: // Buff me Armor Penetration
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,34106,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3020: // Buff me Increased Stamina
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,25661,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3025: // Buff me Razorhide
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,16610,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3030: // Buff me Rising Spirit
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,10767,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3035: // Buff me Spirit of the Wind
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,16618,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 3040: // Buff me Wisdom of Agamaggan
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,7764,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

//////////////////////////////////////////////////Trainers ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////Class ///////////////////////////////////////////////////////////////
case 601: // Death Knight
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28472,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 602: // Druid
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(12042,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 603: // Hunter
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(303911,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 604: // Mage
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28958,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 605: // Paladin
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(352811,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 606: // Priest
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(166581,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 607: // Rogue
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(6707,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 608: // Shaman
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(172041,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 609: // Warlock
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(23534,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 610: // Warrior
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(45931,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

//////////////////////////////////////////////////Profession and Extras ///////////////////////////////////////////////////////////////

case 620: // Alchemy
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(33630,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
	pPlayer->SummonCreature(28703,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 621: // Blacksmithing
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28694,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 622: // Enchanting
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28693,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
	pPlayer->SummonCreature(33633,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 623: // Engineering
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28697,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 624: // Herbalism
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28704,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 625: // Inscription
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28702,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 626: // Jewelcrafting
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28701,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 627: // Leatherworking
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28700,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 628: // Mining
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28698,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 629: // Skinning
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28696,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 630: // Tailoring
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28699,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 631: // Cooking
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28705,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 632: // First Aid
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28706,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);;
break;

case 633: // Fishing
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28742,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 634: // Mount
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(31238,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 613: // Weapon Master
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(12704,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 611: // Hunters Pet
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(100901,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 612: // Stable Master
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SummonCreature(28690,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

//////////////////////////////////////////////////Morph ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////Famous ///////////////////////////////////////////////////////////////

case 711: // Algalon
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(28641);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2);
break;

case 712: // Doomwalker
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(16630);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1);
break;

case 713: // Doomlord kazzak
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(17887);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.10);
break;

case 714: // Onyxia
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(8570);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1);
break;

case 715: // Thaddius
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(16137);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2);
break;

case 716: // Gluth
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(16064);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2);
break;

case 717: // Maexxna
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15928);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.15);
break;

case 718: // Sapphiron
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(16033);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1);
break;

case 719: // Noth The Plaguebringer
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(16590);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7);
break;

case 720: // Kel'Thuzad
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15945);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2);
break;

case 721: // Kael'thas Sunstrider
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(20023);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
break;

case 722: // C'thun
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15786);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1);
break;

case 723: // Thrall
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(4527);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0);
break;

//////////////////////////////////////////////////Funny ///////////////////////////////////////////////////////////////

case 751: // Gerky
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(29348);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5);
break;

case 752: // Water elemental
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(12129);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.3);
break;

case 753: // Pandaren Monk
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(30414);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5);
break;

case 754: // Lil' K.T
	pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->DeMorph();
    pPlayer->SetDisplayId(30507);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5);
break;

case 755: // The Weapon you're Wearing
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15880);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4);
break;

case 756: // Red Bubbles
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(20262);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
break;

case 757: // A Shark
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15555);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2);
break;

case 758: // A Fel cannon
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(18505);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
break;

case 759: // Human Female
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(100);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5);
break;

case 760: // A Fat Human
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(103);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5);
break;

case 761: // A Rabbit
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(328);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 3.0);
break;

case 762: // A Cool looking Bug
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15695);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1);
break;

case 763: // Eye Of C'thun
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetDisplayId(15556);
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1);
break;

case 771: // Make me Bigger
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5);
break;

case 772: // Make me Smaller
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
break;

case 799: // DeMorph
    pPlayer->CLOSE_GOSSIP_MENU();
	pPlayer->DeMorph();
	pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0);
break;

//////////////////////////////////////////////////Player Tools///////////////////////////////////////////////////////////////

case 4001://Give me Gold
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,46642,false); // 5000 gold
break;

case 4005://Give me Soul Shard
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,24827,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 4010: // Heal me please
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,38588,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 4015: // Heal me and party members Please
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,53251,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 4020: // Conjure Refreshment
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,42956,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
break;

case 4025: // Conjure Mana Gem
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,42985,false);
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));

case 4026: // Update Skill to Max for Level
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->UpdateSkillsToMaxSkillsForLevel();
    pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneBuffGoldCost",0)));
    pCreature->MonsterSay("Your Skills have now been Maxed for your Level.", LANG_UNIVERSAL, NULL);
    
break;

case 4027: // Drunk
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,46876,false);
    pCreature->MonsterSay("Hahahaha! You are Drunk Now!", LANG_UNIVERSAL, NULL);
    
break;

case 4028: // Sober
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->SetDrunkValue(0, 9);
    pCreature->MonsterSay("Hahahaha! I knew you could not handle you drink!", LANG_UNIVERSAL, NULL);
    
break;

case 7000://Remove Res Sickness
	if (!pPlayer->HasAura(15007,0))
	{
		pCreature->MonsterWhisper("You don't have resurrection sickness.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pCreature->CastSpell(pPlayer,38588,false); // Healing effect
	pPlayer->RemoveAurasDueToSpell(15007,0);
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Professions///////////////////////////////////////////////////////////////

case 1101: // Alchemy
	if (!pPlayer->UpdateSkill(171,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(171,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1102: // Blacksmithing
	if (!pPlayer->UpdateSkill(164,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(164,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1103: // Enchanting
	if (!pPlayer->UpdateSkill(333,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(333,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1104: // Engineering
	if (!pPlayer->UpdateSkill(202,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(202,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1105: // Herbalism
	if (!pPlayer->UpdateSkill(182,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(182,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1106: // Inscription
	if (!pPlayer->UpdateSkill(773,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(773,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1107: // Jewelcrafting
	if (!pPlayer->UpdateSkill(755,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(755,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1108: // Leatherworking
	if (!pPlayer->UpdateSkill(165,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(165,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1109: // Mining
	if (!pPlayer->UpdateSkill(186,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(186,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1110: // Skinning
	if (!pPlayer->UpdateSkill(393,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(393,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1111: // Tailoring
	if (!pPlayer->UpdateSkill(197,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(197,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Secondary Skills///////////////////////////////////////////////////////////////

case 2101: // Cooking
	if (!pPlayer->UpdateSkill(185,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(185,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2102: // First Aid
	if (!pPlayer->UpdateSkill(129,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(129,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2103: // Fishing
	if (!pPlayer->UpdateSkill(356,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_AllInOne(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(356,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//case 2104: // Riding
//	if (!pPlayer->UpdateSkill(,0))
//    {
//		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
//		GossipHello_ProfessionNPC(pPlayer, pCreature);
//		return;
//	}
//
//	pPlayer->UpdateSkill(,(TScriptConfig.GetFloatDefault("AllInOneSkillPoints",0)));
//	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("AllInOneSkillGoldCost",0)));
//	pPlayer->CLOSE_GOSSIP_MENU();
//break;
 pPlayer->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function



bool GossipSelect_AllInOne(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_AllInOne(pPlayer, pCreature, uiAction);

return true;
}

void AddSC_AllInOne()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "AllInOne";
    newscript->pGossipHello = &GossipHello_AllInOne;
    newscript->pGossipSelect = &GossipSelect_AllInOne;
    newscript->RegisterSelf();
}
