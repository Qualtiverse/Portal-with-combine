#include "cbase.h"
#include "point_combine_spawner.h"
#include "ai_basenpc.h"

LINK_ENTITY_TO_CLASS( point_combine_spawner, CPointCombineSpawner );

BEGIN_DATADESC( CPointCombineSpawner )
	DEFINE_KEYFIELD( m_bEnabled, FIELD_BOOLEAN, "StartEnabled" ),
	DEFINE_KEYFIELD( m_bInfinitelyRespawn, FIELD_BOOLEAN, "InfinitelyRespawn" ),
	DEFINE_KEYFIELD( m_iSpawnType, FIELD_INTEGER, "SpawnType" ),
	DEFINE_KEYFIELD( m_iMaxActiveNPCs, FIELD_INTEGER, "MaxActiveNPCs" ),
	DEFINE_KEYFIELD( m_flSpawnInterval, FIELD_FLOAT, "SpawnInterval" ),
	DEFINE_KEYFIELD( m_iSquadSize, FIELD_INTEGER, "SquadSize" ),
	DEFINE_KEYFIELD( m_flSpawnRadius, FIELD_FLOAT, "SpawnRadius" ),
	DEFINE_KEYFIELD( m_iTotalToSpawn, FIELD_INTEGER, "TotalToSpawn" ),
	DEFINE_KEYFIELD( m_iszSpawnType, FIELD_STRING, "SpawnTypeName" ),

	DEFINE_FIELD( m_iSpawnedCount, FIELD_INTEGER ),
	DEFINE_FIELD( m_iWaveCount, FIELD_INTEGER ),

	DEFINE_INPUTFUNC( FIELD_VOID, "StartSpawn", InputStartSpawn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "StopSpawn", InputStopSpawn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SpawnNow", InputSpawnNow ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetSpawnType", InputSetSpawnType ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),

	DEFINE_OUTPUT( m_OnNPCSpawned, "OnNPCSpawned" ),
	DEFINE_OUTPUT( m_OnWaveComplete, "OnWaveComplete" ),
	DEFINE_OUTPUT( m_OnAllSpawned, "OnAllSpawned" ),
	DEFINE_OUTPUT( m_OnAllDead, "OnAllDead" ),

	DEFINE_THINKFUNC( SpawnThink ),
END_DATADESC()

CPointCombineSpawner::CPointCombineSpawner()
{
	m_bEnabled = true;
	m_bInfinitelyRespawn = true;
	m_iSpawnType = COMBINE_SPAWN_SOLDIER;
	m_iMaxActiveNPCs = 6;
	m_flSpawnInterval = 5.0f;
	m_iSquadSize = 2;
	m_flSpawnRadius = 256.0f;
	m_iTotalToSpawn = 10;
	m_iSpawnedCount = 0;
	m_iWaveCount = 0;
}

void CPointCombineSpawner::Spawn()
{
	Precache();
	SetSolid( SOLID_NONE );
	SetMoveType( MOVETYPE_NONE );

	if ( m_bEnabled )
	{
		SetThink( &CPointCombineSpawner::SpawnThink );
		SetNextThink( gpGlobals->curtime + m_flSpawnInterval );
	}
}

void CPointCombineSpawner::Precache()
{
	PrecacheModel( "models/combine_soldier.mdl" );
	PrecacheModel( "models/manhack.mdl" );
	PrecacheModel( "models/combine_scanner.mdl" );
	PrecacheModel( "models/police.mdl" );
	PrecacheModel( "models/combine_strider.mdl" );
}

int CPointCombineSpawner::GetAliveNPCCount()
{
	int count = 0;
	const char *classes[] = {
		"npc_combine", "npc_combine_s", "npc_manhack",
		"npc_combine_scanner", "npc_metropolice", "npc_strider", NULL
	};
	for ( int i = 0; classes[i] != NULL; i++ )
	{
		CBaseEntity *pEnt = NULL;
		while ( (pEnt = gEntList.FindEntityByClassname( pEnt, classes[i] )) != NULL )
		{
			CAI_BaseNPC *pNPC = dynamic_cast<CAI_BaseNPC *>( pEnt );
			if ( pNPC && pNPC->IsAlive() )
				count++;
		}
	}
	return count;
}

const char *CPointCombineSpawner::GetNPCClassnameForType( CombineSpawnType_t type )
{
	switch ( type )
	{
		case COMBINE_SPAWN_SOLDIER:
		case COMBINE_SPAWN_SHOTGUN_SOLDIER:
		case COMBINE_SPAWN_ELITE:
			return "npc_combine";
		case COMBINE_SPAWN_MANHACK:
			return "npc_manhack";
		case COMBINE_SPAWN_SCANNER:
			return "npc_combine_scanner";
		case COMBINE_SPAWN_METROPOLICE:
			return "npc_metropolice";
		case COMBINE_SPAWN_STRIDER:
			return "npc_strider";
		default:
			return "npc_combine";
	}
}

