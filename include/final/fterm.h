/***********************************************************************
* fterm.h - Base class for terminal detection and control              *
*                                                                      *
* This file is part of the Final Cut widget toolkit                    *
*                                                                      *
* Copyright 2012-2018 Markus Gans                                      *
*                                                                      *
* The Final Cut is free software; you can redistribute it and/or       *
* modify it under the terms of the GNU Lesser General Public License   *
* as published by the Free Software Foundation; either version 3 of    *
* the License, or (at your option) any later version.                  *
*                                                                      *
* The Final Cut is distributed in the hope that it will be useful,     *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

/*  Base class
 *  ══════════
 *
 * ▕▔▔▔▔▔▔▔▏1       1▕▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ FTerm ▏-┬- - - -▕ FOptiMove ▏
 * ▕▁▁▁▁▁▁▁▏ :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FOptiAttr ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      *▕▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FString ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      *▕▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FPoint ▏
 *           :       ▕▁▁▁▁▁▁▁▁▏
 *           :
 *           :      *▕▔▔▔▔▔▔▔▏
 *           └- - - -▕ FRect ▏
 *                   ▕▁▁▁▁▁▁▁▏
 */

#ifndef FTERM_H
#define FTERM_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

// Typecast to c-string
#define C_STR const_cast<char*>

#include "final/fconfig.h"

#if defined(__linux__)
  #include <linux/fb.h>        // Linux framebuffer console

  #if defined(__x86_64__) || defined(__i386) || defined(__arm__)
    #include <sys/io.h>        // <asm/io.h> is deprecated
  #endif

  #include <sys/kd.h>
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__)
  #include <sys/consio.h>
  #include <sys/kbio.h>
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__)
  #include <sys/time.h>
  #include <dev/wscons/wsconsio.h>
#endif

#include <sys/ioctl.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <langinfo.h>
#include <termios.h>


#if defined(__sun) && defined(__SVR4)
  #include <termio.h>
  typedef struct termio SGTTY;
  typedef struct termios SGTTYS;

  #ifdef _LP64
    typedef unsigned int chtype;
  #else
    typedef unsigned long chtype;
  #endif

  #include <term.h>  // termcap
#else
  #include <term.h>  // termcap
#endif

#ifdef F_HAVE_LIBGPM
  #undef buttons  // from term.h
#endif

#if F_HAVE_GETTTYNAM && F_HAVE_TTYENT_H
  #include <ttyent.h>
#endif


#include <unistd.h>

#include <clocale>
#include <cmath>
#include <csignal>
#include <map>
#include <queue>
#include <string>

#include "final/fc.h"
#include "final/fcolorpalette.h"
#include "final/fmouse.h"
#include "final/fobject.h"
#include "final/foptiattr.h"
#include "final/foptimove.h"
#include "final/fpoint.h"
#include "final/frect.h"
#include "final/fstring.h"
#include "final/ftermcap.h"


//----------------------------------------------------------------------
// class FTerm
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FTerm
{
  public:
    // Typedefs
    typedef FOptiAttr::char_data  char_data;

#if defined(__linux__)
    static struct modifier_key  // bit field
    {
      uChar shift  : 1;  // 0..1
      uChar alt_gr : 1;  // 0..1
      uChar ctrl   : 1;  // 0..1
      uChar alt    : 1;  // 0..1
      uChar        : 4;  // padding bits
    } mod_key;
#endif

    // Constructor
    explicit FTerm (bool = false);

    // Destructor
    virtual ~FTerm();

    // Accessors
    virtual const char*   getClassName() const;
    static termios        getTTY();
    static int            getLineNumber();
    static int            getColumnNumber();
    static const FString  getKeyName (int);
    static FMouseControl* getMouseControl();

#if defined(__linux__)
    static modifier_key&  getLinuxModifierKey();
#endif

    static char*          getTermType();
    static char*          getTermFileName();
    static int            getTabstop();
    static int            getMaxColor();

#if defined(__linux__)
    static fc::linuxConsoleCursorStyle getLinuxConsoleCursorStyle();
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__)
    static fc::freebsdConsoleCursorStyle getFreeBSDConsoleCursorStyle();
