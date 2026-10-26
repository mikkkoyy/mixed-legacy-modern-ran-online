#include "stdafx.h"
#include "AutoPilotSettings.h"

#include "BasicTextButton.h"
#include "BasicTextBox.h"
#include "InnerInterface.h"
#include "GLCharacter.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"

CAutoPilotSettings::CAutoPilotSettings()
	: m_pTabAttack(NULL)
	, m_pTabPotion(NULL)
	, m_pTabLoot(NULL)
	, m_pAttackLabel(NULL)
	, m_pAttackStatus(NULL)
	, m_pAttackStart(NULL)
	, m_pAttackStop(NULL)
	, m_pPotionLabel(NULL)
	, m_pPotionStatus(NULL)
	, m_pPotionStart(NULL)
	, m_pPotionStop(NULL)
	, m_pLootLabel(NULL)
	, m_pLootStatus(NULL)
	, m_pLootStart(NULL)
	, m_pLootStop(NULL)
	, m_pCloseBtn(NULL)
	, m_bSynced(FALSE)
	, m_bLastRunBot(false)
	, m_bLastRunAutoPots(false)
	, m_bLastRunAutoLoot(false)
	, m_nCurTab(TAB_ATTACK)
{
}

CAutoPilotSettings::~CAutoPilotSettings()
{
}

void CAutoPilotSettings::CreateSubControl()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_EX_FLAG );

	//------------------------------------------------------------------
	// Tab buttons (Attack | Potion | Loot) - radio flip for selected state
	//------------------------------------------------------------------
	const int nTabY = 31;
	const int nTabH = 18;
	const int nTabW = 78;
	const int nTabX[3] = { 6, 90, 174 };

	m_pTabAttack = new CBasicTextButton;
	m_pTabAttack->CreateSub ( this, "BASIC_TEXT_BUTTON191", UI_FLAG_DEFAULT, TAB_ATTACK );
	m_pTabAttack->CreateBaseButton ( "BASIC_TEXT_BUTTON191", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, "Attack" );
	m_pTabAttack->SetLocalPos ( UIRECT ( nTabX[0], nTabY, nTabW, nTabH ) );
	RegisterControl ( m_pTabAttack );

	m_pTabPotion = new CBasicTextButton;
	m_pTabPotion->CreateSub ( this, "BASIC_TEXT_BUTTON191", UI_FLAG_DEFAULT, TAB_POTION );
	m_pTabPotion->CreateBaseButton ( "BASIC_TEXT_BUTTON191", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, "Potion" );
	m_pTabPotion->SetLocalPos ( UIRECT ( nTabX[1], nTabY, nTabW, nTabH ) );
	RegisterControl ( m_pTabPotion );

	m_pTabLoot = new CBasicTextButton;
	m_pTabLoot->CreateSub ( this, "BASIC_TEXT_BUTTON191", UI_FLAG_DEFAULT, TAB_LOOT );
	m_pTabLoot->CreateBaseButton ( "BASIC_TEXT_BUTTON191", CBasicTextButton::SIZE18, CBasicButton::RADIO_FLIP, "Loot" );
	m_pTabLoot->SetLocalPos ( UIRECT ( nTabX[2], nTabY, nTabW, nTabH ) );
	RegisterControl ( m_pTabLoot );

	//------------------------------------------------------------------
	// Page content (all pages share the same slot area; only one visible)
	//------------------------------------------------------------------
	const int nPageY = 54;
	const int nStatusY = 72;
	const int nBtnY = 96;
	const int nBtnW = 52;
	const int nBtnH = 16;

	auto CreateLabel = [this, pFont, nPageY] ( CBasicTextBox*& pOut, UIGUID, const char* szText )
	{
		pOut = new CBasicTextBox;
		pOut->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
		pOut->SetFont ( pFont );
		pOut->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		pOut->SetOneLineText ( szText, NS_UITEXTCOLOR::WHITE );
		pOut->SetLocalPos ( UIRECT ( 10, nPageY, 150, 14 ) );
		RegisterControl ( pOut );
	};

	auto CreateStatus = [this, pFont, nStatusY] ( CBasicTextBox*& pOut )
	{
		pOut = new CBasicTextBox;
		pOut->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
		pOut->SetFont ( pFont );
		pOut->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		pOut->SetLocalPos ( UIRECT ( 10, nStatusY, 150, 14 ) );
		RegisterControl ( pOut );
	};

	auto CreateStartStop = [this, pFont, nBtnY, nBtnW, nBtnH]
		( CBasicTextButton*& pStart, CBasicTextButton*& pStop, UIGUID idStart, UIGUID idStop )
	{
		pStart = new CBasicTextButton;
		pStart->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, idStart );
		pStart->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "START" );
		pStart->SetLocalPos ( UIRECT ( 10, nBtnY, nBtnW, nBtnH ) );
		RegisterControl ( pStart );

		pStop = new CBasicTextButton;
		pStop->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, idStop );
		pStop->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "STOP" );
		pStop->SetLocalPos ( UIRECT ( 70, nBtnY, nBtnW, nBtnH ) );
		RegisterControl ( pStop );
	};

	// Attack page
	CreateLabel ( m_pAttackLabel, TAB_ATTACK, "Auto Attack" );
	CreateStatus ( m_pAttackStatus );
	CreateStartStop ( m_pAttackStart, m_pAttackStop, SETTINGS_AUTOATTACK_START, SETTINGS_AUTOATTACK_STOP );

	// Potion page
	CreateLabel ( m_pPotionLabel, TAB_POTION, "Auto Potion" );
	CreateStatus ( m_pPotionStatus );
	CreateStartStop ( m_pPotionStart, m_pPotionStop, SETTINGS_AUTOPOTS_START, SETTINGS_AUTOPOTS_STOP );

	// Loot page
	CreateLabel ( m_pLootLabel, TAB_LOOT, "Auto Loot" );
	CreateStatus ( m_pLootStatus );
	CreateStartStop ( m_pLootStart, m_pLootStop, SETTINGS_AUTOLOOT_START, SETTINGS_AUTOLOOT_STOP );

	//------------------------------------------------------------------
	// Close button
	//------------------------------------------------------------------
	m_pCloseBtn = new CBasicTextButton;
	m_pCloseBtn->CreateSub ( this, "BASIC_TEXT_BUTTON16", UI_FLAG_DEFAULT, SETTINGS_CLOSE );
	m_pCloseBtn->CreateBaseButton ( "BASIC_TEXT_BUTTON16", CBasicTextButton::SIZE16, CBasicButton::CLICK_FLIP, "Close" );
	m_pCloseBtn->SetLocalPos ( UIRECT ( 174, nBtnY, 60, nBtnH ) );
	RegisterControl ( m_pCloseBtn );

	// Show the default tab
	ChangePage ( TAB_ATTACK );
	SyncState ();
}

