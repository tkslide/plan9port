/*
 *  fontnames-array takes two fonts, first one
 *  it treats as a proportional-width font and uses
 *  everywhere possible and as a main UI font, while
 *  the second font it treats as a fixed-width font,
 *  changeable to any text window by executing `Font`
 *  from tag window.
 *
 *  Note: `Font` can also be executed
 *  with arguments with any `fontsrv -p .` approved
 *  fonts, thus allowing you to experiment with font
 *  sizes and whether you want anti-aliasing or not.
 */

char *fontnames[2] = {
	/* "/lib/font/bit/profont/profont-12.font", */
	"/mnt/font/Hack-Regular/9a/font",
	/*"/mnt/font/UbuntuMono-Regular/8a/font", */
	"/mnt/font/Ubuntu-Regular/9a/font",
};

/*
 *  globalautoindent tries to guess where to
 *  indent by the context of the previous line.
 *
 *  comes highly suggested.
 */

int globalautoindent	= TRUE;

/*
 *  swapping scroll buttons makes B1 scroll down
 *  instead of up and B2 up instead of down
 */

int	swapscrollbuttons	= FALSE;

/*
 *  bartflag triggers quote unquote experimental
 *  mode, in which instead of mouse following focus
 *  from window to window etc. it in fact, does not.
 *
 *  comes highly suggested.
 */

int bartflag			= TRUE;

/*
 *  colors-constants. now let me take a minute to explain
 *  them. C_TAGBG/FG/HLBG/HLFG are respectively your tag
 *  windows background, foreground, highlighted back- and
 *  foreground color. The same exact applies to C_TXT*-
 *  variants of the same constant just in this case it handles
 *  the colorscheme of the text window.
 *
 *  C_WINBUTTON is the button which you adjust your columns
 *  position with. C_COLBUTTON represents a separate text
 *  windows button. C_TMPBUTTON is the so-called "dirty"
 *  or "unclean" -marker aka notifying the file as modified put
 *  in the middle of C_COLBUTTON. C_SCROLLBG is plain and simple
 *  your scrollbars background color.
 *
 *  the last two defines are for defining the color of the specific
 *  buttons highlight background.
 */

/* Base 16 Ocean */
#define C_TAGBG			0x2B303BFF
#define C_TAGFG			0xC0C5CEFF
#define C_TAGHLBG		0x006CA5FF
#define C_TAGHLFG		0xEFF1F5FF

#define C_TXTBG			0x2B303BFF
#define C_TXTFG			0xC0C5CEFF
#define C_TXTHLBG		0x006CA5FF
#define C_TXTHLFG		0xEFF1F5FF

#define C_WINBUTTON		0x543F5EFF
#define C_COLBUTTON		0x65737EFF
#define C_TMPBUTTON		0x65737EFF
#define C_SCROLLBG		0x222222FF

#define C_BUTTON2HL		0xB47EADFF
#define C_BUTTON3HL		0xBF616AFF
/* Dark scheme */
/*
#define C_TAGFG			0xFDF5E6FF
#define C_TAGBG			0x000000FF
#define C_TAGHLFG		0xFDF5E6FF
#define C_TAGHLBG		0x666666FF

#define C_TXTFG			0xFDF5E6FF
#define C_TXTBG			0x000000FF
#define C_TXTHLBG		0x999999FF
#define C_TXTHLFG		0x000000FF

#define C_WINBUTTON		0xb2b2b2FF
#define C_COLBUTTON		0xaaaa55FF
#define C_TMPBUTTON		0xaaaa55FF
#define C_SCROLLBG		0x666666FF

#define C_BUTTON2HL		0xaaaa55FF
#define C_BUTTON3HL		0xaaaa55FF
*/

/* Light scheme */
/*
#define C_TAGBG			0xFDF5E6FF
#define C_TAGFG			0x000000FF
#define C_TAGHLBG		0x999999FF
#define C_TAGHLFG		0x000000FF

#define C_TXTBG			0xFDF5E6FF
#define C_TXTFG			0x000000FF
#define C_TXTHLBG		0x999999FF
#define C_TXTHLFG		0x000000FF

#define C_WINBUTTON		0x4d4d4dFF
#define C_COLBUTTON		0x55aaaaFF
#define C_TMPBUTTON		0x55aaaaFF
#define C_SCROLLBG		0x999999FF

#define C_BUTTON2HL		0x55aaaaFF
#define C_BUTTON3HL		0x55aaaaFF
*/