#endif

#if DEBUG
    static const FString& getAnswerbackString();
    static const FString& getSecDAString();
#endif

    // Inquiries
    static bool           isKeypressTimeout (timeval*);
    static bool           isNormal (char_data*&);
    static bool           isRaw();
    static bool           hasPCcharset();
    static bool           hasUTF8();
    static bool           hasVT100();
    static bool           hasASCII();
    static bool           isMonochron();
    static bool           isXTerminal();
    static bool           isAnsiTerminal();
    static bool           isRxvtTerminal();
    static bool           isUrxvtTerminal();
    static bool           isMltermTerminal();
    static bool           isPuttyTerminal();
    static bool           isKdeTerminal();
    static bool           isGnomeTerminal();
    static bool           isKtermTerminal();
    static bool           isTeraTerm();
    static bool           isSunTerminal();
    static bool           isCygwinTerminal();
    static bool           isMinttyTerm();
    static bool           isLinuxTerm();
    static bool           isScreenTerm();
    static bool           isTmuxTerm();
    static bool           isInputDataPending();
    static bool           isNewFont();
    static bool           isUTF8();
    static bool           isUTF8_linux_terminal();

    // Mutators
    static bool           setCursorOptimisation (bool);
    static void           setXTermDefaultColors (bool);

#if defined(__linux__)
    static void           setLinuxConsoleCursorStyle \
                              (fc::linuxConsoleCursorStyle, bool);
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__)
    static void           setFreeBSDConsoleCursorStyle \
                              (fc::freebsdConsoleCursorStyle, bool);
#endif

    static void           setKeypressTimeout (const long);
    static void           setDblclickInterval (const long);
    static void           setTTY (const termios&);
    static void           noHardwareEcho();
    static bool           setRawMode (bool);
    static bool           setRawMode();
    static bool           unsetRawMode();
    static bool           setCookedMode();
    static void           disableAltScreen();
    static bool           setUTF8 (bool);
    static bool           setUTF8();
    static bool           unsetUTF8();
    static bool           setNonBlockingInput (bool);
    static bool           setNonBlockingInput();
    static bool           unsetNonBlockingInput();

    // Methods
    static int            parseKeyString (char[], int, timeval*);
    static bool&          unprocessedInput();
    static bool           setVGAFont();
    static bool           setNewFont();
    static bool           setOldFont();
    static char*          moveCursor (int, int, int, int);
    static void           printMoveDurations();
    static char*          enableCursor();
    static char*          disableCursor();
    static void           detectTermSize();
    static void           setTermSize (int, int);
    static void           setKDECursor (fc::kdeKonsoleCursorShape);
    static const FString* getXTermFont();
    static const FString* getXTermTitle();
    static const FString  getXTermColorName (int);
    static void           setXTermCursorStyle (fc::xtermCursorStyle);
    static void           setXTermTitle (const FString&);
    static void           setXTermForeground (const FString&);
    static void           setXTermBackground (const FString&);
    static void           setXTermCursorColor (const FString&);
    static void           setXTermMouseForeground (const FString&);
    static void           setXTermMouseBackground (const FString&);
    static void           setXTermHighlightBackground (const FString&);
    static void           setXTermDefaults();
    static void           resetXTermColors();
    static void           resetXTermForeground();
    static void           resetXTermBackground();
    static void           resetXTermCursorColor();
    static void           resetXTermMouseForeground();
    static void           resetXTermMouseBackground();
    static void           resetXTermHighlightBackground();
    static void           resetXTermDefaults();
    static void           saveColorMap();
    static void           resetColorMap();
    static void           setPalette (short, int, int, int);
    static void           setBeep (int, int);
    static void           resetBeep();
    static void           beep();

    static void           setEncoding (fc::encoding);
    static fc::encoding   getEncoding();
    static std::string    getEncodingString();

    static bool           scrollTermForward();
    static bool           scrollTermReverse();

    static const FString  getAnswerbackMsg();
    static const FString  getSecDA();

    // function pointer -> static function
    static int            (*Fputchar)(int);

    static void           putstringf (const char[], ...)
