/* Pridefire Gaming > All others. This is our legal statement. FUCK YALL!*/
#include "World.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "BattleGroundMgr.h"
#include "BattleGround.h"
#include "BattleGroundIC.h"
#include "Creature.h"
#include "Language.h"
#include "Object.h"
#include "Player.h"
#include "Util.h"
#include "Unit.h"

// these variables aren't used outside of this file, so declare them only here
uint32 BG_IC_HonorScoreTicks[BG_HONOR_MODE_NUM] = {
    330, // normal honor
    200  // holiday
};

BattleGroundIC::BattleGroundIC()
{
    m_BuffChange = true;
    m_BgObjects.resize(BG_IC_OBJECT_MAX);
    m_BgCreatures.resize(BG_IC_ALL_NODES_COUNT);

    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_IC_START_TWO_MINUTES;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_IC_START_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_IC_START_HALF_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_IC_HAS_BEGUN;
}

BattleGroundIC::~BattleGroundIC()
{
}

void BattleGroundIC::Update(uint32 diff)
{
    BattleGround::Update(diff);

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        int team_points[BG_TEAMS_COUNT] = { 0, 0 };

        for (int node = 0; node < BG_IC_DYNAMIC_NODES_COUNT; ++node)
        {
            if (m_BannerTimers[node].timer)
            {
                if (m_BannerTimers[node].timer > diff)
                    m_BannerTimers[node].timer -= diff;
                else
                {
                    m_BannerTimers[node].timer = 0;
                    _CreateBanner(node, m_BannerTimers[node].type, m_BannerTimers[node].teamIndex, false);
                }
            }
            if (m_NodeTimers[node])
            {
                if (m_NodeTimers[node] > diff)
                    m_NodeTimers[node] -= diff;
                else
                {
                    m_NodeTimers[node] = 0;
                    uint8 teamIndex = m_Nodes[node]-1;
                    m_prevNodes[node] = m_Nodes[node];
                    m_Nodes[node] += 2;
                    _DelBanner(node, BG_IC_NODE_TYPE_CONTESTED, teamIndex);
                    _CreateBanner(node, BG_IC_NODE_TYPE_OCCUPIED, teamIndex, true);
                    _SendNodeUpdate(node);
                    _NodeOccupied(node,(teamIndex == 0) ? ALLIANCE:HORDE);

                    if (teamIndex == 0)
                    {
                        SendMessage2ToAll(LANG_BG_IC_NODE_TAKEN,CHAT_MSG_BG_SYSTEM_ALLIANCE,NULL,LANG_BG_IC_ALLY,_GetNodeNameId(node));
                        PlaySoundToAll(BG_IC_SOUND_NODE_CAPTURED_ALLIANCE);
                    }
                    else
                    {
                        SendMessage2ToAll(LANG_BG_IC_NODE_TAKEN,CHAT_MSG_BG_SYSTEM_HORDE,NULL,LANG_BG_IC_HORDE,_GetNodeNameId(node));
                        PlaySoundToAll(BG_IC_SOUND_NODE_CAPTURED_HORDE);
                    }
                }
            }

            for (int team = 0; team < BG_TEAMS_COUNT; ++team)
                if (m_Nodes[node] == team + BG_IC_NODE_TYPE_OCCUPIED)
                    ++team_points[team];
        }
    }
}

void BattleGroundIC::HandleKillUnit(Creature *unit, Player *killer)
{
    sLog.outDebug("bg_ic HandleKillUnit %i",unit->GetEntry());
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
    uint32 entry = unit->GetEntry();

    if(entry == BG_IC_StaticCreatureInfo[IC_NPC_A_BOSS][0])
    {
        RewardHonorToTeam(GetBonusHonor(BG_IC_KILL_BOSS),HORDE);
        EndBattleGround(HORDE);
    }
	else if ( entry == BG_IC_StaticCreatureInfo[IC_NPC_H_BOSS][0] )
    {
        RewardHonorToTeam(GetBonusHonor(BG_IC_KILL_BOSS),ALLIANCE);
        EndBattleGround(ALLIANCE);
    }
}

