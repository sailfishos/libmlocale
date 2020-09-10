Name:       libmlocale-qt5
Summary:    Contains classes MLocale and friends originally from libmeegotouch
Version:    0.5.0
Release:    1
Group:      System/Libraries
License:    LGPLv2
URL:        https://git.sailfishos.org/mer-core/libmlocale
Source0:    %{name}-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(icu-uc)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  qt5-qttools-linguist

%description
Contains classes MLocale and friends originally from libmeegotouch. 
With that separation one can use those localization facilities without 
having to bring on board also the MeeGo Touch GUI toolkit.


%package devel
Summary:    Devel files for limlocale
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
%{summary}.

%package tests
Summary:    Tests for limlocale
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description tests
%{summary}.

%package benchmarks
Summary:    Benchmarks files for limlocale
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description benchmarks
%{summary}.

%prep
%setup -q -n %{name}-%{version}

%build
export QT_SELECT=5

%configure --disable-static -icu
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
export QT_SELECT=5
%make_install INSTALL_ROOT=%{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc LICENSE.LGPL
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%{_datadir}/qt5/mkspecs/features/*.prf
%{_libdir}/*.so
%{_libdir}/*.prl
%{_includedir}/mlocale5/*.h
%{_includedir}/mlocale5/M*
%{_libdir}/pkgconfig/*.pc

%files tests
%defattr(-,root,root,-)
%{_libdir}/libmlocale-tests5
%dir %{_datadir}/libmlocale-tests5
%{_datadir}/libmlocale-tests5/tests.xml

%files benchmarks
%defattr(-,root,root,-)
%{_libdir}/libmlocale-benchmarks5
%dir %{_datadir}/libmlocale-benchmarks5
%{_datadir}/libmlocale-benchmarks5/tests.xml

