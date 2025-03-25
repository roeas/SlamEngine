#pragma once

// Copy from SL_scancode.h and SDL_keycode.h

#define SL_KEY_UNKNOWN 0

/**
 *  \name Usage page 0x07
 *
 *  These values are from usage page 0x07 (USB keyboard page).
 */
 /* @{ */

#define SL_KEY_A 4
#define SL_KEY_B 5
#define SL_KEY_C 6
#define SL_KEY_D 7
#define SL_KEY_E 8
#define SL_KEY_F 9
#define SL_KEY_G 10
#define SL_KEY_H 11
#define SL_KEY_I 12
#define SL_KEY_J 13
#define SL_KEY_K 14
#define SL_KEY_L 15
#define SL_KEY_M 16
#define SL_KEY_N 17
#define SL_KEY_O 18
#define SL_KEY_P 19
#define SL_KEY_Q 20
#define SL_KEY_R 21
#define SL_KEY_S 22
#define SL_KEY_T 23
#define SL_KEY_U 24
#define SL_KEY_V 25
#define SL_KEY_W 26
#define SL_KEY_X 27
#define SL_KEY_Y 28
#define SL_KEY_Z 29

#define SL_KEY_1 30
#define SL_KEY_2 31
#define SL_KEY_3 32
#define SL_KEY_4 33
#define SL_KEY_5 34
#define SL_KEY_6 35
#define SL_KEY_7 36
#define SL_KEY_8 37
#define SL_KEY_9 38
#define SL_KEY_0 39

#define SL_KEY_RETURN 40
#define SL_KEY_ESCAPE 41
#define SL_KEY_BACKSPACE 42
#define SL_KEY_TAB 43
#define SL_KEY_SPACE 44

#define SL_KEY_MINUS 45
#define SL_KEY_EQUALS 46
#define SL_KEY_LEFTBRACKET 47
#define SL_KEY_RIGHTBRACKET 48
#define SL_KEY_BACKSLASH 49 /**< Located at the lower left of the return
                              *   key on ISO keyboards and at the right end
                              *   of the QWERTY row on ANSI keyboards.
                              *   Produces REVERSE SOLIDUS (backslash) and
                              *   VERTICAL LINE in a US layout REVERSE
                              *   SOLIDUS and VERTICAL LINE in a UK Mac
                              *   layout NUMBER SIGN and TILDE in a UK
                              *   Windows layout DOLLAR SIGN and POUND SIGN
                              *   in a Swiss German layout NUMBER SIGN and
                              *   APOSTROPHE in a German layout GRAVE
                              *   ACCENT and POUND SIGN in a French Mac
                              *   layout and ASTERISK and MICRO SIGN in a
                              *   French Windows layout.
                              */
#define SL_KEY_NONUSHASH 50 /**< ISO USB keyboards actually use this code
                              *   instead of 49 for the same key but all
                              *   OSes I've seen treat the two codes
                              *   identically. So as an implementor unless
                              *   your keyboard generates both of those
                              *   codes and your OS treats them differently
                              *   you should generate #define SL_KEY_BACKSLASH
                              *   instead of this code. As a user you
                              *   should not rely on this code because SDL
                              *   will never generate it with most (all?)
                              *   keyboards.
                              */
#define SL_KEY_SEMICOLON 51
#define SL_KEY_APOSTROPHE 52
#define SL_KEY_GRAVE 53 /**< Located in the top left corner (on both ANSI
                          *   and ISO keyboards). Produces GRAVE ACCENT and
                          *   TILDE in a US Windows layout and in US and UK
                          *   Mac layouts on ANSI keyboards GRAVE ACCENT
                          *   and NOT SIGN in a UK Windows layout SECTION
                          *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                          *   layouts on ISO keyboards SECTION SIGN and
                          *   DEGREE SIGN in a Swiss German layout (Mac:
                          *   only on ISO keyboards) CIRCUMFLEX ACCENT and
                          *   DEGREE SIGN in a German layout (Mac: only on
                          *   ISO keyboards) SUPERSCRIPT TWO and TILDE in a
                          *   French Windows layout COMMERCIAL AT and
                          *   NUMBER SIGN in a French Mac layout on ISO
                          *   keyboards and LESS-THAN SIGN and GREATER-THAN
                          *   SIGN in a Swiss German German or French Mac
                          *   layout on ANSI keyboards.
                          */
