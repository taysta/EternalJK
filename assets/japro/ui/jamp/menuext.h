#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define MAX_CHARS 26

#define FONT_SMALL 1
#define FONT_MEDIUM 2
#define FONT_LARGE 3
#define FONT_SMALL2 4

#define MENU_WIDTH 240
#define MENU_WIDTH_HALF 120
#define MENU_X $evalint(SCREEN_WIDTH-MENU_WIDTH)

#define HEADER_TITLE "OPTIONS"

#define HEADER_HEIGHT 60
#define HEADER_HEIGHT_HALF $evalint(HEADER_HEIGHT/2)
#define HEADER_HEIGHT_SEVENTH $evalint(HEADER_HEIGHT/7)

#define HEADER_TAB_WIDTH $evalint(MENU_WIDTH/2)

#define HEADER_EXIT_ICON_SIZE 10
#define HEADER_PADDING 5
#define HEADER_EXIT_ICON_X $evalint(MENU_WIDTH-HEADER_EXIT_ICON_SIZE-HEADER_PADDING)

#define HEADER_HEIGHT_REMAINING $evalint(SCREEN_HEIGHT-HEADER_HEIGHT)

#define HEADER_OPTIONS_TABS 4
#define HEADER_OPTIONS_TAB_WIDTH $evalint(MENU_WIDTH/HEADER_OPTIONS_TABS)
#define HEADER_OPTIONS_TAB_WIDTH_HALF $evalint(MENU_WIDTH/HEADER_OPTIONS_TABS/2)

#define HEADER_OPTIONS_TAB_1_X $evalint(MENU_WIDTH/HEADER_OPTIONS_TABS*0)
#define HEADER_OPTIONS_TAB_2_X $evalint(MENU_WIDTH/HEADER_OPTIONS_TABS*1)
#define HEADER_OPTIONS_TAB_3_X $evalint(MENU_WIDTH/HEADER_OPTIONS_TABS*2)
#define HEADER_OPTIONS_TAB_4_X $evalint(MENU_WIDTH/HEADER_OPTIONS_TABS*3)

#define HEADER_OPTIONS_TAB_1_TEXT "VIDEO"
#define HEADER_OPTIONS_TAB_2_TEXT "AUDIO"
#define HEADER_OPTIONS_TAB_3_TEXT "CONTROLS"
#define HEADER_OPTIONS_TAB_4_TEXT "GAME"

#define HEADER_OPTIONS_TAB_1_DESC_TEXT "Video Settings"
#define HEADER_OPTIONS_TAB_2_DESC_TEXT "Audio Settings"
#define HEADER_OPTIONS_TAB_3_DESC_TEXT "Controls and Keybinds"
#define HEADER_OPTIONS_TAB_4_DESC_TEXT "Game Options"

#define COL_DARKGREY 0.07 0.09 0.1 0.8
#define COL_LIGHTGREY 0.04 0.05 0.06 0.8
#define COL_BLACK 0.01 0.01 0.01 1
#define COL_NONE 0 0 0 0
#define COL_WHITE 1 1 1 1

#define HEADER_EXIT_ICON_SHADER "gfx/menus/close-cross"

#define HEADER_FONT 2
#define HEADER_FONT_SIZE 0.5
#define HEADER_FONT_STYLE 3

#define BODY_FONT 2
#define BODY_FONT_SIZE 0.4

#define BODY_ELEMENT_HEIGHT 16
#define BODY_PADDING 5

#define DESC_TEXT_X 405
#define DESC_TEXT_Y 470
#define DESC_TEXT_SCALE 0.5

#define INVISIBLE 0
#define VISIBLE 1

#define SUB_TAB_HEIGHT 30
#define SUB_TAB_BOTTOM_Y $evalint(HEADER_HEIGHT + SUB_TAB_HEIGHT)

#define SUB_OPTION1_Y $evalint(0 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION2_Y $evalint(1 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION3_Y $evalint(2 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION4_Y $evalint(3 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION5_Y $evalint(4 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION6_Y $evalint(5 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION7_Y $evalint(6 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION8_Y $evalint(7 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION9_Y $evalint(8 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION10_Y $evalint(9 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION11_Y $evalint(10 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION12_Y $evalint(11 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION13_Y $evalint(12 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION14_Y $evalint(13 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION15_Y $evalint(14 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION16_Y $evalint(15 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION17_Y $evalint(16 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION18_Y $evalint(17 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION19_Y $evalint(18 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION20_Y $evalint(19 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION21_Y $evalint(20 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION22_Y $evalint(21 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION23_Y $evalint(22 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)
#define SUB_OPTION24_Y $evalint(23 * BODY_ELEMENT_HEIGHT + HEADER_HEIGHT + SUB_TAB_HEIGHT)

