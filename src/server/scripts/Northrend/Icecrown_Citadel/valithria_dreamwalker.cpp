/* 
Krowow Valithria dreamwalker script. 
*/ 
 
#include "ScriptPCH.h"
#include "icecrown_citadel.h"
 
//Yells 
#define STARTEVENT                 -1666062  
#define BELOW25PCTS                -1666065 
#define ABOVE75PCTS                -1666064 
#define DEATH                      -1666066 
#define PDEATH                     -1666067 
#define ENDEVENT                   -1666069 
#define OPENPORTAL                 -1666063
//Loot Chest 
#define LOOT_CHEST                 201959
//Valithria 
#define CORRUPTION                 70904 
#define DREAM_SLIP                 71196 
#define RAGE                       71189 
//Mage 
#define VOLLEY                     70759 
#define COLUMN                     70702 
#define MANA_VOID                  71085
#define CORRUPTING                 70602
//Skeleton 
#define WASTE                      69325 
#define FIREBALL                   70754 
//Supressor 
#define SUPRESSION                 70588 
//Zombie 
#define CORROSION                  70751 
#define BURST                      70744 
//Abomination 
#define SPRAY                      71283  
//Worm 
#define ROT                        72963 
//Portal 
#define DREAM_STATE                70766 
#define PORTAL_VISUAL              71304
//Cloud 
#define VIGOR                      70873 
#define CLOUD_VISUAL               70876 
//Valithria Adds
#define WORM                       37907 
#define PORTAL                     37945 
#define CLOUD                      37985   
#define VOID                       38068  
#define ABOMINATION                37886 
#define SKELETON                   36791 
#define ARCHMAGE                   37868 
#define SUPPRESSER                 37863 
#define ZOMBIE                     37934  
#define COMBAT_TRIGGER             38752 
 
//Spawns    
const Position Pos[4] =
{
    {4164.7 , 2547.67, 364.873322, 4.788825},     
    {4164.33, 2421.581, 364.872864, 1.812952},    
    {4251.139 , 2547.673, 364.868988, 4.444821},                        
    {4238.911, 2421.531, 364.868652, 1.315012},  
};
#define LOC_Z                      364
//Loot Spawn 
#define LOOT_X                     4203.665
#define LOOT_Y                     2484.679
 
//Global Variables//
Unit* pValithria;  
Unit* pPlayer; 
Unit* pBuff;  
Creature* combat_trigger= NULL;

///////////////////
 
