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
#include "ScriptPCH.h"
#include "icecrown_citadel.h"

enum Yells
{
	SAY_AGGRO		=	-1666025,
	SAY_AIRLOCK		=	-1666026,	
	SAY_PHASE_HC	=	-1666027,
	SAY_TRANSFORM_1	=	-1666028,
	SAY_TRANSFORM_2	=	-1666029,
	SAY_KILL_1		=	-1666030,
	SAY_KILL_2		=	-1666031,
	SAY_BERSERK		=	-1666032,
	SAY_DEATH		=	-1666033,
};

enum Spells
{
	// Professor All Phase Spells
	SPELL_UNSTABLE_EXPERIMENT			=	71968,

	// Phase Change Spells
	N_SPELL_TEAR_GAS					=	71617,	// Phasenchange Spell
	N_SPELL_CREATE_CONCOTION			=	71621,	// Phase 1 to 2.
	N_SPELL_GUZZLE_POTIONS				=	73122,	// Phase 2 to 3.

	// 80% - 35%
	N_10_SPELL_MALLEABLE_GOO			=	72296,
	N_25_SPELL_MALLEABLE_GOO			=	70852,

	// 35% - 0%
	N_10_SPELL_MUTATED_STRENGTH			=	71603,
	N_10_MUTATED_PLAGUE					=	72672,

	// Ooze Spells
	N_10_SPELL_OOZE_ERUPTION			=	70492,
	N_10_SPELL_OOZE_ADHESIV				=	70447,
	// Gas Cloude Spells
	// Choking Gas Spells
	N_10_SPELL_CHOKING_GAS				=	71278, // Wenn Gas Spawnt.
	N_10_SPELL_CHOKING_GAS_EXPLOSION	=	71279, // 20 Sek.
};

enum Summons
{
	SUMMON_GASCLOUD			=	37562,
	SUMMON_VOLATILE_OOZE	=	37697,
};

enum Achievements
{
};

#define EMOTE_UNSTABLE_EXPERIMENT "Professor Seuchenmord beginnt Unstabiles Experiment zu wirken!"

struct Boss_ProfessorPutricideAI : public ScriptedAI
{
    Boss_ProfessorPutricideAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* m_pInstance;

	uint32 m_uiPhase;
	uint32 m_uiUnstableExperimentTimer;
	uint32 m_uiAddSpawnTimer;

    void Reset()
    {
		m_uiPhase = 1;
		m_uiUnstableExperimentTimer = 10000;
		m_uiAddSpawnTimer = 60000;
    }

    void EnterCombat(Unit* who)
    {
		DoScriptText(SAY_AGGRO, me);
    }

	void JustDied(Unit* killer)
    {  
		DoScriptText(SAY_DEATH, me);
    }

	void KilledUnit(Unit *victim)
    {
        DoScriptText(RAND(SAY_KILL_1,SAY_KILL_2), me);
    }

	void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

		if (m_uiUnstableExperimentTimer < uiDiff)
        {
			DoCast(me, SPELL_UNSTABLE_EXPERIMENT);
			me->MonsterTextEmote(EMOTE_UNSTABLE_EXPERIMENT,NULL);
			m_uiUnstableExperimentTimer = 40000;
			m_uiAddSpawnTimer = 5000;
        }
		else m_uiUnstableExperimentTimer -= uiDiff;

		if (m_uiAddSpawnTimer < uiDiff)
        {
			me->SummonCreature(SUMMON_VOLATILE_OOZE, me->GetPositionX()+20, me->GetPositionY()+20, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 9999999);
			m_uiAddSpawnTimer = 60000;
        }
		else m_uiAddSpawnTimer -= uiDiff;

		if (m_uiPhase == 1)
        {
		}

		if (m_uiPhase == 2)
        {
		}
			DoMeleeAttackIfReady();
		}
};

struct VolatileOozeAI : public ScriptedAI
{
    VolatileOozeAI(Creature *pCreature) : ScriptedAI(pCreature) 
	{
		me->ApplySpellImmune(0, IMMUNITY_ID, N_10_SPELL_OOZE_ADHESIV, true);
	}

    uint64 TargetGUID;