#if defined(__clang__)
      __attribute__((__format__ (__printf__, 1, 2)))
#elif defined(__GNUC__)
      __attribute__ ((format (printf, 1, 2)))
#endif
                          ;
    static void           putstring (const char[], int = 1);

#if defined(__sun) && defined(__SVR4)
    static int            putchar_ASCII (register char);
#endif

    static int            putchar_ASCII (register int);
    static int            putchar_UTF8  (register int);
    static int            UTF8decode (const char[]);

#if DEBUG
    static char           termtype_256color[256];
    static char           termtype_Answerback[256];
    static char           termtype_SecDA[256];
    static int            framebuffer_bpp;
#endif

  protected:
    // Methods
#if defined(__linux__)
    static void           initLinuxConsoleCharMap();
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__)
    static void           initFreeBSDConsoleCharMap();
#endif

    static bool           charEncodable (uInt);
    static uInt           charEncode (uInt);
    static uInt           charEncode (uInt, fc::encoding);
    static char*          changeAttribute ( char_data*&
                                          , char_data*& );
    static bool           hasChangedTermSize();
    static void           changeTermSizeFinished();
    static void           xtermMetaSendsESC (bool);
    static void           exitWithMessage (std::string)
    #if defined(__clang__) || defined(__GNUC__)
      __attribute__((noreturn))
    #endif
                          ;
    // Data Members
    static int            stdin_no;
    static int            stdout_no;
    static int            erase_ch_length;
    static int            repeat_char_length;
    static int            clr_bol_length;
    static int            clr_eol_length;
    static int            cursor_addres_lengths;
    static bool           NewFont;
    static bool           VGAFont;
    static bool           no_shadow_character;
    static bool           no_half_block_character;
    static bool           cursor_optimisation;
    static bool           xterm_default_colors;
    static bool           use_alternate_screen;
    static fc::encoding   term_encoding;
    static char           exit_message[8192];

    static struct initializationValues
    {
      public:
        void setDefault()
        {
          terminal_detection = true;
          cursor_optimisation = true;
          color_change = true;
          vgafont = false;
          newfont = false;
          encoding = fc::UNKNOWN;
        }

        uInt8 terminal_detection  : 1;
        uInt8 cursor_optimisation : 1;
        uInt8 color_change        : 1;
        uInt8 vgafont             : 1;
        uInt8 newfont             : 1;
        uInt8                     : 3;  // padding bits
        fc::encoding encoding;
    } init_values;

  private:
    // Typedefs
    typedef FTermcap::tcap_map termcap_map;

    typedef struct
    {
      uChar red;
      uChar green;
      uChar blue;
    } dacreg;

    // Constants
    static const int NEED_MORE_DATA = -1;  // parseKeyString return value

    // Disable copy constructor
    FTerm (const FTerm&);
    // Disable assignment operator (=)
    FTerm& operator = (const FTerm&);

#if defined(__linux__)
    static int            isLinuxConsole();
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__)
    static bool           isFreeBSDConsole();
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__)
    static bool           isWSConsConsole();
#endif

    // Methods
#if defined(__linux__)
#if defined(__x86_64__) || defined(__i386) || defined(__arm__)
    static uInt16         getInputStatusRegisterOne();
    static uChar          readAttributeController (uChar);
    static void           writeAttributeController (uChar, uChar);
    static uChar          getAttributeMode();
    static void           setAttributeMode (uChar);
    static int            setBlinkAsIntensity (bool);
