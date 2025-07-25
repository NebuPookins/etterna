#include "Etterna/Globals/global.h"
#include "Etterna/Models/Misc/Game.h"
#include "Etterna/Models/Misc/GameConstantsAndTypes.h"
#include "Etterna/Models/Misc/GameInput.h" // for GameButton constants
#include "Etterna/Globals/GameLoop.h"	   // for ChangeGame
#include "GameManager.h"
#include "NoteSkinManager.h"
#include "RageUtil/Misc/RageInputDevice.h"
#include "RageUtil/Utils/RageUtil.h"
#include "Etterna/Models/StepsAndStyles/Style.h"
#include "ThemeManager.h"

GameManager* GAMEMAN =
  nullptr; // global and accessible from anywhere in our program

enum
{
	TRACK_1 = 0,
	TRACK_2,
	TRACK_3,
	TRACK_4,
	TRACK_5,
	TRACK_6,
	TRACK_7,
	TRACK_8,
	TRACK_9,
	TRACK_10,
	TRACK_11,
	TRACK_12,
	TRACK_13,
	TRACK_14,
	TRACK_15,
	TRACK_16,
	// 16 tracks needed for beat-double7 and techno-double8
};

std::string
StepsTypeInfo::GetLocalizedString() const
{
	if (THEME->HasString("StepsType", szName))
		return THEME->GetString("StepsType", szName);
	return szName;
}

static const StepsTypeInfo g_StepsTypeInfos[] = {
	// dance
	{ "dance-single", 4, true, StepsTypeCategory_Single },
	{ "dance-double", 8, true, StepsTypeCategory_Double },
	{ "dance-solo", 6, true, StepsTypeCategory_Single },
	{ "dance-threepanel",
	  3,
	  true,
	  StepsTypeCategory_Single }, // thanks to kurisu
	// pump
	{ "pump-single", 5, true, StepsTypeCategory_Single },
	{ "pump-halfdouble", 6, true, StepsTypeCategory_Double },
	{ "pump-double", 10, true, StepsTypeCategory_Double },
	// kb7
	{ "kb7-single", 7, true, StepsTypeCategory_Single },
	// { "kb7-small",		7,	true,	StepsTypeCategory_Single },
	// ez2dancer
	{ "ez2-single",
	  5,
	  true,
	  StepsTypeCategory_Single }, // Single: TL,LHH,D,RHH,TR
	{ "ez2-double", 10, true, StepsTypeCategory_Double }, // Double: Single x2
	{ "ez2-real",
	  7,
	  true,
	  StepsTypeCategory_Single }, // Real: TL,LHH,LHL,D,RHL,RHH,TR
	// ds3ddx
	{ "ds3ddx-single", 8, true, StepsTypeCategory_Single },
	// beatmania
	{ "bm-single5",
	  6,
	  true,
	  StepsTypeCategory_Single }, // called "bm" for backward compat
	{ "bm-double5",
	  12,
	  true,
	  StepsTypeCategory_Double }, // called "bm" for backward compat
	{ "bm-single7",
	  8,
	  true,
	  StepsTypeCategory_Single }, // called "bm" for backward compat
	{ "bm-double7",
	  16,
	  true,
	  StepsTypeCategory_Double }, // called "bm" for backward compat
	// dance maniax
	{ "maniax-single", 4, true, StepsTypeCategory_Single },
	{ "maniax-double", 8, true, StepsTypeCategory_Double },
	// pop'n music
	{ "pnm-five",
	  5,
	  true,
	  StepsTypeCategory_Single }, // called "pnm" for backward compat
	{ "pnm-nine",
	  9,
	  true,
	  StepsTypeCategory_Single }, // called "pnm" for backward compat
};

// Important:  Every game must define the buttons: "Start", "Back", "MenuLeft",
// "Operator" and "MenuRight"
static const AutoMappings g_AutoKeyMappings_Dance = AutoMappings(
  "",
  "",
  "",

  AutoMappingEntry(0, KEY_LEFT, GAME_BUTTON_MENULEFT, false),
  AutoMappingEntry(0, KEY_RIGHT, GAME_BUTTON_MENURIGHT, false),
  AutoMappingEntry(0, KEY_UP, GAME_BUTTON_MENUUP, false),
  AutoMappingEntry(0, KEY_DOWN, GAME_BUTTON_MENUDOWN, false),
  AutoMappingEntry(0, KEY_Cz, DANCE_BUTTON_LEFT, false),
  AutoMappingEntry(0, KEY_Cx, DANCE_BUTTON_DOWN, false),
  AutoMappingEntry(0, KEY_COMMA, DANCE_BUTTON_UP, false),
  AutoMappingEntry(0, KEY_PERIOD, DANCE_BUTTON_RIGHT, false),
  AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
  AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false),
  AutoMappingEntry(0, KEY_ACCENT, GAME_BUTTON_RESTART, false),
  AutoMappingEntry(0, KEY_KP_SLASH, GAME_BUTTON_MENULEFT, true),
  AutoMappingEntry(0, KEY_KP_ASTERISK, GAME_BUTTON_MENURIGHT, true),
  AutoMappingEntry(0, KEY_KP_HYPHEN, GAME_BUTTON_MENUUP, true),
  AutoMappingEntry(0, KEY_KP_PLUS, GAME_BUTTON_MENUDOWN, true),
  AutoMappingEntry(0, KEY_KP_C4, DANCE_BUTTON_LEFT, true),
  AutoMappingEntry(0, KEY_KP_C6, DANCE_BUTTON_RIGHT, true),
  AutoMappingEntry(0, KEY_KP_C8, DANCE_BUTTON_UP, true),
  AutoMappingEntry(0, KEY_KP_C2, DANCE_BUTTON_DOWN, true));

static const AutoMappings g_AutoKeyMappings_Solo = AutoMappings(
  "",
  "",
  "",
  AutoMappingEntry(0, KEY_LEFT, GAME_BUTTON_MENULEFT, false),
  AutoMappingEntry(0, KEY_RIGHT, GAME_BUTTON_MENURIGHT, false),
  AutoMappingEntry(0, KEY_UP, GAME_BUTTON_MENUUP, false),
  AutoMappingEntry(0, KEY_DOWN, GAME_BUTTON_MENUDOWN, false),
  AutoMappingEntry(0, KEY_Cz, SOLO_BUTTON_LEFT, false),
  AutoMappingEntry(0, KEY_PERIOD, SOLO_BUTTON_RIGHT, false),
  AutoMappingEntry(0, KEY_COMMA, SOLO_BUTTON_UPRIGHT, false),
  AutoMappingEntry(0, KEY_Cx, SOLO_BUTTON_UPLEFT, false),
  AutoMappingEntry(0, KEY_Cc, SOLO_BUTTON_DOWN, false),
  AutoMappingEntry(0, KEY_Cm, SOLO_BUTTON_UP, false),
  AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
  AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false),
  AutoMappingEntry(0, KEY_ACCENT, GAME_BUTTON_RESTART, false),
  AutoMappingEntry(0, KEY_KP_SLASH, GAME_BUTTON_MENULEFT, true),
  AutoMappingEntry(0, KEY_KP_ASTERISK, GAME_BUTTON_MENURIGHT, true),
  AutoMappingEntry(0, KEY_KP_HYPHEN, GAME_BUTTON_MENUUP, true),
  AutoMappingEntry(0, KEY_KP_PLUS, GAME_BUTTON_MENUDOWN, true),
  AutoMappingEntry(0, KEY_KP_C4, SOLO_BUTTON_LEFT, true),
  AutoMappingEntry(0, KEY_KP_C6, SOLO_BUTTON_RIGHT, true),
  AutoMappingEntry(0, KEY_KP_C8, SOLO_BUTTON_UP, true),
  AutoMappingEntry(0, KEY_KP_C2, SOLO_BUTTON_DOWN, true),
  AutoMappingEntry(0, KEY_KP_C7, SOLO_BUTTON_UPLEFT, true),
  AutoMappingEntry(0, KEY_KP_C9, SOLO_BUTTON_UPRIGHT, true));