	uint32 OozeAdhesivTimer;
	uint32 OozeExplosion;
	uint32 MovechaseTimer;

    void EnterCombat(Unit *who) 
	{
		DoZoneInCombat();
	}

    void Reset()
    {
        TargetGUID = 0;
		OozeAdhesivTimer	= 1000;
		OozeExplosion		= 1000;
		MovechaseTimer		= 999999;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim()) 
			return;

        if (me->getVictim()->GetTypeId() != TYPEID_PLAYER) 
			return; // Only cast the below on players.

        if (!me->getVictim()->HasAura(N_10_SPELL_OOZE_ADHESIV))
        {
			if (OozeAdhesivTimer < diff)
			{
				uint32 count = RAID_MODE(1,1,1,1); // 10 Normal x1 / 25 Normal 3x / 10 Heroic 1x / 25 Heroic 3x
                for (uint8 i = 1; i <= count; i++)
                {
					DoCast(me->getVictim(), N_10_SPELL_OOZE_ADHESIV, true);
					me->GetMotionMaster()->MoveChase(me->getVictim());
					OozeAdhesivTimer = 999999;
					MovechaseTimer = 10000;
				}
			}
		else OozeAdhesivTimer -= diff;
        }

		if (MovechaseTimer < diff)
		{
			me->AddThreat(me->getVictim(), 10000000.0f);
			me->GetMotionMaster()->MoveChase(me->getVictim());
			MovechaseTimer = 2000;
		}
		else MovechaseTimer -= diff;
		

		if (OozeAdhesivTimer < diff)
		{
			if (me->IsWithinDistInMap(me->getVictim(), 3))
            DoCast(me->getVictim(), N_10_SPELL_OOZE_ERUPTION);
			OozeAdhesivTimer = 10000;
		}
		else OozeAdhesivTimer -= diff;
    }
};

CreatureAI* GetAI_VolatileOoze(Creature* pCreature)
{
    return new VolatileOozeAI(pCreature);
}

CreatureAI* GetAI_Boss_ProfessorPutricide(Creature* pCreature)
{
    return new Boss_ProfessorPutricideAI(pCreature);
}

void AddSC_Boss_ProfessorPutricide()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "Boss_ProfessorPutricide";
    NewScript->GetAI = &GetAI_Boss_ProfessorPutricide;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "Mob_VolatileOoze";
    NewScript->GetAI = &GetAI_VolatileOoze;
    NewScript->RegisterSelf();
}

/* UPDATE `creature_template` SET `ScriptName`='Boss_ProfessorPutricide' WHERE (`entry`='36678')  
   UPDATE `creature_template` SET `ScriptName`='Mob_VolatileOoze' WHERE (`entry`='37697')  
*/