//subtab1
//SUBMENU 1 TABS
#define SUB_TAB_1_TABS 4
#define SUB_TAB_1_WIDTH $evalint(MENU_WIDTH / SUB_TAB_1_TABS)
#define SUB_TAB_1_HALF_WIDTH $evalint(MENU_WIDTH / SUB_TAB_1_TABS / 2)

#define SUB_TAB_1_X_1 $evalint(MENU_WIDTH / SUB_TAB_1_TABS * 0)
#define SUB_TAB_1_X_2 $evalint(MENU_WIDTH / SUB_TAB_1_TABS * 1)
#define SUB_TAB_1_X_3 $evalint(MENU_WIDTH / SUB_TAB_1_TABS * 2)
#define SUB_TAB_1_X_4 $evalint(MENU_WIDTH / SUB_TAB_1_TABS * 3)

#define SUB_TAB_1_TEXT_1 "V_SUBTAB1"
#define SUB_TAB_1_TEXT_2 "V_SUBTAB2"
#define SUB_TAB_1_TEXT_3 "V_SUBTAB3"
#define SUB_TAB_1_TEXT_4 "V_SUBTAB4"

#define SUB_TAB_1_DESC_TEXT_1 "Video SUBTAB1 Settings"
#define SUB_TAB_1_DESC_TEXT_2 "Video SUBTAB2 Settings"
#define SUB_TAB_1_DESC_TEXT_3 "Video SUBTAB3 Settings"
#define SUB_TAB_1_DESC_TEXT_4 "Video SUBTAB4 Settings"

//subtab2
#define SUB_TAB_2_TABS 2
#define SUB_TAB_2_WIDTH $evalint(MENU_WIDTH / SUB_TAB_2_TABS)
#define SUB_TAB_2_HALF_WIDTH $evalint(MENU_WIDTH / SUB_TAB_2_TABS / 2)

#define SUB_TAB_2_X_1 $evalint(MENU_WIDTH / SUB_TAB_2_TABS * 0)
#define SUB_TAB_2_X_2 $evalint(MENU_WIDTH / SUB_TAB_2_TABS * 1)

#define SUB_TAB_2_TEXT_1 "A_SUBTAB1"
#define SUB_TAB_2_TEXT_2 "A_SUBTAB2"

#define SUB_TAB_2_DESC_TEXT_1 "Audio SUBTAB1 Settings"
#define SUB_TAB_2_DESC_TEXT_2 "Audio SUBTAB2 Settings"

//subtab3
#define SUB_TAB_3_TABS 5
#define SUB_TAB_3_WIDTH $evalint(MENU_WIDTH / SUB_TAB_3_TABS)
#define SUB_TAB_3_HALF_WIDTH $evalint(MENU_WIDTH / SUB_TAB_3_TABS / 2)

#define SUB_TAB_3_X_1 $evalint(MENU_WIDTH / SUB_TAB_3_TABS * 0)
#define SUB_TAB_3_X_2 $evalint(MENU_WIDTH / SUB_TAB_3_TABS * 1)
#define SUB_TAB_3_X_3 $evalint(MENU_WIDTH / SUB_TAB_3_TABS * 2)
#define SUB_TAB_3_X_4 $evalint(MENU_WIDTH / SUB_TAB_3_TABS * 3)
#define SUB_TAB_3_X_5 $evalint(MENU_WIDTH / SUB_TAB_3_TABS * 4)

#define SUB_TAB_3_TEXT_1 "C_SUBTAB1"
#define SUB_TAB_3_TEXT_2 "C_SUBTAB2"
#define SUB_TAB_3_TEXT_3 "C_SUBTAB3"
#define SUB_TAB_3_TEXT_4 "C_SUBTAB4"
#define SUB_TAB_3_TEXT_5 "C_SUBTAB5"

#define SUB_TAB_3_DESC_TEXT_1 "Controls SUBTAB1 Settings"
#define SUB_TAB_3_DESC_TEXT_2 "Controls SUBTAB2 Settings"
#define SUB_TAB_3_DESC_TEXT_3 "Controls SUBTAB3 Settings"
#define SUB_TAB_3_DESC_TEXT_4 "Controls SUBTAB4 Settings"
#define SUB_TAB_3_DESC_TEXT_5 "Controls SUBTAB5 Settings"