// xxx: get this from the theme? (see others)
// the problem with getting it from the noteskin is that this is meant to be
// static const; if we switch to anything we likely won't get const anymore
// but i may be talking out of my ass -aj
static const int DANCE_COL_SPACING = 64;

// static ThemeMetric<int>	DANCE_COL_SPACING("ColumnSpacing","Dance");
// named after a similar metric in Aldo_MX's build for
// compatibility/familiarity:
// float	DANCE_COL_SPACING
// NOTESKIN->GetMetricF("NoteDisplay","ArrowColSpacing");
/* looking for ARROW_SIZE should be enough (ArrowSize)
 * just arroweffects.cpp for rowspacing (ArrowRowSpacing)
 */

static const Style g_Style_Dance_Single = {
	// STYLE_DANCE_SINGLE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	true,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_dance_single,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	4,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -DANCE_COL_SPACING * 1.5f, nullptr },
	  { TRACK_2, -DANCE_COL_SPACING * 0.5f, nullptr },
	  { TRACK_3, +DANCE_COL_SPACING * 0.5f, nullptr },
	  { TRACK_4, +DANCE_COL_SPACING * 1.5f, nullptr },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, Style::END_MAPPING },
	  { Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3 },
};

static const Style g_Style_Dance_Double = {
	// STYLE_DANCE_DOUBLE
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	true,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"double",					 // m_szName
	StepsType_dance_double,		 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	8,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -DANCE_COL_SPACING * 3.5f, nullptr },
	  { TRACK_2, -DANCE_COL_SPACING * 2.5f, nullptr },
	  { TRACK_3, -DANCE_COL_SPACING * 1.5f, nullptr },
	  { TRACK_4, -DANCE_COL_SPACING * 0.5f, nullptr },
	  { TRACK_5, +DANCE_COL_SPACING * 0.5f, nullptr },
	  { TRACK_6, +DANCE_COL_SPACING * 1.5f, nullptr },
	  { TRACK_7, +DANCE_COL_SPACING * 2.5f, nullptr },
	  { TRACK_8, +DANCE_COL_SPACING * 3.5f, nullptr },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, Style::END_MAPPING },
	  { 4, 5, 6, 7, Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7 },
};

static const Style g_Style_Solo_Single = {
	// STYLE_DANCE_SOLO
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_dance_solo,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	6,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -DANCE_COL_SPACING * 2.5f, nullptr },
	  { TRACK_2, -DANCE_COL_SPACING * 1.5f, nullptr },
	  { TRACK_3, -DANCE_COL_SPACING * 0.5f, nullptr },
	  { TRACK_4, +DANCE_COL_SPACING * 0.5f, nullptr },
	  { TRACK_5, +DANCE_COL_SPACING * 1.5f, nullptr },
	  { TRACK_6, +DANCE_COL_SPACING * 2.5f, nullptr },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, 4, 5, Style::END_MAPPING },
	  { Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5 },
};

static const Style g_Style_Dance_ThreePanel = {
	// STYLE_DANCE_THREEPANEL
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	false,						// m_bUsedForHowToPlay
	"threepanel",				// m_szName
	StepsType_dance_threepanel, // m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	3,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -DANCE_COL_SPACING * 1.0f, nullptr },
	  { TRACK_2, +DANCE_COL_SPACING * 0.0f, nullptr },
	  { TRACK_3, +DANCE_COL_SPACING * 1.0f, nullptr },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  // 4 3 5
	  { 0, 1, Style::NO_MAPPING, 2, Style::END_MAPPING },
	  { Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2 },
};

static const Style* g_apGame_Dance_Styles[] = { &g_Style_Dance_Single,
												&g_Style_Dance_Double,
												&g_Style_Dance_ThreePanel,
												nullptr };

static const Game g_Game_Dance = {
	"dance",			   // m_szName
	g_apGame_Dance_Styles, // m_apStyles
	false,				   // m_bCountNotesSeparately
	false,				   // m_bTickHolds
	false,				   // m_PlayersHaveSeparateStyles
	{					   // m_InputScheme
	  "dance",			   // m_szName
	  NUM_DANCE_BUTTONS,   // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "Left", GAME_BUTTON_LEFT },
		{ "Down", GAME_BUTTON_DOWN },
		{ "Up", GAME_BUTTON_UP },
		{ "Right", GAME_BUTTON_RIGHT },
	  },
	  &g_AutoKeyMappings_Dance },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

static const Style* g_apGame_Dance4KOnly_Styles[] = { &g_Style_Dance_Single, nullptr };

static const Game g_Game_Dance4KOnly = {
	"dance-4k-only",              // m_szName
	g_apGame_Dance4KOnly_Styles,   // m_apStyles
	false,                         // m_bCountNotesSeparately
	false,                         // m_bTickHolds
	false,                         // m_PlayersHaveSeparateStyles
	{                              // m_InputScheme
	  "dance",                    // m_szName
	  NUM_DANCE_BUTTONS,           // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "Left", GAME_BUTTON_LEFT },
		{ "Down", GAME_BUTTON_DOWN },
		{ "Up", GAME_BUTTON_UP },
		{ "Right", GAME_BUTTON_RIGHT },
	  },
	  &g_AutoKeyMappings_Dance },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

static const Style* g_apGame_Solo_Styles[] = { &g_Style_Solo_Single, nullptr };

static const Game g_Game_Solo = {
	"solo",				  // m_szName
	g_apGame_Solo_Styles, // m_apStyles
	false,				  // m_bCountNotesSeparately
	false,				  // m_bTickHolds
	false,				  // m_PlayersHaveSeparateStyles
	{					  // m_InputScheme
	  "solo",			  // m_szName
	  NUM_SOLO_BUTTONS,	  // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "Left", GAME_BUTTON_LEFT },
		{ "UpLeft", GameButton_Invalid },
		{ "Down", GAME_BUTTON_DOWN },
		{ "Up", GAME_BUTTON_UP },
		{ "UpRight", GameButton_Invalid },
		{ "Right", GAME_BUTTON_RIGHT }
	  },
	  &g_AutoKeyMappings_Solo },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

static const AutoMappings g_AutoKeyMappings_Pump =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Cx, PUMP_BUTTON_UPLEFT, false),
			   AutoMappingEntry(0, KEY_COMMA, PUMP_BUTTON_UPRIGHT, false),
			   AutoMappingEntry(0, KEY_SPACE, PUMP_BUTTON_CENTER, false),
			   AutoMappingEntry(0, KEY_Cz, PUMP_BUTTON_DOWNLEFT, false),
			   AutoMappingEntry(0, KEY_PERIOD, PUMP_BUTTON_DOWNRIGHT, false),
			   AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
			   AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false),

			   // unmap confusing default MenuButtons
			   AutoMappingEntry(0, KEY_KP_C8, GameButton_Invalid, false),
			   AutoMappingEntry(0, KEY_KP_C2, GameButton_Invalid, false),
			   AutoMappingEntry(0, KEY_KP_C4, GameButton_Invalid, false),
			   AutoMappingEntry(0, KEY_KP_C6, GameButton_Invalid, false));

