/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx     = 1;        /* border pixel of windows */
static const int gappx           = 10;       /* gaps between windows */
static unsigned int snap         = 32;       /* snap pixel */
static int showbar               = 1;        /* 0 means no bar */
static int topbar                = 1;        /* 0 means bottom bar */
static const int vertpad         = 0;        /* DISABLED vertical padding of bar */
static const int sidepad         = 0;        /* DISABLED horizontal padding of bar */
static const int swallowfloating = 0;        /* 1 means swallow floating windows by default */
static const char *fonts[]       = { "FiraCode Nerd Font:size=12" };
static const char dmenufont[]    = "FiraCode Nerd Font:size=12";
static char normbgcolor[]        = "#222222";
static char normbordercolor[]    = "#444444";
static char normfgcolor[]        = "#bbbbbb";
static char selfgcolor[]         = "#eeeeee";
static char selbordercolor[]     = "#005577";
static char selbgcolor[]         = "#005577";
static char *colors[][3]         = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeHid]  = { selfgcolor,  normbgcolor, normbordercolor },
};

/* tagging */
static const char *tags[]    = { "", "", "", "", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "﩯"   , tile    }                , /* first entry is default */
	{ ""   , NULL    }                , /* no layout function means floating behavior */
	{ ""   , monocle }                ,
	{ "[D]" , deck    }                ,
	{ " " , spiral  }                ,
	{ " " , dwindle }                ,
	{ "יִ"   , centeredmaster }         ,
	{ ""   , centeredfloatingmaster } ,
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
static char dmenumon[2]       = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-l", "20", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor     } ,
		{ "normbordercolor",    STRING,  &normbordercolor } ,
		{ "normfgcolor",        STRING,  &normfgcolor     } ,
		{ "selbgcolor",         STRING,  &selbgcolor      } ,
		{ "selbordercolor",     STRING,  &selbordercolor  } ,
		{ "selfgcolor",         STRING,  &selfgcolor      } ,
		{ "borderpx",          	INTEGER, &borderpx        } ,
		{ "snap",          		INTEGER, &snap            } ,
		{ "showbar",          	INTEGER, &showbar         } ,
		{ "topbar",          	INTEGER, &topbar          } ,
		{ "nmaster",          	INTEGER, &nmaster         } ,
		{ "resizehints",       	INTEGER, &resizehints     } ,
		{ "mfact",      	 	FLOAT,   &mfact           } ,
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd    }  } ,
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd     }  } ,
	{ MODKEY,                       XK_f,      spawn,          {.v = firefoxcmd  }  } ,
	{ MODKEY,                       XK_b,      togglebar,      {0                }  } ,
	{ MODKEY,                       XK_j,      focusstackvis,  {.i = +1          }  } ,
	{ MODKEY,                       XK_k,      focusstackvis,  {.i = -1          }  } ,
	{ MODKEY|ShiftMask,             XK_j,      focusstackhid,  {.i = +1          }  } ,
	{ MODKEY|ShiftMask,             XK_k,      focusstackhid,  {.i = -1          }  } ,
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1          }  } ,
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1          }  } ,
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05       }  } ,
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05       }  } ,
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0                }  } ,
	{ MODKEY,                       XK_Tab,    view,           {0                }  } ,
	{ MODKEY,                       XK_q,      killclient,     {0                }  } ,
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0] }  } , //tile
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1] }  } , //floating
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2] }  } , //monocle
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3] }  } , //deck
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[4] }  } , //spiral
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[5] }  } , //dwindle
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[6] }  } , //centeredmaster
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[7] }  } , //centeredfloatingmaster
	{ MODKEY,                       XK_p,      setlayout,      {0                }  } ,
	{ MODKEY|ShiftMask,             XK_p,      togglefloating, {0                }  } ,
	{ MODKEY,                       XK_0,      view,           {.ui = ~0         }  } ,
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0         }  } ,
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1          }  } ,
	{ MODKEY,                       XK_period, focusmon,       {.i = +1          }  } ,
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1          }  } ,
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1          }  } ,
	{ MODKEY|ShiftMask,             XK_s,      show,           {0                }  } ,
	{ MODKEY|ShiftMask,             XK_h,      hide,           {0                }  } ,
	{ MODKEY,                       XK_n,      togglealttag,   {0                }  } ,
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5          }  } ,
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5          }  } ,
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0           }  } ,
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0                }  } ,
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0                }  } ,
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2] }  } ,
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0                }  } ,
	{ ClkWinTitle,          0,              Button2,        zoom,           {0                }  } ,
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd     }  } ,
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0                }  } ,
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0                }  } ,
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0                }  } ,
	{ ClkTagBar,            0,              Button1,        view,           {0                }  } ,
	{ ClkTagBar,            0,              Button3,        toggleview,     {0                }  } ,
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0                }  } ,
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0                }  } ,
};

