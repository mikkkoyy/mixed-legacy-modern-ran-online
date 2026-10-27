#pragma	once

#include "UIWindowEx.h"

class CBasicTextButton;
class CBasicTextBox;
class CUIEditBox;

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
		SETTINGS_CLOSE,

		SETTINGS_ATTACK_SUPPORT,
		SETTINGS_ATTACK_PK,
		SETTINGS_ATTACK_FOLLOWLEADER,
		SETTINGS_ATTACK_LOCKSPOT,
		SETTINGS_ATTACK_RESURRECTLEADER,
		SETTINGS_POTION_HP_EDIT,
		SETTINGS_POTION_MP_EDIT,
		SETTINGS_POTION_SP_EDIT
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
	CBasicTextButton*	m_pAttackSupport;
	CBasicTextButton*	m_pAttackPK;
	CBasicTextButton*	m_pAttackFollowLeader;
	CBasicTextButton*	m_pAttackLockSpot;
	CBasicTextButton*	m_pAttackResurrectPtLeader;

	// potion page
	CBasicTextBox*		m_pPotionLabel;
	CBasicTextBox*		m_pPotionStatus;
	CBasicTextButton*	m_pPotionStart;
	CBasicTextButton*	m_pPotionStop;
	CBasicTextBox*		m_pPotionHPLabel;
	CBasicTextBox*		m_pPotionMPLabel;
	CBasicTextBox*		m_pPotionSPLabel;
	CUIEditBox*			m_pPotionHPEdit;
	CUIEditBox*			m_pPotionMPEdit;
	CUIEditBox*			m_pPotionSPEdit;

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
	bool				m_bLastBotSupport;
	bool				m_bLastBotPK;
	bool				m_bLastBotFollowLeader;
	bool				m_bLastBotLockSpot;
	bool				m_bLastBotResuPtLeader;
	int					m_nLastAutoPotionHP;
	int					m_nLastAutoPotionMP;
	int					m_nLastAutoPotionSP;
	UIGUID				m_nCurTab;
};