//subtab4
#define SUB_TAB_4_TABS 4
#define SUB_TAB_4_WIDTH $evalint(MENU_WIDTH / SUB_TAB_4_TABS)
#define SUB_TAB_4_HALF_WIDTH $evalint(MENU_WIDTH / SUB_TAB_4_TABS / 2)

#define SUB_TAB_4_X_1 $evalint(MENU_WIDTH / SUB_TAB_4_TABS * 0)
#define SUB_TAB_4_X_2 $evalint(MENU_WIDTH / SUB_TAB_4_TABS * 1)
#define SUB_TAB_4_X_3 $evalint(MENU_WIDTH / SUB_TAB_4_TABS * 2)
#define SUB_TAB_4_X_4 $evalint(MENU_WIDTH / SUB_TAB_4_TABS * 3)

#define SUB_TAB_4_TEXT_1 "G_SUBTAB1"
#define SUB_TAB_4_TEXT_2 "G_SUBTAB2"
#define SUB_TAB_4_TEXT_3 "G_SUBTAB3"
#define SUB_TAB_4_TEXT_4 "G_SUBTAB4"

#define SUB_TAB_4_DESC_TEXT_1 "Game SUBTAB1 Settings"
#define SUB_TAB_4_DESC_TEXT_2 "Game SUBTAB2 Settings"
#define SUB_TAB_4_DESC_TEXT_3 "Game SUBTAB3 Settings"
#define SUB_TAB_4_DESC_TEXT_4 "Game SUBTAB4 Settings"

#define BODY_OPTION_PADDING 5
#define BODY_OPTION_OFFSET 2
#define BODY_OPTION_FONT FONT_SMALL2
#define BODY_OPTION_SCALE 0.6
#define BODY_OPTION_TEXT ":"

#define HIDE_TABS \
    hide        tribes_video1 \
    hide        tribes_audio \
    hide        tribes_controls \
    hide        tribes_game

#define HIDE_SUBTABS \
    hide        subtab1 \
    hide        subtab2 \
    hide        subtab3 \
    hide        subtab4 \
    HIDE_SUB1 \
    HIDE_SUB2 \
    HIDE_SUB3 \
    HIDE_SUB4

#define HIDE_SUB1 \
    hide        tribes_video_1 \
    hide        tribes_video_2 \
    hide        tribes_video_3 \
    hide        tribes_video_4

#define HIDE_SUB2 \
    hide        tribes_audio_1 \
    hide        tribes_audio_2

#define HIDE_SUB3 \
    hide        tribes_controls_1 \
    hide        tribes_controls_2 \
    hide        tribes_controls_3 \
    hide        tribes_controls_4 \
    hide        tribes_controls_5

#define HIDE_SUB4 \
    hide        tribes_game_1 \
    hide        tribes_game_2 \
    hide        tribes_game_3 \
    hide        tribes_game_4 \
    hide        tribes_game_5

#define HIGHLIGHT \
itemDef \
{ \
    name                highlight \
    rect                0 0 0 0 \
    style               WINDOW_STYLE_FILLED \
    forecolor           COL_BLACK \
    backcolor           COL_BLACK \
    visible             1 \
    decoration \
}

#define DUMMYHIGHLIGHT \
itemDef \
{ \
    name                dummy \
    rect                0 0 0 0 \
    style               WINDOW_STYLE_FILLED \
    forecolor           COL_BLACK \
    backcolor           COL_BLACK \
    visible             0 \
    decoration \
}

#define MAKE_INTERACTIVE(_rect) \
    mouseEnter { \
        setitemrect highlight _rect \
    } \
    mouseExit { \
        setitemrect highlight _rect \
        setfocus dummy \
    }

#define NOBORDER \
    border      0 \
    bordercolor COL_NONE

#define ITEM_COMMON(_n, _g, _r, _s, _c, _b, _v) \
    name        _n \
    group       _g \
    rect        _r \
    style       _s \
    backcolor   _c \
    forecolor   _c \
    _b \
    visible     _v

#define ITEM_BACKGROUND( _n, _g, _r, _c) \
    itemDef { \
        ITEM_COMMON(_n, _g, _r, WINDOW_STYLE_FILLED, _c, NOBORDER, INVISIBLE) \
        decoration \
    }

#define ITEM_EXIT( _n, _g, _r, _c, _i) \
    itemDef { \
        ITEM_COMMON(_n, _g, _r, WINDOW_STYLE_SHADER, _c, NOBORDER, INVISIBLE) \
        type            ITEM_TYPE_BUTTON \
        background      _i \
        MAKE_INTERACTIVE(_r) \
        action \
        { \
            play				"sound/interface/button1" \
            hide                topbar \
            HIDE_TABS \
            close ingame_tribessetup \
        } \
    }

