// File: fmenuitem.cpp
// Provides: class FMenuItem

#include "fapp.h"
#include "fmenu.h"
#include "fmenubar.h"
#include "fmenulist.h"
#include "fmenuitem.h"
#include "fstatusbar.h"

//----------------------------------------------------------------------
// class FMenuItem
//----------------------------------------------------------------------

// constructor and destructor
//----------------------------------------------------------------------
FMenuItem::FMenuItem (FWidget* parent)
  : FWidget(parent)
  , text()
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(0)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::FMenuItem (FString& txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(0)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::FMenuItem (const std::string& txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(0)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::FMenuItem (const char* txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(0)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::FMenuItem (int k, FString& txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(k)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::FMenuItem (int k, const std::string& txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(k)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::FMenuItem (int k, const char* txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , selected(false)
  , separator(false)
  , checkable(false)
  , checked(false)
  , text_length(0)
  , hotkey(0)
  , accel_key(k)
  , menu(0)
  , super_menu(0)
  , radio_button(false)
{
  init (parent);
}

//----------------------------------------------------------------------
FMenuItem::~FMenuItem()  // destructor
{
  delAccelerator();
}


// private methods of FMenuItem
//----------------------------------------------------------------------
void FMenuItem::init (FWidget* parent)
{
  text_length = text.getLength();
  hotkey = hotKey();

  if ( hotkey )
    text_length--;

  setGeometry (1,1,int(text_length+2),1, false);

  if ( parent )
  {
    FMenuList* menu_list;
    setSuperMenu (parent);

    if ( accel_key  )
      addAccelerator (accel_key);

    menu_list = dynamic_cast<FMenuList*>(parent);
    if ( menu_list )
      menu_list->insert(this);

    if ( isMenuBar(parent) ) // Parent is menubar
    {
      FMenuBar* menubar_ptr = dynamic_cast<FMenuBar*>(parent);
      if ( menubar_ptr )
      {
        menubar_ptr->menu_dimension();

        if ( hotkey )  // Meta + hotkey
          menubar_ptr->addAccelerator (fc::Fmkey_meta + tolower(hotkey), this);
      }

      this->addCallback
      (
        "deactivate",
        _METHOD_CALLBACK (parent, &FMenuBar::cb_item_deactivated)
      );
    }
    else if ( isMenu(parent) ) // Parent is menu
    {
      FMenu* menu_ptr = dynamic_cast<FMenu*>(parent);
      if ( menu_ptr )
        menu_ptr->menu_dimension();
    }
  }
  if ( hasFocus() )
    flags = fc::focus;

  if ( isEnabled() )
    flags |= fc::active;
}

//----------------------------------------------------------------------
uChar FMenuItem::hotKey()
{
  uInt length;

  if ( text.isEmpty() )
    return 0;

  length = text.getLength();

  for (uInt i=0; i < length; i++)
  {
    try
    {
      if ( (i+1 < length) && (text[i] == '&') )
        return uChar(text[++i]);
    }
    catch (const std::out_of_range&)
    {
      return 0;;
    }
  }
  return 0;
}

//----------------------------------------------------------------------
void FMenuItem::processActivate()
{
  emitCallback("activate");
}

//----------------------------------------------------------------------
void FMenuItem::processDeactivate()
{
  emitCallback("deactivate");
}

//----------------------------------------------------------------------
void FMenuItem::processClicked()
{
  emitCallback("clicked");
}


// protected methods of FMenuItem
//----------------------------------------------------------------------
bool FMenuItem::isMenuBar (FWidget* w) const
{
  return bool ( strcmp ( w->getClassName()
                       , const_cast<char*>("FMenuBar") ) == 0 );
}

//----------------------------------------------------------------------
bool FMenuItem::isMenu (FWidget* w) const
{
  return bool ( strcmp ( w->getClassName()
                       , const_cast<char*>("FMenu") ) == 0 );
}


// public methods of FMenuItem
//----------------------------------------------------------------------
void FMenuItem::addAccelerator (int key, FWidget* obj)
{
  FWidget* super = super_menu;
  if ( ! super )
    return;

  while ( super && strncmp ( super->getClassName()
                           , const_cast<char*>("FMenu"), 5) == 0 )
  {
    super = super->parentWidget();
  }

  if ( super )
  {
    FWidget* window = FWindow::getWindowWidget(super);
    accelerator accel = { key, obj };

    if ( ! window )
      window = getRootWidget();
    if ( window && window->accelerator_list )
    {
      accel_key = key;
      window->accelerator_list->push_back(accel);
    }
  }

  if ( isMenu(super_menu) )
  {
    FMenu* menu_ptr = dynamic_cast<FMenu*>(super_menu);
    if ( menu_ptr )
      menu_ptr->menu_dimension();
  }
}

//----------------------------------------------------------------------
void FMenuItem::delAccelerator (FWidget* obj)
{
  FWidget* super = super_menu;
  if ( ! super )
    return;

  while ( super && strncmp ( super->getClassName()
                           , const_cast<char*>("FMenu"), 5) == 0 )
  {
    super = super->parentWidget();
  }

  if ( super )
  {
    FWidget* window = FWindow::getWindowWidget(super);

    if ( ! window )
      window = getRootWidget();
    if (  window
       && window->accelerator_list
       && ! window->accelerator_list->empty() )
    {
      FWidget::Accelerators::iterator iter;
      iter = window->accelerator_list->begin();

      while ( iter != window->accelerator_list->end() )
      {
        if ( iter->object == obj )
        {
          accel_key = 0;
          iter = window->accelerator_list->erase(iter);
        }
        else
          ++iter;
      }
    }
  }

  if ( isMenu(super_menu) )
  {
    FMenu* menu_ptr = dynamic_cast<FMenu*>(super_menu);
    if ( menu_ptr )
      menu_ptr->menu_dimension();
  }
}

//----------------------------------------------------------------------
void FMenuItem::onKeyPress (FKeyEvent* ev)
{
  if ( super_menu )
  {
    if ( isMenu(super_menu) )
    {
      FMenu* smenu = dynamic_cast<FMenu*>(super_menu);
      if ( smenu )
        smenu->onKeyPress(ev);
    }

    if ( isMenuBar(super_menu) )
    {
      FMenuBar* mbar = dynamic_cast<FMenuBar*>(super_menu);
      if ( mbar )
      {
        if ( mbar->hotkeyMenu(ev) )
          return;
        mbar->onKeyPress(ev);
      }
    }
  }
}

//----------------------------------------------------------------------
void FMenuItem::onMouseDown (FMouseEvent* ev)
{
  if ( super_menu )
  {
    const FPoint& g = ev->getGlobalPos();
    int b = ev->getButton();

    if ( isMenu(super_menu) )
    {
      FMenu* smenu = dynamic_cast<FMenu*>(super_menu);
      if ( smenu )
      {
        const FPoint& p2 = smenu->globalToLocalPos(g);
        FMouseEvent* _ev = new FMouseEvent (fc::MouseDown_Event, p2, g, b);
        smenu->onMouseDown(_ev);
        delete _ev;
      }
    }

    if ( isMenuBar(super_menu) )
    {
      FMenuBar* mbar = dynamic_cast<FMenuBar*>(super_menu);
      if ( mbar )
      {
        const FPoint& p2 = mbar->globalToLocalPos(g);
        FMouseEvent* _ev = new FMouseEvent (fc::MouseDown_Event, p2, g, b);
        mbar->onMouseDown(_ev);
        delete _ev;
      }
    }
  }
}

//----------------------------------------------------------------------
void FMenuItem::onMouseUp (FMouseEvent* ev)
{
  if ( super_menu )
  {
    const FPoint& g = ev->getGlobalPos();
    int b = ev->getButton();

    if ( isMenu(super_menu) )
    {
      FMenu* smenu = dynamic_cast<FMenu*>(super_menu);
      if ( smenu )
      {
        const FPoint& p2 = smenu->globalToLocalPos(g);
        FMouseEvent* _ev = new FMouseEvent (fc::MouseUp_Event, p2, g, b);
        smenu->onMouseUp(_ev);
        delete _ev;
      }
    }

    if ( isMenuBar(super_menu) )
    {
      FMenuBar* mbar = dynamic_cast<FMenuBar*>(super_menu);
      if ( mbar )
      {
        const FPoint& p2 = mbar->globalToLocalPos(g);
        FMouseEvent* _ev = new FMouseEvent (fc::MouseUp_Event, p2, g, b);
        mbar->onMouseUp(_ev);
        delete _ev;
      }
    }
  }
}

//----------------------------------------------------------------------
void FMenuItem::onMouseMove (FMouseEvent* ev)
{
  if ( super_menu )
  {
    const FPoint& g = ev->getGlobalPos();
    int b = ev->getButton();

    if ( isMenu(super_menu) )
    {
      FMenu* smenu = dynamic_cast<FMenu*>(super_menu);
      if ( smenu )
      {
        const FPoint& p2 = smenu->globalToLocalPos(g);
        FMouseEvent* _ev = new FMouseEvent (fc::MouseMove_Event, p2, g, b);
        smenu->onMouseMove(_ev);
        delete _ev;
      }
    }

    if ( isMenuBar(super_menu) )
    {
      FMenuBar* mbar = dynamic_cast<FMenuBar*>(super_menu);
      if ( mbar )
      {
        const FPoint& p2 = mbar->globalToLocalPos(g);
        FMouseEvent* _ev = new FMouseEvent (fc::MouseMove_Event, p2, g, b);
        mbar->onMouseMove(_ev);
        delete _ev;
      }
    }

  }
}

//----------------------------------------------------------------------
void FMenuItem::onAccel (FAccelEvent* ev)
{
  if ( isEnabled() && ! isSelected() )
  {
    if ( super_menu && isMenuBar(super_menu) )
    {
      FMenuBar* mbar = dynamic_cast<FMenuBar*>(super_menu);
      if ( mbar )
      {
        if ( menu )
        {
          FWidget* focused_widget;

          if ( mbar->getSelectedItem() )
            mbar->getSelectedItem()->unsetSelected();
          setSelected();
          mbar->selectedItem = this;
          openMenu();

          focused_widget = static_cast<FWidget*>(ev->focusedWidget());
          FFocusEvent out (fc::FocusOut_Event);
          FApplication::queueEvent(focused_widget, &out);
          menu->unselectItem();
          menu->selectFirstItem();
          menu->getSelectedItem()->setFocus();
          if ( focused_widget )
            focused_widget->redraw();
          menu->redraw();
          if ( statusBar() )
            statusBar()->drawMessage();
          mbar->redraw();
          mbar->drop_down = true;
        }
        else
        {
          unsetSelected();
          mbar->selectedItem = 0;
          mbar->redraw();
          processClicked();
          mbar->drop_down = false;
        }
        ev->accept();
      }
    }
    else
      processClicked();
  }
}

//----------------------------------------------------------------------
void FMenuItem::onFocusIn (FFocusEvent*)
{
  if ( statusBar() )
    statusBar()->drawMessage();
}

//----------------------------------------------------------------------
void FMenuItem::onFocusOut (FFocusEvent*)
{
  unsetSelected();
  if ( super_menu && isMenuBar(super_menu) )
  {
    FMenuBar* mbar = dynamic_cast<FMenuBar*>(super_menu);
    if ( mbar )
      mbar->redraw();
  }
  if ( statusBar() )
  {
    statusBar()->clearMessage();
    statusBar()->drawMessage();
  }
}

//----------------------------------------------------------------------
bool FMenuItem::setEnable (bool on)
{
  FWidget::setEnable(on);

  FWidget* super = getSuperMenu();

  if ( on )
  {
    flags |= fc::active;

    if ( super && isMenuBar(super) )
    {
      // Meta + hotkey
      super->addAccelerator (fc::Fmkey_meta + tolower(hotkey), this);
    }
  }
  else
  {
    flags &= ~fc::active;

    if ( super && isMenuBar(super) )
      super->delAccelerator (this);
  }
  return on;
}

//----------------------------------------------------------------------
bool FMenuItem::setFocus (bool on)
{
  FWidget::setFocus(on);

  if ( on )
  {
    flags |= fc::focus;

    if ( isEnabled() )
    {
      if ( ! selected )
      {
        FMenuList* menu_list = dynamic_cast<FMenuList*>(getSuperMenu());
        setSelected();
        if ( menu_list )
        {
          menu_list->unselectItem();
          menu_list->setSelectedItem(this);
        }

        if ( statusBar() )
          statusBar()->drawMessage();

        FWidget* parent = getSuperMenu();
        if ( isMenuBar(parent) )
        {
          FMenuBar* menubar_ptr = dynamic_cast<FMenuBar*>(parent);
          if ( menubar_ptr )
            menubar_ptr->redraw();
        }
        else if ( isMenu(parent) )
        {
          FMenu* menu_ptr = dynamic_cast<FMenu*>(parent);
          if ( menu_ptr )
            menu_ptr->redraw();
        }
      }

      if ( statusBar() )
      {
        FString msg = getStatusbarMessage();
        FString curMsg = statusBar()->getMessage();
        if ( curMsg != msg )
          statusBar()->setMessage(msg);
      }
    }
  }
  else
  {
    flags &= ~fc::focus;

    if ( isEnabled() && statusBar() )
      statusBar()->clearMessage();
  }
  return on;
}

//----------------------------------------------------------------------
void FMenuItem::setSelected()
{
  if ( isEnabled() )
  {
    selected = true;
    processActivate();
  }
}

//----------------------------------------------------------------------
void FMenuItem::unsetSelected()
{
  selected = false;
  processDeactivate();
}

//----------------------------------------------------------------------
void FMenuItem::openMenu()
{
  FMenu* dd_menu;  // Drop-down menu
  FMenu* open_menu;

  if ( hasMenu() )
  {
    dd_menu = getMenu();
    if ( dd_menu->isVisible() )
      return;

    open_menu = static_cast<FMenu*>(getOpenMenu());
    if ( open_menu && open_menu != dd_menu )
    {
      open_menu->hide();
      open_menu->hideSubMenus();
    }
    setOpenMenu(dd_menu);

    dd_menu->setVisible();
    dd_menu->show();
    dd_menu->raiseWindow(dd_menu);
    dd_menu->redraw();
    updateTerminal();
    flush_out();
  }
}

//----------------------------------------------------------------------
void FMenuItem::setText (FString& txt)
{
  text = txt;
  text_length = text.getLength();
  hotkey = hotKey();
  if ( hotkey )
    text_length--;
  setWidth(int(text_length));
}

//----------------------------------------------------------------------
void FMenuItem::setText (const std::string& txt)
{
  FString s = FString(txt);
  setText (s);
}

//----------------------------------------------------------------------
void FMenuItem::setText (const char* txt)
{
  FString s = FString(txt);
  setText (s);
}
