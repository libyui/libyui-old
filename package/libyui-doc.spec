#
# spec file for package libyui-doc
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


%define parent libyui
%define so_version 11

Name:           %{parent}-doc
Version:        3.9.2
Release:        0
Source:         %{parent}-%{version}.tar.bz2

BuildArch:      noarch

BuildRequires:  cmake >= 2.8
BuildRequires:  doxygen
BuildRequires:  fdupes
BuildRequires:  gcc-c++
BuildRequires:  graphviz-gnome
BuildRequires:  libyui-devel >= 3.0.4
BuildRequires:  texlive-latex

Url:            http://github.com/libyui/
Summary:        Libyui documentation
License:        LGPL-2.1 or LGPL-3.0
Group:          Documentation/HTML

%description
This is the user interface engine that provides the abstraction from
graphical user interfaces (Qt, Gtk) and text based user interfaces
(ncurses).

Originally developed for YaST, it can now be used independently of
YaST for generic (C++) applications. This package has very few
dependencies.

This package provides the documentation. (HTML & PDF)


%prep

%setup -n %{parent}-%{version}

%build

export CFLAGS="$RPM_OPT_FLAGS -DNDEBUG"
export CXXFLAGS="$RPM_OPT_FLAGS -DNDEBUG"

./bootstrap.sh %{_prefix}

mkdir build
cd build
cmake .. \
        -DDOC_DIR=%{_docdir} \
        -DDOCS_ONLY=ON

make %{?jobs:-j%jobs} docs

%install
cd build
make install DESTDIR="$RPM_BUILD_ROOT"

%fdupes -s $RPM_BUILD_ROOT/%_docdir/%{parent}%{so_version}

%clean
rm -rf "$RPM_BUILD_ROOT"

%files
%defattr(-,root,root)
%doc %{_docdir}/%{parent}%{so_version}

%changelog
