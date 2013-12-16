# yum install  python3-devel
# yum install  rpm-build

Name:           angela_typing_tutor
Version:        2.2
Release:        0%{?dist}
Epoch:          1
Packager:	Nalin.x.Linux@gmail.com
Summary:        eye's free key-board practicing application.

Group:          Applications/Games
License:        GPLv3+
URL:            https://codeload.github.com/Nalin-x-Linux/aott/zip/angela_typing_tutor-2.2.zip
Source0:        https://codeload.github.com/Nalin-x-Linux/aott/zip/angela_typing_tutor-2.2.zip

BuildArch:      x86_64

BuildRequires:	espeak-devel
BuildRequires:	gtk3-devel
BuildRequires:	libcanberra-devel
BuildRequires:	libcanberra-gtk3


Requires:       espeak 
Requires:       espeak-devel
Requires:       libcanberra-gtk3
Requires:	gtk3

%description
 angela-typing-tutor is a simple eye's free key-board
 practicing application.

%prep
%setup -q

%build
make

%install
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/applications/
mkdir -p $RPM_BUILD_ROOT/%{_bindir}/

cp -r usr/share/angela-typing-tutor $RPM_BUILD_ROOT/%{_datadir}/
cp angela-typing-tutor.desktop $RPM_BUILD_ROOT/%{_datadir}/applications/
cp angela-typing-tutor $RPM_BUILD_ROOT/%{_bindir}/

%files
%defattr(-,root,root,-)
%{_datadir}/angela-typing-tutor/*
%{_datadir}/applications/*
%{_bindir}/*


%clean
rm -rf $RPM_BUILD_ROOT