void CAutoPilotSettings::ChangePage( UIGUID cTabID )
{
	// Unflip every tab button
	if ( m_pTabAttack )	m_pTabAttack->SetFlip( FALSE );
	if ( m_pTabPotion )	m_pTabPotion->SetFlip( FALSE );
	if ( m_pTabLoot )	m_pTabLoot->SetFlip( FALSE );

	// Hide every page's controls
	if ( m_pAttackLabel )	m_pAttackLabel->SetVisibleSingle( FALSE );
	if ( m_pAttackStatus )	m_pAttackStatus->SetVisibleSingle( FALSE );
	if ( m_pAttackStart )	m_pAttackStart->SetVisibleSingle( FALSE );
	if ( m_pAttackStop )	m_pAttackStop->SetVisibleSingle( FALSE );

	if ( m_pPotionLabel )	m_pPotionLabel->SetVisibleSingle( FALSE );
	if ( m_pPotionStatus )	m_pPotionStatus->SetVisibleSingle( FALSE );
	if ( m_pPotionStart )	m_pPotionStart->SetVisibleSingle( FALSE );
	if ( m_pPotionStop )	m_pPotionStop->SetVisibleSingle( FALSE );

	if ( m_pLootLabel )		m_pLootLabel->SetVisibleSingle( FALSE );
	if ( m_pLootStatus )	m_pLootStatus->SetVisibleSingle( FALSE );
	if ( m_pLootStart )		m_pLootStart->SetVisibleSingle( FALSE );
	if ( m_pLootStop )		m_pLootStop->SetVisibleSingle( FALSE );

	CBasicTextButton* pVisibleButton = NULL;

	switch ( cTabID )
	{
	case TAB_ATTACK:
		pVisibleButton = m_pTabAttack;
		if ( m_pAttackLabel )	m_pAttackLabel->SetVisibleSingle( TRUE );
		if ( m_pAttackStatus )	m_pAttackStatus->SetVisibleSingle( TRUE );
		if ( m_pAttackStart )	m_pAttackStart->SetVisibleSingle( TRUE );
		if ( m_pAttackStop )	m_pAttackStop->SetVisibleSingle( TRUE );
		break;

	case TAB_POTION:
		pVisibleButton = m_pTabPotion;
		if ( m_pPotionLabel )	m_pPotionLabel->SetVisibleSingle( TRUE );
		if ( m_pPotionStatus )	m_pPotionStatus->SetVisibleSingle( TRUE );
		if ( m_pPotionStart )	m_pPotionStart->SetVisibleSingle( TRUE );
		if ( m_pPotionStop )	m_pPotionStop->SetVisibleSingle( TRUE );
		break;

	case TAB_LOOT:
		pVisibleButton = m_pTabLoot;
		if ( m_pLootLabel )		m_pLootLabel->SetVisibleSingle( TRUE );
		if ( m_pLootStatus )	m_pLootStatus->SetVisibleSingle( TRUE );
		if ( m_pLootStart )		m_pLootStart->SetVisibleSingle( TRUE );
		if ( m_pLootStop )		m_pLootStop->SetVisibleSingle( TRUE );
		break;
	}

	if ( pVisibleButton )	pVisibleButton->SetFlip( TRUE );

	m_nCurTab = cTabID;
}