const uint16 BattleGroundIC::GetBonusHonor(uint8 kills)
{
    return Trinity::Honor::hk_honor_at_level(m_MaxLevel, kills);
}

void BattleGroundIC::StartingEventCloseDoors()
{

    uint16 i;

    for (int obj = BG_IC_OBJECT_BANNER_NEUTRAL; obj < BG_IC_DYNAMIC_NODES_COUNT * 8; ++obj)
        SpawnBGObject(obj, RESPAWN_ONE_DAY);

    for (i = BG_IC_OBJECT_HORDE_PORTCULLIS; i <= BG_IC_OBJECT_SEAFOAM_BOMB_3 ; i++)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);

    // Starting doors
    DoorClose(BG_IC_OBJECT_GATE_H_1);
    DoorClose(BG_IC_OBJECT_GATE_A_1);
    DoorClose(BG_IC_OBJECT_GATE_H_2);
    DoorClose(BG_IC_OBJECT_GATE_A_2);
    DoorClose(BG_IC_OBJECT_GATE_H_3);
    DoorClose(BG_IC_OBJECT_GATE_A_3);
    SpawnBGObject(BG_IC_OBJECT_GATE_H_1, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_IC_OBJECT_GATE_A_1, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_IC_OBJECT_GATE_H_2, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_IC_OBJECT_GATE_A_2, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_IC_OBJECT_GATE_H_3, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_IC_OBJECT_GATE_A_3, RESPAWN_IMMEDIATELY);

    _NodeOccupied(BG_IC_SPIRIT_ALIANCE,ALLIANCE);
    _NodeOccupied(BG_IC_SPIRIT_HORDE,HORDE);
}

void BattleGroundIC::StartingEventOpenDoors()
{
    for (int banner = BG_IC_OBJECT_BANNER_NEUTRAL, i = 0; i < 5; banner += 8, ++i)
        SpawnBGObject(banner, RESPAWN_IMMEDIATELY);
    DoorOpen(BG_IC_OBJECT_GATE_H_1);
    DoorOpen(BG_IC_OBJECT_GATE_A_1);
    DoorOpen(BG_IC_OBJECT_GATE_H_2);
    DoorOpen(BG_IC_OBJECT_GATE_A_2);
    DoorOpen(BG_IC_OBJECT_GATE_H_3);
    DoorOpen(BG_IC_OBJECT_GATE_A_3);
}

void BattleGroundIC::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);
    BattleGroundICScore* sc = new BattleGroundICScore;

    m_PlayerScores[plr->GetGUID()] = sc;
}

void BattleGroundIC::RemovePlayer(Player * /*plr*/, uint64 /*guid*/)
{

}

void BattleGroundIC::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

Creature* BattleGroundIC::AddICCreature(uint16 cinfoid, uint16 type)
{
// In retail these only spawn if you've destroyed a gate, but who cares right now. We'll fix it l8r
// Which also techincally makes them not static, ill fix that later to.
    uint8 level;
    bool isStatic = false;
    Creature* creature = NULL;
    if (type >= IC_STATICCPLACE_MAX)
    {
        type -= IC_STATICCPLACE_MAX;
        cinfoid=uint16(BG_IC_StaticCreaturePos[type][4]);
        creature = AddCreature(BG_IC_StaticCreatureInfo[cinfoid][0],(type+IC_STATICCPLACE_MAX),BG_IC_StaticCreatureInfo[cinfoid][1],BG_IC_StaticCreaturePos[type][0],BG_IC_StaticCreaturePos[type][1],BG_IC_StaticCreaturePos[type][2],BG_IC_StaticCreaturePos[type][3]);
        level = (BG_IC_StaticCreatureInfo[cinfoid][2] == BG_IC_StaticCreatureInfo[cinfoid][3]) ? BG_IC_StaticCreatureInfo[cinfoid][2] : urand(BG_IC_StaticCreatureInfo[cinfoid][2],BG_IC_StaticCreatureInfo[cinfoid][3]);
        isStatic = true;
    }
    if (!creature)
        return NULL;
        creature->GetMotionMaster()->Initialize();
        creature->setDeathState(JUST_DIED);
        creature->Respawn();

    return creature;
}

