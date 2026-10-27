#include "stdafx.h"
#include "AutoPilotSettings.h"

#include "BasicTextButton.h"
#include "BasicTextBox.h"
#include "InnerInterface.h"
#include "GLCharacter.h"
#include "GLGaeaClient.h"
#include "UIEditBox.h"
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
	, m_pAttackSupport(NULL)
	, m_pAttackPK(NULL)
	, m_pAttackFollowLeader(NULL)
	, m_pAttackLockSpot(NULL)
	, m_pAttackResurrectPtLeader(NULL)
	, m_pPotionLabel(NULL)
	, m_pPotionStatus(NULL)
	, m_pPotionStart(NULL)
	, m_pPotionStop(NULL)
	, m_pPotionHPLabel(NULL)
	, m_pPotionMPLabel(NULL)
	, m_pPotionSPLabel(NULL)
	, m_pPotionHPEdit(NULL)
	, m_pPotionMPEdit(NULL)
	, m_pPotionSPEdit(NULL)
	, m_pLootLabel(NULL)
	, m_pLootStatus(NULL)
	, m_pLootStart(NULL)
	, m_pLootStop(NULL)
	, m_pCloseBtn(NULL)
	, m_bSynced(FALSE)
	, m_bLastRunBot(false)
	, m_bLastRunAutoPots(false)
	, m_bLastRunAutoLoot(false)
	, m_bLastBotSupport(false)
	, m_bLastBotPK(false)
	, m_bLastBotFollowLeader(false)
	, m_bLastBotLockSpot(false)
	, m_bLastBotResuPtLeader(false)
	, m_nLastAutoPotionHP(80)
	, m_nLastAutoPotionMP(50)
	, m_nLastAutoPotionSP(50)
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
	// Page content (each tab positioned within its own area)
	//------------------------------------------------------------------

	// Attack page
	m_pAttackLabel = new CBasicTextBox;
	m_pAttackLabel->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pAttackLabel->SetFont ( pFont );
	m_pAttackLabel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pAttackLabel->SetOneLineText ( "Auto Attack", NS_UITEXTCOLOR::WHITE );
	m_pAttackLabel->SetLocalPos ( UIRECT ( 10, 54, 150, 14 ) );
	RegisterControl ( m_pAttackLabel );

	m_pAttackStatus = new CBasicTextBox;
	m_pAttackStatus->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pAttackStatus->SetFont ( pFont );
	m_pAttackStatus->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pAttackStatus->SetLocalPos ( UIRECT ( 10, 70, 150, 14 ) );
	RegisterControl ( m_pAttackStatus );

	m_pAttackStart = new CBasicTextButton;
	m_pAttackStart->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_AUTOATTACK_START );
	m_pAttackStart->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "START" );
	m_pAttackStart->SetLocalPos ( UIRECT ( 130, 118, 52, 16 ) );
	RegisterControl ( m_pAttackStart );

	m_pAttackStop = new CBasicTextButton;
	m_pAttackStop->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_AUTOATTACK_STOP );
	m_pAttackStop->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "STOP" );
	m_pAttackStop->SetLocalPos ( UIRECT ( 190, 118, 52, 16 ) );
	RegisterControl ( m_pAttackStop );

	// Attack configuration checkboxes (CLICK_FLIP = checkbox style)
	m_pAttackSupport = new CBasicTextButton;
	m_pAttackSupport->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_ATTACK_SUPPORT );
	m_pAttackSupport->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "Support" );
	m_pAttackSupport->SetLocalPos ( UIRECT ( 10, 86, 100, 16 ) );
	RegisterControl ( m_pAttackSupport );

	m_pAttackLockSpot = new CBasicTextButton;
	m_pAttackLockSpot->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_ATTACK_LOCKSPOT );
	m_pAttackLockSpot->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "Lock Spot" );
	m_pAttackLockSpot->SetLocalPos ( UIRECT ( 120, 86, 100, 16 ) );
	RegisterControl ( m_pAttackLockSpot );

	m_pAttackPK = new CBasicTextButton;
	m_pAttackPK->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_ATTACK_PK );
	m_pAttackPK->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "PK" );
	m_pAttackPK->SetLocalPos ( UIRECT ( 10, 102, 100, 16 ) );
	RegisterControl ( m_pAttackPK );

	m_pAttackFollowLeader = new CBasicTextButton;
	m_pAttackFollowLeader->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_ATTACK_FOLLOWLEADER );
	m_pAttackFollowLeader->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "Follow Leader" );
	m_pAttackFollowLeader->SetLocalPos ( UIRECT ( 120, 102, 100, 16 ) );
	RegisterControl ( m_pAttackFollowLeader );

	m_pAttackResurrectPtLeader = new CBasicTextButton;
	m_pAttackResurrectPtLeader->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_ATTACK_RESURRECTLEADER );
	m_pAttackResurrectPtLeader->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "Resurrect Leader" );
	m_pAttackResurrectPtLeader->SetLocalPos ( UIRECT ( 10, 118, 100, 16 ) );
	RegisterControl ( m_pAttackResurrectPtLeader );

	// Potion page
	m_pPotionLabel = new CBasicTextBox;
	m_pPotionLabel->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pPotionLabel->SetFont ( pFont );
	m_pPotionLabel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPotionLabel->SetOneLineText ( "Auto Potion", NS_UITEXTCOLOR::WHITE );
	m_pPotionLabel->SetLocalPos ( UIRECT ( 10, 54, 150, 14 ) );
	RegisterControl ( m_pPotionLabel );

	m_pPotionStatus = new CBasicTextBox;
	m_pPotionStatus->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pPotionStatus->SetFont ( pFont );
	m_pPotionStatus->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPotionStatus->SetLocalPos ( UIRECT ( 10, 70, 150, 14 ) );
	RegisterControl ( m_pPotionStatus );

	m_pPotionStart = new CBasicTextButton;
	m_pPotionStart->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_AUTOPOTS_START );
	m_pPotionStart->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "START" );
	m_pPotionStart->SetLocalPos ( UIRECT ( 10, 96, 52, 16 ) );
	RegisterControl ( m_pPotionStart );

	m_pPotionStop = new CBasicTextButton;
	m_pPotionStop->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_AUTOPOTS_STOP );
	m_pPotionStop->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "STOP" );
	m_pPotionStop->SetLocalPos ( UIRECT ( 70, 96, 52, 16 ) );
	RegisterControl ( m_pPotionStop );

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	m_pPotionHPLabel = new CBasicTextBox;
	m_pPotionHPLabel->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pPotionHPLabel->SetFont ( pFont );
	m_pPotionHPLabel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPotionHPLabel->SetOneLineText ( "HP %", NS_UITEXTCOLOR::WHITE );
	m_pPotionHPLabel->SetLocalPos ( UIRECT ( 10, 88, 40, 14 ) );
	RegisterControl ( m_pPotionHPLabel );

	m_pPotionMPLabel = new CBasicTextBox;
	m_pPotionMPLabel->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pPotionMPLabel->SetFont ( pFont );
	m_pPotionMPLabel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPotionMPLabel->SetOneLineText ( "MP %", NS_UITEXTCOLOR::WHITE );
	m_pPotionMPLabel->SetLocalPos ( UIRECT ( 90, 88, 40, 14 ) );
	RegisterControl ( m_pPotionMPLabel );

	m_pPotionSPLabel = new CBasicTextBox;
	m_pPotionSPLabel->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pPotionSPLabel->SetFont ( pFont );
	m_pPotionSPLabel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pPotionSPLabel->SetOneLineText ( "SP %", NS_UITEXTCOLOR::WHITE );
	m_pPotionSPLabel->SetLocalPos ( UIRECT ( 170, 88, 40, 14 ) );
	RegisterControl ( m_pPotionSPLabel );

	m_pPotionHPEdit = new CUIEditBox;
	m_pPotionHPEdit->CreateSub( this, "BASIC_EDIT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE, SETTINGS_POTION_HP_EDIT );
	m_pPotionHPEdit->CreateCarrat( "MODAL_CARRAT", TRUE, UINT_MAX );
	m_pPotionHPEdit->SetLimitInput( 2 );
	m_pPotionHPEdit->SetFont( pFont9 );
	m_pPotionHPEdit->DoUSE_NUMBER( true );
	m_pPotionHPEdit->SetLocalPos( D3DXVECTOR2( 48, 88 ) );
	m_pPotionHPEdit->SetGlobalPos( D3DXVECTOR2( 48, 88 ) );
	RegisterControl( m_pPotionHPEdit );

	m_pPotionMPEdit = new CUIEditBox;
	m_pPotionMPEdit->CreateSub( this, "BASIC_EDIT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE, SETTINGS_POTION_MP_EDIT );
	m_pPotionMPEdit->CreateCarrat( "MODAL_CARRAT", TRUE, UINT_MAX );
	m_pPotionMPEdit->SetLimitInput( 2 );
	m_pPotionMPEdit->SetFont( pFont9 );
	m_pPotionMPEdit->DoUSE_NUMBER( true );
	m_pPotionMPEdit->SetLocalPos( D3DXVECTOR2( 128, 88 ) );
	m_pPotionMPEdit->SetGlobalPos( D3DXVECTOR2( 128, 88 ) );
	RegisterControl( m_pPotionMPEdit );

	m_pPotionSPEdit = new CUIEditBox;
	m_pPotionSPEdit->CreateSub( this, "BASIC_EDIT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE, SETTINGS_POTION_SP_EDIT );
	m_pPotionSPEdit->CreateCarrat( "MODAL_CARRAT", TRUE, UINT_MAX );
	m_pPotionSPEdit->SetLimitInput( 2 );
	m_pPotionSPEdit->SetFont( pFont9 );
	m_pPotionSPEdit->DoUSE_NUMBER( true );
	m_pPotionSPEdit->SetLocalPos( D3DXVECTOR2( 208, 88 ) );
	m_pPotionSPEdit->SetGlobalPos( D3DXVECTOR2( 208, 88 ) );
	RegisterControl( m_pPotionSPEdit );

	// Loot page
	m_pLootLabel = new CBasicTextBox;
	m_pLootLabel->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pLootLabel->SetFont ( pFont );
	m_pLootLabel->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pLootLabel->SetOneLineText ( "Auto Loot", NS_UITEXTCOLOR::WHITE );
	m_pLootLabel->SetLocalPos ( UIRECT ( 10, 54, 150, 14 ) );
	RegisterControl ( m_pLootLabel );

	m_pLootStatus = new CBasicTextBox;
	m_pLootStatus->CreateSub ( this, "BASIC_WINDOW_TEXTBOX" );
	m_pLootStatus->SetFont ( pFont );
	m_pLootStatus->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pLootStatus->SetLocalPos ( UIRECT ( 10, 70, 150, 14 ) );
	RegisterControl ( m_pLootStatus );

	m_pLootStart = new CBasicTextButton;
	m_pLootStart->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_AUTOLOOT_START );
	m_pLootStart->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "START" );
	m_pLootStart->SetLocalPos ( UIRECT ( 10, 96, 52, 16 ) );
	RegisterControl ( m_pLootStart );

	m_pLootStop = new CBasicTextButton;
	m_pLootStop->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_DEFAULT, SETTINGS_AUTOLOOT_STOP );
	m_pLootStop->CreateBaseButton ( "BASIC_TEXT_BUTTON18", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "STOP" );
	m_pLootStop->SetLocalPos ( UIRECT ( 70, 96, 52, 16 ) );
	RegisterControl ( m_pLootStop );

	//------------------------------------------------------------------
	// Close button (always visible)
	//------------------------------------------------------------------
	m_pCloseBtn = new CBasicTextButton;
	m_pCloseBtn->CreateSub ( this, "BASIC_TEXT_BUTTON16", UI_FLAG_DEFAULT, SETTINGS_CLOSE );
	m_pCloseBtn->CreateBaseButton ( "BASIC_TEXT_BUTTON16", CBasicTextButton::SIZE16, CBasicButton::CLICK_FLIP, "Close" );
	m_pCloseBtn->SetLocalPos ( UIRECT ( 190, 134, 60, 16 ) );
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
	if ( m_pAttackLabel )		m_pAttackLabel->SetVisibleSingle( FALSE );
	if ( m_pAttackStatus )		m_pAttackStatus->SetVisibleSingle( FALSE );
	if ( m_pAttackStart )		m_pAttackStart->SetVisibleSingle( FALSE );
	if ( m_pAttackStop )		m_pAttackStop->SetVisibleSingle( FALSE );
	if ( m_pAttackSupport )		m_pAttackSupport->SetVisibleSingle( FALSE );
	if ( m_pAttackPK )			m_pAttackPK->SetVisibleSingle( FALSE );
	if ( m_pAttackFollowLeader )	m_pAttackFollowLeader->SetVisibleSingle( FALSE );
	if ( m_pAttackLockSpot )		m_pAttackLockSpot->SetVisibleSingle( FALSE );
	if ( m_pAttackResurrectPtLeader ) m_pAttackResurrectPtLeader->SetVisibleSingle( FALSE );

	if ( m_pPotionLabel )		m_pPotionLabel->SetVisibleSingle( FALSE );
	if ( m_pPotionStatus )		m_pPotionStatus->SetVisibleSingle( FALSE );
	if ( m_pPotionStart )		m_pPotionStart->SetVisibleSingle( FALSE );
	if ( m_pPotionStop )		m_pPotionStop->SetVisibleSingle( FALSE );
	if ( m_pPotionHPLabel )		m_pPotionHPLabel->SetVisibleSingle( FALSE );
	if ( m_pPotionMPLabel )		m_pPotionMPLabel->SetVisibleSingle( FALSE );
	if ( m_pPotionSPLabel )		m_pPotionSPLabel->SetVisibleSingle( FALSE );
	if ( m_pPotionHPEdit )		m_pPotionHPEdit->SetVisibleSingle( FALSE );
	if ( m_pPotionMPEdit )		m_pPotionMPEdit->SetVisibleSingle( FALSE );
	if ( m_pPotionSPEdit )		m_pPotionSPEdit->SetVisibleSingle( FALSE );

	if ( m_pLootLabel )			m_pLootLabel->SetVisibleSingle( FALSE );
	if ( m_pLootStatus )		m_pLootStatus->SetVisibleSingle( FALSE );
	if ( m_pLootStart )			m_pLootStart->SetVisibleSingle( FALSE );
	if ( m_pLootStop )			m_pLootStop->SetVisibleSingle( FALSE );

	CBasicTextButton* pVisibleButton = NULL;

	switch ( cTabID )
	{
	case TAB_ATTACK:
		pVisibleButton = m_pTabAttack;
		if ( m_pAttackLabel )		m_pAttackLabel->SetVisibleSingle( TRUE );
		if ( m_pAttackStatus )		m_pAttackStatus->SetVisibleSingle( TRUE );
		if ( m_pAttackStart )		m_pAttackStart->SetVisibleSingle( TRUE );
		if ( m_pAttackStop )		m_pAttackStop->SetVisibleSingle( TRUE );
		if ( m_pAttackSupport )		m_pAttackSupport->SetVisibleSingle( TRUE );
		if ( m_pAttackPK )			m_pAttackPK->SetVisibleSingle( TRUE );
		if ( m_pAttackFollowLeader )	m_pAttackFollowLeader->SetVisibleSingle( TRUE );
		if ( m_pAttackLockSpot )		m_pAttackLockSpot->SetVisibleSingle( TRUE );
		if ( m_pAttackResurrectPtLeader ) m_pAttackResurrectPtLeader->SetVisibleSingle( TRUE );
		break;

	case TAB_POTION:
		pVisibleButton = m_pTabPotion;
		if ( m_pPotionLabel )		m_pPotionLabel->SetVisibleSingle( TRUE );
		if ( m_pPotionStatus )		m_pPotionStatus->SetVisibleSingle( TRUE );
		if ( m_pPotionStart )		m_pPotionStart->SetVisibleSingle( TRUE );
		if ( m_pPotionStop )		m_pPotionStop->SetVisibleSingle( TRUE );
		if ( m_pPotionHPLabel )		m_pPotionHPLabel->SetVisibleSingle( TRUE );
		if ( m_pPotionMPLabel )		m_pPotionMPLabel->SetVisibleSingle( TRUE );
		if ( m_pPotionSPLabel )		m_pPotionSPLabel->SetVisibleSingle( TRUE );
		if ( m_pPotionHPEdit )		m_pPotionHPEdit->SetVisibleSingle( TRUE );
		if ( m_pPotionMPEdit )		m_pPotionMPEdit->SetVisibleSingle( TRUE );
		if ( m_pPotionSPEdit )		m_pPotionSPEdit->SetVisibleSingle( TRUE );
		break;

	case TAB_LOOT:
		pVisibleButton = m_pTabLoot;
		if ( m_pLootLabel )			m_pLootLabel->SetVisibleSingle( TRUE );
		if ( m_pLootStatus )		m_pLootStatus->SetVisibleSingle( TRUE );
		if ( m_pLootStart )			m_pLootStart->SetVisibleSingle( TRUE );
		if ( m_pLootStop )			m_pLootStop->SetVisibleSingle( TRUE );
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

	case SETTINGS_POTION_HP_EDIT:
	case SETTINGS_POTION_MP_EDIT:
	case SETTINGS_POTION_SP_EDIT:
		{
			if( CHECK_MOUSE_IN_LBDOWNLIKE( dwMsg ) )
			{
				if( ControlID == SETTINGS_POTION_HP_EDIT && m_pPotionHPEdit ) m_pPotionHPEdit->BeginEdit();
				else if( ControlID == SETTINGS_POTION_MP_EDIT && m_pPotionMPEdit ) m_pPotionMPEdit->BeginEdit();
				else if( ControlID == SETTINGS_POTION_SP_EDIT && m_pPotionSPEdit ) m_pPotionSPEdit->BeginEdit();
			}
			else if( CHECK_LB_DOWN_LIKE( dwMsg ) )
			{
				if( ControlID == SETTINGS_POTION_HP_EDIT && m_pPotionHPEdit ) m_pPotionHPEdit->EndEdit();
				if( ControlID == SETTINGS_POTION_MP_EDIT && m_pPotionMPEdit ) m_pPotionMPEdit->EndEdit();
				if( ControlID == SETTINGS_POTION_SP_EDIT && m_pPotionSPEdit ) m_pPotionSPEdit->EndEdit();
				GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
				if ( pChar )
				{
					if ( ControlID == SETTINGS_POTION_HP_EDIT && m_pPotionHPEdit )
					{
						const CString & strText = m_pPotionHPEdit->GetEditString();
						int nVal = _ttoi( strText.GetString() );
						pChar->SetAutoPotionHP( nVal );
						CString strFmt;
						strFmt.Format( "%d", pChar->GetAutoPotionHP() );
						m_pPotionHPEdit->SetEditString( strFmt );
						m_pPotionHPEdit->EndEdit();
					}
					else if ( ControlID == SETTINGS_POTION_MP_EDIT && m_pPotionMPEdit )
					{
						const CString & strText = m_pPotionMPEdit->GetEditString();
						int nVal = _ttoi( strText.GetString() );
						pChar->SetAutoPotionMP( nVal );
						CString strFmt;
						strFmt.Format( "%d", pChar->GetAutoPotionMP() );
						m_pPotionMPEdit->SetEditString( strFmt );
						m_pPotionMPEdit->EndEdit();
					}
					else if ( ControlID == SETTINGS_POTION_SP_EDIT && m_pPotionSPEdit )
					{
						const CString & strText = m_pPotionSPEdit->GetEditString();
						int nVal = _ttoi( strText.GetString() );
						pChar->SetAutoPotionSP( nVal );
						CString strFmt;
						strFmt.Format( "%d", pChar->GetAutoPotionSP() );
						m_pPotionSPEdit->SetEditString( strFmt );
						m_pPotionSPEdit->EndEdit();
					}
					SyncState ();
				}
			}
		}
		break;

	case SETTINGS_ATTACK_SUPPORT:
	case SETTINGS_ATTACK_PK:
	case SETTINGS_ATTACK_FOLLOWLEADER:
	case SETTINGS_ATTACK_LOCKSPOT:
	case SETTINGS_ATTACK_RESURRECTLEADER:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
				if ( pChar )
				{
					// Read current values, toggle the clicked one, write back via SetBotSettings
					bool bSupport	= pChar->IsBotSupport();
					bool bLoot		= pChar->IsRunAutoLoot();
					bool bPk		= pChar->IsBotPK();
					bool bResuPtLdr	= pChar->IsBotResuPtLeader();
					bool bFollowLdr	= pChar->IsBotFollowLeader();
					bool bLockSpot	= pChar->IsBotLockSpot();

					switch ( ControlID )
					{
					case SETTINGS_ATTACK_SUPPORT:		bSupport = !bSupport;		break;
					case SETTINGS_ATTACK_PK:			bPk = !bPk;				break;
					case SETTINGS_ATTACK_FOLLOWLEADER:	bFollowLdr = !bFollowLdr;	break;
					case SETTINGS_ATTACK_LOCKSPOT:		bLockSpot = !bLockSpot;		break;
					case SETTINGS_ATTACK_RESURRECTLEADER:	bResuPtLdr = !bResuPtLdr;	break;
					}

					pChar->SetBotSettings ( bSupport, bLoot, bPk, bResuPtLdr, bFollowLdr, bLockSpot );
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
	const bool bBotSupport = pChar->IsBotSupport();
	const bool bBotPK = pChar->IsBotPK();
	const bool bBotFollowLeader = pChar->IsBotFollowLeader();
	const bool bBotLockSpot = pChar->IsBotLockSpot();
	const bool bBotResuPtLeader = pChar->IsBotResuPtLeader();
	const int nAutoPotionHP = pChar->GetAutoPotionHP();
	const int nAutoPotionMP = pChar->GetAutoPotionMP();
	const int nAutoPotionSP = pChar->GetAutoPotionSP();

	if ( m_bSynced && m_bLastRunBot == bRunBot && m_bLastRunAutoPots == bRunAutoPots && m_bLastRunAutoLoot == bRunAutoLoot
		&& m_bLastBotSupport == bBotSupport && m_bLastBotPK == bBotPK && m_bLastBotFollowLeader == bBotFollowLeader
		&& m_bLastBotLockSpot == bBotLockSpot && m_bLastBotResuPtLeader == bBotResuPtLeader
		&& m_nLastAutoPotionHP == nAutoPotionHP && m_nLastAutoPotionMP == nAutoPotionMP && m_nLastAutoPotionSP == nAutoPotionSP )	return;

	m_bLastRunBot = bRunBot;
	m_bLastRunAutoPots = bRunAutoPots;
	m_bLastRunAutoLoot = bRunAutoLoot;
	m_bLastBotSupport = bBotSupport;
	m_bLastBotPK = bBotPK;
	m_bLastBotFollowLeader = bBotFollowLeader;
	m_bLastBotLockSpot = bBotLockSpot;
	m_bLastBotResuPtLeader = bBotResuPtLeader;
	m_nLastAutoPotionHP = nAutoPotionHP;
	m_nLastAutoPotionMP = nAutoPotionMP;
	m_nLastAutoPotionSP = nAutoPotionSP;
	m_bSynced = TRUE;

	auto SetStatus = [this] ( CBasicTextBox* pStatus, bool bOn )
	{
		if ( pStatus )
			pStatus->SetOneLineText ( bOn ? "Status: ON" : "Status: OFF", bOn ? NS_UITEXTCOLOR::BRIGHTGREEN : NS_UITEXTCOLOR::DARKGRAY );
	};

	SetStatus ( m_pAttackStatus, bRunBot );
	SetStatus ( m_pPotionStatus, bRunAutoPots );
	SetStatus ( m_pLootStatus, bRunAutoLoot );

	// Attack config checkboxes - flip state reflects real runtime setting
	if ( m_pAttackSupport )		m_pAttackSupport->SetFlip( bBotSupport );
	if ( m_pAttackPK )			m_pAttackPK->SetFlip( bBotPK );
	if ( m_pAttackFollowLeader )	m_pAttackFollowLeader->SetFlip( bBotFollowLeader );
	if ( m_pAttackLockSpot )		m_pAttackLockSpot->SetFlip( bBotLockSpot );
	if ( m_pAttackResurrectPtLeader ) m_pAttackResurrectPtLeader->SetFlip( bBotResuPtLeader );

	if ( m_pPotionHPEdit )
	{
		CString strFmt;
		strFmt.Format( "%d", nAutoPotionHP );
		if ( m_pPotionHPEdit->GetEditString() != strFmt )
			m_pPotionHPEdit->SetEditString( strFmt );
	}
	if ( m_pPotionMPEdit )
	{
		CString strFmt;
		strFmt.Format( "%d", nAutoPotionMP );
		if ( m_pPotionMPEdit->GetEditString() != strFmt )
			m_pPotionMPEdit->SetEditString( strFmt );
	}
	if ( m_pPotionSPEdit )
	{
		CString strFmt;
		strFmt.Format( "%d", nAutoPotionSP );
		if ( m_pPotionSPEdit->GetEditString() != strFmt )
			m_pPotionSPEdit->SetEditString( strFmt );
	}
}