//Valithria Script------------------------ 
struct boss_valithriaAI : public ScriptedAI
{
    boss_valithriaAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData(); 
    }
    
    ScriptedInstance* pInstance;
    

	
     uint32 Phase;
     uint32 Portal_Timer;  
	 uint32 End_Timer; 
     uint32 Summon_Timer;
     uint32 End2_Timer;  


     bool valithria;
	 bool end;  
	 bool ABOVEHP; 
	 bool BELOWHP;

	    void InitializeAI()
    {    
        Phase = 0;
        DoCast(CORRUPTION);   
		me->SetHealth(me->GetHealth() * 0.50);  
        
        Summon_Timer = 999999999;
		Portal_Timer = 999999999; 
	    valithria = false;
        end = false;
	    ABOVEHP = false; 
		BELOWHP = false;
 
		ScriptedAI::InitializeAI();
    } 

	    void Reset()
    { 

     
	   
      if (pInstance) 
	pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, NOT_STARTED);   

    }	  

	 void MoveInLineOfSight(Unit *who)
    {  
    if (!valithria && who->IsWithinDistInMap(me,10.0f))
        {   

		  
          if (pInstance) 
		  {
          pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, IN_PROGRESS);  
		  }   
		  DoScriptText(STARTEVENT, me); 
          combat_trigger = me->SummonCreature(COMBAT_TRIGGER,me->GetPositionX(), me->GetPositionY(),me->GetPositionZ(),0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,20000);  
		  me->AddThreat(combat_trigger, 10000000.0f); 
		  combat_trigger->AddThreat(me, 10000000.0f);
          me->AI()->AttackStart(combat_trigger);  
		  combat_trigger->AI()->AttackStart(me);
		  end = false;
		  valithria = true;  
		  ABOVEHP = false; 
		  BELOWHP = false;
	}
	 
	
		ScriptedAI::MoveInLineOfSight(who);
	 }

     void EnterCombat(Unit *who) 
	 { 
		  me->SetHealth(me->GetMaxHealth() * 0.50); 
		  
	      
          Summon_Timer = 15000;
		  Portal_Timer = 30000;  
		  Phase = 1;
	 }
			 
	 	void JustSummoned(Creature* pSummoned)
    {   
		if (!pSummoned->HasAura(PORTAL_VISUAL))
		{
           pSummoned->AI()->AttackStart(me); 
		}
    } 

	     void UpdateAI(const uint32 diff)
		 {
        if (!UpdateVictim())
            return;    
 
          if (Phase == 1) 
	 {
        DoStartNoMovement(me->getVictim()); 
		me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
		End_Timer = 999999999; 
		
    
	if (Summon_Timer <= diff) 
	{  
    DoSummon(ZOMBIE, Pos[RAND(0,1,2,3)]);  
	DoSummon(SKELETON, Pos[RAND(0,1,2,3)]);  
	DoSummon(ARCHMAGE, Pos[RAND(0,1,2,3)]);  
	DoSummon(SUPPRESSER, Pos[RAND(0,1,2,3)]);  
	DoSummon(ABOMINATION, Pos[RAND(0,1,2,3)]);   
	Summon_Timer = 30000;
	}else Summon_Timer -= diff;

	if  (Portal_Timer <= diff) 
	{  
		DoScriptText(OPENPORTAL,me);
        me->SummonCreature(PORTAL,me->GetPositionX()+15,me->GetPositionY()+15,me->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 15000);  
		me->SummonCreature(PORTAL,me->GetPositionX()+10,me->GetPositionY()+25,me->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 15000);  
		me->SummonCreature(PORTAL,me->GetPositionX()+15,me->GetPositionY()-25,me->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 15000);   
		Portal_Timer = 30000;
	}else Portal_Timer -= diff;

    if (!ABOVEHP && (me->GetHealth()*100 / me->GetMaxHealth()) > 75) 
	{ 
	DoScriptText(ABOVE75PCTS,me); 
	ABOVEHP = true; 
	} 
	 
	    if (!BELOWHP && (me->GetHealth()*100 / me->GetMaxHealth()) < 25) 
	{ 
	DoScriptText(BELOW25PCTS,me); 
	BELOWHP = true; 
	}

    	if ((me->GetHealth()*100 / me->GetMaxHealth()) > 99)
		{     
			Phase = 2; 
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);  
			me->SetReactState(REACT_PASSIVE);
			me->RemoveAurasDueToSpell(CORRUPTION);       
			

			end = true; 
			} 
		  }
	 
	 if (Phase == 2)
		{    
		Phase = 3; 
		End_Timer = 1000; 
		End2_Timer = 6000;
		DoScriptText(ENDEVENT , me); 
		 }

     if (Phase == 3) 
	 { 
	  if (End_Timer <= diff) 
	  { 
	  
	  combat_trigger->CastSpell(combat_trigger,RAGE,true,0,0,0); 
	  combat_trigger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	  End_Timer = 50000;   
	  
	  }else End_Timer -= diff; 
	    
	  if (End2_Timer <= diff) 
	  { 
	  combat_trigger->ForcedDespawn(); 
	  me->CastSpell(me,DREAM_SLIP,true,0,0,0); 
	  End2_Timer = 50000; 
	  }else End2_Timer -= diff;

	 }
	  

      if (me->HasAura(DREAM_SLIP))
	  {  
          me->ForcedDespawn();  
		  me->SummonGameObject(LOOT_CHEST,LOOT_X,LOOT_Y,LOC_Z,0,0,0,0,0,-10); 
          	if (pInstance) 
			{ 
            pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, DONE);  
			} 
	  }
	 
		 }
	
 	
 
	 void JustDied(Unit* killer)  
    {
			DoScriptText(DEATH, me);  
    }
      

}; 
//Portal 
struct npc_dreamportalAI : public ScriptedAI
{
	npc_dreamportalAI(Creature *c) : ScriptedAI(c){}   
	 
	uint32 STATE_Timer; 
	 
    void InitializeAI() 
	{ 
		DoCast(PORTAL_VISUAL);      
		me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);  

     	ScriptedAI::InitializeAI();
    } 

		 void MoveInLineOfSight(Unit *who)
		 { 
		 if (who->IsControlledByPlayer())
		{
	  if (me->IsWithinDistInMap(who,5.0f))
            {   
			pPlayer = who; 
             pPlayer->CastSpell(pPlayer,DREAM_STATE,false,0,0,0);
			 pPlayer->AddUnitMovementFlag(MOVEMENTFLAG_FLY_MODE);
             pPlayer->SendMovementFlagUpdate();  
			 STATE_Timer = 15000;    
			 me->ForcedDespawn();
	  }  
		 }
		 } 
	       
       void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;    
       
		 	 if (STATE_Timer <= diff) 
			 {  
		    pPlayer->RemoveAurasDueToSpell(DREAM_STATE);
			pPlayer->RemoveUnitMovementFlag(MOVEMENTFLAG_FLY_MODE); 
			pPlayer->SendMovementFlagUpdate();
			}else STATE_Timer -= diff;   
     }
};
/////////////////////////////////////////////////// 
//Archmage  
struct npc_skellmageAI : public ScriptedAI
{
	npc_skellmageAI(Creature *c) : ScriptedAI(c){}   
	 
