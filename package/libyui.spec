#
# spec file for package libyui
#
# Copyright (c) 2014 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#


Name:           libyui
Version:        3.2.1
Release:        0
Source:         libyui-%{version}.tar.bz2

BuildRequires:  boost-devel
BuildRequires:  cmake >= 2.8
BuildRequires:  gcc-c++
BuildRequires:  pkg-config

Url:            http://github.com/libyui/
Summary:        GUI-abstraction library
License:        LGPL-2.1 or LGPL-3.0
Group:          System/Libraries

%description
This is the user interface engine that provides the abstraction from
graphical user interfaces (Qt, Gtk) and text based user interfaces
(ncurses).

Originally developed for YaST, it can now be used independently of
YaST for generic (C++) applications. This package has very few
dependencies.

%package -n libyui7

Provides:       yast2-libyui = 2.42.0
Obsoletes:      yast2-libyui < 2.42.0
Requires:       yui_backend = 7

Url:            http://github.com/libyui/
Summary:        Libyui - GUI-abstraction library
Group:          System/Libraries

%description -n libyui7
This is the user interface engine that provides the abstraction from
graphical user interfaces (Qt, Gtk) and text based user interfaces
(ncurses).

Originally developed for YaST, it can now be used independently of
YaST for generic (C++) applications. This package has very few
dependencies.


%package devel

Requires:       boost-devel
Requires:       glibc-devel
Requires:       libstdc++-devel
Requires:       libyui7 = %{version}

Url:            http://github.com/libyui/
Summary:        Libyui header files
Group:          Development/Languages/C and C++

%description devel
This is the user interface engine that provides the abstraction from
graphical user interfaces (Qt, Gtk) and text based user interfaces
(ncurses).

Originally developed for YaST, it can now be used independently of
YaST for generic (C++) applications. This package has very few
dependencies.

This can be used independently of YaST for generic (C++) applications.
This package has very few dependencies.


%prep
%setup -q -n libyui-%{version}

%build

./bootstrap.sh

export CFLAGS="$RPM_OPT_FLAGS -DNDEBUG $(getconf LFS_CFLAGS)"
export CXXFLAGS="$RPM_OPT_FLAGS -DNDEBUG $(getconf LFS_CFLAGS)"

mkdir build
cd build

%if %{?_with_debug:1}%{!?_with_debug:0}
cmake .. \
        -DYPREFIX=%{_prefix} \
        -DDOC_DIR=%{_docdir} \
        -DLIB_DIR=%{_lib} \
        -DCMAKE_BUILD_TYPE=RELWITHDEBINFO
%else
cmake .. \
        -DYPREFIX=%{_prefix} \
        -DDOC_DIR=%{_docdir} \
        -DLIB_DIR=%{_lib} \
        -DCMAKE_BUILD_TYPE=RELEASE
%endif

make %{?jobs:-j%jobs}

%install
cd build
make install DESTDIR="$RPM_BUILD_ROOT"
install -m0755 -d $RPM_BUILD_ROOT/%{_docdir}/libyui7/
install -m0755 -d $RPM_BUILD_ROOT/%{_libdir}/yui
install -m0644 ../COPYING* $RPM_BUILD_ROOT/%{_docdir}/libyui7/

%clean
rm -rf "$RPM_BUILD_ROOT"

%post -n libyui7 -p /sbin/ldconfig

%postun -n libyui7 -p /sbin/ldconfig

%files -n libyui7
%defattr(-,root,root)
%dir %{_libdir}/yui
%dir %{_datadir}/libyui
%{_libdir}/lib*.so.*
%doc %dir %{_docdir}/libyui7
%doc %{_docdir}/libyui7/COPYING*

%files devel
%defattr(-,root,root)
%dir %{_docdir}/libyui7
%{_libdir}/lib*.so
%{_prefix}/include/yui
%{_libdir}/pkgconfig/libyui.pc
%{_libdir}/cmake/libyui
%{_datadir}/libyui/buildtools
%doc %{_docdir}/libyui7/examples

%changelog
