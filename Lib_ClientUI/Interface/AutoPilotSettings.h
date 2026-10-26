#pragma	once

#include "UIWindowEx.h"

class CBasicTextButton;
class CBasicTextBox;

class CAutoPilotSettings : public CUIWindowEx
{
public:
	CAutoPilotSettings();
	virtual	~CAutoPilotSettings();

public:
	void	CreateSubControl();
	void	SyncState();
	void	ChangePage( UIGUID cTabID );

public:
	virtual void	TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual void	SetVisibleSingle( BOOL bVisible );
	virtual HRESULT	FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );

protected:
	enum
	{
		TAB_ATTACK = ET_CONTROL_NEXT,
		TAB_POTION,
		TAB_LOOT,

		SETTINGS_AUTOATTACK_START,
		SETTINGS_AUTOATTACK_STOP,
		SETTINGS_AUTOPOTS_START,
		SETTINGS_AUTOPOTS_STOP,
		SETTINGS_AUTOLOOT_START,
		SETTINGS_AUTOLOOT_STOP,
		SETTINGS_CLOSE
	};

private:
	// tab buttons
	CBasicTextButton*	m_pTabAttack;
	CBasicTextButton*	m_pTabPotion;
	CBasicTextButton*	m_pTabLoot;

	// attack page
	CBasicTextBox*		m_pAttackLabel;
	CBasicTextBox*		m_pAttackStatus;
	CBasicTextButton*	m_pAttackStart;
	CBasicTextButton*	m_pAttackStop;

	// potion page
	CBasicTextBox*		m_pPotionLabel;
	CBasicTextBox*		m_pPotionStatus;
	CBasicTextButton*	m_pPotionStart;
	CBasicTextButton*	m_pPotionStop;

	// loot page
	CBasicTextBox*		m_pLootLabel;
	CBasicTextBox*		m_pLootStatus;
	CBasicTextButton*	m_pLootStart;
	CBasicTextButton*	m_pLootStop;

	CBasicTextButton*	m_pCloseBtn;

	BOOL				m_bSynced;
	bool				m_bLastRunBot;
	bool				m_bLastRunAutoPots;
	bool				m_bLastRunAutoLoot;
	UIGUID				m_nCurTab;
};