void CAutoPilotSettings::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case TAB_ATTACK:
	case TAB_POTION:
	case TAB_LOOT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				ChangePage ( ControlID );
		}
		break;

	case SETTINGS_AUTOATTACK_START:
	case SETTINGS_AUTOATTACK_STOP:
	case SETTINGS_AUTOPOTS_START:
	case SETTINGS_AUTOPOTS_STOP:
	case SETTINGS_AUTOLOOT_START:
	case SETTINGS_AUTOLOOT_STOP:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
				if ( pChar )
				{
					switch ( ControlID )
					{
					case SETTINGS_AUTOATTACK_START:	pChar->SetRunBot ( true );			break;
					case SETTINGS_AUTOATTACK_STOP:	pChar->SetRunBot ( false );			break;
					case SETTINGS_AUTOPOTS_START:	pChar->SetRunAutoPots ( true );		break;
					case SETTINGS_AUTOPOTS_STOP:	pChar->SetRunAutoPots ( false );		break;
					case SETTINGS_AUTOLOOT_START:	pChar->SetRunAutoLoot ( true );		break;
					case SETTINGS_AUTOLOOT_STOP:	pChar->SetRunAutoLoot ( false );		break;
					}

					SyncState ();
				}
			}
		}
		break;

	case SETTINGS_CLOSE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( GetWndID() );
			}
		}
		break;
	}
}

void CAutoPilotSettings::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if ( bVisible )
	{
		m_bSynced = FALSE;
		ChangePage ( m_nCurTab );
		SyncState ();
	}
}

HRESULT CAutoPilotSettings::FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime )
{
	if ( IsVisible() )	SyncState();

	return CUIWindowEx::FrameMove ( pd3dDevice, fTime, fElapsedTime );
}

void CAutoPilotSettings::SyncState()
{
	GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pChar )	return;

	const bool bRunBot = pChar->IsRunBot();
	const bool bRunAutoPots = pChar->IsRunAutoPots();
	const bool bRunAutoLoot = pChar->IsRunAutoLoot();

	if ( m_bSynced && m_bLastRunBot == bRunBot && m_bLastRunAutoPots == bRunAutoPots && m_bLastRunAutoLoot == bRunAutoLoot )	return;

	m_bLastRunBot = bRunBot;
	m_bLastRunAutoPots = bRunAutoPots;
	m_bLastRunAutoLoot = bRunAutoLoot;
	m_bSynced = TRUE;

	auto SetStatus = [this] ( CBasicTextBox* pStatus, bool bOn )
	{
		if ( pStatus )
			pStatus->SetOneLineText ( bOn ? "Status: ON" : "Status: OFF", bOn ? NS_UITEXTCOLOR::BRIGHTGREEN : NS_UITEXTCOLOR::DARKGRAY );
	};

	SetStatus ( m_pAttackStatus, bRunBot );
	SetStatus ( m_pPotionStatus, bRunAutoPots );
	SetStatus ( m_pLootStatus, bRunAutoLoot );
}