#endif
    static int            getFramebuffer_bpp();
#endif

    static int            openConsole();
    static int            closeConsole();
    static void           getSystemTermType();
    static void           getTTYtype();
#if F_HAVE_GETTTYNAM
    static bool           getTTYSFileEntry();
#endif
    static void           storeTTYsettings();
    static void           restoreTTYsettings();

#if defined(__linux__)
    static int            getScreenFont();
    static int            setScreenFont ( uChar[], uInt, uInt, uInt
                                        , bool = false );
    static int            setUnicodeMap (struct unimapdesc*);
    static int            getUnicodeMap ();
    static void           initLinuxConsole();
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__)
    static bool           saveFreeBSDAltKey();
    static bool           setFreeBSDAltKey (uInt);
    static bool           setFreeBSDAlt2Meta();
    static bool           resetFreeBSDAlt2Meta();
    static void           initFreeBSDConsole();
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__)
    static bool           saveWSConsEncoding();
    static bool           setWSConsEncoding (kbd_t);
    static bool           setWSConsMetaEsc();
    static bool           resetWSConsEncoding();
    static void           initWSConsConsole();
#endif

    static uInt           getBaudRate (const struct termios*);
    static void           init_global_values();
    static void           detectTerminal();
    static void           termtypeAnalysis();
    static bool           get256colorEnvString();
    static char*          termtype_256color_quirks();
    static char*          init_256colorTerminal();
    static char*          determineMaxColor (char[]);
    static char*          parseAnswerbackMsg (char[]);
    static char*          parseSecDA (char[]);
    static char*          secDA_Analysis (char[]);
    static char*          secDA_Analysis_0 (char[]);
    static char*          secDA_Analysis_1 (char[]);
    static char*          secDA_Analysis_24 (char[]);
    static char*          secDA_Analysis_32 (char[]);
    static char*          secDA_Analysis_77 (char[]);
    static char*          secDA_Analysis_82 (char[]);
    static char*          secDA_Analysis_83 (char[]);
    static char*          secDA_Analysis_85 (char[]);
    static void           oscPrefix();
    static void           oscPostfix();
    static void           init_alt_charset();
    static void           init_pc_charset();
    static void           init_cygwin_charmap();
    static void           init_teraterm_charmap();
    static void           init_termcaps();
    static void           init_termcaps_booleans();
    static void           init_termcaps_numeric();
    static void           init_termcaps_strings (char*&);
    static void           init_termcaps_quirks();
#if defined(__FreeBSD__) || defined(__DragonFly__)
    static void           init_termcaps_freebsd_quirks();
