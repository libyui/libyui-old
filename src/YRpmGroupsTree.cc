/*
  Copyright (C) 2000-2012 Novell, Inc
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


/*-/

  File:	      YRpmGroupsTree.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include "YRpmGroupsTree.h"

#include "Libyui_config.h"

#define TEXTDOMAIN "rpm-groups"


YRpmGroupsTree::YRpmGroupsTree()
    : YStringTree( TEXTDOMAIN )
{
    bindtextdomain( TEXTDOMAIN, LOCALEDIR );
    bind_textdomain_codeset( TEXTDOMAIN, "utf8" );
}


YRpmGroupsTree::~YRpmGroupsTree()
{
    // NOP
}


static const char *fallback_rpm_groups[] =
{
    "Amusements/Games/3D/Other",
    "Amusements/Games/3D/Race",
    "Amusements/Games/3D/Shoot",
    "Amusements/Games/3D/Simulation",
    "Amusements/Games/Action/Arcade",
    "Amusements/Games/Action/Breakout",
    "Amusements/Games/Action/Other",
    "Amusements/Games/Action/Race",
    "Amusements/Games/Action/Shoot",
    "Amusements/Games/Board/Card",
    "Amusements/Games/Board/Chess",
    "Amusements/Games/Board/Other",
    "Amusements/Games/Board/Pool",
    "Amusements/Games/Board/Puzzle",
    "Amusements/Games/Logic",
    "Amusements/Games/Other",
    "Amusements/Games/RPG",
    "Amusements/Games/Strategy/Other",
    "Amusements/Games/Strategy/Real Time",
    "Amusements/Games/Strategy/Turn Based",
    "Amusements/Toys/Background",
    "Amusements/Toys/Clocks",
    "Amusements/Toys/Graphics",
    "Amusements/Toys/Other",
    "Amusements/Toys/Screensavers",
    "Development/Languages/C and C++",
    "Development/Languages/Fortran",
    "Development/Languages/Java",
    "Development/Languages/Other",
    "Development/Languages/Perl",
    "Development/Languages/Python",
    "Development/Languages/Scheme",
    "Development/Languages/Tcl",
    "Development/Libraries/C and C++",
    "Development/Libraries/Cross",
    "Development/Libraries/GNOME",
    "Development/Libraries/Java",
    "Development/Libraries/KDE",
    "Development/Libraries/Other",
    "Development/Libraries/Parallel",
    "Development/Libraries/Perl",
    "Development/Libraries/Python",
    "Development/Libraries/Tcl",
    "Development/Libraries/X11",
    "Development/Libraries/YaST",
    "Development/Tools/Building",
    "Development/Tools/Debuggers",
    "Development/Tools/Doc Generators",
    "Development/Tools/GUI Builders",
    "Development/Tools/IDE",
    "Development/Tools/Navigators",
    "Development/Tools/Other",
    "Development/Tools/Version Control",
    "Documentation/Howto",
    "Documentation/Man",
    "Documentation/Other",
    "Documentation/SuSE",
    "Hardware/Fax",
    "Hardware/ISDN",
    "Hardware/Joystick",
    "Hardware/Mobile",
    "Hardware/Modem",
    "Hardware/Other",
    "Hardware/Palm",
    "Hardware/Printing",
    "Hardware/Psion",
    "Hardware/Radio",
    "Hardware/Scanner",
    "Hardware/TV",
    "Hardware/UPS",
    "Productivity/Archiving/Backup",
    "Productivity/Archiving/Compression",
    "Productivity/Clustering/Computing",
    "Productivity/Clustering/HA",
    "Productivity/Databases/Clients",
    "Productivity/Databases/Servers",
    "Productivity/Databases/Tools",
    "Productivity/Editors/Emacs",
    "Productivity/Editors/Other",
    "Productivity/Editors/Vi",
    "Productivity/File utilities",
    "Productivity/Graphics/3D Editors",
    "Productivity/Graphics/Bitmap Editors",
    "Productivity/Graphics/CAD",
    "Productivity/Graphics/Convertors",
    "Productivity/Graphics/Other",
    "Productivity/Graphics/Vector Editors",
    "Productivity/Graphics/Viewers",
    "Productivity/Graphics/Visualization/Graph",
    "Productivity/Graphics/Visualization/Other",
    "Productivity/Graphics/Visualization/Raytracers",
    "Productivity/Hamradio/Fax",
    "Productivity/Hamradio/Logging",
    "Productivity/Hamradio/Morse",
    "Productivity/Hamradio/Other",
    "Productivity/Hamradio/Packet",
    "Productivity/Hamradio/Psk31",
    "Productivity/Hamradio/Satellite",
    "Productivity/Multimedia/CD/Grabbers",
    "Productivity/Multimedia/CD/Players",
    "Productivity/Multimedia/CD/Record",
    "Productivity/Multimedia/Other",
    "Productivity/Multimedia/Sound/Editors and Convertors",
    "Productivity/Multimedia/Sound/Midi",
    "Productivity/Multimedia/Sound/Mixers",
    "Productivity/Multimedia/Sound/Players",
    "Productivity/Multimedia/Sound/Utilities",
    "Productivity/Multimedia/Sound/Visualization",
    "Productivity/Multimedia/Video/Editors and Convertors",
    "Productivity/Multimedia/Video/Players",
    "Productivity/Networking/AOLInstantMessenger",
    "Productivity/Networking/Archie",
    "Productivity/Networking/Boot/Clients",
    "Productivity/Networking/Boot/Servers",
    "Productivity/Networking/Boot/Utilities",
    "Productivity/Networking/DNS/Servers",
    "Productivity/Networking/DNS/Utilities",
    "Productivity/Networking/Diagnostic",
    "Productivity/Networking/Email/Clients",
    "Productivity/Networking/Email/Mailinglists",
    "Productivity/Networking/Email/Servers",
    "Productivity/Networking/Email/Utilities",
    "Productivity/Networking/Ftp/Clients",
    "Productivity/Networking/Ftp/Servers",
    "Productivity/Networking/H323/Clients",
    "Productivity/Networking/H323/Servers",
    "Productivity/Networking/H323/Utilities",
    "Productivity/Networking/ICQ",
    "Productivity/Networking/IRC",
    "Productivity/Networking/LDAP/Clients",
    "Productivity/Networking/LDAP/Servers",
    "Productivity/Networking/NFS",
    "Productivity/Networking/NIS",
    "Productivity/Networking/Napster",
    "Productivity/Networking/News/Clients",
    "Productivity/Networking/News/Servers",
    "Productivity/Networking/News/Utilities",
    "Productivity/Networking/Novell",
    "Productivity/Networking/Other",
    "Productivity/Networking/PPP",
    "Productivity/Networking/Radius/Clients",
    "Productivity/Networking/Radius/Servers",
    "Productivity/Networking/Routing",
    "Productivity/Networking/SSH",
    "Productivity/Networking/Samba",
    "Productivity/Networking/Security",
    "Productivity/Networking/System",
    "Productivity/Networking/Talk/Clients",
    "Productivity/Networking/Talk/Servers",
    "Productivity/Networking/Web/Browsers",
    "Productivity/Networking/Web/Frontends",
    "Productivity/Networking/Web/Proxy",
    "Productivity/Networking/Web/Servers",
    "Productivity/Networking/Web/Utilities",
    "Productivity/Office/Dictionary",
    "Productivity/Office/Finance",
    "Productivity/Office/Organizers",
    "Productivity/Office/Other",
    "Productivity/Office/Spreadsheets",
    "Productivity/Office/Suite",
    "Productivity/Other",
    "Productivity/Publishing/DocBook",
    "Productivity/Publishing/HTML/Editors",
    "Productivity/Publishing/HTML/Tools",
    "Productivity/Publishing/Other",
    "Productivity/Publishing/PDF",
    "Productivity/Publishing/PS",
    "Productivity/Publishing/Presentation",
    "Productivity/Publishing/SGML",
    "Productivity/Publishing/TeX/Base",
    "Productivity/Publishing/TeX/Fonts",
    "Productivity/Publishing/TeX/Frontends",
    "Productivity/Publishing/TeX/Utilities",
    "Productivity/Publishing/Texinfo",
    "Productivity/Publishing/Troff",
    "Productivity/Publishing/Word",
    "Productivity/Publishing/XML",
    "Productivity/Scientific/Astronomy",
    "Productivity/Scientific/Chemistry",
    "Productivity/Scientific/Electronics",
    "Productivity/Scientific/Math",
    "Productivity/Scientific/Other",
    "Productivity/Scientific/Physics",
    "Productivity/Security",
    "Productivity/Text/Convertors",
    "Productivity/Text/Spell",
    "Productivity/Text/Utilities",
    "SuSE internal",
    "System/Base",
    "System/Benchmark",
    "System/Boot",
    "System/Console",
    "System/Daemons",
    "System/Emulators/Other",
    "System/Emulators/PC",
    "System/Fhs",
    "System/Filesystems",
    "System/GUI/GNOME",
    "System/GUI/KDE",
    "System/GUI/Other",
    "System/I18n/Chinesse",
    "System/I18n/Japanesse",
    "System/I18n/Korean",
    "System/Kernel",
    "System/Libraries",
    "System/Monitoring",
    "System/Packages",
    "System/Shells",
    "System/Sound Daemons",
    "System/X11/Displaymanagers",
    "System/X11/Fonts",
    "System/X11/Icons",
    "System/X11/Servers/XF86_3",
    "System/X11/Servers/XF86_4",
    "System/X11/Terminals",
    "System/X11/Utilities",
    "System/YaST",
    "Unsorted"
};

#define DIM(ARRAY)	( sizeof( ARRAY ) / sizeof( ARRAY[0] ) )


void
YRpmGroupsTree::addFallbackRpmGroups()
{
    for ( unsigned i=0; i < DIM( fallback_rpm_groups ); i++ )
    {
	addRpmGroup( fallback_rpm_groups[ i ] );
    }

#if 0
    logTree();
#endif
}
