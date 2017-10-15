/************************************************************************
* dialog.cpp - A FDialog example                                        *
*                                                                       *
* This file is part of the Final Cut widget toolkit                     *
*                                                                       *
* Copyright 2015-2017 Markus Gans                                       *
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

#include <final/fapplication.h>
#include <final/fbutton.h>
#include <final/fdialog.h>
#include <final/flabel.h>

// function prototype
void cb_quit (FWidget*, FWidget::data_ptr);


//----------------------------------------------------------------------
// callback function
//----------------------------------------------------------------------
void cb_quit (FWidget*, FWidget::data_ptr data)
{
  FApplication* app = static_cast<FApplication*>(data);
  app->quit();
}


//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------
int main (int argc, char* argv[])
{
  // Create the application object
  FApplication app(argc, argv);

  // Create a simple dialog box
  FDialog dgl(&app);
  dgl.setText ("FDialog");
  dgl.setGeometry (4, 3, 41, 11);

  // Create text labels
  FLabel label_1(&dgl);
  FLabel label_2(&dgl);

  label_1 << wchar_t(fc::BlackUpPointingTriangle)
          << std::wstring(L"\n")
          << wchar_t(fc::BoxDrawingsUpAndRight)
          << FString(2, wchar_t(fc::BoxDrawingsHorizontal))
          << " Double click the title bar button,";
  label_2 << "press Q on the keyboard,\n"
          << "or push the button below to exit\n"
          << "the program.";

  label_1.setGeometry (1, 1, 38, 2);
  label_2.setGeometry (5, 3, 34, 3);

  // Create the quit button
  FButton btn("&Quit", &dgl);
  btn.setGeometry (16, 7, 9, 1);

  // Connect the button signal "clicked" with the callback function
  btn.addCallback
  (
    "clicked",
    F_FUNCTION_CALLBACK (&cb_quit),
    &app
  );

  // Set dialog object as main widget
  app.setMainWidget(&dgl);

  // Show and start the application
  dgl.show();
  return app.exec();
}