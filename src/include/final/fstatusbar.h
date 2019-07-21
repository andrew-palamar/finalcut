/***********************************************************************
* fstatusbar.h - Widget FStatusBar and FStatusKey                      *
*                                                                      *
* This file is part of the Final Cut widget toolkit                    *
*                                                                      *
* Copyright 2014-2019 Markus Gans                                      *
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

/*  Inheritance diagram
 *  ═══════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▏
 * ▕  FTerm  ▏
 * ▕▁▁▁▁▁▁▁▁▁▏
 *      ▲
 *      │
 * ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
 * ▕ FVTerm  ▏ ▕ FObject ▏
 * ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
 *      ▲           ▲
 *      │           │
 *      └─────┬─────┘
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWidget ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *            ├────────────────────┐
 *            │                    │
 *       ▕▔▔▔▔▔▔▔▔▔▏               │
 *       ▕ FWindow ▏               │
 *       ▕▁▁▁▁▁▁▁▁▁▏               │
 *            ▲                    │
 *            │                    │
 *     ▕▔▔▔▔▔▔▔▔▔▔▔▔▏1     *▕▔▔▔▔▔▔▔▔▔▔▔▔▏
 *     ▕ FStatusBar ▏- - - -▕ FStatusKey ▏
 *     ▕▁▁▁▁▁▁▁▁▁▁▁▁▏       ▕▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FSTATUSBAR_H
#define FSTATUSBAR_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <vector>

#include "final/fwidget.h"
#include "final/fwindow.h"

namespace finalcut
{

// class forward declaration
class FStatusBar;

//----------------------------------------------------------------------
// class FStatusKey
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FStatusKey : public FWidget
{
  public:
    // Constructors
    explicit FStatusKey (FWidget* = nullptr);
    FStatusKey (FKey, const FString&, FWidget* = nullptr);

    // Disable copy constructor
    FStatusKey (const FStatusKey&) = delete;

    // Destructor
    virtual ~FStatusKey();

    // Disable assignment operator (=)
    FStatusKey& operator = (const FStatusKey&) = delete;

    // Accessors
    virtual const char* getClassName() const override;
    virtual FKey        getKey() const;
    virtual FString     getText() const;

    // Mutators
    void                setKey (FKey);
    void                setText (const FString&);
    void                setActive();
    void                unsetActive();
    bool                setMouseFocus(bool);
    bool                setMouseFocus();
    bool                unsetMouseFocus();

    // Inquiry
    bool                isActivated() const;
    bool                hasMouseFocus() const;

    // Event handler
    virtual void        onAccel (FAccelEvent*) override;

  private:
    // Methods
    void                init (FWidget*);
    void                processActivate();
    FStatusBar*         getConnectedStatusbar() const;
    void                setConnectedStatusbar (FStatusBar*);

    // Friend class
    friend class FStatusBar;

    // Data Members
    FKey        key{0};
    FString     text{};
    bool        active{false};
    bool        mouse_focus{false};
    FStatusBar* bar{nullptr};
};
#pragma pack(pop)


// FStatusKey inline functions
//----------------------------------------------------------------------
inline const char* FStatusKey::getClassName() const
{ return "FStatusKey"; }

//----------------------------------------------------------------------
inline FKey FStatusKey::getKey() const
{ return key; }

//----------------------------------------------------------------------
inline FString FStatusKey::getText() const
{ return text; }

//----------------------------------------------------------------------
inline void FStatusKey::setKey (FKey k)
{ key = k; }

//----------------------------------------------------------------------
inline void FStatusKey::setText (const FString& txt)
{ text = txt; }

//----------------------------------------------------------------------
inline void FStatusKey::unsetActive()
{ active = false; }

//----------------------------------------------------------------------
inline bool FStatusKey::setMouseFocus()
{ return setMouseFocus(true); }

//----------------------------------------------------------------------
inline bool FStatusKey::unsetMouseFocus()
{ return setMouseFocus(false); }

//----------------------------------------------------------------------
inline bool FStatusKey::isActivated() const
{ return active; }

//----------------------------------------------------------------------
inline bool FStatusKey::hasMouseFocus() const
{ return mouse_focus; }

//----------------------------------------------------------------------
inline FStatusBar* FStatusKey::getConnectedStatusbar() const
{ return bar; }

//----------------------------------------------------------------------
inline void FStatusKey::setConnectedStatusbar (FStatusBar* sb)
{ bar = sb; }


//----------------------------------------------------------------------
// class FStatusBar
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FStatusBar : public FWindow
{
  public:
    // Constructor
    explicit FStatusBar (FWidget* = nullptr);

    // Disable copy constructor
    FStatusBar (const FStatusBar&) = delete;

    // Destructor
    virtual ~FStatusBar();

    // Disable assignment operator (=)
    FStatusBar& operator = (const FStatusBar&) = delete;

    // Accessors
    virtual const char* getClassName() const override;
    FStatusKey*         getStatusKey (int) const;
    FString             getMessage() const;
    std::size_t         getCount() const;

    // Mutators
    void                activateKey (int);
    void                deactivateKey (int);
    void                setMessage (const FString&);

    // Inquiries
    bool                isActivated (int) const;
    bool                hasActivatedKey();

    // Methods
    virtual void        hide() override;
    void                drawMessage();
    void                clearMessage();
    void                insert (FStatusKey*);
    void                remove (FStatusKey*);
    void                remove (int);
    void                clear();
    virtual void        adjustSize() override;

    // Event handlers
    virtual void        onMouseDown (FMouseEvent*) override;
    virtual void        onMouseUp (FMouseEvent*) override;
    virtual void        onMouseMove (FMouseEvent*) override;

    // Callback method
    void                cb_statuskey_activated (FWidget*, FDataPtr);

  private:
    // Typedef
    typedef std::vector<FStatusKey*> keyList;

    // Methods
    void                init();
    virtual void        draw() override;
    void                drawKeys();
    void                drawKey (keyList::const_iterator);
    void                drawActiveKey (keyList::const_iterator);

    // Data Members
    keyList             key_list{};
    FString             text{""};
    bool                mouse_down{};
    std::size_t         screenWidth{80};
    int                 keyname_len{0};
    int                 x{-1};
    int                 x_msg{-1};
};
#pragma pack(pop)


// FStatusBar inline functions
//----------------------------------------------------------------------
inline const char* FStatusBar::getClassName() const
{ return "FStatusBar"; }

//----------------------------------------------------------------------
inline FStatusKey* FStatusBar::getStatusKey (int index) const
{ return key_list[uInt(index - 1)]; }

//----------------------------------------------------------------------
inline std::size_t FStatusBar::getCount() const
{ return key_list.size(); }

//----------------------------------------------------------------------
inline void FStatusBar::activateKey (int index)
{ key_list[uInt(index - 1)]->setActive(); }

//----------------------------------------------------------------------
inline void FStatusBar::deactivateKey (int index)
{ key_list[uInt(index - 1)]->unsetActive(); }

//----------------------------------------------------------------------
inline bool FStatusBar::isActivated(int index) const
{ return key_list[uInt(index - 1)]->isActivated(); }

//----------------------------------------------------------------------
inline FString FStatusBar::getMessage() const
{ return text; }

//----------------------------------------------------------------------
inline void FStatusBar::clearMessage()
{ text.clear(); }

}  // namespace finalcut

#endif  // FSTATUSBAR_H
