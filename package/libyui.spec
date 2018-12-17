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
Version:        3.5.0
Release:        0
Source:         %{name}-%{version}.tar.bz2

%define so_version 9
%define bin_name %{name}%{so_version}

# optionally build with code coverage reporting,
# this uses debug build, do not use in production!
%bcond_with coverage

%if 0%{?suse_version} > 1325
BuildRequires:  libboost_headers-devel
BuildRequires:  libboost_test-devel
%else
BuildRequires:  boost-devel
%endif
BuildRequires:  cmake >= 2.8
BuildRequires:  gcc-c++
BuildRequires:  pkg-config

%if %{with coverage}
# normally the coverage feature should not be used out of CI
# but to be on the safe side...
BuildRequires: lcov
%endif

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

%package -n %{bin_name}

Provides:       yast2-libyui = 2.42.0
Obsoletes:      yast2-libyui < 2.42.0
Requires:       yui_backend = %{so_version}

Url:            http://github.com/libyui/
Summary:        Libyui - GUI-abstraction library
Group:          System/Libraries

%description -n %{bin_name}
This is the user interface engine that provides the abstraction from
graphical user interfaces (Qt, Gtk) and text based user interfaces
(ncurses).

Originally developed for YaST, it can now be used independently of
YaST for generic (C++) applications. This package has very few
dependencies.

%package devel

%if 0%{?suse_version} > 1325
Requires:       libboost_headers-devel
Requires:       libboost_test-devel
%else
Requires:       boost-devel
%endif
Requires:       glibc-devel
Requires:       libstdc++-devel
Requires:       %{bin_name} = %{version}

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
%setup -q -n %{name}-%{version}

%build

./bootstrap.sh
mkdir build
cd build

%if %{with coverage}
CMAKE_OPTS="-DCMAKE_BUILD_TYPE=DEBUG -DENABLE_CODE_COVERAGE=ON"
# the debug build type is incompatible with the default $RPM_OPT_FLAGS,
# do not use them
export CFLAGS="-DNDEBUG $(getconf LFS_CFLAGS)"
export CXXFLAGS="-DNDEBUG $(getconf LFS_CFLAGS)"
%else
export CFLAGS="$RPM_OPT_FLAGS -DNDEBUG $(getconf LFS_CFLAGS)"
export CXXFLAGS="$RPM_OPT_FLAGS -DNDEBUG $(getconf LFS_CFLAGS)"
%if %{?_with_debug:1}%{!?_with_debug:0}
CMAKE_OPTS="-DCMAKE_BUILD_TYPE=RELWITHDEBINFO"
%else
CMAKE_OPTS="-DCMAKE_BUILD_TYPE=RELEASE"
%endif
%endif

cmake .. \
        -DYPREFIX=%{_prefix} \
        -DDOC_DIR=%{_docdir} \
        -DLIB_DIR=%{_lib} \
        $CMAKE_OPTS

make %{?jobs:-j%jobs}

%check
cd build
make test
%if %{with coverage}
# generate code coverage data
make coverage
%endif

%install
cd build
make install DESTDIR="$RPM_BUILD_ROOT"
install -m0755 -d $RPM_BUILD_ROOT/%{_docdir}/%{bin_name}/
install -m0755 -d $RPM_BUILD_ROOT/%{_libdir}/yui
install -m0644 ../COPYING* $RPM_BUILD_ROOT/%{_docdir}/%{bin_name}/

%clean
rm -rf "$RPM_BUILD_ROOT"

%post -n %{bin_name} -p /sbin/ldconfig

%postun -n %{bin_name} -p /sbin/ldconfig

%files -n %{bin_name}
%defattr(-,root,root)
%dir %{_libdir}/yui
%dir %{_datadir}/libyui
%{_libdir}/lib*.so.*
%doc %dir %{_docdir}/%{bin_name}
%license %{_docdir}/%{bin_name}/COPYING*

%files devel
%defattr(-,root,root)
%dir %{_docdir}/%{bin_name}
%{_libdir}/lib*.so
%{_prefix}/include/yui
%{_libdir}/pkgconfig/%{name}.pc
%{_libdir}/cmake/%{name}
%{_datadir}/libyui/buildtools
%doc %{_docdir}/%{bin_name}/examples

%changelog
