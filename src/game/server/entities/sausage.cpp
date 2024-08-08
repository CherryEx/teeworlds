// By CherryEx

#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "character.h"
#include "sausage.h"

CSausage::CSausage(CGameWorld* pGameWorld, vec2 Pos, int Owner)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER, Pos)
{
	m_Owner = Owner;
	m_StartTick = Server()->Tick();
	m_Length = 50.0f;
	m_SpecialType = SPECIAL_SAUSAGE;
	GameWorld()->InsertEntity(this);
}

void CSausage::Tick()
{
	m_StartTick = Server()->Tick();
	CCharacter *pChr = GameServer()->GetPlayerChar(m_Owner);
	if (pChr && pChr->IsAlive())
	{
		float CorrectOffset = pChr->GetDirection() == CCharacter::DIRECTION_RIGHT ? 30.0f : -30.0f; // Right(30) or Left(-30)
		vec2 pPos = pChr->GetPos();
		m_From = vec2(pPos.x + CorrectOffset, pPos.y - m_Length);
		m_Pos = vec2(pPos.x + CorrectOffset, pPos.y + 10.0f);
	}
	else
	{
		if(GameServer()->m_apPlayers[m_Owner])
			GameServer()->m_apPlayers[m_Owner]->m_SpecialEntity = nullptr;
		GameWorld()->DestroyEntity(this);
	}
}

void CSausage::Consume()
{
	m_Length -= 22.0f;
	if(m_From.y - m_Length >= m_Pos.y)
	{
		CCharacter *pChar = GameServer()->GetPlayerChar(m_Owner);
		GameServer()->m_apPlayers[m_Owner]->m_SpecialEntity = nullptr;
		GameServer()->CreateDeath(pChar->GetPos(), m_Owner);
		pChar->SetEmote(EMOTE_HAPPY, Server()->Tick() + Server()->TickSpeed() * 1.5f);
		GameServer()->CreateSound(pChar->GetPos(), SOUND_PLAYER_SPAWN);
		GameWorld()->DestroyEntity(this);
	}
}

void CSausage::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient) && NetworkClipped(SnappingClient, m_From))
		return;

	CCharacter *pChar = GameServer()->GetPlayerChar(m_Owner);
	if(!pChar || pChar->GetActiveWeapon() != WEAPON_HAMMER)
		return;

	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, GetID(), sizeof(CNetObj_Laser)));
	if(!pObj)
		return;

	pObj->m_X = round_to_int(m_Pos.x);
	pObj->m_Y = round_to_int(m_Pos.y);
	pObj->m_FromX = round_to_int(m_From.x);
	pObj->m_FromY = round_to_int(m_From.y);
	pObj->m_StartTick = m_StartTick;
	pObj->m_OuterColor = 0xFF8DB3;
	pObj->m_InnerColor = 0xBA3030;
}

void CSausage::Reset()
{
	if(GameServer()->m_apPlayers[m_Owner])
			GameServer()->m_apPlayers[m_Owner]->m_SpecialEntity = nullptr;
	GameWorld()->DestroyEntity(this);
}