#define SL_KEY_COMMA 54
#define SL_KEY_PERIOD 55
#define SL_KEY_SLASH 56

#define SL_KEY_CAPSLOCK 57

#define SL_KEY_F1 58
#define SL_KEY_F2 59
#define SL_KEY_F3 60
#define SL_KEY_F4 61
#define SL_KEY_F5 62
#define SL_KEY_F6 63
#define SL_KEY_F7 64
#define SL_KEY_F8 65
#define SL_KEY_F9 66
#define SL_KEY_F10 67
#define SL_KEY_F11 68
#define SL_KEY_F12 69

#define SL_KEY_PRINTSCREEN 70
#define SL_KEY_SCROLLLOCK 71
#define SL_KEY_PAUSE 72
#define SL_KEY_INSERT 73 /**< insert on PC help on some Mac keyboards (but
                               does send code 73 not 117) */
#define SL_KEY_HOME 74
#define SL_KEY_PAGEUP 75
#define SL_KEY_DELETE 76
#define SL_KEY_END 77
#define SL_KEY_PAGEDOWN 78
#define SL_KEY_RIGHT 79
#define SL_KEY_LEFT 80
#define SL_KEY_DOWN 81
#define SL_KEY_UP 82

#define SL_KEY_NUMLOCKCLEAR 83 /**< num lock on PC clear on Mac keyboards
                                 */
#define SL_KEY_KP_DIVIDE 84
#define SL_KEY_KP_MULTIPLY 85
#define SL_KEY_KP_MINUS 86
#define SL_KEY_KP_PLUS 87
#define SL_KEY_KP_ENTER 88
#define SL_KEY_KP_1 89
#define SL_KEY_KP_2 90
#define SL_KEY_KP_3 91
#define SL_KEY_KP_4 92
#define SL_KEY_KP_5 93
#define SL_KEY_KP_6 94
#define SL_KEY_KP_7 95
#define SL_KEY_KP_8 96
#define SL_KEY_KP_9 97
#define SL_KEY_KP_0 98
#define SL_KEY_KP_PERIOD 99

#define SL_KEY_NONUSBACKSLASH 100 /**< This is the additional key that ISO
                                    *   keyboards have over ANSI ones
                                    *   located between left shift and Y.
                                    *   Produces GRAVE ACCENT and TILDE in a
                                    *   US or UK Mac layout REVERSE SOLIDUS
                                    *   (backslash) and VERTICAL LINE in a
                                    *   US or UK Windows layout and
                                    *   LESS-THAN SIGN and GREATER-THAN SIGN
                                    *   in a Swiss German German or French
                                    *   layout. */
#define SL_KEY_APPLICATION 101 /**< windows contextual menu compose */
#define SL_KEY_POWER 102 /**< The USB document says this is a status flag
                           *   not a physical key - but some Mac keyboards
                           *   do have a power key. */
