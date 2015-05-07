/*
 *  PearBox
 *  configuration.cpp
 *
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

#include <fstream>

#include "configuration.h"

using namespace std;

/**
 * Load Config for given path and store them to given structure
 * Returns true if succeeded, false otherwise.
 */
bool load_config ( CONF& config, String path )
{
	bool bRet = false;
	/* Load Configuration from path */
	try {
		gConfig = new ConfigParser();
		/*                         Screen                            */
		gConfig->acceptConfigEntryStringDef("ppc_start_resolution", "800x600x15");
		gConfig->acceptConfigEntryIntDef("ppc_start_full_screen", 0);
		gConfig->acceptConfigEntryIntDef("redraw_interval_msec", 20);
		/*                         Memory                            */
		gConfig->acceptConfigEntryIntDef("memory_size", 128*1024*1024);
		/*                          CPU                              */
		gConfig->acceptConfigEntryIntDef("cpu_pvr", 0x000c0201);
		gConfig->acceptConfigEntryIntDef("page_table_pa", 0x00300000);
		/*                        Key Codes                           */
		gConfig->acceptConfigEntryStringDef("key_compose_dialog", "F11");
		gConfig->acceptConfigEntryStringDef("key_change_cd_0", "none");
		gConfig->acceptConfigEntryStringDef("key_change_cd_1", "none");
		gConfig->acceptConfigEntryStringDef("key_toggle_mouse_grab", "F12");
		gConfig->acceptConfigEntryStringDef("key_toggle_full_screen", "Ctrl+Alt+Return");
		/*                       Loader(PROM)                         */
		gConfig->acceptConfigEntryStringDef("prom_bootmethod", "auto");
		gConfig->acceptConfigEntryString("prom_env_bootpath", false);
		gConfig->acceptConfigEntryStringDef("prom_env_bootargs", "");
		gConfig->acceptConfigEntryStringDef("prom_env_machargs", "");
		gConfig->acceptConfigEntryString("prom_loadfile", false);
		gConfig->acceptConfigEntryStringDef("prom_driver_graphic", "");
		/*                          IDE                               */
		gConfig->acceptConfigEntryIntDef("pci_ide0_master_installed", 0);
		gConfig->acceptConfigEntryString("pci_ide0_master_type", false);
		gConfig->acceptConfigEntryString("pci_ide0_master_image", false);
		gConfig->acceptConfigEntryIntDef("pci_ide0_slave_installed", 0);
		gConfig->acceptConfigEntryString("pci_ide0_slave_type", false);
		gConfig->acceptConfigEntryString("pci_ide0_slave_image", false);
		/*                        Network                             */
		gConfig->acceptConfigEntryIntDef("pci_3c90x_installed", 0);
		gConfig->acceptConfigEntryString("pci_3c90x_mac", false);
		gConfig->acceptConfigEntryIntDef("pci_rtl8139_installed", 0);
		gConfig->acceptConfigEntryString("pci_rtl8139_mac", false);
		/*                          USB                               */
		gConfig->acceptConfigEntryIntDef("pci_usb_installed", 0);
		/*                         Serial                             */
		gConfig->acceptConfigEntryIntDef("pci_serial_installed", 0);
		/*                         NVRAM                              */
		gConfig->acceptConfigEntryStringDef("nvram_file", "nvram");

		try {
			LocalFile *config;
			config = new LocalFile(path);
			gConfig->loadConfig(*config);
			delete config;
		} catch (const Exception &e) {
			String res;
			e.reason(res);
			ht_printf("%y: %y\n", &path, &res);
			bRet = false;
			exit(1);
		}

	ht_printf("\n[LOAD 1] Configuration loaded successfully from '%y'\n",&path);

	/*             Save configuration in 'config' struct                */
	/*                          Screen                                  */
	gConfig->getConfigString("ppc_start_resolution", config.resolution);
	config.full_screen = gConfig->getConfigInt("ppc_start_full_screen");
	config.redraw = gConfig->getConfigInt("redraw_interval_msec");
	/*                          Memory                                  */
	config.memory = gConfig->getConfigInt("memory_size");
	/*                           CPU                                    */
	config.pvr = gConfig->getConfigInt("cpu_pvr");
	config.pagetable = gConfig->getConfigInt("page_table_pa");
	/*                         Key Codes                                */
	gConfig->getConfigString("key_compose_dialog", config.compose_dialog);
	gConfig->getConfigString("key_change_cd_0", config.change_cd);
	gConfig->getConfigString("key_toggle_mouse_grab", config.mouse_grab);
	gConfig->getConfigString("key_toggle_full_screen", config.fullscreen_k);
	/*                        Loader(PROM)                              */
	gConfig->getConfigString("prom_bootmethod", config.bootmethod);
	// Check that if boot method is "force"
	char *boot = config.bootmethod.contentChar();
	if ( boot == "force" )
	{
		gConfig->getConfigString("prom_loadfile", config.loadfile);
		gConfig->getConfigString("prom_env_bootpath", config.bootpath);
	}
	gConfig->getConfigString("prom_env_bootargs", config.bootargs);
	gConfig->getConfigString("prom_env_machargs", config.machargs);
	gConfig->getConfigString("prom_driver_graphic", config.driver_graph);
	/*                            IDE                                   */
	config.ide0 = gConfig->getConfigInt("pci_ide0_master_installed");
	gConfig->getConfigString("pci_ide0_master_type", config.ide0_type);
	gConfig->getConfigString("pci_ide0_master_image", config.ide0_path);
	config.ide0_s = gConfig->getConfigInt("pci_ide0_slave_installed");
	gConfig->getConfigString("pci_ide0_slave_type", config.ide0_s_type);
	gConfig->getConfigString("pci_ide0_slave_image", config.ide0_s_path);
	/*                          Network                                 */
	config.net_3c = gConfig->getConfigInt("pci_3c90x_installed");
	gConfig->getConfigString("pci_3c90x_mac", config.net_3c_mac);
	config.net_rtl = gConfig->getConfigInt("pci_rtl8139_installed");
	gConfig->getConfigString("pci_rtl8139_mac", config.net_rtl_mac);
	/*                            USB                                   */
	config.usb = gConfig->getConfigInt("pci_usb_installed");
	/*                           Serial                                 */
	config.serial = gConfig->getConfigInt("pci_serial_installed");
	/*                           NVRAM                                  */
	gConfig->getConfigString("nvram_file", config.nvram);

	bRet = true; // Loaded successfully
	ht_printf("\n[LOAD 2] Configuration stored successfully\n");

	}catch (const std::exception &e) {
		bRet = false;
		ht_printf("\n[ERROR/LOAD] load_config() caught exception: %s\n", e.what());
		return bRet;
	} catch (const Exception &e) {
		String res;
		e.reason(res);
		bRet = false;
		ht_printf("\n[ERROR/LOAD] load_config() caught exception: %y\n", &res);
		return bRet;
	}
	return bRet;
}