/*Taktik - Phase 1 (100% - 80%)

In der Kennenlern-Phase kann sich einer der Tanks mit den Fähigkeiten der Monstrosität vertraut machen, während die Gruppe gegen die zwei Schleimtypen kämpft.

Mutierte Monströsität
Einer der beiden Tanks kann sich durchs Trinken einer der Tränke auf Professor Seuchenmords Labortisch in eine Monstrosität verwandeln. Diese Monstrosität besitzt drei Fähigkeiten:

    * 1. Brühschlammer essen
      Diese Fähigkeit saugt die anwachsenden Brühschlammpfützen auf, hält somit den Raum sauber und gibt der Monstrosität Energie für Fähigkeit Nummer zwei. Es bedarf mehrerer Klicks, um eine Brühschlammerpfütze vollständig aufzulösen.
    * 2. Erbrochener Brühschlammer
      Diese Fähigkeit muss auf die Schlammer angewendet werden. Sie verlangsamt diese um 50%.
    * 3. Mutiertes Schlitzen
      Der Standardangriff der Monstrosität und die "Wenn du mal Zeit hast"-Fähigkeit. Der Angriff ist sehr praktisch. Da er einen bis zu fünf mal stapelbaren Debuff auf dem Ziel hinterlässt, welcher dessen Widerstand verringert und somit den Schaden aller Spieler erhöht. Er sollte daher auf die Schleimer angewendet werden.

Positionierung:
Eine feste Aufstellung gibt es in diesem Kampf nicht. Es gibt jedoch zwei Punkte zu beachten. Flüchtige Brühschlammer (Grüner Schleimer) entstehen immer im Norden und Gaswolken (Orange Schleimer) immer im Süden. Da sie abwechselnd entstehen muss die gesamte Gruppe während oder nach der Zerstörung des Schleimers immer auf die gegenüberliegende Seite der Halle laufen. Davon abgesehen muss der Tank lediglich Acht geben, Professor Seuchenmord von dem Schleimpfützen weg zu ziehen.

Kampfverlauf:
Von Phasenbeginn bis zum Ende des Kampfes werden immer wieder anwachsende Brühschleimpfützen im Raum entstehen. Diese richten bei Berührung 4.000 Schaden an und müssen von dem Monstrosität-kontrollierenden-Spieler gegessen und somit aufgelöst werden.
So fern keine anderen Gegner vorhanden sind, müssen alle Spieler selbstverständlich Professor Seuchenmords Lebensenergie verringern. Wirkt er ein Instabiles Experiment so entsteht am nördlichen oder südlichen Ende des Raumes ein Schleimer. Die Art und Ort wechseln sich immer ab. Sie entstehen ungefähr alle 40 Sekunden durch Professor Seuchenmord und werden fünf Sekunden später aktiv. Beide Schleimer können und müssen von der Monstrosität verlangsamt werden. Dieses kann über Distanz geschehen und verringert die Bewegungsgeschwindigkeit des Schleimers um 50%.

Flüchtige Brühschlammer (Grüner Schleimer)
Dieser grüne Schleimer entsteht immer im Norden. Sobald er aktiv wird, wirkt er Flüchtiger Brühschlammerkleber auf einen zufälligen Spieler und macht den somit unbeweglich. Optisch bildet sich ein grüner Strahl zwischen dem Schleimer und dem Spieler. Der Schleimer wird den Spieler relativ schnell erreichen (innerhalb von drei bis vier Sekunden). Wenn er am Spieler angelangt ist, wirkt der Schleimer augenblicklich seine Brühschlammeruption, die 70.000 Schaden anrichtet. Der Schaden wird jedoch durch alle Spieler geteilt, die sich in einem zehn Meter Bereich um ihn befinden. Alle Schadensverursacher müssen sich also am festgeklebten Spieler sammeln. Die Brühschlammeruption schleudert außerdem alle zurück. Anschließend sucht sich der grüne Schleimer ein neues Ziel zum Festkleben. Ihr solltet den Schleimer innerhalb von zwei bis drei Zielphasen getötet bekommen.

Gaswolke (Oranger Schleimer)
Diese Gaswolke sieht eher wie ein orange Schleimer aus und entsteht immer im Süden. Sie wählt ebenfalls einen zufälligen Spieler aus, zu dem sie einen orangen Strahl etabliert. Der Spieler erhält zehn Stapel der Gasförmigen Blähung die alle zwei Sekunden tickt und sich somit um einen Stapel verringert. D.h. der Spieler erleidet zwei Sekunden nach dem Anvisieren 12.000 Schaden und alle zwei Sekunden darauf 1.200 Schaden weniger (10.800, 9.600, 8.400 usw.). Ist der Stapel nach 20 Sekunden ausgelaufen und die Gaswolke noch nicht tot, sucht sie sich ein weiteres Ziel, dem sie erneut zehn Stapel der Gasförmigen Blähung zufügt. Der anvisierte Spieler muss vor der Gaswolke weglaufen und sie hinter sich her locken. Er kann dabei z.B. im Halbkreis zur gegenüberliegenden Seite des Raumes laufen. Wenn die Gaswolke den verfolgten Spieler erreicht, richtet sie für nahezu alle Spieler tödlichen Schaden an.

Phase 2 (80% - 35%)

Durch die Tränke bekommt Professor Seuchenmord in dieser Phase das Erstickende Gas und den Formbaren Schleim. Die beiden Schleimer bleiben ebenfalls aktiv.

Erstickendes Gas
Professor Seuchenmord legt mehrere Phiolen mit diesem bräunlichen Gas auf dem Boden ab, die wenige Sekunden darauf explodieren und in einem zehn Meter Radius 20.000 Flächenschaden anrichten. Es ist sehr wichtig, dass niemand diese Explosion erleidet. Denn selbst wenn er sie überlebt, verringert sich dessen Trefferchance um 75%. Was einen Schadensverursacher unbrauchbar macht und auch den Tank am weiteren Hassaufbau hindern wird.

Formbaren Schleim
Professor Seuchenmord visiert einen Spieler an und schleudert diesen Schleimflummi in dessen Richtungen. Getroffene Spieler erleiden 10.000 Schaden und müssen die nächsten 20 Sekunden mit einer 200% verringerten Angriffs-/Zaubergeschwindigkeit leben. So lange es Heiler oder Fernkämpfer auf Distanz gibt, werden diese als Ziel für den Formbaren Schleim ausgewählt. Andernfalls treffen sie Nahkämpfer. Es sollten sich schlichtweg alle Spieler bewegen, sobald Professor Seuchenmord den Flummi in die Luft schleudert.

Wenn sich Professor Seuchenmords Gesundheit der 35% Marke nähert, solltet ihr darauf achten, dass gerade kein Schleimer aktiv ist und nur dann den Phasenwechsel einleiten, damit ihr euch in Phase drei ungehindert um den Professor kümmern könnt.
Taktik - Phase 3 (34% - Tod)

In dieser Phase löst sich die Monstrosität auf und der zweite Tank wird somit frei. Die beiden Tanks müssen sich fortan beim Tanken von Professor Seuchenmord abwechseln. Das hat außerdem zu Folge, dass die Phase zeitlich begrenzt ist, da die anwachsenden Brühschlammpfützen nicht mehr zerstört werden und ähnlich wie im Lady Vashj Kampf nach kurzer Zeit den Raum ausfüllen und alle töten.

Während der Professor weiterhin Erstickendes Gas und Formbaren Schleim verteilt, müsst ihr mit zwei neuen Fähigkeiten zu Recht kommen:

1. Mutierte Stärke
Durch diesen Buff auf Professor Seuchenmord erhöht sich alle drei Sekunden der Schaden und das Angriffstempo um 50%.

2. Mutierte Seuche
Dieser Debuff stapelt sich auf dem aktuellen Tank und erhöht sich alle zehn Sekunden. Mit jedem neuen Stapel erleidet die gesamte Schlachtgruppe 72.000 Schaden x Stapel. Der Schaden wird bei sechs Stapeln auf dem Tank für alle Spieler tödlich.

Durch diese zwei (De)Buffs ist es erforderlich, dass der Tank ständig wechselt und Professor Seuchenmord nach ca. 80 Sekunden sterben sollte:

10.  Sekunde: 1-0 Stapel => Tankschadensteigerung 15%  & Gruppenschaden    300 jede 3. Sek.
20.  Sekunde: 1-1 Stapel => Tankschadensteigerung 35%  & Gruppenschaden    600 jede 3. Sek.
30.  Sekunde: 2-1 Stapel => Tankschadensteigerung 50%  & Gruppenschaden  1.050 jede 3. Sek.
40.  Sekunde: 2-2 Stapel => Tankschadensteigerung 65%  & Gruppenschaden  1.500 jede 3. Sek.
50.  Sekunde: 3-2 Stapel => Tankschadensteigerung 85%  & Gruppenschaden  2.550 jede 3. Sek.
60.  Sekunde: 3-3 Stapel => Tankschadensteigerung 100% & Gruppenschaden  3.600 jede 3. Sek.
70.  Sekunde: 4-3 Stapel => Tankschadensteigerung 115% & Gruppenschaden  6.600 jede 3. Sek.
80.  Sekunde: 4-4 Stapel => Tankschadensteigerung 135% & Gruppenschaden  9.600 jede 3. Sek.

Stirbt hingegen einer Tanks oder läuft der Stapel auf ihm aus, wird Professor Seuchenmord für 300.000 x Stapel auf dem Tank geheilt.

Grundlegend ist diese Phase für den Einsatz von Kampfrausch und alle Spezialfähigkeiten mit hoher Abklingzeit prädestiniert. Haut den Professor um, bevor ihr im Schleim ertrinkt! 
*/