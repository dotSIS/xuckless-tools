/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int default_border = 3;   /* to switch back to default border after dynamic border resizing via keybinds */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails,display systray on the 1st monitor,False: display systray on last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int showtab            = showtab_auto;
static const int toptab             = 0;        /* 0 means bottom tab */
static const int floatbar           = 1;/* 1 means the bar will float(don't have padding),0 means the bar have padding */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 5;
static const int vertpadbar         = 11;
static const int vertpadtab         = 35;
static const int horizpadtabi       = 10;
static const int horizpadtabo       = 10;
static const int scalepreview       = 4;
static const int tag_preview        = 0;        /* 1 means enable, 0 is off */
static const int colorfultag        = 1;        /* 0 means use SchemeSel for selected non vacant tag */
static const int new_window_attach_on_end = 0; /*  1 means the new window will attach on the end; 0 means the new window will attach on the front,default is front */
#define ICONSIZE 15   /* icon size */
#define ICONSPACING 5 /* space between icon and title */

static const char *fonts[]          = { "JetBrainsMono Nerd Font:pixelsize=10:antialias=true:autohint=true",
										"Liberation Mono:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";

// theme
#include "themes/catppuccin.h"

static const char *colors[][3]      = {
    /*                     fg       bg      border */
    [SchemeNorm]       = { gray3,   black,  gray2 },
    [SchemeSel]        = { gray4,   blue,   blue  },
    [SchemeTitle]      = { white,   black,  black  }, // active window title
    [TabSel]           = { blue,    gray2,  black },
    [TabNorm]          = { gray3,   black,  black },
    [SchemeTag]        = { gray3,   black,  black },
    [SchemeTag1]       = { red,    black,  black },
    [SchemeTag2]       = { orange,     black,  black },
    [SchemeTag3]       = { yellow,  black,  black },
    [SchemeTag4]       = { green,   black,  black },
    [SchemeTag5]       = { blue,    black,  black },
    [SchemeTag6]       = { green,    black,  black },
    [SchemeTag7]       = { yellow,    black,  black },
    [SchemeTag8]       = { orange,    black,  black },
    [SchemeTag9]       = { red,    black,  black },
    [SchemeLayout]     = { green,   black,  black },
    [SchemeBtnPrev]    = { green,   black,  black },
    [SchemeBtnNext]    = { yellow,  black,  black },
    [SchemeBtnClose]   = { red,     black,  black },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
/* static const char *tags[] = { "ter", "web", "des", "gam", "mp3", "mp4", "rec", "ser", "hom" }; */
/* static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; */

static const char* eww[] = { "eww", "open" , "eww", NULL };

static const Launcher launchers[] = {
    /* command     name to display */
    { eww,         " |" },
};

static const int tagschemes[] = {
    SchemeTag1, SchemeTag2, SchemeTag3, SchemeTag4, SchemeTag5, SchemeTag6, SchemeTag7, SchemeTag8, SchemeTag9
};

static const unsigned int ulinepad      = 0; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0; /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
	/* class                instance  title           tags mask  iscentered  isfloating  monitor */
	{ "obs",                NULL,     NULL,           1 << 6,    0,          1,          -1 },
	{ "mousepad",   		NULL,     NULL,           1 << 2,    0,          1,          -1 },
	{ "thunar",   			NULL,     NULL,           1 << 2,    0,          1,          -1 },
	{ "firefox",   			NULL,     NULL,           1 << 1,    0,          1,          -1 },
	{ "kitty",              NULL,     NULL,           0,         0,          0,          -1 },
	{ "st",                 NULL,     NULL,           0,         0,          0,          -1 },
	{ "eww",                NULL,     NULL,           0,         0,          1,          -1 },
	{ NULL,                 NULL,     "Event Tester", 0,         0,          0,          -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "functions.h"


static const Layout layouts[] = {
    /* symbol       arrange function */
    { "|[]=|",      tile },    /* first entry is default */
    { "|[M]|",      monocle },
    { "|[@]|",      spiral },
    { "|[\]|",      dwindle },
    { "|H[]|",      deck },
    { "|TTT|",      bstack },
    { "|===|",      bstackhoriz },
    { "|HHH|",      grid },
    { "|###|",      nrowgrid },
    { "|---|",      horizgrid },
    { "|:::|",      gaplessgrid },
    { "||M||",      centeredmaster },
    { "|>M>|",      centeredfloatingmaster },
    { "|><>|",      NULL },    /* no layout function means floating behavior */
    { NULL,         NULL },
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
static const char *voltog[] = { "/usr/bin/amixer", "sset", "Master", "toggle", NULL };
static const char *volup[] = { "/usr/bin/amixer", "set", "Master", "10%+", NULL };
static const char *voldown[] = { "/usr/bin/amixer", "set", "Master", "10%-", NULL };
static const char *brightup[] = { "/usr/bin/xbacklight", "-inc", "10%", NULL };
static const char *brightdown[] = { "/usr/bin/xbacklight", "-dec", "10%", NULL };
static const char *scrotdef[] = { "/usr/bin/scrot", "/home/zhy7ne/Pictures/Screenshots/%Y-%m-%d-%s.jpg", NULL };
static const char *scrotfoc[] = { "/usr/bin/scrot", "--focused", "/home/zhy7ne/Pictures/Screenshots/%Y-%m-%d-%s.jpg", NULL };
static const char *scrotsel[] = { "/usr/bin/scrot", "--select", "/home/zhy7ne/Pictures/Screenshots/%Y-%m-%d-%s.jpg", NULL };
static const char *killer[] = { "/home/zhy7ne/.scripts/.killer.sh", NULL };
static const char *connect[] = { "/home/zhy7ne/.scripts/.connect.sh", NULL };

static const Key keys[] = {
	/* modifier                     key						function					 argument */

    // brightness and audio 
    { MODKEY,                       XK_F6,                  spawn,                      {.v = brightup} },
    { MODKEY,                       XK_F5,                  spawn,                      {.v = brightdown} },
	{ MODKEY,						XK_F3,					spawn,						{.v = volup } },
	{ MODKEY,						XK_F2,					spawn,						{.v = voldown } },
	{ MODKEY,						XK_F1,					spawn,						{.v = voltog } },
    // terminals and launchers
	{ MODKEY,						XK_bracketleft,			spawn,						{.v = stcmd } },
	{ MODKEY,						XK_bracketright,		spawn,						{.v = kittycmd } },
	{ MODKEY,						XK_Return,				spawn,						{.v = dmenucmd } },
	{ MODKEY,						XK_BackSpace,			spawn,						SHCMD("rofi -show drun") },
    // scripts
    { MODKEY|ShiftMask,             XK_F1,                  spawn,                      {.v = connect } },
    { MODKEY|ShiftMask,             XK_Delete,              spawn,                      {.v = killer } },
    // screenshot
	{ 0,							XK_Print,				spawn,						{.v = scrotdef } },
	{ ShiftMask,					XK_Print,				spawn,						{.v = scrotfoc } },
	{ ControlMask|ShiftMask,		XK_Print,				spawn,						{.v = scrotsel } },
    // toggle stuff
	{ 0,             				XK_F10,      			fullscreen,     			{0} },
	{ MODKEY,             			XK_F10,      			togglefullscr,  			{0} },
	{ MODKEY,                       XK_g,					togglebar,					{0} },
    { MODKEY|ShiftMask,             XK_g,                   togglegaps,                 {0} },
	{ MODKEY|ShiftMask,             XK_r,					togglefloating,				{0} },
    { MODKEY|ShiftMask,             XK_w,                   tabmode,                    { -1 } },
    // master and stack
	{ MODKEY,                       XK_s,					incnmaster,					{.i = +1 } },
	{ MODKEY,                       XK_d,					incnmaster,					{.i = -1 } },
	{ MODKEY,                       XK_j,					focusstack,					{.i = -1 } },
	{ MODKEY,                       XK_k,					focusstack,					{.i = +1 } },
    // shift view
    { MODKEY,                       XK_Left,                shiftview,                  {.i = -1 } },
    { MODKEY,                       XK_Right,               shiftview,                  {.i = +1 } },
    // master and child sizes 
	{ MODKEY,                       XK_f,					zoom,						{0} },
    { MODKEY,                       XK_h,                   setmfact,                   {.f = -0.05} },
    { MODKEY,                       XK_l,                   setmfact,                   {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_h,                   setcfact,                   {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_l,                   setcfact,                   {.f = -0.25} },
    { MODKEY|ShiftMask,             XK_j,                   movestack,                  {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,                   movestack,                  {.i = -1 } },
	{ MODKEY,                       XK_Tab,					view,						{0} },
    // overall gaps
	{ MODKEY,						XK_minus,				incrgaps,					{.i = -1 } },
	{ MODKEY,						XK_equal,				incrgaps,					{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_equal,				incrgaps,					{.i =  5 } },
    // inner gaps
    { MODKEY,                       XK_i,                   incrigaps,                  {.i = -1 } },
    { MODKEY|ShiftMask,             XK_i,                   incrigaps,                  {.i = +1 } },
    // outer gaps
    { MODKEY,                       XK_o,                   incrogaps,                  {.i = -1 } },
    { MODKEY|ShiftMask,             XK_o,                   incrogaps,                  {.i = +1 } },
    // inner+outer hori, vert gaps 
    { MODKEY,                       XK_F7,                  incrihgaps,                 {.i = -1 } },
    { MODKEY|ShiftMask,             XK_F7,                  incrihgaps,                 {.i = +1 } },
    { MODKEY,                       XK_F8,                  incrivgaps,                 {.i = -1 } },
    { MODKEY|ShiftMask,             XK_F8,                  incrivgaps,                 {.i = +1 } },
    { MODKEY,                       XK_F11,                 incrohgaps,                 {.i = -1 } },
    { MODKEY|ShiftMask,             XK_F11,                 incrohgaps,                 {.i = +1 } },
    { MODKEY,                       XK_F12,                 incrovgaps,                 {.i = -1 } },
    { MODKEY|ShiftMask,             XK_F12,                 incrovgaps,                 {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask, XK_d,                   defaultgaps,                {.i = 5  } },
    // change border size
    { MODKEY,                       XK_b,                   setborderpx,                {.i = -1 } },
    { MODKEY|ShiftMask,             XK_b,                   setborderpx,                {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask, XK_b,                   setborderpx,                {.i = default_border } },
    // layout
	{ MODKEY,                       XK_w,					setlayout,					{.v = &layouts[0]} },
	{ MODKEY,                       XK_e,					setlayout,					{.v = &layouts[1]} },
    { MODKEY,                       XK_comma,               cyclelayout,                {.i = -1 } },
    { MODKEY,                       XK_period,              cyclelayout,                {.i = +1 } },
    { MODKEY|ShiftMask,             XK_0,                   tag,                        {.ui = ~0 } },
    { MODKEY,                       XK_comma,               focusmon,                   {.i = -1 } },
    { MODKEY,                       XK_period,              focusmon,                   {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,               tagmon,                     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,              tagmon,                     {.i = +1 } },
    // restart
    { MODKEY|ShiftMask,             XK_r,                   restart,                    {0} },
    // hide & restore windows
    { MODKEY|ShiftMask,             XK_d,                   hidewin,                    {0} },
    { MODKEY|ShiftMask,             XK_a,                   restorewin,                 {0} },
    // kill window
    { MODKEY,                       XK_q,                   killclient,                 {0} },
    // kill dwm
    { MODKEY|ShiftMask,             XK_F4,                  spawn,                      SHCMD("killall bar.sh chadwm") },
    // tags
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
static const Button buttons[] = {
    /* click                event mask              button          function        argument */
    { ClkLtSymbol,          0,                      Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,                      Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,                      Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                      Button2,        spawn,          {.v = stcmd } },

    /* Keep movemouse? */
    /* { ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} }, */

    /* placemouse options, choose which feels more natural:
    *    0 - tiled position is relative to mouse cursor
    *    1 - tiled position is relative to window center
    *    2 - mouse pointer warps to window center
    *
    * The moveorplace uses movemouse or placemouse depending on the floating state
    * of the selected client. Set up individual keybindings for the two if you want
    * to control these separately (i.e. to retain the feature to move a tiled window
    * into a floating position).
    */
    { ClkClientWin,         MODKEY,                 Button1,        moveorplace,    {.i = 0} },
    { ClkClientWin,         MODKEY,                 Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,                 Button3,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY|ShiftMask,       Button1,        dragmfact,      {0} },
    { ClkClientWin,         MODKEY|ShiftMask,       Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,                      Button1,        view,           {0} },
    { ClkTagBar,            0,                      Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,                 Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,                 Button3,        toggletag,      {0} },
    { ClkTabBar,            0,                      Button1,        focuswin,       {0} },
    { ClkTabBar,            0,                      Button1,        focuswin,       {0} },
    { ClkTabPrev,           0,                      Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,                      Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,                      Button1,        killclient,     {0} },
};
