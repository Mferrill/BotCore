/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: item_custom_summon
SD%Complete: 100
SDComment: Summon a NPC
SDCategory: Items
EndScriptData */

#include "ScriptPCH.h"

bool ItemUse_item_custom_summon(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
{
      pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
}
if (pPlayer->IsMounted()) // Is player mounted
{
      pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
}
{
      pPlayer->SummonCreature(99001,pPlayer->GetPositionX()+4 ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
      //pPlayer->SummonCreature(99002,pPlayer->GetPositionX() ,pPlayer->GetPositionY()+2, pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
      pPlayer->SummonCreature(111000,pPlayer->GetPositionX() ,pPlayer->GetPositionY(), pPlayer->GetPositionZ()+2, 0,TEMPSUMMON_TIMED_DESPAWN,120000);
	  pPlayer->SummonCreature(13,pPlayer->GetPositionX()+2 ,pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
	  pPlayer->SummonCreature(24780,pPlayer->GetPositionX() ,pPlayer->GetPositionY()+4, pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
}
        return false;
}
void AddSC_item_custom_summon()
{
    Script *newscript;

      newscript = new Script;
    newscript->Name="item_custom_summon";
    newscript->pItemUse = &ItemUse_item_custom_summon;
    newscript->RegisterSelf();
}
