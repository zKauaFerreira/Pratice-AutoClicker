; Script NSIS para o AutoClicker C++

!define APP_NAME "AutoClicker"
!define COMPANY_NAME "YourCompany"
!define VERSION "1.0.0"

; --- Metadados do Instalador ---
Name "${APP_NAME}"
Caption "${APP_NAME} ${VERSION}"
OutFile "AutoClicker_Installer.exe"
SetCompressor lzma
RequestExecutionLevel user ; Não precisa de admin, pois instala em HKCU e AppData

; --- Interface Gráfica (Modern UI) ---
!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_ICON "res\icon.ico"
!define MUI_UNICON "res\icon.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "PortugueseBR"

; --- Diretório de Instalação ---
InstallDir "$LOCALAPPDATA\${APP_NAME}"

; --- Seção de Instalação ---
Section "Install" SEC_INSTALL
    SetOutPath $INSTDIR

    ; Adicione o executável aqui. Ajuste o caminho se necessário.
    ; O caminho deve ser relativo à localização deste script .nsi
    File "build\Release\AutoClicker.exe"
    File "res\icon.ico"

    ; --- Atalhos ---
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\AutoClicker.exe"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    ; --- Desinstalador ---
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; --- Chaves de Registro para Adicionar/Remover Programas ---
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME}"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" "\"$INSTDIR\Uninstall.exe\""
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayIcon" "$INSTDIR\icon.ico"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayVersion" "${VERSION}"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "Publisher" "${COMPANY_NAME}"
SectionEnd

; --- Seção de Desinstalação ---
Section "Uninstall" SEC_UNINSTALL
    ; Garante que o app não está rodando
    nsExec::ExecToStack 'taskkill /F /IM AutoClicker.exe'

    ; Remove arquivos
    Delete "$INSTDIR\AutoClicker.exe"
    Delete "$INSTDIR\icon.ico"
    Delete "$INSTDIR\Uninstall.exe"

    ; Remove atalhos
    Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
    Delete "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${APP_NAME}"

    ; Remove diretório de instalação
    RMDir "$INSTDIR"

    ; Remove chaves do registro
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
    ; Remove a chave de inicialização, se existir
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}"
SectionEnd