void BattleGroundIC::_CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay)
{
    if (delay)
    {
        m_BannerTimers[node].timer = 2000;
        m_BannerTimers[node].type = type;
        m_BannerTimers[node].teamIndex = teamIndex;
        return;
    }

    uint8 obj = node*8 + type + teamIndex;

    SpawnBGObject(obj, RESPAWN_IMMEDIATELY);

    if (!type)
        return;
    obj = node * 8 + ((type == BG_IC_NODE_TYPE_OCCUPIED) ? (5 + teamIndex) : 7);
    SpawnBGObject(obj, RESPAWN_IMMEDIATELY);
}

void BattleGroundIC::_DelBanner(uint8 node, uint8 type, uint8 teamIndex)
{
    uint8 obj = node*8 + type + teamIndex;
    SpawnBGObject(obj, RESPAWN_ONE_DAY);

    if (!type)
        return;
    obj = node * 8 + ((type == BG_IC_NODE_TYPE_OCCUPIED) ? (5 + teamIndex) : 7);
    SpawnBGObject(obj, RESPAWN_ONE_DAY);
}

int32 BattleGroundIC::_GetNodeNameId(uint8 node)
{
    switch (node)
    {
        case BG_IC_NODE_WORKSHOP:    return LANG_BG_IC_NODE_WORKSHOP;
        case BG_IC_NODE_QUARRY: return LANG_BG_IC_NODE_QUARRY;
        case BG_IC_NODE_REFINERY:       return LANG_BG_IC_NODE_REFINERY;
        case BG_IC_NODE_DOCKS:return LANG_BG_IC_NODE_DOCKS;
        case BG_IC_NODE_HANGAR:  return LANG_BG_IC_NODE_HANGAR;
        default:
            ASSERT(0);
    }
    return 0;
}

void BattleGroundIC::FillInitialWorldStates(WorldPacket& data)
{
    data << uint32(IC_Alliance_Score)  << uint32(m_Team_Scores[0]);
    data << uint32(IC_Horde_Score) << uint32(m_Team_Scores[1]);
    if(GetStatus() == STATUS_IN_PROGRESS){ 
        data << uint32(IC_SHOW_A_SCORE) << uint32(1);
        data << uint32(IC_SHOW_H_SCORE) << uint32(1);
    }
    else
    {
        data << uint32(IC_SHOW_A_SCORE) << uint32(0);
        data << uint32(IC_SHOW_H_SCORE) << uint32(0);
    }
}