#endif
    static void           init_termcaps_cygwin_quirks();
    static void           init_termcaps_linux_quirks();
    static void           init_termcaps_xterm_quirks();
    static void           init_termcaps_rxvt_quirks();
    static void           init_termcaps_vte_quirks();
    static void           init_termcaps_putty_quirks();
    static void           init_termcaps_teraterm_quirks();
    static void           init_termcaps_sun_quirks();
    static void           init_termcaps_screen_quirks();
    static void           init_termcaps_general_quirks();
    static void           init_termcaps_keys (char*&);
    static void           init_OptiMove();
    static void           init_OptiAttr();
    static void           init_font();
    static void           init_locale();
    static void           init_encoding();
    static void           redefineColorPalette();
    static void           restoreColorPalette();
    static void           enableMouse();
    static void           disableMouse();
    static void           useAlternateScreenBuffer();
    static void           useNormalScreenBuffer();
    static void           captureXTermFontAndTitle();
    void                  allocationValues();
    void                  deallocationValues();
    void                  init();
    void                  initOSspecifics();
    void                  finish();
    void                  finishOSspecifics1();
    void                  finishOSspecifics2();
    static uInt           cp437_to_unicode (uChar);
    static int            getMouseProtocolKey (char[]);
    static int            getTermcapKey (char[], int);
    static int            getMetaKey (char[], int, timeval*);
    static int            getSingleKey (char[], int);
    static void           setSignalHandler();
    static void           resetSignalHandler();
    static void           signal_handler (int);

    // Data Members
    static std::map <uChar,uChar>* vt100_alt_char;
    static std::map <std::string,fc::encoding>* encoding_set;
    static FTermcap::tcap_map* tcap;

    static bool           decscusr_support;
    static bool           terminal_detection;
    static bool           raw_mode;
    static bool           input_data_pending;
    static bool           non_blocking_stdin;
    static bool           pc_charset_console;
    static bool           utf8_input;
    static bool           utf8_state;
    static bool           utf8_console;
    static bool           utf8_linux_terminal;
    static bool           force_vt100;
    static bool           vt100_console;
    static bool           ascii_console;
    static bool           color256;
    static bool           monochron;
    static bool           xterm_terminal;
    static bool           ansi_terminal;
    static bool           rxvt_terminal;
    static bool           urxvt_terminal;
    static bool           mlterm_terminal;
    static bool           putty_terminal;
    static bool           kde_konsole;
    static bool           gnome_terminal;
    static bool           kterm_terminal;
    static bool           tera_terminal;
    static bool           sun_terminal;
    static bool           cygwin_terminal;
    static bool           mintty_terminal;
    static bool           linux_terminal;
    static bool           netbsd_terminal;
    static bool           openbsd_terminal;
    static bool           screen_terminal;
    static bool           tmux_terminal;
    static char           termtype[256];
    static char           termfilename[256];
    static char*          locale_name;
    static char*          locale_xterm;
    static FRect*         term;     // current terminal geometry

    static int            gnome_terminal_id;
    static int            stdin_status_flags;
    static int            fd_tty;
    static uInt           baudrate;
    static long           key_timeout;
    static bool           resize_term;

    static struct         termios term_init;
    static fc::linuxConsoleCursorStyle linux_console_cursor_style;
    static fc::freebsdConsoleCursorStyle freebsd_console_cursor_style;
    static struct         console_font_op screen_font;
    static struct         unimapdesc      screen_unicode_map;

#if defined(__FreeBSD__) || defined(__DragonFly__)
    static uInt           bsd_alt_keymap;
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__)
    static kbd_t          wscons_keyboard_encoding;
#endif

    static FOptiMove*     opti_move;
    static FOptiAttr*     opti_attr;
    static FMouseControl* mouse;
    static const FString* xterm_font;
    static const FString* xterm_title;
    static const FString* answer_back;
    static const FString* sec_da;

    static struct colorEnv
    {
      void setDefault()
      {
        string1 = 0;
        string2 = 0;
        string3 = 0;
        string4 = 0;
        string5 = 0;
        string6 = 0;
      }

      char* string1;
      char* string2;
      char* string3;
      char* string4;
      char* string5;
      char* string6;
    } color_env;

    static struct secondaryDA
    {
      void setDefault()
      {
        terminal_id_type = -1;
        terminal_id_version = -1;
        terminal_id_hardware = -1;
      }

      int terminal_id_type;
      int terminal_id_version;
      int terminal_id_hardware;
    } secondary_da;

    struct
    {
      dacreg d[16];
    } color_map;
};

#pragma pack(pop)

// FTerm inline functions
//----------------------------------------------------------------------
inline const char* FTerm::getClassName() const
{ return "FTerm"; }

//----------------------------------------------------------------------
inline char* FTerm::getTermType()
{ return termtype; }

//----------------------------------------------------------------------
inline char* FTerm::getTermFileName()
{ return termfilename; }

//----------------------------------------------------------------------
inline int FTerm::getTabstop()
{ return FTermcap::tabstop; }

//----------------------------------------------------------------------
inline int FTerm::getMaxColor()
{ return FTermcap::max_color; }