// PIU Defaults: RowSpacing = 60; ColSpacing = 52; ArrowSize = 54;
// apparently column spacing is 48px
// static ThemeMetric<int>	PUMP_COL_SPACING	("ColumnSpacing","Pump");
static const int PUMP_COL_SPACING = 48;

static const Style g_Style_Pump_Single = {
	// STYLE_PUMP_SINGLE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_pump_single,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	5,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -PUMP_COL_SPACING * 2.0f, nullptr },
	  { TRACK_2, -PUMP_COL_SPACING * 1.0f, nullptr },
	  { TRACK_3, +PUMP_COL_SPACING * 0.0f, nullptr },
	  { TRACK_4, +PUMP_COL_SPACING * 1.0f, nullptr },
	  { TRACK_5, +PUMP_COL_SPACING * 2.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, 4, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4 },
};

static const Style g_Style_Pump_HalfDouble = {
	// STYLE_PUMP_HALFDOUBLE
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	false,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"halfdouble",				 // m_szName
	StepsType_pump_halfdouble,	 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	6,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -PUMP_COL_SPACING * 2.5f - 4, nullptr },
	  { TRACK_2, -PUMP_COL_SPACING * 1.5f - 4, nullptr },
	  { TRACK_3, -PUMP_COL_SPACING * 0.5f - 4, nullptr },
	  { TRACK_4, +PUMP_COL_SPACING * 0.5f + 4, nullptr },
	  { TRACK_5, +PUMP_COL_SPACING * 1.5f + 4, nullptr },
	  { TRACK_6, +PUMP_COL_SPACING * 2.5f + 4, nullptr },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { Style::NO_MAPPING, Style::NO_MAPPING, 0, 1, 2, Style::END_MAPPING },
	  { 3, 4, 5, Style::NO_MAPPING, Style::NO_MAPPING, Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5 },
};

static const Style g_Style_Pump_Double = {
	// STYLE_PUMP_DOUBLE
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	false,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"double",					 // m_szName
	StepsType_pump_double,		 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	10,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -PUMP_COL_SPACING * 4.5f - 4, nullptr },
	  { TRACK_2, -PUMP_COL_SPACING * 3.5f - 4, nullptr },
	  { TRACK_3, -PUMP_COL_SPACING * 2.5f - 4, nullptr },
	  { TRACK_4, -PUMP_COL_SPACING * 1.5f - 4, nullptr },
	  { TRACK_5, -PUMP_COL_SPACING * 0.5f - 4, nullptr },
	  { TRACK_6, +PUMP_COL_SPACING * 0.5f + 4, nullptr },
	  { TRACK_7, +PUMP_COL_SPACING * 1.5f + 4, nullptr },
	  { TRACK_8, +PUMP_COL_SPACING * 2.5f + 4, nullptr },
	  { TRACK_9, +PUMP_COL_SPACING * 3.5f + 4, nullptr },
	  { TRACK_10, +PUMP_COL_SPACING * 4.5f + 4, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, 4, Style::END_MAPPING },
	  { 5, 6, 7, 8, 9, Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  0 + 5,
	  1 + 5,
	  2 + 5,
	  3 + 5,
	  4 + 5 },

};

static const Style* g_apGame_Pump_Styles[] = { &g_Style_Pump_Single,
											   &g_Style_Pump_HalfDouble,
											   &g_Style_Pump_Double,
											   nullptr };

static const Game g_Game_Pump = {
	"pump",				  // m_szName
	g_apGame_Pump_Styles, // m_apStyles
	false,				  // m_bCountNotesSeparately
	false,				  // m_bTickHolds
	false,				  // m_PlayersHaveSeparateStyles
	{					  // m_InputScheme
	  "pump",			  // m_szName
	  NUM_PUMP_BUTTONS,	  // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "DownLeft", GAME_BUTTON_LEFT },
		{ "UpLeft", GAME_BUTTON_DOWN },
		{ "Center", GameButton_Invalid },
		{ "UpRight", GAME_BUTTON_UP },
		{ "DownRight", GAME_BUTTON_RIGHT },
	  },
	  &g_AutoKeyMappings_Pump },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

static const AutoMappings g_AutoKeyMappings_KB7 =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Cs, KB7_BUTTON_KEY1, false),
			   AutoMappingEntry(0, KEY_Cd, KB7_BUTTON_KEY2, false),
			   AutoMappingEntry(0, KEY_Cf, KB7_BUTTON_KEY3, false),
			   AutoMappingEntry(0, KEY_SPACE, KB7_BUTTON_KEY4, false),
			   AutoMappingEntry(0, KEY_Cj, KB7_BUTTON_KEY5, false),
			   AutoMappingEntry(0, KEY_Ck, KB7_BUTTON_KEY6, false),
			   AutoMappingEntry(0, KEY_Cl, KB7_BUTTON_KEY7, false),
			   AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
			   AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false));