void BattleGroundIC::_SendNodeUpdate(uint8 node)
{
    const uint8 plusArray[] = {0, 2, 3, 0, 1};

    if (m_prevNodes[node])
        UpdateWorldState(BG_IC_OP_NODESTATES[node] + plusArray[m_prevNodes[node]], 0);
    else
        UpdateWorldState(BG_IC_OP_NODEICONS[node], 0);

    UpdateWorldState(BG_IC_OP_NODESTATES[node] + plusArray[m_Nodes[node]], 1);

    uint8 ally = 0, horde = 0;
    for (uint8 i = 0; i < BG_IC_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == BG_IC_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == BG_IC_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;
}

void BattleGroundIC::_NodeOccupied(uint8 node,Team team)
{
   if (!AddSpiritGuide(node, BG_IC_SpiritGuidePos[node][0], BG_IC_SpiritGuidePos[node][1], BG_IC_SpiritGuidePos[node][2], BG_IC_SpiritGuidePos[node][3], team))
        sLog.outError("Failed to spawn spirit guide! point: %u, team: %u,", node, team);
//   SpawnBGCreature(node,RESPAWN_IMMEDIATELY);

    uint8 capturedNodes = 0;
    for (uint8 i = 0; i < BG_IC_DYNAMIC_NODES_COUNT; ++i)
    {
        if (m_Nodes[node] == GetTeamIndexByTeamId(team) + BG_IC_NODE_TYPE_OCCUPIED && !m_NodeTimers[i])
            ++capturedNodes;
    }
}

void BattleGroundIC::_NodeDeOccupied(uint8 node)
{
    if (node >= BG_IC_DYNAMIC_NODES_COUNT)
        return;

    std::vector<uint64> ghost_list = m_ReviveQueue[m_BgCreatures[node]];
    if (!ghost_list.empty())
    {
        WorldSafeLocsEntry const *ClosestGrave = NULL;
        for (std::vector<uint64>::const_iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr)
        {
            Player* plr = objmgr.GetPlayer(*itr);
            if (!plr)
                continue;

            if (!ClosestGrave)
                ClosestGrave = GetClosestGraveYard(plr);

            if (ClosestGrave)
                plr->TeleportTo(GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, plr->GetOrientation());
        }
    }

    if (m_BgCreatures[node])
        DelCreature(node);
}

void BattleGroundIC::EventPlayerClickedOnFlag(Player *source, GameObject* /*target_obj*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 node = BG_IC_NODE_WORKSHOP;
    GameObject* obj=GetBgMap()->GetGameObject(m_BgObjects[node*8+7]);
    while ( (node < BG_IC_DYNAMIC_NODES_COUNT) && ((!obj) || (!source->IsWithinDistInMap(obj,10))))
    {
        ++node;
        obj=GetBgMap()->GetGameObject(m_BgObjects[node*8+BG_IC_OBJECT_AURA_CONTESTED]);
    }

    if (node == BG_IC_DYNAMIC_NODES_COUNT)
    {
        return;
    }

    BattleGroundTeamId teamIndex = GetTeamIndexByTeamId(source->GetTeam());

    if (!(m_Nodes[node] == 0 || teamIndex == m_Nodes[node]%2))
        return;

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    uint32 sound = 0;
    if (m_Nodes[node] == BG_IC_NODE_TYPE_NEUTRAL)
    {
        UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + 1;
        _DelBanner(node, BG_IC_NODE_TYPE_NEUTRAL, 0);
        _CreateBanner(node, BG_IC_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        m_NodeTimers[node] = BG_IC_FLAG_CAPTURING_TIME;

        if (teamIndex == 0)
            SendMessage2ToAll(LANG_BG_IC_NODE_CLAIMED,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node), LANG_BG_IC_ALLY);
        else
            SendMessage2ToAll(LANG_BG_IC_NODE_CLAIMED,CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node), LANG_BG_IC_HORDE);

        sound = BG_IC_SOUND_NODE_CLAIMED;
    }
    else if ((m_Nodes[node] == BG_IC_NODE_STATUS_ALLY_CONTESTED) || (m_Nodes[node] == BG_IC_NODE_STATUS_HORDE_CONTESTED))
    {
        if (m_prevNodes[node] < BG_IC_NODE_TYPE_OCCUPIED)
        {
            UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + BG_IC_NODE_TYPE_CONTESTED;
            _DelBanner(node, BG_IC_NODE_TYPE_CONTESTED, !teamIndex);
            _CreateBanner(node, BG_IC_NODE_TYPE_CONTESTED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = BG_IC_FLAG_CAPTURING_TIME;

            if (teamIndex == BG_TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_IC_NODE_ASSAULTED,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_IC_NODE_ASSAULTED,CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        else
        {
            UpdatePlayerScore(source, SCORE_BASES_DEFENDED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + BG_IC_NODE_TYPE_OCCUPIED;
            _DelBanner(node, BG_IC_NODE_TYPE_CONTESTED, !teamIndex);
            _CreateBanner(node, BG_IC_NODE_TYPE_OCCUPIED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = 0;
            _NodeOccupied(node,(teamIndex == BG_TEAM_ALLIANCE) ? ALLIANCE:HORDE);

            if (teamIndex == BG_TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_IC_NODE_DEFENDED,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_IC_NODE_DEFENDED,CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_IC_SOUND_NODE_ASSAULTED_ALLIANCE : BG_IC_SOUND_NODE_ASSAULTED_HORDE;
    }
    else
    {
        UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + BG_IC_NODE_TYPE_CONTESTED;
        _DelBanner(node, BG_IC_NODE_TYPE_OCCUPIED, !teamIndex);
        _CreateBanner(node, BG_IC_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        _NodeDeOccupied(node);
        m_NodeTimers[node] = BG_IC_FLAG_CAPTURING_TIME;

        if (teamIndex == BG_TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_IC_NODE_ASSAULTED,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_IC_NODE_ASSAULTED,CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));

        sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_IC_SOUND_NODE_ASSAULTED_ALLIANCE : BG_IC_SOUND_NODE_ASSAULTED_HORDE;
    }

    if (m_Nodes[node] >= BG_IC_NODE_TYPE_OCCUPIED)
    {
        if (teamIndex == BG_TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_IC_NODE_TAKEN,CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_IC_ALLY, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_IC_NODE_TAKEN,CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_IC_HORDE, _GetNodeNameId(node));
    }
    PlaySoundToAll(sound);
}

bool BattleGroundIC::SetupBattleGround()
{
    uint16 i;

    sLog.outDebug("BG_IC: start spawning static creatures");
    for (i=0; i < IC_STATICCPLACE_MAX; i++ )
        AddICCreature(0,i+IC_STATICCPLACE_MAX);

    for (int i = 0 ; i < BG_IC_DYNAMIC_NODES_COUNT; ++i)
    {
        if (!AddObject(BG_IC_OBJECT_BANNER_NEUTRAL + 8*i,BG_IC_OBJECTID_NODE_BANNER_0 + i,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_BANNER_CONT_A + 8*i,BG_IC_OBJECTID_BANNER_CONT_A,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_BANNER_CONT_H + 8*i,BG_IC_OBJECTID_BANNER_CONT_H,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_BANNER_ALLY + 8*i,BG_IC_OBJECTID_BANNER_A,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_BANNER_HORDE + 8*i,BG_IC_OBJECTID_BANNER_H,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_AURA_ALLY + 8*i,BG_IC_OBJECTID_AURA_A,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_AURA_HORDE + 8*i,BG_IC_OBJECTID_AURA_H,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
            || !AddObject(BG_IC_OBJECT_AURA_CONTESTED + 8*i,BG_IC_OBJECTID_AURA_C,BG_IC_NodePositions[i][0],BG_IC_NodePositions[i][1],BG_IC_NodePositions[i][2],BG_IC_NodePositions[i][3], 0, 0, sin(BG_IC_NodePositions[i][3]/2), cos(BG_IC_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
        )
        {
            sLog.outErrorDb("BatteGroundIC: Failed to spawn some object BattleGround not created!");
            return false;
        }
    }
    if (!AddObject(BG_IC_OBJECT_GATE_H_1,BG_IC_OBJECTID_GATE_H_1,BG_IC_DoorPositions[0][0],BG_IC_DoorPositions[0][1],BG_IC_DoorPositions[0][2],BG_IC_DoorPositions[0][3],BG_IC_DoorPositions[0][4],BG_IC_DoorPositions[0][5],BG_IC_DoorPositions[0][6],BG_IC_DoorPositions[0][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_GATE_A_1,BG_IC_OBJECTID_GATE_A_1,BG_IC_DoorPositions[1][0],BG_IC_DoorPositions[1][1],BG_IC_DoorPositions[1][2],BG_IC_DoorPositions[1][3],BG_IC_DoorPositions[1][4],BG_IC_DoorPositions[1][5],BG_IC_DoorPositions[1][6],BG_IC_DoorPositions[1][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_GATE_H_2,BG_IC_OBJECTID_GATE_H_2,BG_IC_DoorPositions[2][0],BG_IC_DoorPositions[2][1],BG_IC_DoorPositions[2][2],BG_IC_DoorPositions[2][3],BG_IC_DoorPositions[2][4],BG_IC_DoorPositions[2][5],BG_IC_DoorPositions[2][6],BG_IC_DoorPositions[2][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_GATE_A_2,BG_IC_OBJECTID_GATE_A_2,BG_IC_DoorPositions[3][0],BG_IC_DoorPositions[3][1],BG_IC_DoorPositions[3][2],BG_IC_DoorPositions[3][3],BG_IC_DoorPositions[3][4],BG_IC_DoorPositions[3][5],BG_IC_DoorPositions[3][6],BG_IC_DoorPositions[3][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_GATE_H_3,BG_IC_OBJECTID_GATE_H_3,BG_IC_DoorPositions[4][0],BG_IC_DoorPositions[4][1],BG_IC_DoorPositions[4][2],BG_IC_DoorPositions[4][3],BG_IC_DoorPositions[4][4],BG_IC_DoorPositions[4][5],BG_IC_DoorPositions[4][6],BG_IC_DoorPositions[4][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_GATE_A_3,BG_IC_OBJECTID_GATE_A_3,BG_IC_DoorPositions[5][0],BG_IC_DoorPositions[5][1],BG_IC_DoorPositions[5][2],BG_IC_DoorPositions[5][3],BG_IC_DoorPositions[5][4],BG_IC_DoorPositions[5][5],BG_IC_DoorPositions[5][6],BG_IC_DoorPositions[5][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_DESTRUCT_GATE_H_1,BG_IC_OBJECTID_DESTRUCT_GATE_H_1,BG_IC_DestructDoorPositions[0][0],BG_IC_DestructDoorPositions[0][1],BG_IC_DestructDoorPositions[0][2],BG_IC_DestructDoorPositions[0][3],BG_IC_DestructDoorPositions[0][4],BG_IC_DestructDoorPositions[0][5],BG_IC_DestructDoorPositions[0][6],BG_IC_DestructDoorPositions[0][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_DESTRUCT_GATE_A_1,BG_IC_OBJECTID_DESTRUCT_GATE_A_1,BG_IC_DestructDoorPositions[1][0],BG_IC_DestructDoorPositions[1][1],BG_IC_DestructDoorPositions[1][2],BG_IC_DestructDoorPositions[1][3],BG_IC_DestructDoorPositions[1][4],BG_IC_DestructDoorPositions[1][5],BG_IC_DestructDoorPositions[1][6],BG_IC_DestructDoorPositions[1][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_DESTRUCT_GATE_H_2,BG_IC_OBJECTID_DESTRUCT_GATE_H_2,BG_IC_DestructDoorPositions[2][0],BG_IC_DestructDoorPositions[2][1],BG_IC_DestructDoorPositions[2][2],BG_IC_DestructDoorPositions[2][3],BG_IC_DestructDoorPositions[2][4],BG_IC_DestructDoorPositions[2][5],BG_IC_DestructDoorPositions[2][6],BG_IC_DestructDoorPositions[2][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_DESTRUCT_GATE_A_2,BG_IC_OBJECTID_DESTRUCT_GATE_A_2,BG_IC_DestructDoorPositions[3][0],BG_IC_DestructDoorPositions[3][1],BG_IC_DestructDoorPositions[3][2],BG_IC_DestructDoorPositions[3][3],BG_IC_DestructDoorPositions[3][4],BG_IC_DestructDoorPositions[3][5],BG_IC_DestructDoorPositions[3][6],BG_IC_DestructDoorPositions[3][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_DESTRUCT_GATE_H_3,BG_IC_OBJECTID_DESTRUCT_GATE_H_3,BG_IC_DestructDoorPositions[4][0],BG_IC_DestructDoorPositions[4][1],BG_IC_DestructDoorPositions[4][2],BG_IC_DestructDoorPositions[4][3],BG_IC_DestructDoorPositions[4][4],BG_IC_DestructDoorPositions[4][5],BG_IC_DestructDoorPositions[4][6],BG_IC_DestructDoorPositions[4][7],RESPAWN_ONE_DAY)
        || !AddObject(BG_IC_OBJECT_DESTRUCT_GATE_A_3,BG_IC_OBJECTID_DESTRUCT_GATE_A_3,BG_IC_DestructDoorPositions[5][0],BG_IC_DestructDoorPositions[5][1],BG_IC_DestructDoorPositions[5][2],BG_IC_DestructDoorPositions[5][3],BG_IC_DestructDoorPositions[5][4],BG_IC_DestructDoorPositions[5][5],BG_IC_DestructDoorPositions[5][6],BG_IC_DestructDoorPositions[5][7],RESPAWN_ONE_DAY)
        )
    {
        sLog.outErrorDb("BatteGroundIC: Failed to spawn door object BattleGround not created!");
        return false;
    }
    return true;
}

void BattleGroundIC::Reset()
{
    BattleGround::Reset();

    m_TeamScores[BG_TEAM_ALLIANCE]          = 0;
    m_TeamScores[BG_TEAM_HORDE]             = 0;
    m_lastTick[BG_TEAM_ALLIANCE]            = 0;
    m_lastTick[BG_TEAM_HORDE]               = 0;
    m_HonorScoreTics[BG_TEAM_ALLIANCE]      = 0;
    m_HonorScoreTics[BG_TEAM_HORDE]         = 0;
    m_IsInformedNearVictory                 = false;
    bool isBGWeekend = sBattleGroundMgr.IsBGWeekend(GetTypeID());
    m_HonorTics = (isBGWeekend) ? BG_IC_ICBGWeekendHonorTicks : BG_IC_NotICBGWeekendHonorTicks;

    for (uint8 i = 0; i < BG_IC_DYNAMIC_NODES_COUNT; ++i)
    {
        m_Nodes[i] = 0;
        m_prevNodes[i] = 0;
        m_NodeTimers[i] = 0;
        m_BannerTimers[i].timer = 0;
    }

    for (uint8 i = 0; i < BG_IC_ALL_NODES_COUNT; ++i)
        if (m_BgCreatures[i])
            DelCreature(i);
}

void BattleGroundIC::EndBattleGround(uint32 winner)
{
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    BattleGround::EndBattleGround(winner);
}

WorldSafeLocsEntry const* BattleGroundIC::GetClosestGraveYard(Player* player)
{
    BattleGroundTeamId teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    std::vector<uint8> nodes;
    for (uint8 i = 0; i < BG_IC_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == teamIndex + 3)
            nodes.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;
    if (!nodes.empty())
    {
        float plr_x = player->GetPositionX();
        float plr_y = player->GetPositionY();

        float mindist = 999999.0f;
        for (uint8 i = 0; i < nodes.size(); ++i)
        {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry( BG_IC_GraveyardIds[nodes[i]] );
            if (!entry)
                continue;
            float dist = (entry->x - plr_x)*(entry->x - plr_x)+(entry->y - plr_y)*(entry->y - plr_y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        nodes.clear();
    }
    if (!good_entry)
        good_entry = sWorldSafeLocsStore.LookupEntry( BG_IC_GraveyardIds[teamIndex+5] );

    return good_entry;
}

void BattleGroundIC::UpdatePlayerScore(Player *Source, uint32 type, uint32 value)
{
    BattleGroundScoreMap::iterator itr = m_PlayerScores.find(Source->GetGUID());
    if( itr == m_PlayerScores.end() )
        return;

    switch(type)
    {
        case SCORE_BASES_ASSAULTED:
            ((BattleGroundICScore*)itr->second)->BasesAssaulted += value;
            break;
        case SCORE_BASES_DEFENDED:
            ((BattleGroundICScore*)itr->second)->BasesDefended += value;
            break;
        default:
            BattleGround::UpdatePlayerScore(Source,type,value);
            break;
    }
}