#if DEBUG
//----------------------------------------------------------------------
inline const FString& FTerm::getAnswerbackString()
{ return ( answer_back ) ? *answer_back : fc::emptyFString::get(); }

//----------------------------------------------------------------------
inline const FString& FTerm::getSecDAString()
{ return ( sec_da ) ? *sec_da : fc::emptyFString::get(); }
#endif

//----------------------------------------------------------------------
inline bool FTerm::isKeypressTimeout (timeval* time)
{ return FObject::isTimeout (time, key_timeout); }

//----------------------------------------------------------------------
inline bool FTerm::isRaw()
{ return raw_mode; }

//----------------------------------------------------------------------
inline bool FTerm::hasPCcharset()
{ return pc_charset_console; }

//----------------------------------------------------------------------
inline bool FTerm::hasUTF8()
{ return utf8_console; }

//----------------------------------------------------------------------
inline bool FTerm::hasVT100()
{ return vt100_console; }

//----------------------------------------------------------------------
inline bool FTerm::hasASCII()
{ return ascii_console; }

//----------------------------------------------------------------------
inline bool FTerm::isMonochron()
{ return monochron; }

//----------------------------------------------------------------------
inline bool FTerm::isXTerminal()
{ return xterm_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isAnsiTerminal()
{ return ansi_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isRxvtTerminal()
{ return rxvt_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isUrxvtTerminal()
{ return urxvt_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isMltermTerminal()
{ return mlterm_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isPuttyTerminal()
{ return putty_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isKdeTerminal()
{ return kde_konsole; }

//----------------------------------------------------------------------
inline bool FTerm::isGnomeTerminal()
{ return gnome_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isKtermTerminal()
{ return kterm_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isTeraTerm()
{ return tera_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isSunTerminal()
{ return sun_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isCygwinTerminal()
{ return cygwin_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isMinttyTerm()
{ return mintty_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isLinuxTerm()
{ return linux_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isScreenTerm()
{ return screen_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isTmuxTerm()
{ return tmux_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isInputDataPending()
{ return input_data_pending; }

//----------------------------------------------------------------------
inline bool FTerm::isNewFont()
{ return NewFont; }

//----------------------------------------------------------------------
inline bool FTerm::isUTF8()
{ return utf8_state; }

//----------------------------------------------------------------------
inline bool FTerm::isUTF8_linux_terminal()
{ return utf8_linux_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::setCursorOptimisation (bool on)
{ return cursor_optimisation = ( on ) ? true : false; }

//----------------------------------------------------------------------
inline void FTerm::setXTermDefaultColors (bool on)
{ xterm_default_colors = on; }

//----------------------------------------------------------------------
inline void FTerm::setKeypressTimeout (const long timeout)
{ key_timeout = timeout; }

//----------------------------------------------------------------------
inline bool FTerm::setRawMode()
{ return setRawMode(true); }

//----------------------------------------------------------------------
inline bool FTerm::unsetRawMode()
{ return setRawMode(false); }

//----------------------------------------------------------------------
inline bool FTerm::setCookedMode()
{ return setRawMode(false); }

//----------------------------------------------------------------------
inline void FTerm::disableAltScreen()
{ use_alternate_screen = false; }

//----------------------------------------------------------------------
inline bool FTerm::setUTF8()
{ return setUTF8(true); }

//----------------------------------------------------------------------
inline bool FTerm::unsetUTF8()
{ return setUTF8(false); }

//----------------------------------------------------------------------
inline bool FTerm::setNonBlockingInput()
{ return setNonBlockingInput(true); }

//----------------------------------------------------------------------
inline bool FTerm::unsetNonBlockingInput()
{ return setNonBlockingInput(false); }

//----------------------------------------------------------------------
inline bool FTerm::hasChangedTermSize()
{ return resize_term; }

//----------------------------------------------------------------------
inline void FTerm::changeTermSizeFinished()
{ resize_term = false; }


#endif  // FTERM_H
