/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=10:antialias=true:autohint=true",
										"Liberation Mono:size=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
/* static const char *tags[] = { "ter", "web", "des", "gam", "mp3", "mp4", "rec", "ser", "hom" }; */
/* static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "obs",                NULL,     NULL,           1 << 6,    1,          0,           0,        -1 },
	{ "mousepad",   		NULL,     NULL,           1 << 2,    1,          0,           0,        -1 },
	{ "thunar",   			NULL,     NULL,           1 << 2,    1,          0,           0,        -1 },
	{ "firefox",   			NULL,     NULL,           1 << 1,    1,          0,           0,        -1 },
	{ "kitty",              NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "st",                 NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,                 NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|[]=|",      tile },    /* first entry is default */
	{ "|><>|",      NULL },    /* no layout function means floating behavior */
	{ "|[M]|",      monocle },
	{ "|TTT|",      bstack },
	{ "|===|",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont};
static const char *stcmd[]  = { "st", NULL };
static const char *kittycmd[]  = { "kitty", NULL };
static const char *volup[] = { "/usr/bin/amixer", "set", "Master", "10%+", NULL };
static const char *voldown[] = { "/usr/bin/amixer", "set", "Master", "10%-", NULL };
static const char *brightup[] = { "/usr/bin/xbacklight", "-inc", "10%", NULL };
static const char *brightdown[] = { "/usr/bin/xbacklight", "-dec", "10%", NULL };
static const char *scrotdef[] = { "/usr/bin/scrot", "/home/zhy7ne/Pictures/Screenshots/%Y-%m-%d-%s.jpg", NULL };
static const char *scrotfoc[] = { "/usr/bin/scrot", "--focused", "/home/zhy7ne/Pictures/Screenshots/%Y-%m-%d-%s.jpg", NULL };
static const char *scrotsel[] = { "/usr/bin/scrot", "--select", "/home/zhy7ne/Pictures/Screenshots/%Y-%m-%d-%s.jpg", NULL };
static const char *killer[] = { "/home/zhy7ne/.scripts/.killer.sh", NULL };
static const char *connect[] = { "/home/zhy7ne/.scripts/.connect.sh", NULL };

static Key keys[] = {
	/* modifier                     key						function					 argument */
	{ MODKEY,						XK_bracketleft,			spawn,						{.v = stcmd } },
	{ MODKEY,						XK_bracketright,		spawn,						{.v = kittycmd } },
	{ MODKEY,						XK_Return,				spawn,						{.v = dmenucmd } },
	{ MODKEY,						XK_BackSpace,			spawn,						SHCMD("rofi -show drun") },
	{ MODKEY,                       XK_l,					setmfact,					{.f = +0.05} },
	{ MODKEY,                       XK_j,					focusstack,					{.i = -1 } },
	{ MODKEY,                       XK_k,					focusstack,					{.i = +1 } },
	{ MODKEY,                       XK_h,					setmfact,					{.f = -0.05} },
	{ MODKEY,                       XK_g,					togglebar,					{0} },
	{ MODKEY,                       XK_f,					zoom,						{0} },
	{ MODKEY,                       XK_d,					incnmaster,					{.i = -1 } },
	{ MODKEY,                       XK_s,					incnmaster,					{.i = +1 } },
	{ MODKEY,		        		XK_q,					killclient,					{0} },
	{ MODKEY,                       XK_w,					setlayout,					{.v = &layouts[0]} },
	{ MODKEY,                       XK_e,					setlayout,					{.v = &layouts[1]} },
	{ MODKEY,                       XK_r,					setlayout,					{.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      				setlayout,      			{.v = &layouts[3]} },
	{ MODKEY,                       XK_i,      				setlayout,      			{.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_r,					togglefloating,				{0} },
	{ MODKEY,                       XK_t,					setlayout,					{0} },
	{ MODKEY,                       XK_Tab,					view,						{0} },
	{ MODKEY,                       XK_0,					view,						{.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,					tag,						{.ui = ~0 } },
	{ MODKEY,                       XK_comma,				focusmon,					{.i = -1 } },
	{ MODKEY,                       XK_period,				focusmon,					{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,				tagmon,						{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,				tagmon,						{.i = +1 } },
	{ MODKEY,						XK_minus,				setgaps,					{.i = -1 } },
	{ MODKEY,						XK_equal,				setgaps,					{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_equal,				setgaps,					{.i =  0 } },
	{ 0,             				XK_F10,      			fullscreen,     			{0} },
	{ MODKEY,             			XK_F10,      			togglefullscr,  			{0} },
    { MODKEY,                       XK_F7,                  spawn,                      {.v = brightup} },
    { MODKEY,                       XK_F6,                  spawn,                      {.v = brightdown} },
	{ MODKEY,                       XK_F5,					xrdb,						{.v = NULL } },
	{ MODKEY|ShiftMask,				XK_F4,					quit,						{0} },
	{ MODKEY,						XK_F3,					spawn,						{.v = volup } },
	{ MODKEY,						XK_F2,					spawn,						{.v = voldown } },
    { MODKEY|ShiftMask,             XK_F1,                  spawn,                      {.v = connect } },
    { MODKEY|ShiftMask,             XK_Delete,              spawn,                      {.v = killer } },
	{ 0,							XK_Print,				spawn,						{.v = scrotdef } },
	{ ShiftMask,					XK_Print,				spawn,						{.v = scrotfoc } },
	{ ControlMask|ShiftMask,		XK_Print,				spawn,						{.v = scrotsel } },
	TAGKEYS(                        XK_1,												0)
	TAGKEYS(                        XK_2,												1)
	TAGKEYS(                        XK_3,												2)
	TAGKEYS(                        XK_4,												3)
	TAGKEYS(                        XK_5,												4)
	TAGKEYS(                        XK_6,												5)
	TAGKEYS(                        XK_7,												6)
	TAGKEYS(                        XK_8,												7)
	TAGKEYS(                        XK_9,												8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = stcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