#define SL_KEY_KP_EQUALS 103
#define SL_KEY_F13 104
#define SL_KEY_F14 105
#define SL_KEY_F15 106
#define SL_KEY_F16 107
#define SL_KEY_F17 108
#define SL_KEY_F18 109
#define SL_KEY_F19 110
#define SL_KEY_F20 111
#define SL_KEY_F21 112
#define SL_KEY_F22 113
#define SL_KEY_F23 114
#define SL_KEY_F24 115
#define SL_KEY_EXECUTE 116
#define SL_KEY_HELP 117    /**< AL Integrated Help Center */
#define SL_KEY_MENU 118    /**< Menu (show menu) */
#define SL_KEY_SELECT 119
#define SL_KEY_STOP 120    /**< AC Stop */
#define SL_KEY_AGAIN 121   /**< AC Redo/Repeat */
#define SL_KEY_UNDO 122    /**< AC Undo */
#define SL_KEY_CUT 123     /**< AC Cut */
#define SL_KEY_COPY 124    /**< AC Copy */
#define SL_KEY_PASTE 125   /**< AC Paste */
#define SL_KEY_FIND 126    /**< AC Find */
#define SL_KEY_MUTE 127
#define SL_KEY_VOLUMEUP 128
#define SL_KEY_VOLUMEDOWN 129
/* not sure whether there's a reason to enable these */
/*     #define SL_KEY_LOCKINGCAPSLOCK 130  */
/*     #define SL_KEY_LOCKINGNUMLOCK 131 */
/*     #define SL_KEY_LOCKINGSCROLLLOCK 132 */
#define SL_KEY_KP_COMMA 133
#define SL_KEY_KP_EQUALSAS400 134

#define SL_KEY_INTERNATIONAL1 135 /**< used on Asian keyboards see
                                        footnotes in USB doc */
#define SL_KEY_INTERNATIONAL2 136
#define SL_KEY_INTERNATIONAL3 137 /**< Yen */
#define SL_KEY_INTERNATIONAL4 138
#define SL_KEY_INTERNATIONAL5 139
#define SL_KEY_INTERNATIONAL6 140
#define SL_KEY_INTERNATIONAL7 141
#define SL_KEY_INTERNATIONAL8 142
#define SL_KEY_INTERNATIONAL9 143
#define SL_KEY_LANG1 144 /**< Hangul/English toggle */
#define SL_KEY_LANG2 145 /**< Hanja conversion */
#define SL_KEY_LANG3 146 /**< Katakana */
#define SL_KEY_LANG4 147 /**< Hiragana */
#define SL_KEY_LANG5 148 /**< Zenkaku/Hankaku */
#define SL_KEY_LANG6 149 /**< reserved */
#define SL_KEY_LANG7 150 /**< reserved */
#define SL_KEY_LANG8 151 /**< reserved */
#define SL_KEY_LANG9 152 /**< reserved */

#define SL_KEY_ALTERASE 153    /**< Erase-Eaze */
#define SL_KEY_SYSREQ 154
#define SL_KEY_CANCEL 155      /**< AC Cancel */
#define SL_KEY_CLEAR 156
#define SL_KEY_PRIOR 157
#define SL_KEY_RETURN2 158
#define SL_KEY_SEPARATOR 159
#define SL_KEY_OUT 160
#define SL_KEY_OPER 161
#define SL_KEY_CLEARAGAIN 162
#define SL_KEY_CRSEL 163
#define SL_KEY_EXSEL 164