// ThemeMetric<int>	KB7_COL_SPACING	("ColumnSpacing","KB7");
static const int KB7_COL_SPACING = 64;
static const Style g_Style_KB7_Single = {
	// STYLE_KB7_SINGLE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_kb7_single,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	7,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -KB7_COL_SPACING * 3.0f, nullptr },
	  { TRACK_2, -KB7_COL_SPACING * 2.0f, nullptr },
	  { TRACK_3, -KB7_COL_SPACING * 1.0f, nullptr },
	  { TRACK_4, +KB7_COL_SPACING * 0.0f, nullptr },
	  { TRACK_5, +KB7_COL_SPACING * 1.0f, nullptr },
	  { TRACK_6, +KB7_COL_SPACING * 2.0f, nullptr },
	  { TRACK_7, +KB7_COL_SPACING * 3.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, 4, 5, 6, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{
	  // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6 // doesn't work?
	},

};

static const Style* g_apGame_KB7_Styles[] = { &g_Style_KB7_Single, nullptr };

static const Game g_Game_KB7 = {
	"kb7",				 // m_szName
	g_apGame_KB7_Styles, // m_apStyles
	true,				 // m_bCountNotesSeparately
	false,				 // m_bTickHolds
	false,				 // m_PlayersHaveSeparateStyles
	{					 // m_InputScheme
	  "kb7",			 // m_szName
	  NUM_KB7_BUTTONS,	 // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "Key1", GameButton_Invalid },
		{ "Key2", GAME_BUTTON_LEFT },
		{ "Key3", GAME_BUTTON_DOWN },
		{ "Key4", GameButton_Invalid },
		{ "Key5", GAME_BUTTON_UP },
		{ "Key6", GAME_BUTTON_RIGHT },
		{ "Key7", GameButton_Invalid },
	  },
	  &g_AutoKeyMappings_KB7 },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

// ThemeMetric<int>	EZ2_COL_SPACING	("ColumnSpacing","EZ2");
static const int EZ2_COL_SPACING = 46;
// do you even need this if they're the same now -aj
// ThemeMetric<int>	EZ2__REAL_COL_SPACING	("ColumnSpacing","EZ2Real");
static const int EZ2_REAL_COL_SPACING = 46;
static const Style g_Style_Ez2_Single = {
	// STYLE_EZ2_SINGLE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_ez2_single,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	5,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -EZ2_COL_SPACING * 2.0f, nullptr },
	  { TRACK_2, -EZ2_COL_SPACING * 1.0f, nullptr },
	  { TRACK_3, +EZ2_COL_SPACING * 0.0f, nullptr },
	  { TRACK_4, +EZ2_COL_SPACING * 1.0f, nullptr },
	  { TRACK_5, +EZ2_COL_SPACING * 2.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 4, 2, 1, 3, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{
	  // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  2,
	  0,
	  4,
	  1,
	  3 // This should be from back to front: Down, UpLeft, UpRight, Upper Left
		// Hand, Upper Right Hand
	},
};

static const Style g_Style_Ez2_Real = {
	// STYLE_EZ2_REAL
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	false,						// m_bUsedForHowToPlay
	"real",						// m_szName
	StepsType_ez2_real,			// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	7,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -EZ2_REAL_COL_SPACING * 2.3f, nullptr },
	  { TRACK_2, -EZ2_REAL_COL_SPACING * 1.6f, nullptr },
	  { TRACK_3, -EZ2_REAL_COL_SPACING * 0.9f, nullptr },
	  { TRACK_4, +EZ2_REAL_COL_SPACING * 0.0f, nullptr },
	  { TRACK_5, +EZ2_REAL_COL_SPACING * 0.9f, nullptr },
	  { TRACK_6, +EZ2_REAL_COL_SPACING * 1.6f, nullptr },
	  { TRACK_7, +EZ2_REAL_COL_SPACING * 2.3f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 6, 3, 2, 4, 1, 5, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{
	  // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  3,
	  0,
	  6,
	  1,
	  5,
	  2,
	  4 // This should be from back to front: Down, UpLeft, UpRight, Lower Left
		// Hand, Lower Right Hand, Upper Left Hand, Upper Right Hand
	},
};

static const Style g_Style_Ez2_Double = {
	// STYLE_EZ2_DOUBLE
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	false,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"double",					 // m_szName
	StepsType_ez2_double,		 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	10,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -EZ2_COL_SPACING * 4.5f, nullptr },
	  { TRACK_2, -EZ2_COL_SPACING * 3.5f, nullptr },
	  { TRACK_3, -EZ2_COL_SPACING * 2.5f, nullptr },
	  { TRACK_4, -EZ2_COL_SPACING * 1.5f, nullptr },
	  { TRACK_5, -EZ2_COL_SPACING * 0.5f, nullptr },
	  { TRACK_6, +EZ2_COL_SPACING * 0.5f, nullptr },
	  { TRACK_7, +EZ2_COL_SPACING * 1.5f, nullptr },
	  { TRACK_8, +EZ2_COL_SPACING * 2.5f, nullptr },
	  { TRACK_9, +EZ2_COL_SPACING * 3.5f, nullptr },
	  { TRACK_10, +EZ2_COL_SPACING * 4.5f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 4, 2, 1, 3, Style::END_MAPPING },
	  { 5, 9, 7, 6, 8, Style::END_MAPPING },
	},
	{
	  // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  2,
	  0,
	  4,
	  1,
	  3,
	  7,
	  5,
	  9,
	  6,
	  8 // This should be from back to front: Down, UpLeft, UpRight, Upper Left
		// Hand, Upper Right Hand
	},
};

static const Style* g_apGame_Ez2_Styles[] = { &g_Style_Ez2_Single,
											  &g_Style_Ez2_Real,
											  &g_Style_Ez2_Double,
											  nullptr };

static const AutoMappings g_AutoKeyMappings_Ez2 =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Cz, EZ2_BUTTON_FOOTUPLEFT, false),
			   AutoMappingEntry(0, KEY_Cb, EZ2_BUTTON_FOOTUPRIGHT, false),
			   AutoMappingEntry(0, KEY_Cc, EZ2_BUTTON_FOOTDOWN, false),
			   AutoMappingEntry(0, KEY_Cx, EZ2_BUTTON_HANDUPLEFT, false),
			   AutoMappingEntry(0, KEY_Cv, EZ2_BUTTON_HANDUPRIGHT, false),
			   AutoMappingEntry(0, KEY_Cs, EZ2_BUTTON_HANDLRLEFT, false),
			   AutoMappingEntry(0, KEY_Cf, EZ2_BUTTON_HANDLRRIGHT, false),
			   AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
			   AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false));

static const Game g_Game_Ez2 = {
	"ez2",				 // m_szName
	g_apGame_Ez2_Styles, // m_apStyles
	true,				 // m_bCountNotesSeparately
	false,				 // m_bTickHolds
	false,				 // m_PlayersHaveSeparateStyles
	{					 // m_InputScheme
	  "ez2",			 // m_szName
	  NUM_EZ2_BUTTONS,	 // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "FootUpLeft", GAME_BUTTON_UP },
		{ "FootUpRight", GAME_BUTTON_DOWN },
		{ "FootDown", GAME_BUTTON_START },
		{ "HandUpLeft", GAME_BUTTON_LEFT },
		{ "HandUpRight", GAME_BUTTON_RIGHT },
		{ "HandLrLeft", GameButton_Invalid },
		{ "HandLrRight", GameButton_Invalid },
	  },
	  &g_AutoKeyMappings_Ez2 },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W2,	  // m_mapW1To
	TNS_W2,	  // m_mapW2To
	TNS_W2,	  // m_mapW3To
	TNS_W4,	  // m_mapW4To
	TNS_Miss, // m_mapW5To
};

// ThemeMetric<int>	DS3DDX_COL_SPACING	("ColumnSpacing","DS3DDX");
static const int DS3DDX_COL_SPACING = 46;
static const Style g_Style_DS3DDX_Single = {
	// STYLE_DS3DDX_SINGLE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	true,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_ds3ddx_single,	// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	8,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -DS3DDX_COL_SPACING * 3.0f, nullptr },
	  { TRACK_2, -DS3DDX_COL_SPACING * 2.0f, nullptr },
	  { TRACK_3, -DS3DDX_COL_SPACING * 1.0f, nullptr },
	  { TRACK_4, -DS3DDX_COL_SPACING * 0.0f, nullptr },
	  { TRACK_5, +DS3DDX_COL_SPACING * 0.0f, nullptr },
	  { TRACK_6, +DS3DDX_COL_SPACING * 1.0f, nullptr },
	  { TRACK_7, +DS3DDX_COL_SPACING * 2.0f, nullptr },
	  { TRACK_8, +DS3DDX_COL_SPACING * 3.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, 4, 5, 6, 7, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7 },
};

static const Style* g_apGame_DS3DDX_Styles[] = { &g_Style_DS3DDX_Single,
												 nullptr };

