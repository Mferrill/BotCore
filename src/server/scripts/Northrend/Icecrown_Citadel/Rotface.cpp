/*
 * Copyright (C) 2009 - 2010 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*Comment:
Script created by Goga (TrinDev Team special for trinity-core.ru and wowjp.net) Please if you use this script contact copiryght*/

#include "ScriptPCH.h"
#include "icecrown_citadel.h"
#include "ScriptedEscortAI.h"

enum Yells
{
SAY_AGGRO                  = -1666016,
SAY_DEATH                  = -1666017,
SAY_DEATH_2                = -1666018,
SAY_KILLING                = -1666019,
SAY_KILLING_2              = -1666020, 
SAY_BERSERK                = -1666021,
SAY_SLIME_SPRAY            = -1666022,
SAY_EXPLOSION              = -1666023,

};

enum Creatures
{
CREATURE_OOZE_BIG             = 36899,
CREATURE_LITTLE_OOZE          = 36897,
CREATURE_PROFFESOR_PUTRICIDE  = 36678,
};

enum RotfaceSpells
{
SPELL_OOZE_FLOOD           = 71215,
SPELL_SLIME_SPRAY          = 71213,
SPELL_MUTATED_INFECTION    = 71224,
SPELL_BERSERK              = 47008,
};

enum OozeSpells
{
SPELL_STICKY_OOZE          = 71208,
SPELL_RADIATING_OOZE       = 71212,
SPELL_UNSTABLE_OOZE        = 69558,
SPELL_UNSTABLE_EXPLOSION   = 71209,
};

Creature* pProfessor_Putricide;

struct boss_RotfaceAI : public ScriptedAI
{
    boss_RotfaceAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* m_pInstance;

	uint32 m_uiOozeFloodTimer;
	uint32 m_uiSlimeSprayTimer;
	uint32 m_uiMutatedInfectionTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiSummonBigTimer;
    uint32 m_uiSummonLittleTimer;

    void Reset()
    {
	m_uiOozeFloodTimer = 12000+rand()%3000;
	m_uiSlimeSprayTimer = 20000+rand()%2000;
	m_uiMutatedInfectionTimer = 6000+rand()%4000;
    m_uiBerserkTimer = 360000;
	m_uiSummonLittleTimer = 5000; // not officially ?
	m_uiSummonBigTimer = 15000; // not officially
	 

		if(m_pInstance)
		m_pInstance->SetData(DATA_ROTFACE_EVENT, NOT_STARTED);  
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, me);
        DoScriptText(SAY_DEATH_2, pProfessor_Putricide);

		if(m_pInstance)
		m_pInstance->SetData(DATA_ROTFACE_EVENT, DONE);  
    }

    void EnterCombat(Unit* who)
    {
		DoScriptText(SAY_AGGRO, me);
    }

    void JustReachedHome()
    {
		if(m_pInstance)
		m_pInstance->SetData(DATA_ROTFACE_EVENT, FAIL);  
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_KILLING, me);
        DoScriptText(SAY_KILLING_2, me);
    }

	void UpdateAI(const uint32 diff)
	{
		if (!UpdateVictim())
			return;

            if (m_uiSummonLittleTimer<= diff)  
            { 
                 me->SummonCreature(36897, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);

                m_uiSummonLittleTimer = 23000+rand()%4000; 
            }  
            else m_uiSummonLittleTimer -= diff;

            if (m_uiSummonBigTimer<= diff)  
            { 
                 me->SummonCreature(36899, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);

                m_uiSummonBigTimer = 32000+rand()%4000; 
            }  
            else m_uiSummonBigTimer -= diff;


		if (m_uiOozeFloodTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71215);
			m_uiOozeFloodTimer = 12000+rand()%3000;
		} else m_uiOozeFloodTimer -= diff;

		if (m_uiSlimeSprayTimer <= diff)
		{
			DoScriptText(SAY_SLIME_SPRAY, me);
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71213);
			m_uiSlimeSprayTimer = 20000+rand()%2000;
		} else m_uiSlimeSprayTimer -= diff;

		if (m_uiMutatedInfectionTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71213);
			m_uiMutatedInfectionTimer = 6000+rand()%4000;
		} else m_uiMutatedInfectionTimer -= diff;

		if (m_uiBerserkTimer <= diff)
		{
                  DoScriptText(SAY_BERSERK, me);
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 47008);
			m_uiBerserkTimer = 360000;
		} else m_uiBerserkTimer -= diff;

		DoMeleeAttackIfReady();
	}
};

