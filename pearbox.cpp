/*
 *  PearBox
 *  Copyright (C) 2015 Muhammad Mominul Huque
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <exception>
//#include <QApplication>
//#include <QPushButton>

#include "configuration.h"
#include "createimage.h"
#include "tools/snprintf.h"


int main ( int argc, char *argv[] )
{
	bool ret;
	CONF conf;

	ret = load_config(conf,"/home/mominul/src/ppccfg.ppc");

	ht_printf("\nCompose Dialog = %y\n",&conf.compose_dialog);
	ht_printf("\n3x mac = %y\n",&conf.net_3c_mac);
	ht_printf("\npvr = %d\n",&conf.pvr);
	ht_printf("\nmemory = %d\n",&conf.memory);

	ret = save_config(conf,"/home/mominul/src/newcfg.ppc");
	ret = load_config(conf,"/home/mominul/src/newcfg.ppc");
	//String path = "/home/mominul/src/newcfg.ppc";
	//ht_printf("\nkey_compose_dialog = %s",key_compose_dialog_string.contentChar());

  ret = Create_HD_Image(1024,"/home/mominul/src/HD1.img",false);
  ret = Create_HD_Image(41984,"/home/mominul/src/HD2.img",true);

  /*QApplication app(argc, argv);
  QPushButton *button = new QPushButton("Quit");
  QObject::connect(button, SIGNAL(clicked()),
                   &app, SLOT(quit()));
  button->show();*/
  return 0; // app.exec();
}
