#ifndef PROP_FLOOR_BUTTON_H
#define PROP_FLOOR_BUTTON_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "baseentity.h"

class CPropFloorButton : public CBaseAnimating
{
	DECLARE_CLASS( CPropFloorButton, CBaseAnimating );
	DECLARE_DATADESC();

public:
	CPropFloorButton();

	void Spawn();
	void Precache();
	void Activate();

	void PressThink();

	void InputPressIn(inputdata_t &inputdata);
	void InputPressOut(inputdata_t &inputdata);
	void InputLock(inputdata_t &inputdata);
	void InputUnlock(inputdata_t &inputdata);
	void InputCancelPress(inputdata_t &inputdata);

	bool IsPressed() { return m_bPressed; }
	bool IsLocked() { return m_bLocked; }

private:
	bool	m_bPressed;
	bool	m_bLocked;
	float	m_flDelay;
	float	m_flPressTime;

	Vector	m_vecPressedPos;
	Vector	m_vecUnpressedPos;

	void SetPressedState(bool bPressed);

	COutputEvent m_OnPressed;
	COutputEvent m_OnUnPressed;
	COutputEvent m_OnButtonReset;
};

#endif