struct npc_OozeBigAI : public ScriptedAI
{
    npc_OozeBigAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* m_pInstance;

	uint32 m_uiStickyOozeTimer;
	uint32 m_uiRadiatingOozeTimer;
	uint32 m_uiUnstableOozeTimer;
      uint32 m_uiUnstableExplosionTimer;

    void Reset()
    {
	m_uiStickyOozeTimer = 5000+rand()%3000;
	m_uiRadiatingOozeTimer = 7000+rand()%2000;
      m_uiUnstableOozeTimer = 4000;
      m_uiUnstableExplosionTimer = 20000;
    }

    void EnterCombat(Unit* who)
    {
    }

    void KilledUnit(Unit *victim)
    {
    }

	void UpdateAI(const uint32 diff)
	{
		if (!UpdateVictim())
			return;

		if (m_uiStickyOozeTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71208);
			m_uiStickyOozeTimer = 5000+rand()%3000;
		} else m_uiStickyOozeTimer -= diff;

		if (m_uiRadiatingOozeTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71212);
			m_uiRadiatingOozeTimer = 360000;
		} else m_uiRadiatingOozeTimer -= diff;

		if (m_uiUnstableOozeTimer <= diff)
		{
			DoCast(me, 69558);
			 m_uiUnstableOozeTimer = 4000;
		} else m_uiUnstableOozeTimer -= diff;

		if (m_uiUnstableExplosionTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71212);
			m_uiUnstableExplosionTimer = 20000;
		} else m_uiUnstableExplosionTimer -= diff;

		DoMeleeAttackIfReady();
	}
};

struct npc_OozeLittleAI : public ScriptedAI
{
    npc_OozeLittleAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* m_pInstance;

	uint32 m_uiStickyOozeTimer;
	uint32 m_uiRadiatingOozeTimer;

    void Reset()
    {
	m_uiStickyOozeTimer = 5000+rand()%2000;
	m_uiRadiatingOozeTimer = 6000+rand()%3000;
    }

    void EnterCombat(Unit* who)
    {
    }

    void KilledUnit(Unit *victim)
    {
    }

	void UpdateAI(const uint32 diff)
	{
		if (!UpdateVictim())
			return;

		if (m_uiStickyOozeTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71208);
			m_uiStickyOozeTimer = 5000+rand()%3000;
		} else m_uiStickyOozeTimer -= diff;

		if (m_uiRadiatingOozeTimer <= diff)
		{
			DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true), 71210);
			m_uiRadiatingOozeTimer = 360000;
		} else m_uiRadiatingOozeTimer -= diff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_Rotface(Creature* pCreature)
{
    return new boss_RotfaceAI(pCreature);
}

CreatureAI* GetAI_npc_OozeBig(Creature* pCreature)
{
    return new npc_OozeBigAI(pCreature);
}

CreatureAI* GetAI_npc_OozeLittle(Creature* pCreature)
{
    return new npc_OozeLittleAI(pCreature);
}

void AddSC_boss_Rotface()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_rotface";
    NewScript->GetAI = &GetAI_boss_Rotface;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "npc_OozeBig";
    NewScript->GetAI = &GetAI_npc_OozeBig;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "npc_OozeLittle";
    NewScript->GetAI = &GetAI_npc_OozeLittle;
    NewScript->RegisterSelf();
}