#define SL_KEY_KP_00 176
#define SL_KEY_KP_000 177
#define SL_KEY_THOUSANDSSEPARATOR 178
#define SL_KEY_DECIMALSEPARATOR 179
#define SL_KEY_CURRENCYUNIT 180
#define SL_KEY_CURRENCYSUBUNIT 181
#define SL_KEY_KP_LEFTPAREN 182
#define SL_KEY_KP_RIGHTPAREN 183
#define SL_KEY_KP_LEFTBRACE 184
#define SL_KEY_KP_RIGHTBRACE 185
#define SL_KEY_KP_TAB 186
#define SL_KEY_KP_BACKSPACE 187
#define SL_KEY_KP_A 188
#define SL_KEY_KP_B 189
#define SL_KEY_KP_C 190
#define SL_KEY_KP_D 191
#define SL_KEY_KP_E 192
#define SL_KEY_KP_F 193
#define SL_KEY_KP_XOR 194
#define SL_KEY_KP_POWER 195
#define SL_KEY_KP_PERCENT 196
#define SL_KEY_KP_LESS 197
#define SL_KEY_KP_GREATER 198
#define SL_KEY_KP_AMPERSAND 199
#define SL_KEY_KP_DBLAMPERSAND 200
#define SL_KEY_KP_VERTICALBAR 201
#define SL_KEY_KP_DBLVERTICALBAR 202
#define SL_KEY_KP_COLON 203
#define SL_KEY_KP_HASH 204
#define SL_KEY_KP_SPACE 205
#define SL_KEY_KP_AT 206
#define SL_KEY_KP_EXCLAM 207
#define SL_KEY_KP_MEMSTORE 208
#define SL_KEY_KP_MEMRECALL 209
#define SL_KEY_KP_MEMCLEAR 210
#define SL_KEY_KP_MEMADD 211
#define SL_KEY_KP_MEMSUBTRACT 212
#define SL_KEY_KP_MEMMULTIPLY 213
#define SL_KEY_KP_MEMDIVIDE 214
#define SL_KEY_KP_PLUSMINUS 215
#define SL_KEY_KP_CLEAR 216
#define SL_KEY_KP_CLEARENTRY 217
#define SL_KEY_KP_BINARY 218
#define SL_KEY_KP_OCTAL 219
#define SL_KEY_KP_DECIMAL 220
#define SL_KEY_KP_HEXADECIMAL 221

#define SL_KEY_LCTRL 224
#define SL_KEY_LSHIFT 225
#define SL_KEY_LALT 226 /**< alt option */
#define SL_KEY_LGUI 227 /**< windows command (apple) meta */
#define SL_KEY_RCTRL 228
#define SL_KEY_RSHIFT 229
#define SL_KEY_RALT 230 /**< alt gr option */
#define SL_KEY_RGUI 231 /**< windows command (apple) meta */

#define SL_KEY_MODE 257    /**< I'm not sure if this is really not covered
                             *   by any of the above but since there's a
                             *   special #define SL_KMOD_MODE for it I'm adding it here
                             */

                             /* @} *//* Usage page 0x07 */

                             /**
                              *  \name Usage page 0x0C
                              *
                              *  These values are mapped from usage page 0x0C (USB consumer page).
                              *
                              *  There are way more keys in the spec than we can represent in the
                              *  current scancode range so pick the ones that commonly come up in
                              *  real world usage.
                              */
                              /* @{ */

#define SL_KEY_SLEEP 258                   /**< Sleep */
#define SL_KEY_WAKE 259                    /**< Wake */

#define SL_KEY_CHANNEL_INCREMENT 260       /**< Channel Increment */
#define SL_KEY_CHANNEL_DECREMENT 261       /**< Channel Decrement */

#define SL_KEY_MEDIA_PLAY 262          /**< Play */
#define SL_KEY_MEDIA_PAUSE 263         /**< Pause */
#define SL_KEY_MEDIA_RECORD 264        /**< Record */
#define SL_KEY_MEDIA_FAST_FORWARD 265  /**< Fast Forward */
#define SL_KEY_MEDIA_REWIND 266        /**< Rewind */
#define SL_KEY_MEDIA_NEXT_TRACK 267    /**< Next Track */
#define SL_KEY_MEDIA_PREVIOUS_TRACK 268 /**< Previous Track */
#define SL_KEY_MEDIA_STOP 269          /**< Stop */
#define SL_KEY_MEDIA_EJECT 270         /**< Eject */
#define SL_KEY_MEDIA_PLAY_PAUSE 271    /**< Play / Pause */
#define SL_KEY_MEDIA_SELECT 272        /* Media Select */

