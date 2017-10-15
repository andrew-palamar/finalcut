/************************************************************************
* fdialog.h - Widget FDialog                                            *
*                                                                       *
* This file is part of the Final Cut widget toolkit                     *
*                                                                       *
* Copyright 2012-2017 Markus Gans                                       *
*                                                                       *
* The Final Cut is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation; either version 3 of the License, or     *
* (at your option) any later version.                                   *
*                                                                       *
* The Final Cut is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* along with this program.  If not, see <http://www.gnu.org/licenses/>. *
************************************************************************/

/*  Inheritance diagram
 *  ═══════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
 * ▕ FObject ▏ ▕  FTerm  ▏
 * ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
 *      ▲           ▲
 *      │           │
 *      └─────┬─────┘
 *            │
 *       ▕▔▔▔▔▔▔▔▔▏
 *       ▕ FVTerm ▏
 *       ▕▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWidget ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWindow ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FDialog ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FDIALOG_H
#define FDIALOG_H

#include "final/fmenu.h"
#include "final/fmenuitem.h"
#include "final/ftooltip.h"
#include "final/fwindow.h"


//----------------------------------------------------------------------
// class FDialog
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FDialog : public FWindow
{
  public:
     // Using-declaration
    using FWindow::setResizeable;
    using FWindow::move;
    using FWindow::setPos;

    // Enumeration
    enum DialogCode
    {
      Reject = 0,
      Accept = 1
    };

    // Constructors
    explicit FDialog (FWidget* = 0);
    FDialog (const FString&, FWidget* = 0);

    // Destructor
    virtual ~FDialog();

    // Accessors
    virtual const char* getClassName() const;
    FString             getText() const;

    // Mutators
    bool                setDialogWidget (bool);
    bool                setDialogWidget();
    bool                unsetDialogWidget();
    bool                setModal (bool);
    bool                setModal();
    bool                unsetModal();
    bool                setResizeable (bool);
    bool                setScrollable (bool);
    bool                setScrollable();
    bool                unsetScrollable();
    void                setText (const FString&);

    // Inquiries
    bool                isModal();
    bool                isScrollable();

    // Methods
    void                show();
    void                hide();
    int                 exec();
    void                setPos (int, int, bool = true);
    void                move (int, int);
    void                setSize (int, int, bool = true);
    void                activateDialog();

    // Event handlers
    void                onKeyPress (FKeyEvent*);
    void                onMouseDown (FMouseEvent*);
    void                onMouseUp (FMouseEvent*);
    void                onMouseMove (FMouseEvent*);
    void                onMouseDoubleClick (FMouseEvent*);
    void                onAccel (FAccelEvent*);
    void                onWindowActive (FEvent*);
    void                onWindowInactive (FEvent*);
    void                onWindowRaised (FEvent*);
    void                onWindowLowered (FEvent*);

  protected:
    // Methods
    virtual void        done (int);
    virtual void        draw();
    void                drawDialogShadow();

    // Event handlers
    virtual void        onShow (FShowEvent*);
    virtual void        onHide (FHideEvent*);
    virtual void        onClose (FCloseEvent*);

  private:
    // Using-declaration
    using FWidget::drawBorder;

    // Disable copy constructor
    FDialog (const FDialog&);

    // Disable assignment operator (=)
    FDialog& operator = (const FDialog&);

    // Methods
    void                init();
    virtual void        drawBorder();
    void                drawTitleBar();
    void                leaveMenu();
    void                openMenu();
    void                selectFirstMenuItem();
    void                setZoomItem();
    static void         addDialog (FWidget*);
    static void         delDialog (FWidget*);

    // Callback methods
    void                cb_move (FWidget*, data_ptr);
    void                cb_zoom (FWidget*, data_ptr);
    void                cb_close (FWidget*, data_ptr);

    // Data Members
    FString             tb_text;        // title bar text
    int                 result_code;
    bool                zoom_button_pressed;
    bool                zoom_button_active;
    FPoint              titlebar_click_pos;
    FPoint              resize_click_pos;
    FRect               save_geometry;  // required by keyboard move/size
    FMenu*              dialog_menu;
    FMenuItem*          dgl_menuitem;
    FMenuItem*          move_size_item;
    FMenuItem*          zoom_item;
    FMenuItem*          close_item;
    FToolTip*           tooltip;

    // Friend function from FMenu
    friend void FMenu::hideSuperMenus();
};
#pragma pack(pop)

// FDialog inline functions
//----------------------------------------------------------------------
inline const char* FDialog::getClassName() const
{ return "FDialog"; }

//----------------------------------------------------------------------
inline FString FDialog::getText() const
{ return tb_text; }

//----------------------------------------------------------------------
inline bool FDialog::setDialogWidget()
{ return setDialogWidget(true); }

//----------------------------------------------------------------------
inline bool FDialog::unsetDialogWidget()
{ return setDialogWidget(false); }

//----------------------------------------------------------------------
inline bool FDialog::setModal()
{ return setModal(true); }

//----------------------------------------------------------------------
inline bool FDialog::unsetModal()
{ return setModal(false); }

//----------------------------------------------------------------------
inline bool FDialog::setScrollable()
{ return setScrollable(true); }

//----------------------------------------------------------------------
inline bool FDialog::unsetScrollable()
{ return setScrollable(false); }

//----------------------------------------------------------------------
inline void FDialog::setText (const FString& txt)
{ tb_text = txt; }

//----------------------------------------------------------------------
inline bool FDialog::isModal()
{ return ((flags & fc::modal) != 0); }

//----------------------------------------------------------------------
inline bool FDialog::isScrollable()
{ return ((flags & fc::scrollable) != 0); }



#endif  // FDIALOG_H