#define ITEM_TEXT(_n, _g, _r, _s, _c, _b, _v, _t, _ta, _f, _tsc, _tax, _tay, _oc, _tst) \
        ITEM_COMMON(_n, _g, _r, _s, _c, _b, _v) \
        text            _t \
        textalign       _ta \
        font            _f \
        textscale       _tsc \
        textalignx      _tax \
        textaligny      _tay \
        outlinecolor    _oc \
        textstyle       _tst

#define TEXT_BOX_HEADER(_n, _g, _r, _v, _t) \
    itemDef \
    { \
    ITEM_TEXT(_n, _g, _r, WINDOW_STYLE_EMPTY, COL_WHITE, NOBORDER, _v, _t, ITEM_ALIGN_LEFT, HEADER_FONT, HEADER_FONT_SIZE, HEADER_PADDING,  0, COL_NONE, ITEM_TEXTSTYLE_NORMAL) \
    }

#define TEXT_BOX_BODY(_n, _g, _r, _v, _t) \
    itemDef \
    { \
    ITEM_TEXT(_n, _g, _r, WINDOW_STYLE_EMPTY, COL_WHITE, NOBORDER, _v, _t, ITEM_ALIGN_LEFT, BODY_FONT, BODY_FONT_SIZE, BODY_PADDING,  0, COL_NONE, ITEM_TEXTSTYLE_NORMAL) \
    MAKE_INTERACTIVE(_r) \
    }

#define ITEM_BUTTON(_n, _g, _t, _r, _w, _descText, _show, _show2, _v) \
    itemDef \
    { \
        name            _n \
        group           _g \
        text            _t \
        style           WINDOW_STYLE_EMPTY \
        type            ITEM_TYPE_BUTTON \
        rect            _r \
        font            HEADER_FONT \
        textscale       HEADER_FONT_SIZE \
        textalignx      _w \
        textaligny      HEADER_HEIGHT_SEVENTH \
        textstyle       HEADER_FONT_STYLE \
        textalign       ITEM_ALIGN_CENTER \
        forecolor       COL_WHITE \
        visible         _v \
        descText        _descText \
		MAKE_INTERACTIVE(_r) \
        action \
        { \
            play        "sound/interface/button1.wav" \
            HIDE_TABS \
            HIDE_SUBTABS \
            show        _show \
            show        _show2 \
        } \
    }

#define ITEM_BODY_GENERIC(_n, _g, _r, _d) \
    name                _n \
    group               _g \
    text				BODY_OPTION_TEXT \
    rect                _r \
    textalign		    ITEM_ALIGN_RIGHT \
    textalignx		    BODY_OPTION_PADDING \
    textaligny		    BODY_OPTION_OFFSET \
    textscale 		    BODY_OPTION_SCALE \
    font 			    BODY_OPTION_FONT \
    style			    WINDOW_STYLE_FILLED \
    forecolor		    COL_WHITE \
    descText            _d \
    visible				VISIBLE \
    MAKE_INTERACTIVE(_r) \
    action \
    { \
        play			"sound/interface/button1" \
        defer VideoSetup videowarningMenu \
    }

#define ITEM_BODY_MULTI(_n, _g, _r, _t, _d, _c, _s) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type                ITEM_TYPE_MULTI \
        cvar                _c \
        cvarStrList         _s \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_NUMERIC(_n, _g, _r, _t, _d, _c) \
TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_NUMERICFIELD \
        cvar				_c \
            ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_SLIDERFL(_n, _g, _r, _t, _d, _c) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_SLIDER \
        cvarFloat			_c \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_SLIDERINT(_n, _g, _r, _t, _d, _c) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_INTSLIDER \
        cvarInt 			_c \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_CHECKBOX(_n, _g, _r, _t, _d, _c, _b) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_CHECKBOX \
        cvar 			    _c \
        bitMask             _b \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_YESNO(_n, _g, _r, _t, _d, _c) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_YESNO \
        cvar 			    _c \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_EDITFIELD(_n, _g, _r, _t, _d, _c, _m) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_EDITFIELD \
        cvar 			    _c \
        maxchars            _m \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }

#define ITEM_BODY_BIND(_n, _g, _r, _t, _d, _c) \
    TEXT_BOX_BODY(_n, _g, 0 _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, INVISIBLE, _t) \
    itemDef \
    { \
        type 			    ITEM_TYPE_BIND \
        cvar 			    _c \
        ITEM_BODY_GENERIC(_n, _g, MENU_WIDTH_HALF _r MENU_WIDTH_HALF BODY_ELEMENT_HEIGHT, _d) \
    }