#define SL_KEY_AC_NEW 273              /**< AC New */
#define SL_KEY_AC_OPEN 274             /**< AC Open */
#define SL_KEY_AC_CLOSE 275            /**< AC Close */
#define SL_KEY_AC_EXIT 276             /**< AC Exit */
#define SL_KEY_AC_SAVE 277             /**< AC Save */
#define SL_KEY_AC_PRINT 278            /**< AC Print */
#define SL_KEY_AC_PROPERTIES 279       /**< AC Properties */

#define SL_KEY_AC_SEARCH 280           /**< AC Search */
#define SL_KEY_AC_HOME 281             /**< AC Home */
#define SL_KEY_AC_BACK 282             /**< AC Back */
#define SL_KEY_AC_FORWARD 283          /**< AC Forward */
#define SL_KEY_AC_STOP 284             /**< AC Stop */
#define SL_KEY_AC_REFRESH 285          /**< AC Refresh */
#define SL_KEY_AC_BOOKMARKS 286        /**< AC Bookmarks */

/* @} *//* Usage page 0x0C */


/**
 *  \name Mobile keys
 *
 *  These are values that are often used on mobile phones.
 */
 /* @{ */

#define SL_KEY_SOFTLEFT 287 /**< Usually situated below the display on phones and
                                  used as a multi-function feature key for selecting
                                  a software defined function shown on the bottom left
                                  of the display. */
#define SL_KEY_SOFTRIGHT 288 /**< Usually situated below the display on phones and
                                   used as a multi-function feature key for selecting
                                   a software defined function shown on the bottom right
                                   of the display. */
#define SL_KEY_CALL 289 /**< Used for accepting phone calls. */
#define SL_KEY_ENDCALL 290 /**< Used for rejecting phone calls. */

/* @} *//* Mobile keys */

/* Add any other keys here. */

#define SL_KEY_RESERVED 400    /**< 400-500 reserved for dynamic keycodes */

#define SL_KEY_COUNT 512 /**< not a key just marks the number of scancodes for array bounds */

#define SL_KEY_MOD_NONE   0x0000u /**< no modifier is applicable. */
#define SL_KEY_MOD_LSHIFT 0x0001u /**< the left Shift key is down. */
#define SL_KEY_MOD_RSHIFT 0x0002u /**< the right Shift key is down. */
#define SL_KEY_MOD_LEVEL5 0x0004u /**< the Level 5 Shift key is down. */
#define SL_KEY_MOD_LCTRL  0x0040u /**< the left Ctrl (Control) key is down. */
#define SL_KEY_MOD_RCTRL  0x0080u /**< the right Ctrl (Control) key is down. */
#define SL_KEY_MOD_LALT   0x0100u /**< the left Alt key is down. */
#define SL_KEY_MOD_RALT   0x0200u /**< the right Alt key is down. */
#define SL_KEY_MOD_LGUI   0x0400u /**< the left GUI key (often the Windows key) is down. */
#define SL_KEY_MOD_RGUI   0x0800u /**< the right GUI key (often the Windows key) is down. */
#define SL_KEY_MOD_NUM    0x1000u /**< the Num Lock key (may be located on an extended keypad) is down. */
#define SL_KEY_MOD_CAPS   0x2000u /**< the Caps Lock key is down. */
#define SL_KEY_MOD_MODE   0x4000u /**< the !AltGr key is down. */
#define SL_KEY_MOD_SCROLL 0x8000u /**< the Scroll Lock key is down. */
#define SL_KEY_MOD_CTRL   (SDL_KMOD_LCTRL | SDL_KMOD_RCTRL)   /**< Any Ctrl key is down. */
#define SL_KEY_MOD_SHIFT  (SDL_KMOD_LSHIFT | SDL_KMOD_RSHIFT) /**< Any Shift key is down. */
#define SL_KEY_MOD_ALT    (SDL_KMOD_LALT | SDL_KMOD_RALT)     /**< Any Alt key is down. */
#define SL_KEY_MOD_GUI    (SDL_KMOD_LGUI | SDL_KMOD_RGUI)     /**< Any GUI key is down. */
