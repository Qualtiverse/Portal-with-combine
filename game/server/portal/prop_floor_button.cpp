#include "cbase.h"
#include "prop_floor_button.h"
#include "portal_player.h"
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( prop_floor_button, CPropFloorButton );

BEGIN_DATADESC( CPropFloorButton )
	DEFINE_KEYFIELD( m_flDelay, FIELD_FLOAT, "Delay" ),
	DEFINE_FIELD( m_bPressed, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bLocked, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flPressTime, FIELD_TIME ),
	DEFINE_FIELD( m_vecPressedPos, FIELD_VECTOR ),
	DEFINE_FIELD( m_vecUnpressedPos, FIELD_VECTOR ),
	DEFINE_THINKFUNC( PressThink ),
	DEFINE_INPUTFUNC( FIELD_VOID, "PressIn", InputPressIn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "PressOut", InputPressOut ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Lock", InputLock ),
	DEFINE_INPUTFUNC( FIELD_VOID, "UnLock", InputUnlock ),
	DEFINE_INPUTFUNC( FIELD_VOID, "CancelPress", InputCancelPress ),
	DEFINE_OUTPUT( m_OnPressed, "OnPressed" ),
	DEFINE_OUTPUT( m_OnUnPressed, "OnUnPressed" ),
	DEFINE_OUTPUT( m_OnButtonReset, "OnButtonReset" ),
END_DATADESC()

CPropFloorButton::CPropFloorButton()
{
	m_bPressed = false;
	m_bLocked = false;
	m_flDelay = 1.0f;
	m_flPressTime = 0;
}

void CPropFloorButton::Spawn()
{
	Precache();
	SetModel( "models/props/button_top_reference.mdl" );
	SetSolid( SOLID_VPHYSICS );
	VPhysicsInitNormal( SOLID_VPHYSICS, 0, false );
	SetMoveType( MOVETYPE_NONE );
	m_vecUnpressedPos = GetAbsOrigin();
	m_vecPressedPos = m_vecUnpressedPos - Vector( 0, 0, 4 );
	SetThink( &CPropFloorButton::PressThink );
	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CPropFloorButton::Precache()
{
	PrecacheModel( "models/props/button_top_reference.mdl" );
}

void CPropFloorButton::Activate()
{
	BaseClass::Activate();
}

void CPropFloorButton::PressThink()
{
	if ( m_bLocked )
	{
		SetNextThink( gpGlobals->curtime + 0.1f );
		return;
	}

	CBaseEntity *pActivator = NULL;
	float flTouchHeight = 0.0f;
	Vector vecOrigin = GetAbsOrigin();
	Vector vecMins = WorldAlignMins();
	Vector vecMaxs = WorldAlignMaxs();
	vecMins.z += ( vecMaxs.z - vecMins.z ) * 0.8f;

	CBaseEntity *pList[64];
	int nCount = UTIL_EntitiesInBox( pList, 64, vecOrigin + vecMins, vecOrigin + vecMaxs, FL_GRENADE | FL_CLIENT | FL_NPC );
	for ( int i = 0; i < nCount; i++ )
	{
		CBaseEntity *pEntity = pList[i];
		if ( pEntity->IsPlayer() || pEntity->IsNPC() )
		{
			pActivator = pEntity;
			break;
		}
	}

	if ( pActivator && !m_bPressed )
	{
		SetPressedState( true );
		m_OnPressed.FireOutput( pActivator, this );
		m_flPressTime = gpGlobals->curtime;
	}
	else if ( !pActivator && m_bPressed )
	{
		if ( ( gpGlobals->curtime - m_flPressTime ) >= m_flDelay )
		{
			SetPressedState( false );
			m_OnUnPressed.FireOutput( pActivator, this );
			m_OnButtonReset.FireOutput( pActivator, this );
		}
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CPropFloorButton::SetPressedState( bool bPressed )
{
	m_bPressed = bPressed;
	if ( bPressed )
	{
		SetAbsOrigin( m_vecPressedPos );
	}
	else
	{
		SetAbsOrigin( m_vecUnpressedPos );
	}
}

void CPropFloorButton::InputPressIn( inputdata_t &inputdata )
{
	if ( !m_bPressed )
	{
		SetPressedState( true );
		m_OnPressed.FireOutput( inputdata.pActivator, this );
	}
}

void CPropFloorButton::InputPressOut( inputdata_t &inputdata )
{
	if ( m_bPressed )
	{
		SetPressedState( false );
		m_OnUnPressed.FireOutput( inputdata.pActivator, this );
	}
}

void CPropFloorButton::InputLock( inputdata_t &inputdata )
{
	m_bLocked = true;
}

void CPropFloorButton::InputUnlock( inputdata_t &inputdata )
{
	m_bLocked = false;
}

void CPropFloorButton::InputCancelPress( inputdata_t &inputdata )
{
	m_bPressed = false;
	m_flPressTime = 0;
	SetPressedState( false );
}
