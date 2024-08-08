// By CherryEx

#ifndef GAME_SERVER_ENTITIES_SAUSAGE_H
#define GAME_SERVER_ENTITIES_SAUSAGE_H

#include <game/server/entity.h>

class CSausage : public CEntity
{
public:
	CSausage(CGameWorld* pGameWorld, vec2 Pos, int Owner);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);
	void Consume();

private:
	vec2 m_From;
	int m_StartTick;
	int m_Owner;
	float m_Length;
};

#endif