static const AutoMappings g_AutoKeyMappings_DS3DDX =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Ca, DS3DDX_BUTTON_HANDLEFT, false),
			   AutoMappingEntry(0, KEY_Cz, DS3DDX_BUTTON_FOOTDOWNLEFT, false),
			   AutoMappingEntry(0, KEY_Cq, DS3DDX_BUTTON_FOOTUPLEFT, false),
			   AutoMappingEntry(0, KEY_Cw, DS3DDX_BUTTON_HANDUP, false),
			   AutoMappingEntry(0, KEY_Cx, DS3DDX_BUTTON_HANDDOWN, false),
			   AutoMappingEntry(0, KEY_Ce, DS3DDX_BUTTON_FOOTUPRIGHT, false),
			   AutoMappingEntry(0, KEY_Cc, DS3DDX_BUTTON_FOOTDOWNRIGHT, false),
			   AutoMappingEntry(0, KEY_Cd, DS3DDX_BUTTON_HANDRIGHT, false),
			   AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
			   AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false));

static const Game g_Game_DS3DDX = {
	"ds3ddx",				// m_szName
	g_apGame_DS3DDX_Styles, // m_apStyles
	false,					// m_bCountNotesSeparately
	false,					// m_bTickHolds
	false,					// m_PlayersHaveSeparateStyles
	{						// m_InputScheme
	  "ds3ddx",				// m_szName
	  NUM_DS3DDX_BUTTONS,	// m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "HandLeft", GAME_BUTTON_LEFT },
		{ "FootDownLeft", GameButton_Invalid },
		{ "FootUpLeft", GameButton_Invalid },
		{ "HandUp", GAME_BUTTON_UP },
		{ "HandDown", GAME_BUTTON_DOWN },
		{ "FootUpRight", GameButton_Invalid },
		{ "FootDownRight", GameButton_Invalid },
		{ "HandRight", GAME_BUTTON_RIGHT },
	  },
	  &g_AutoKeyMappings_DS3DDX },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

// ThemeMetric<int>	BEAT_COL_SPACING	("ColumnSpacing","Beat");
static const int BEAT_COL_SPACING = 34;
static const Style g_Style_Beat_Single5 = {
	// STYLE_BEAT_SINGLE5
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	true,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single5",					// m_szName
	StepsType_beat_single5,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	6,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -BEAT_COL_SPACING * 2.5f, nullptr },
	  { TRACK_2, -BEAT_COL_SPACING * 1.5f, nullptr },
	  { TRACK_3, -BEAT_COL_SPACING * 0.5f, nullptr },
	  { TRACK_4, +BEAT_COL_SPACING * 0.5f, nullptr },
	  { TRACK_5, +BEAT_COL_SPACING * 1.5f, nullptr },
	  { TRACK_6, +BEAT_COL_SPACING * 3.0f, "scratch" },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0,
		1,
		2,
		3,
		4,
		Style::NO_MAPPING,
		Style::NO_MAPPING,
		5,
		5,
		Style::END_MAPPING },
	  { Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5 },
};

static const Style g_Style_Beat_Double5 = {
	// STYLE_BEAT_DOUBLE
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	false,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"double5",					 // m_szName
	StepsType_beat_double5,		 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	12,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -BEAT_COL_SPACING * 6.0f, nullptr },
	  { TRACK_2, -BEAT_COL_SPACING * 5.0f, nullptr },
	  { TRACK_3, -BEAT_COL_SPACING * 4.0f, nullptr },
	  { TRACK_4, -BEAT_COL_SPACING * 3.0f, nullptr },
	  { TRACK_5, -BEAT_COL_SPACING * 2.0f, nullptr },
	  { TRACK_6, -BEAT_COL_SPACING * 1.5f, "scratch" },
	  { TRACK_7, +BEAT_COL_SPACING * 0.5f, nullptr },
	  { TRACK_8, +BEAT_COL_SPACING * 1.5f, nullptr },
	  { TRACK_9, +BEAT_COL_SPACING * 2.5f, nullptr },
	  { TRACK_10, +BEAT_COL_SPACING * 3.5f, nullptr },
	  { TRACK_11, +BEAT_COL_SPACING * 4.5f, nullptr },
	  { TRACK_12, +BEAT_COL_SPACING * 6.0f, "scratch" },

	},
	{ // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0,
		1,
		2,
		3,
		4,
		Style::NO_MAPPING,
		Style::NO_MAPPING,
		5,
		5,
		Style::END_MAPPING },
	  { 6,
		7,
		8,
		9,
		10,
		Style::NO_MAPPING,
		Style::NO_MAPPING,
		11,
		11,
		Style::END_MAPPING } },
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7,
	  8,
	  9,
	  10,
	  11 },
};

static const Style g_Style_Beat_Single7 = {
	// STYLE_BEAT_SINGLE7
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	true,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single7",					// m_szName
	StepsType_beat_single7,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	8,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_8, -BEAT_COL_SPACING * 3.5f, "scratch" },
	  { TRACK_1, -BEAT_COL_SPACING * 2.0f, nullptr },
	  { TRACK_2, -BEAT_COL_SPACING * 1.0f, nullptr },
	  { TRACK_3, -BEAT_COL_SPACING * 0.0f, nullptr },
	  { TRACK_4, +BEAT_COL_SPACING * 1.0f, nullptr },
	  { TRACK_5, +BEAT_COL_SPACING * 2.0f, nullptr },
	  { TRACK_6, +BEAT_COL_SPACING * 3.0f, nullptr },
	  { TRACK_7, +BEAT_COL_SPACING * 4.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 1, 2, 3, 4, 5, 6, 7, 0, 0, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7 },
};

static const Style g_Style_Beat_Double7 = {
	// STYLE_BEAT_DOUBLE7
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	false,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"double7",					 // m_szName
	StepsType_beat_double7,		 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	16,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_8, -BEAT_COL_SPACING * 8.0f, "scratch" },
	  { TRACK_1, -BEAT_COL_SPACING * 6.5f, nullptr },
	  { TRACK_2, -BEAT_COL_SPACING * 5.5f, nullptr },
	  { TRACK_3, -BEAT_COL_SPACING * 4.5f, nullptr },
	  { TRACK_4, -BEAT_COL_SPACING * 3.5f, nullptr },
	  { TRACK_5, -BEAT_COL_SPACING * 2.5f, nullptr },
	  { TRACK_6, -BEAT_COL_SPACING * 1.5f, nullptr },
	  { TRACK_7, -BEAT_COL_SPACING * 0.5f, nullptr },
	  { TRACK_9, +BEAT_COL_SPACING * 0.5f, nullptr },
	  { TRACK_10, +BEAT_COL_SPACING * 1.5f, nullptr },
	  { TRACK_11, +BEAT_COL_SPACING * 2.5f, nullptr },
	  { TRACK_12, +BEAT_COL_SPACING * 3.5f, nullptr },
	  { TRACK_13, +BEAT_COL_SPACING * 4.5f, nullptr },
	  { TRACK_14, +BEAT_COL_SPACING * 5.5f, nullptr },
	  { TRACK_15, +BEAT_COL_SPACING * 6.5f, nullptr },
	  { TRACK_16, +BEAT_COL_SPACING * 8.0f, "scratch" },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 1, 2, 3, 4, 5, 6, 7, 0, 0, Style::END_MAPPING },
	  { 8, 9, 10, 11, 12, 13, 14, 15, 15, Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7,
	  8,
	  9,
	  10,
	  11,
	  12,
	  13,
	  14,
	  15 },
};

static const Style* g_apGame_Beat_Styles[] = { &g_Style_Beat_Single5,
											   &g_Style_Beat_Double5,
											   &g_Style_Beat_Single7,
											   &g_Style_Beat_Double7,
											   nullptr };

