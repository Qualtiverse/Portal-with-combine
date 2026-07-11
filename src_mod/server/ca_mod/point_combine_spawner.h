#ifndef POINT_COMBINE_SPAWNER_H
#define POINT_COMBINE_SPAWNER_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "entityinput.h"
#include "entityoutput.h"

enum CombineSpawnType_t
{
	COMBINE_SPAWN_SOLDIER = 0,
	COMBINE_SPAWN_SHOTGUN_SOLDIER,
	COMBINE_SPAWN_ELITE,
	COMBINE_SPAWN_MANHACK,
	COMBINE_SPAWN_SCANNER,
	COMBINE_SPAWN_METROPOLICE,
	COMBINE_SPAWN_STRIDER,
	COMBINE_SPAWN_TYPE_MAX,
};

class CPointCombineSpawner : public CBaseEntity
{
	DECLARE_CLASS( CPointCombineSpawner, CBaseEntity );
	DECLARE_DATADESC();

public:
	CPointCombineSpawner();

	virtual void Spawn();
	virtual void Precache();

	void SpawnThink();

	void InputStartSpawn( inputdata_t &inputdata );
	void InputStopSpawn( inputdata_t &inputdata );
	void InputSpawnNow( inputdata_t &inputdata );
	void InputSetSpawnType( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );

	int SpawnNPC();
	const char *GetNPCClassnameForType( CombineSpawnType_t type );
	const char *GetNPCWeaponForType( CombineSpawnType_t type );

	COutputEvent m_OnNPCSpawned;
	COutputEvent m_OnWaveComplete;
	COutputEvent m_OnAllSpawned;
	COutputEvent m_OnAllDead;

	bool m_bEnabled;
	bool m_bInfinitelyRespawn;
	int m_iSpawnType;
	int m_iMaxActiveNPCs;
	float m_flSpawnInterval;
	int m_iSquadSize;
	float m_flSpawnRadius;
	int m_iTotalToSpawn;
	int m_iSpawnedCount;
	int m_iWaveCount;
	string_t m_iszSpawnType;

private:
	int GetAliveNPCCount();
};

#endif