const char *CPointCombineSpawner::GetNPCWeaponForType( CombineSpawnType_t type )
{
	switch ( type )
	{
		case COMBINE_SPAWN_SOLDIER:
			return "weapon_smg1";
		case COMBINE_SPAWN_SHOTGUN_SOLDIER:
			return "weapon_shotgun";
		case COMBINE_SPAWN_ELITE:
			return "weapon_ar2";
		case COMBINE_SPAWN_METROPOLICE:
			return "weapon_pistol";
		default:
			return "weapon_smg1";
	}
}

int CPointCombineSpawner::SpawnNPC()
{
	if ( GetAliveNPCCount() >= m_iMaxActiveNPCs )
		return 0;

	CombineSpawnType_t spawnType = (CombineSpawnType_t)m_iSpawnType;
	if ( m_iszSpawnType != NULL_STRING )
	{
		const char *szType = STRING( m_iszSpawnType );
		if ( FStrEq( szType, "soldier" ) ) spawnType = COMBINE_SPAWN_SOLDIER;
		else if ( FStrEq( szType, "shotgun_soldier" ) ) spawnType = COMBINE_SPAWN_SHOTGUN_SOLDIER;
		else if ( FStrEq( szType, "elite" ) ) spawnType = COMBINE_SPAWN_ELITE;
		else if ( FStrEq( szType, "manhack" ) ) spawnType = COMBINE_SPAWN_MANHACK;
		else if ( FStrEq( szType, "scanner" ) ) spawnType = COMBINE_SPAWN_SCANNER;
		else if ( FStrEq( szType, "metropolice" ) ) spawnType = COMBINE_SPAWN_METROPOLICE;
		else if ( FStrEq( szType, "strider" ) ) spawnType = COMBINE_SPAWN_STRIDER;
	}

	const char *szClassname = GetNPCClassnameForType( spawnType );
	CBaseEntity *pEnt = CreateEntityByName( szClassname );
	if ( !pEnt )
		return 0;

	CAI_BaseNPC *pNPC = dynamic_cast<CAI_BaseNPC *>( pEnt );
	if ( !pNPC )
	{
		UTIL_Remove( pEnt );
		return 0;
	}

	Vector spawnPos = GetAbsOrigin();
	if ( m_flSpawnRadius > 0 )
	{
		spawnPos.x += random->RandomFloat( -m_flSpawnRadius, m_flSpawnRadius );
		spawnPos.y += random->RandomFloat( -m_flSpawnRadius, m_flSpawnRadius );
	}

	pNPC->SetAbsOrigin( spawnPos );
	pNPC->SetAbsAngles( GetAbsAngles() );
	pNPC->AddSpawnFlags( SF_NPC_FALL_TO_GROUND );

	const char *weapon = GetNPCWeaponForType( spawnType );
	if ( weapon && weapon[0] )
	{
		pNPC->m_spawnEquipment = AllocPooledString( weapon );
	}

	DispatchSpawn( pNPC );
	pNPC->Activate();

	if ( pNPC->VPhysicsGetObject() )
	{
		pNPC->VPhysicsGetObject()->EnableMotion( false );
		pNPC->VPhysicsGetObject()->EnableMotion( true );
	}

	if ( pNPC->GetActiveWeapon() )
	{
		pNPC->GetActiveWeapon()->SetClip1( 999 );
	}

	m_iSpawnedCount++;
	m_OnNPCSpawned.FireOutput( pNPC, this );

	return 1;
}

void CPointCombineSpawner::SpawnThink()
{
	if ( !m_bEnabled )
		return;

	if ( !m_bInfinitelyRespawn && m_iSpawnedCount >= m_iTotalToSpawn )
	{
		m_OnAllSpawned.FireOutput( this, this );
		SetThink( NULL );
		return;
	}

	int spawned = 0;
	for ( int i = 0; i < m_iSquadSize; i++ )
	{
		spawned += SpawnNPC();
	}

	if ( spawned > 0 )
	{
		m_iWaveCount++;
		m_OnWaveComplete.FireOutput( this, this );
	}

	if ( m_bInfinitelyRespawn || m_iSpawnedCount < m_iTotalToSpawn )
	{
		SetNextThink( gpGlobals->curtime + m_flSpawnInterval );
	}
}

void CPointCombineSpawner::InputStartSpawn( inputdata_t &inputdata )
{
	m_bEnabled = true;
	SetThink( &CPointCombineSpawner::SpawnThink );
	SetNextThink( gpGlobals->curtime + m_flSpawnInterval );
}

void CPointCombineSpawner::InputStopSpawn( inputdata_t &inputdata )
{
	m_bEnabled = false;
	SetThink( NULL );
}

void CPointCombineSpawner::InputSpawnNow( inputdata_t &inputdata )
{
	int spawned = 0;
	for ( int i = 0; i < m_iSquadSize; i++ )
	{
		spawned += SpawnNPC();
	}
	if ( spawned > 0 )
	{
		m_iWaveCount++;
		m_OnWaveComplete.FireOutput( this, this );
	}
}

void CPointCombineSpawner::InputSetSpawnType( inputdata_t &inputdata )
{
	m_iSpawnType = inputdata.value.Int();
}

void CPointCombineSpawner::InputToggle( inputdata_t &inputdata )
{
	if ( m_bEnabled )
	{
		InputStopSpawn( inputdata );
	}
	else
	{
		InputStartSpawn( inputdata );
	}
}