static const AutoMappings g_AutoKeyMappings_Beat =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Cm, BEAT_BUTTON_KEY1, false),
			   AutoMappingEntry(0, KEY_Ck, BEAT_BUTTON_KEY2, false),
			   AutoMappingEntry(0, KEY_COMMA, BEAT_BUTTON_KEY3, false),
			   AutoMappingEntry(0, KEY_Cl, BEAT_BUTTON_KEY4, false),
			   AutoMappingEntry(0, KEY_PERIOD, BEAT_BUTTON_KEY5, false),
			   AutoMappingEntry(0, KEY_SEMICOLON, BEAT_BUTTON_KEY6, false),
			   AutoMappingEntry(0, KEY_SLASH, BEAT_BUTTON_KEY7, false),
			   AutoMappingEntry(0, KEY_SPACE, BEAT_BUTTON_SCRATCHUP, false),
			   AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
			   AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false));

static const Game g_Game_Beat = {
	"beat",				  // m_szName
	g_apGame_Beat_Styles, // m_apStyles
	true,				  // m_bCountNotesSeparately
	false,				  // m_bTickHolds
	false,				  // m_PlayersHaveSeparateStyles
	{					  // m_InputScheme
	  "beat",			  // m_szName
	  NUM_BEAT_BUTTONS,	  // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "Key1", GAME_BUTTON_LEFT },
		{ "Key2", GameButton_Invalid },
		{ "Key3", GameButton_Invalid },
		{ "Key4", GameButton_Invalid },
		{ "Key5", GameButton_Invalid },
		{ "Key6", GameButton_Invalid },
		{ "Key7", GAME_BUTTON_RIGHT },
		{ "Scratch up", GAME_BUTTON_UP },
		{ "Scratch down", GAME_BUTTON_DOWN },
	  },
	  &g_AutoKeyMappings_Beat },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

// ThemeMetric<int>	MANIAX_COL_SPACING	("ColumnSpacing","Maniax");
static const int MANIAX_COL_SPACING = 36;
static const Style g_Style_Maniax_Single = {
	// STYLE_MANIAX_SINGLE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	true,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"single",					// m_szName
	StepsType_maniax_single,	// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	4,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -MANIAX_COL_SPACING * 1.5f, nullptr },
	  { TRACK_2, -MANIAX_COL_SPACING * 0.5f, nullptr },
	  { TRACK_3, +MANIAX_COL_SPACING * 0.5f, nullptr },
	  { TRACK_4, +MANIAX_COL_SPACING * 1.5f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 1, 2, 0, 3, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3 },
};

static const Style g_Style_Maniax_Double = {
	// STYLE_MANIAX_DOUBLE
	true,						 // m_bUsedForGameplay
	true,						 // m_bUsedForEdit
	false,						 // m_bUsedForDemonstration
	false,						 // m_bUsedForHowToPlay
	"double",					 // m_szName
	StepsType_maniax_double,	 // m_StepsType
	StyleType_OnePlayerTwoSides, // m_StyleType
	8,							 // m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -MANIAX_COL_SPACING * 3.5f, nullptr },
	  { TRACK_2, -MANIAX_COL_SPACING * 2.5f, nullptr },
	  { TRACK_3, -MANIAX_COL_SPACING * 1.5f, nullptr },
	  { TRACK_4, -MANIAX_COL_SPACING * 0.5f, nullptr },
	  { TRACK_5, +MANIAX_COL_SPACING * 0.5f, nullptr },
	  { TRACK_6, +MANIAX_COL_SPACING * 1.5f, nullptr },
	  { TRACK_7, +MANIAX_COL_SPACING * 2.5f, nullptr },
	  { TRACK_8, +MANIAX_COL_SPACING * 3.5f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 1, 2, 0, 3, Style::END_MAPPING },
	  { 5, 6, 4, 7, Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7 },
};

static const Style* g_apGame_Maniax_Styles[] = { &g_Style_Maniax_Single,
												 &g_Style_Maniax_Double,
												 nullptr };

static const AutoMappings g_AutoKeyMappings_Maniax =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Ca, MANIAX_BUTTON_HANDUPLEFT, false),
			   AutoMappingEntry(0, KEY_Cs, MANIAX_BUTTON_HANDUPRIGHT, false),
			   AutoMappingEntry(0, KEY_Cz, MANIAX_BUTTON_HANDLRLEFT, false),
			   AutoMappingEntry(0, KEY_Cx, MANIAX_BUTTON_HANDLRRIGHT, false));

static const Game g_Game_Maniax = {
	"maniax",				// m_szName
	g_apGame_Maniax_Styles, // m_apStyles
	false,					// m_bCountNotesSeparately
	false,					// m_bTickHolds
	false,					// m_PlayersHaveSeparateStyles
	{						// m_InputScheme
	  "maniax",				// m_szName
	  NUM_MANIAX_BUTTONS,	// m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "HandUpLeft", GAME_BUTTON_LEFT },
		{ "HandUpRight", GAME_BUTTON_RIGHT },
		{ "HandLrLeft", GAME_BUTTON_DOWN },
		{ "HandLrRight", GAME_BUTTON_UP },
	  },
	  &g_AutoKeyMappings_Maniax },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

/** popn *********************************************************************/
// ThemeMetric<int>	POPN5_COL_SPACING	("ColumnSpacing","Popn5");
static const int POPN5_COL_SPACING = 32;
// ThemeMetric<int>	POPN9_COL_SPACING	("ColumnSpacing","Popn9");
static const int POPN9_COL_SPACING = 32;
static const Style g_Style_Popn_Five = {
	// STYLE_POPN_FIVE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	false,						// m_bUsedForDemonstration
	false,						// m_bUsedForHowToPlay
	"popn-five",				// m_szName
	StepsType_popn_five,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	5,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -POPN5_COL_SPACING * 2.0f, nullptr },
	  { TRACK_2, -POPN5_COL_SPACING * 1.0f, nullptr },
	  { TRACK_3, +POPN5_COL_SPACING * 0.0f, nullptr },
	  { TRACK_4, +POPN5_COL_SPACING * 1.0f, nullptr },
	  { TRACK_5, +POPN5_COL_SPACING * 2.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { Style::NO_MAPPING,
		Style::NO_MAPPING,
		0,
		1,
		2,
		3,
		4,
		Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4 },
};

static const Style g_Style_Popn_Nine = {
	// STYLE_POPN_NINE
	true,						// m_bUsedForGameplay
	true,						// m_bUsedForEdit
	true,						// m_bUsedForDemonstration
	true,						// m_bUsedForHowToPlay
	"popn-nine",				// m_szName
	StepsType_popn_nine,		// m_StepsType
	StyleType_OnePlayerOneSide, // m_StyleType
	9,							// m_iColsPerPlayer
	{
	  // m_ColumnInfo[MAX_COLS_PER_PLAYER];
	  // PLAYER_1
	  { TRACK_1, -POPN9_COL_SPACING * 4.0f, nullptr },
	  { TRACK_2, -POPN9_COL_SPACING * 3.0f, nullptr },
	  { TRACK_3, -POPN9_COL_SPACING * 2.0f, nullptr },
	  { TRACK_4, -POPN9_COL_SPACING * 1.0f, nullptr },
	  { TRACK_5, +POPN9_COL_SPACING * 0.0f, nullptr },
	  { TRACK_6, +POPN9_COL_SPACING * 1.0f, nullptr },
	  { TRACK_7, +POPN9_COL_SPACING * 2.0f, nullptr },
	  { TRACK_8, +POPN9_COL_SPACING * 3.0f, nullptr },
	  { TRACK_9, +POPN9_COL_SPACING * 4.0f, nullptr },

	},
	{
	  // m_iInputColumn[NUM_GameController][NUM_GameButton]
	  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, Style::END_MAPPING },
	  { Style::END_MAPPING },
	},
	{ // m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
	  0,
	  1,
	  2,
	  3,
	  4,
	  5,
	  6,
	  7,
	  8 },

};