/**
 * Save Configuration from structure to path.
 * Returns True if succeed, false otherwise.
 */
bool save_config ( CONF& cnf, String path )
{
	ofstream fout;
	bool bRet = false;

	fout.open(path.contentChar(),ios::out);
	fout.exceptions ( ifstream::failbit | ifstream::badbit );
	try{
		/*                               Screen                                     */
		fout << "ppc_start_resolution = \"" << cnf.resolution.contentChar() << "\"" << endl;
		fout << "ppc_start_full_screen = " << cnf.full_screen << endl;
		fout << "redraw_interval_msec = " << cnf.redraw << endl;
		/*                             Key Codes                                    */
		fout << "key_compose_dialog = \"" << cnf.compose_dialog.contentChar() << "\"" << endl;
		fout << "key_change_cd_0 = \"" << cnf.change_cd.contentChar() << "\"" << endl;
		fout << "key_toggle_mouse_grab = \"" << cnf.mouse_grab.contentChar() << "\"" << endl;
		fout << "key_toggle_full_screen = \"" << cnf.fullscreen_k.contentChar() << "\"" << endl;
		/*                            Loader(PROM)                                  */
		fout << "prom_bootmethod = \"" << cnf.bootmethod.contentChar() << "\"" << endl;
		// Check that if boot method is "force"
		char *boot = cnf.bootmethod.contentChar();
		if ( boot == "force" )
		{
			fout << "prom_loadfile = \"" << cnf.loadfile.contentChar() << "\"" << endl;
			fout << "prom_env_bootpath = \"" << cnf.bootpath.contentChar() << "\"" << endl;
		}
		fout << "prom_env_bootargs = \"" << cnf.bootargs.contentChar() << "\"" << endl;
		fout << "prom_env_machargs = \"" << cnf.machargs.contentChar() << "\"" << endl;
		fout << "prom_driver_graphic = \"" << cnf.driver_graph.contentChar() << "\"" << endl;
		/*                               CPU                                         */
		fout << "cpu_pvr = " << hex << cnf.pvr << endl;
		fout << "page_table_pa = " << hex << cnf.pagetable << endl;
		/*                             Memory                                        */
		fout << "memory_size = " << hex << cnf.memory << endl;
		/*                               IDE                                         */
		fout << "pci_ide0_master_installed = " << cnf.ide0 << endl;
		fout << "pci_ide0_master_image = \"" << cnf.ide0_path.contentChar() << "\"" << endl;
		fout << "pci_ide0_master_type = \"" << cnf.ide0_type.contentChar() << "\"" << endl;
		fout << "pci_ide0_slave_installed = " << cnf.ide0_s << endl;
		fout << "pci_ide0_slave_image = \"" << cnf.ide0_s_path.contentChar() << "\"" << endl;
		fout << "pci_ide0_slave_type = \"" << cnf.ide0_s_type.contentChar() << "\"" << endl;
		/*                             Network                                       */
		fout << "pci_3c90x_installed = " << cnf.net_3c << endl;
		fout << "pci_3c90x_mac = \"" << cnf.net_3c_mac.contentChar() << "\"" << endl;
		fout << "pci_rtl8139_installed = " << cnf.net_rtl << endl;
		fout << "pci_rtl8139_mac = \"" << cnf.net_rtl_mac.contentChar() << "\"" << endl;
		/*                              USB                                          */
		fout << "pci_usb_installed = " << cnf.usb << endl;
		/*                          Serial Port                                      */
		fout << "pci_serial_installed = " << cnf.serial << endl;
		/*                             NVRAM                                         */
		fout << "nvram_file = \"" << cnf.nvram.contentChar() << "\"" << endl;

		fout.close();

		ht_printf("\n[SAVE] Configuration file '%y' saved successfully.\n",&path);
		bRet = true;
	}catch (ifstream::failure e) {
		ht_printf("\n[ERROR/SAVE] Configuration file '%y' cannot be saved.  \
		           Because a error occursed when opening/writing the configuration file\n",&path);
		bRet = false;
	}

	return bRet;
}