	uint32 VOLLEY_Timer; 
    uint32 COLUMN_Timer;  
	uint32 VOID_Timer;



   void EnterCombat(Unit* who)
    {  
	  VOLLEY_Timer = 12000; 
	  COLUMN_Timer = 20000; 
	  VOID_Timer = 30000; 
    }

   void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;   
			    
 
        if (VOLLEY_Timer <= diff)
		{
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, VOLLEY);
           VOLLEY_Timer = 15000; 
		} else VOLLEY_Timer -= diff;  
		 
	    if (VOID_Timer <= diff)
		{
           if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
           me->SummonCreature(VOID,pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 15000);   
           VOID_Timer = 30000; 
		} else VOID_Timer -= diff; 
 
        if (COLUMN_Timer <= diff)
		{
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, COLUMN);
           COLUMN_Timer = 20000; 
		} else COLUMN_Timer -= diff; 
 
      DoMeleeAttackIfReady(); 
   } 
    
     void KilledUnit(Unit* victim)
    {
        DoScriptText(PDEATH, pValithria);
    }
  
};
 
/////////////////////////////////////////////////// 
//Skeleton 
struct npc_fireskellAI : public ScriptedAI
{
	npc_fireskellAI(Creature *c) : ScriptedAI(c){}   
	 
	uint32 WASTE_Timer; 
    uint32 FIREBALL_Timer;  
	   

	 
   void EnterCombat(Unit* who)
    {  
	  WASTE_Timer = 18000; 
	  FIREBALL_Timer = 5000; 
    }

   void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;   

        if (WASTE_Timer <= diff)
		{
           DoCast(WASTE);
           WASTE_Timer = 24000; 
		} else WASTE_Timer -= diff; 
 
        if (FIREBALL_Timer <= diff)
		{
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, FIREBALL);
           FIREBALL_Timer = 5000; 
		} else FIREBALL_Timer -= diff; 
 
      DoMeleeAttackIfReady(); 
   }
   
        void KilledUnit(Unit* victim)
    {
        DoScriptText(PDEATH, pValithria);
    }
}; 
/////////////////////////////////////////////////// 
//Suppressor 
struct npc_suppressorAI : public ScriptedAI
{
	npc_suppressorAI(Creature *c) : ScriptedAI(c){}    

   uint32 Check_Timer; 


   void EnterCombat(Unit* who)
	 { 
	 me->SetReactState(REACT_PASSIVE);  
	 Check_Timer = 2500;
	 } 
 
       void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;   
  
	 if (Check_Timer <= diff) 
	 { 
     me->CastSpell(pValithria,SUPRESSION,true,0,0,0);  
	 Check_Timer = 100000;  
	 }
	 else Check_Timer -= diff; 
	   }
}; 
/////////////////////////////////////////////////// 
//Void 
struct npc_manavoidAI : public ScriptedAI
{
	npc_manavoidAI(Creature *c) : ScriptedAI(c){}   
	 
  
    void InitializeAI() 
	{   
		DoCast(MANA_VOID);
    }
   
		  void EnterCombat(Unit* who)
    {   
		me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		DoStartNoMovement(me->getVictim()); 
    }

};  
/////////////////////////////////////////////////// 
//Abomination 
struct npc_glutabominationAI : public ScriptedAI
{
	npc_glutabominationAI(Creature *c) : ScriptedAI(c){}   
	  
	uint32 SPRAY_Timer;  



   void EnterCombat(Unit* who)
	 { 
	  SPRAY_Timer = 10000; 
	 } 

   void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;   


        if (SPRAY_Timer <= diff)
        {
            DoCast(me->getVictim(), SPRAY);
            SPRAY_Timer = 20000;
        } else SPRAY_Timer -= diff;  


		DoMeleeAttackIfReady();  

        }   
    
        void KilledUnit(Unit* victim)
    {
        DoScriptText(PDEATH, pValithria);
    }
    
       void JustDied(Unit* killer)  
    {
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);   	
        me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
		me->SummonCreature(WORM,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_CORPSE_DESPAWN);
    }

}; 
/////////////////////////////////////////////////// 
//Zombie 
struct npc_blistzombieAI : public ScriptedAI
{
	npc_blistzombieAI(Creature *c) : ScriptedAI(c){}  
	 
	uint32 BURST_Timer;  
	uint32 DELAY_Timer;  

 