static const Style* g_apGame_Popn_Styles[] = { &g_Style_Popn_Five,
											   &g_Style_Popn_Nine,
											   nullptr };

static const AutoMappings g_AutoKeyMappings_Popn =
  AutoMappings("",
			   "",
			   "",
			   AutoMappingEntry(0, KEY_Cz, POPN_BUTTON_LEFT_WHITE, false),
			   AutoMappingEntry(0, KEY_Cs, POPN_BUTTON_LEFT_YELLOW, false),
			   AutoMappingEntry(0, KEY_Cx, POPN_BUTTON_LEFT_GREEN, false),
			   AutoMappingEntry(0, KEY_Cd, POPN_BUTTON_LEFT_BLUE, false),
			   AutoMappingEntry(0, KEY_Cc, POPN_BUTTON_RED, false),
			   AutoMappingEntry(0, KEY_Cf, POPN_BUTTON_RIGHT_BLUE, false),
			   AutoMappingEntry(0, KEY_Cv, POPN_BUTTON_RIGHT_GREEN, false),
			   AutoMappingEntry(0, KEY_Cg, POPN_BUTTON_RIGHT_YELLOW, false),
			   AutoMappingEntry(0, KEY_Cb, POPN_BUTTON_RIGHT_WHITE, false),
			   AutoMappingEntry(0, KEY_EQUAL, GAME_BUTTON_EFFECT_UP, false),
			   AutoMappingEntry(0, KEY_HYPHEN, GAME_BUTTON_EFFECT_DOWN, false));

static const Game g_Game_Popn = {
	"popn",				  // m_szName
	g_apGame_Popn_Styles, // m_apStyles
	true,				  // m_bCountNotesSeparately
	false,				  // m_bTickHolds
	false,				  // m_PlayersHaveSeparateStyles
	{					  // m_InputScheme
	  "popn",			  // m_szName
	  NUM_POPN_BUTTONS,	  // m_iButtonsPerController
	  {
		// m_szButtonNames
		{ "Left White", GameButton_Invalid },
		{ "Left Yellow", GAME_BUTTON_UP },
		{ "Left Green", GameButton_Invalid },
		{ "Left Blue", GAME_BUTTON_LEFT },
		{ "Red", GAME_BUTTON_START },
		{ "Right Blue", GAME_BUTTON_RIGHT },
		{ "Right Green", GameButton_Invalid },
		{ "Right Yellow", GAME_BUTTON_DOWN },
		{ "Right White", GameButton_Invalid },
	  },
	  &g_AutoKeyMappings_Popn },
	{
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	  { GameButtonType_Step },
	},
	TNS_W1, // m_mapW1To
	TNS_W2, // m_mapW2To
	TNS_W3, // m_mapW3To
	TNS_W4, // m_mapW4To
	TNS_W5, // m_mapW5To
};

static const Game* g_Games[] = {
	&g_Game_Dance,  &g_Game_Dance4KOnly, &g_Game_Solo, &g_Game_KB7, &g_Game_Pump, &g_Game_Popn,
	&g_Game_Beat,  &g_Game_Ez2, &g_Game_Maniax, &g_Game_DS3DDX,
};

GameManager::GameManager()
{
	m_bResetModifiers = false;
	m_bResetTurns = false;
	m_fPreviousRate = 1.f;
	// Register with Lua.
	{
		Lua* L = LUA->Get();
		lua_pushstring(L, "GAMEMAN");
		this->PushSelf(L);
		lua_settable(L, LUA_GLOBALSINDEX);
		LUA->Release(L);
	}
}

GameManager::~GameManager()
{
	// Unregister with Lua.
	LUA->UnsetGlobal("GAMEMAN");
}

void
GameManager::GetStylesForGame(const Game* pGame,
							  std::vector<const Style*>& aStylesAddTo,
							  bool editor)
{
	for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
		const Style* style = pGame->m_apStyles[s];
		if (!editor && !style->m_bUsedForGameplay)
			continue;
		if (editor && !style->m_bUsedForEdit)
			continue;

		aStylesAddTo.push_back(style);
	}
}

const Game*
GameManager::GetGameForStyle(const Style* pStyle)
{
	for (auto pGame : g_Games) {
		for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
			if (pGame->m_apStyles[s] == pStyle)
				return pGame;
		}
	}
	FAIL_M(pStyle->m_szName);
}

const Style*
GameManager::GetEditorStyleForStepsType(StepsType st)
{
	for (auto pGame : g_Games) {
		for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
			const Style* style = pGame->m_apStyles[s];
			if (style->m_StepsType == st && style->m_bUsedForEdit)
				return style;
		}
	}

	ASSERT_M(
	  0, ssprintf("The current game cannot use this Style with the editor!"));
	return nullptr;
}

const Style*
GameManager::GetStyleForStepsType(StepsType st)
{
	for (auto pGame : g_Games) {
		for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
			const Style* style = pGame->m_apStyles[s];
			if (style->m_StepsType == st)
				return style;
		}
	}
	return nullptr;
}

void
GameManager::GetStepsTypesForGame(const Game* pGame,
								  std::vector<StepsType>& aStepsTypeAddTo)
{
	for (int i = 0; pGame->m_apStyles[i] != nullptr; ++i) {
		StepsType st = pGame->m_apStyles[i]->m_StepsType;
		ASSERT(st < NUM_StepsType);

		// Some Styles use the same StepsType (e.g. single and versus) so check
		// that we aren't doubling up.
		bool found = false;
		for (auto& j : aStepsTypeAddTo)
			if (st == j) {
				found = true;
				break;
			}
		if (found)
			continue;

		aStepsTypeAddTo.push_back(st);
	}
}

void
GameManager::GetDemonstrationStylesForGame(const Game* pGame,
										   std::vector<const Style*>& vpStylesOut)
{
	vpStylesOut.clear();

	for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
		const Style* style = pGame->m_apStyles[s];
		if (style->m_bUsedForDemonstration)
			vpStylesOut.push_back(style);
	}

	ASSERT(vpStylesOut.size() > 0); // this Game is missing a Style that can be
									// used with the demonstration
}

const Style*
GameManager::GetHowToPlayStyleForGame(const Game* pGame)
{
	for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
		const Style* style = pGame->m_apStyles[s];
		if (style->m_bUsedForHowToPlay)
			return style;
	}

	FAIL_M(ssprintf("Game has no Style that can be used with HowToPlay: %s",
					pGame->m_szName));
}

