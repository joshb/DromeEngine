!include "MUI2.nsh"

!define DROMEVERSION "0.4.4"
!define DROMEROOT "..\.."

Name "Drome Engine"
OutFile "DromeEngine-${DROMEVERSION}.exe"
InstallDir "$PROGRAMFILES\Drome Engine"

VIProductVersion "${DROMEVERSION}.0"
VIAddVersionKey "ProductName" "Drome Engine"
VIAddVersionKey "FileVersion" "${DROMEVERSION}"
VIAddVersionKey "FileDescription" "Drome Engine Setup"
VIAddVersionKey "LegalCopyright" "Copyright (C) 2010 Josh A. Beam"

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section "Install"
	# install libraries and demo executable
	SetOutPath "$INSTDIR\bin"
	File "${DROMEROOT}\build\src\DromeCore\libDromeCore.dll"
	File "${DROMEROOT}\build\src\DromeGfx\libDromeGfx.dll"
	File "${DROMEROOT}\build\src\DromeGui\libDromeGui.dll"
	File "${DROMEROOT}\build\src\DromeMath\libDromeMath.dll"
	File "${DROMEROOT}\build\demo\DromeDemo.exe"

	# install headers
	SetOutPath $INSTDIR\include\DromeCore"
	File "${DROMEROOT}\include\DromeCore\*"
	SetOutPath $INSTDIR\include\DromeGfx"
	File "${DROMEROOT}\include\DromeGfx\*"
	SetOutPath $INSTDIR\include\DromeGui"
	File "${DROMEROOT}\include\DromeGui\*"
	SetOutPath $INSTDIR\include\DromeMath"
	File "${DROMEROOT}\include\DromeMath\*"

	WriteUninstaller "$INSTDIR\Uninstall.exe"

	# create shortcuts
	CreateShortCut "$DESKTOP\Drome Engine Demo.lnk" "$INSTDIR\bin\DromeDemo.exe"
	CreateShortCut "$SMPROGRAMS\Drome Engine Demo.lnk" "$INSTDIR\bin\DromeDemo.exe"
SectionEnd

Section "Uninstall"
	# uninstall libraries and demo executable
	Delete "$INSTDIR\bin\libDromeCore.dll"
	Delete "$INSTDIR\bin\libDromeGfx.dll"
	Delete "$INSTDIR\bin\libDromeGui.dll"
	Delete "$INSTDIR\bin\libDromeMath.dll"
	Delete "$INSTDIR\bin\DromeDemo.exe"
	RMDir "$INSTDIR\bin"

	# uninstall headers
	RMDir /r "$INSTDIR\include\Drome"
	RMDir "$INSTDIR\include"

	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR"
SectionEnd