	void EnterCombat(Unit* who) 
	{ 
	  BURST_Timer = 20000;  
	  DELAY_Timer = 99999; 
	} 
	 
 void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;     
			    
	 if (BURST_Timer <= diff)
        {
            DoCast(BURST); 
			BURST_Timer = 20000;
            DELAY_Timer = 1000;
        } else BURST_Timer -= diff;   
		  
	      if (DELAY_Timer <= diff)
        {
            me->ForcedDespawn();
            DELAY_Timer = 100000;
        } else DELAY_Timer -= diff;  
		  
		 DoMeleeAttackIfReady();  
        }  
  
      void KilledUnit(Unit* victim)
    {
        DoScriptText(PDEATH, pValithria);
    }
};
/////////////////////////////////////////////////// 
//Nightmare Cloud 
struct npc_dreamcloudAI : public ScriptedAI
{
	npc_dreamcloudAI(Creature *c) : ScriptedAI(c){}    
	 
	uint32 SPAWN;
    uint32 Delay_Timer;

	void InitializeAI() 
	{ 
	 DoCast(CLOUD_VISUAL);
	me->AddUnitMovementFlag(MOVEMENTFLAG_FLY_MODE);  
	me->SendMovementFlagUpdate();   
	me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
	me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	} 
	 
	void JustRespawned() 
	{ 
		 DoCast(CLOUD_VISUAL);
	me->AddUnitMovementFlag(MOVEMENTFLAG_FLY_MODE);  
	me->SendMovementFlagUpdate();   
	me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
	}
		 void MoveInLineOfSight(Unit *who)
		 {  

			 if (me->IsWithinDistInMap(who,5.0f))
            {   
	         DoCast(VIGOR);  
			 Delay_Timer = 100;
			 }
		 } 
   
	  void UpdateAI(const uint32 diff)
        {
        if (!UpdateVictim())
            return;    
		  





		 
		if (Delay_Timer <= diff) 
		{ 
		 me->ForcedDespawn(); 
		}else Delay_Timer -= diff;
	  }
	 
};
///////////////////////////////////////////////////  
//Dreamwalker
struct npc_dreamwalkerAI : public ScriptedAI
{
	npc_dreamwalkerAI(Creature *c) : ScriptedAI(c){}   
	 

	 
	  void InitializeAI() 
	  {  
	  me->SetReactState(REACT_PASSIVE);
	  me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
      me->AddUnitMovementFlag(MOVEMENTFLAG_FLY_MODE);  
	me->SendMovementFlagUpdate();   
	  }
	
};

///////////////////////////////////////////////////
CreatureAI* GetAI_boss_valithria(Creature* pCreature)
{
    return new boss_valithriaAI (pCreature);
}  

CreatureAI* GetAI_npc_dreamcloud(Creature* pCreature)
{
    return new npc_dreamcloudAI (pCreature);
}
 
CreatureAI* GetAI_npc_skellmage(Creature* pCreature)
{
    return new npc_skellmageAI (pCreature);
} 
 
CreatureAI* GetAI_npc_fireskell(Creature* pCreature)
{
    return new npc_fireskellAI (pCreature);
} 
 
CreatureAI* GetAI_npc_dreamportal(Creature* pCreature)
{
    return new npc_dreamportalAI (pCreature);
} 
 
CreatureAI* GetAI_npc_suppressor(Creature* pCreature)
{
    return new npc_suppressorAI (pCreature);
}  
 
CreatureAI* GetAI_npc_glutabomination(Creature* pCreature)
{
    return new npc_glutabominationAI (pCreature);
} 
 
CreatureAI* GetAI_npc_manavoid(Creature* pCreature)
{
    return new npc_manavoidAI (pCreature);
} 
 
CreatureAI* GetAI_npc_blistzombie(Creature* pCreature)
{
    return new npc_blistzombieAI (pCreature);
}

void AddSC_boss_valithria()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_valithria";
    newscript->GetAI = &GetAI_boss_valithria;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_skellmage";
    newscript->GetAI = &GetAI_npc_skellmage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_fireskell";
    newscript->GetAI = &GetAI_npc_fireskell;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name="npc_dreamportal";
    newscript->GetAI = &GetAI_npc_dreamportal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_suppressor";
    newscript->GetAI = &GetAI_npc_suppressor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_manavoid";
    newscript->GetAI = &GetAI_npc_manavoid;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_glutabomination";
    newscript->GetAI = &GetAI_npc_glutabomination;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_blistzombie";
    newscript->GetAI = &GetAI_npc_blistzombie;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_dreamcloud";
    newscript->GetAI = &GetAI_npc_dreamcloud;
    newscript->RegisterSelf();
}    

 