void
GameManager::GetCompatibleStyles(const Game* pGame,
								 int iNumPlayers,
								 std::vector<const Style*>& vpStylesOut)
{
	FOREACH_ENUM(StyleType, styleType)
	{
		int iNumPlayersRequired;
		switch (styleType) {
			DEFAULT_FAIL(styleType);
			case StyleType_OnePlayerOneSide:
			case StyleType_OnePlayerTwoSides:
				iNumPlayersRequired = 1;
				break;
		}

		if (iNumPlayers != iNumPlayersRequired)
			continue;

		for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
			const Style* style = pGame->m_apStyles[s];
			if (style->m_StyleType != styleType)
				continue;
			if (!style->m_bUsedForGameplay)
				continue;

			vpStylesOut.push_back(style);
		}
	}
}

const Style*
GameManager::GetFirstCompatibleStyle(const Game* pGame,
									 int iNumPlayers,
									 StepsType st)
{
	std::vector<const Style*> vpStyles;
	GetCompatibleStyles(pGame, iNumPlayers, vpStyles);
	for (auto& s : vpStyles) {
		if (s->m_StepsType == st) {
			return s;
		}
	}
	return nullptr;
}

void
GameManager::GetEnabledGames(std::vector<const Game*>& aGamesOut)
{
	for (auto pGame : g_Games) {
		if (IsGameEnabled(pGame))
			aGamesOut.push_back(pGame);
	}
}

const Game*
GameManager::GetDefaultGame()
{
	const Game* pDefault = nullptr;
	if (pDefault == nullptr) {
		for (size_t i = 0; pDefault == nullptr && i < ARRAYLEN(g_Games); ++i) {
			if (IsGameEnabled(g_Games[i]))
				pDefault = g_Games[i];
		}

		if (pDefault == nullptr)
			RageException::Throw("No NoteSkins found");
	}

	return pDefault;
}

int
GameManager::GetIndexFromGame(const Game* pGame)
{
	for (size_t g = 0; g < ARRAYLEN(g_Games); ++g) {
		if (g_Games[g] == pGame)
			return g;
	}
	FAIL_M(ssprintf("Game not found: %s", pGame->m_szName));
}

const Game*
GameManager::GetGameFromIndex(int index)
{
	ASSERT(index >= 0);
	ASSERT(index < (int)ARRAYLEN(g_Games));
	return g_Games[index];
}

bool
GameManager::IsGameEnabled(const Game* pGame)
{
	return NOTESKIN->DoNoteSkinsExistForGame(pGame);
}

const StepsTypeInfo&
GameManager::GetStepsTypeInfo(StepsType st)
{
	ASSERT(ARRAYLEN(g_StepsTypeInfos) == NUM_StepsType);
	ASSERT_M(st < NUM_StepsType,
			 ssprintf("StepsType %d < NUM_StepsType (%d)", st, NUM_StepsType));
	return g_StepsTypeInfos[st];
}

StepsType
GameManager::StringToStepsType(std::string sStepsType)
{
	sStepsType = make_lower(sStepsType);

	for (int i = 0; i < NUM_StepsType; i++)
		if (g_StepsTypeInfos[i].szName == sStepsType)
			return StepsType(i);

	return StepsType_Invalid;
}

std::string
GameManager::StyleToLocalizedString(const Style* style)
{
	std::string s = style->m_szName;
	s = Capitalize(s);
	if (THEME->HasString("Style", s))
		return THEME->GetString("Style", s);
	else
		return s;
}

const Game*
GameManager::StringToGame(const std::string& sGame)
{
	for (auto& g_Game : g_Games)
		if (!CompareNoCase(sGame, g_Game->m_szName))
			return g_Game;

	return nullptr;
}

const Style*
GameManager::GameAndStringToStyle(const Game* game, const std::string& sStyle)
{
	for (int s = 0; game->m_apStyles[s]; ++s) {
		const Style* style = game->m_apStyles[s];
		if (CompareNoCase(sStyle, style->m_szName) == 0)
			return style;
	}

	return nullptr;
}

// lua start
#include "Etterna/Models/Lua/LuaBinding.h"

/** @brief Allow Lua to have access to the GameManager. */
class LunaGameManager : public Luna<GameManager>
{
  public:
	static int StepsTypeToLocalizedString(T* p, lua_State* L)
	{
		lua_pushstring(L,
					   p->GetStepsTypeInfo(Enum::Check<StepsType>(L, 1))
						 .GetLocalizedString()
						 .c_str());
		return 1;
	}
	static int GetFirstStepsTypeForGame(T* p, lua_State* L)
	{
		Game* pGame = Luna<Game>::check(L, 1);

		std::vector<StepsType> vstAddTo;
		p->GetStepsTypesForGame(pGame, vstAddTo);
		ASSERT(!vstAddTo.empty());
		StepsType st = vstAddTo[0];
		LuaHelpers::Push(L, st);
		return 1;
	}
	static int IsGameEnabled(T* p, lua_State* L)
	{
		const Game* pGame = p->StringToGame(SArg(1));
		if (pGame != nullptr)
			lua_pushboolean(L, p->IsGameEnabled(pGame));
		else
			lua_pushnil(L);

		return 1;
	}
	static int GetStylesForGame(T* p, lua_State* L)
	{
		std::string game_name = SArg(1);
		const Game* pGame = p->StringToGame(game_name);
		if (pGame == nullptr) {
			luaL_error(
			  L, "GetStylesForGame: Invalid Game: '%s'", game_name.c_str());
		}
		std::vector<Style*> aStyles;
		lua_createtable(L, 0, 0);
		for (int s = 0; pGame->m_apStyles[s] != nullptr; ++s) {
			auto* pStyle = const_cast<Style*>(pGame->m_apStyles[s]);
			pStyle->PushSelf(L);
			lua_rawseti(L, -2, s + 1);
		}
		return 1;
	}
	static int GetEnabledGames(T* p, lua_State* L)
	{
		std::vector<const Game*> aGames;
		p->GetEnabledGames(aGames);
		lua_createtable(L, aGames.size(), 0);
		for (size_t i = 0; i < aGames.size(); ++i) {
			lua_pushstring(L, aGames[i]->m_szName);
			lua_rawseti(L, -2, i + 1);
		}
		return 1;
	}

	static int SetGame(T* p, lua_State* L)
	{
		std::string game_name = SArg(1);
		const Game* pGame = p->StringToGame(game_name);
		if (pGame == nullptr) {
			luaL_error(L, "SetGame: Invalid Game: '%s'", game_name.c_str());
		}
		std::string theme;
		if (lua_gettop(L) >= 2 && !lua_isnil(L, 2)) {
			theme = SArg(2);
			if (!THEME->IsThemeSelectable(theme)) {
				luaL_error(L, "SetGame: Invalid Theme: '%s'", theme.c_str());
			}
		}
		GameLoop::ChangeGame(game_name, theme);
		return 0;
	}
	static int GetGameFromString(T* p, lua_State* L)
	{
		std::string game = SArg(1);
		const auto* g = p->StringToGame(game);
		if (g == nullptr)
			luaL_error(L, "GetGameFromString: Invalid Game: %s", game.c_str());
		else
			const_cast<Game*>(g)->PushSelf(L);
		return 1;
	}

	LunaGameManager()
	{
		ADD_METHOD(StepsTypeToLocalizedString);
		ADD_METHOD(GetFirstStepsTypeForGame);
		ADD_METHOD(IsGameEnabled);
		ADD_METHOD(GetStylesForGame);
		ADD_METHOD(GetEnabledGames);
		ADD_METHOD(SetGame);
		ADD_METHOD(GetGameFromString);
	};
};

LUA_REGISTER_CLASS(GameManager)
// lua end
