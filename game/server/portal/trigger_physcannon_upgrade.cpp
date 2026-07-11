#include "cbase.h"
#include "triggers.h"
#include "portal/weapon_physcannon.h"

class CTriggerPhyscannonUpgrade : public CTriggerMultiple
{
	DECLARE_CLASS( CTriggerPhyscannonUpgrade, CTriggerMultiple );
	DECLARE_DATADESC();

public:
	void Spawn();
	void Precache();
	void StartTouch( CBaseEntity *pOther );
	void UpgradeTouch( CBaseEntity *pOther );

private:
	float m_flUpgradeDelay;
};

LINK_ENTITY_TO_CLASS( trigger_physcannon_upgrade, CTriggerPhyscannonUpgrade );

BEGIN_DATADESC( CTriggerPhyscannonUpgrade )

	DEFINE_KEYFIELD( m_flUpgradeDelay, FIELD_FLOAT, "UpgradeDelay" ),
	DEFINE_FUNCTION( UpgradeTouch ),

END_DATADESC()

void CTriggerPhyscannonUpgrade::Spawn()
{
	BaseClass::Spawn();
	InitTrigger();
	SetTouch( &CTriggerPhyscannonUpgrade::UpgradeTouch );
}

void CTriggerPhyscannonUpgrade::Precache()
{
	BaseClass::Precache();
}

void CTriggerPhyscannonUpgrade::StartTouch( CBaseEntity *pOther )
{
	BaseClass::StartTouch( pOther );
}

void CTriggerPhyscannonUpgrade::UpgradeTouch( CBaseEntity *pOther )
{
	if ( !pOther || !pOther->IsPlayer() )
		return;

	CBasePlayer *pPlayer = ToBasePlayer( pOther );
	if ( !pPlayer )
		return;

	CWeaponPhysCannon *pWeapon = dynamic_cast<CWeaponPhysCannon *>( pPlayer->GetActiveWeapon() );
	if ( !pWeapon )
	{
		for ( int i = 0; i < pPlayer->WeaponCount(); i++ )
		{
			CWeaponPhysCannon *pCannon = dynamic_cast<CWeaponPhysCannon *>( pPlayer->GetWeapon( i ) );
			if ( pCannon )
			{
				pWeapon = pCannon;
				break;
			}
		}
	}

	if ( pWeapon )
	{
		pWeapon->BeginUpgrade();
	}
}
