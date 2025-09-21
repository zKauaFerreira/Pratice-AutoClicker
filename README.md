# ✨ AutoClicker C++

O **AutoClicker C++** é um autoclicker de alto desempenho para Windows, criado com o intuito de ser uma ferramenta *prática, leve e configurável* para o dia a dia. Ele roda discretamente na bandeja do sistema (system tray) e não possui uma janela principal, sendo controlado inteiramente por um menu de contexto e uma tecla de atalho global.

---

## 🚀 Como Usar o Aplicativo

Depois de instalar ou executar o `AutoClicker.exe`, um ícone aparecerá na sua bandeja de sistema (próximo ao relógio). Clique com o **botão direito** nesse ícone para abrir o menu de controle.

> 💡 **Dica:** A primeira vez que o programa é executado, ele começa **desativado**. Use a tecla de atalho ou o menu para ativá-lo!

### ⚙️ Comportamento Padrão

- **Ativar/Desativar (Toggle):** Use a tecla `End` para ligar ou desligar o autoclicker.  
- **Segurar para Clicar (Hold-to-Click):** Segure o `botão esquerdo (M1)` ou o `botão direito (M2)` para cliques contínuos.  
- **Spam Contínuo (Toggle):** Use `M4` para spammar cliques esquerdos e `M5` para cliques direitos. Pressione novamente para parar.  

### 📋 Menu de Opções

- **✅ Enabled:** Ativa ou desativa globalmente o autoclicker.  
- **⚡ Set CPS:** Defina a velocidade de cliques por segundo. Também é possível escolher `Custom...` para inserir manualmente.  
- **🖱️ Clicker:**
  - `Mouse 1 (Hold)`: Ativa/desativa o autoclick para o botão esquerdo.
  - `Mouse 2 (Hold)`: Ativa/desativa o autoclick para o botão direito.
  - `Mouse 4 (Left - Toggle)`: Ativa/desativa o spam de cliques esquerdos com o botão lateral M4.
  - `Mouse 5 (Right - Toggle)`: Ativa/desativa o spam de cliques direitos com o botão lateral M5.
- **🔔 Notifications:**
  - `Enable Notifications`: Liga/desliga notificações de ON/OFF.  
  - `Silence on Fullscreen`: Bloqueia notificações em tela cheia.  
  - `Notification Corner`: Escolhe o canto da tela para exibição.  
  - `Notification Duration`: Define a duração (segundos).  
- **⚙️ Other:**
  - `Change Toggle Key`: Define uma nova tecla de atalho.  
  - `Start with Windows`: Inicia o app junto com o sistema.  
- **🔄 Reset Settings:** Restaura todas as configurações.  
- **❌ Exit:** Fecha o aplicativo.  

---

## 📂 Assets

Para usuários que desejam apenas utilizar o programa, o executável `.exe` está disponível para download na seção **Assets** da [página de Releases do GitHub](https://github.com/zKauaFerreira/Pratice-AutoClicker/releases). Basta baixar o arquivo `AutoClicker.exe` e executar.

---

## 🛠️ Como Compilar (Para Desenvolvedores)

Se você deseja compilar o projeto a partir do código-fonte, siga os passos abaixo:

### 📦 Pré-requisitos

1. **Visual Studio 2022**  
   - 📥 [Download](https://visualstudio.microsoft.com/downloads/)  
   - Durante a instalação, marque a carga de trabalho **"Desenvolvimento para desktop com C++"**.  
   - Inclui MSVC, Windows SDK e ferramentas essenciais.  

2. **CMake**  
   - Geralmente já vem no Visual Studio.  
   - Se precisar: 📥 [cmake.org/download](https://cmake.org/download/)  

### 📝 Passos para Compilar

1. **Clone o repositório:**
   ```sh
   git clone https://github.com/zKauaFerreira/Pratice-AutoClicker.git
    ```

2. **Acesse a pasta do projeto:**

   ```sh
   cd Pratice-AutoClicker
   ```

3. **Crie a pasta de build:**

   ```sh
   mkdir build
   cd build
   ```

4. **Gere os arquivos do projeto:**

   ```sh
   cmake ..
   ```

5. **Compile em modo Release:**

   ```sh
   cmake --build . --config Release
   ```

6. **Encontre o executável:**
   O binário final estará em:

   ```
   build\Release\AutoClicker.exe
   ```

---

## 📂 Arquivo de Configuração (`config.json`)

As configurações ficam salvas em:

```
%APPDATA%\AutoClicker\config.json
```

📌 Principais opções:

* **`enabled`** → Ativa/desativa o autoclicker (`true`/`false`).
* **`cps`** → Define os cliques por segundo.
* **`toggleKey`** → Código da tecla de atalho (Virtual-Key Code, ex: `35 = End`).

🔔 **Notificações:**

* `notificationsEnabled` → Liga/desliga notificações.
* `notifyCorner` → Canto da tela (`top-left`, `top-right`, `bottom-left`, `bottom-right`).
* `notifyDurationSeconds` → Tempo em segundos.
* `silenceNotificationsOnFullscreen` → Suprime em tela cheia.

⚙️ **Outras Configurações:**

* `holdBehaviorM1` / `M2` → Hold-to-click (`true`/`false`).
* `m4LimitSeconds` / `m5LimitSeconds` → Limite de spam (0 = infinito).
* `startWithWindows` → Inicia junto com o Windows.
* `killSwitchKey` → Tecla de emergência para encerrar.

---

## 🛡️ Verificação VirusTotal

O executável `AutoClicker.exe` foi verificado no VirusTotal. Mesmo com o código fonte disponível, esta verificação ajuda usuários novatos a se sentirem seguros.

<details>
<summary>🔗 Clique aqui para ver os detalhes técnicos da verificação</summary>

| Propriedade             | Valor                                                                                                    |
| ----------------------- | -------------------------------------------------------------------------------------------------------- |
| **MD5**                 | `081cc9fdcc543cf101f68ab75f656c3f`                                                                     |
| **SHA-1**               | `57631b789e59d7a6632bfc9a96667f2d2cde2e22`                                                             |
| **SHA-256**             | `31a6bf13a373f5168776e39eb17e65750368a2c25cca47f0b18698e0b573cafa`                                     |
| **Vhash**               | `025066655d155555105023z60028z17z22z386z475z`                                                          |
| **Authentihash**        | `40830212e03a958cdb233f865bb2dfb750fd046d29b8862beae1afc2ac1a3c8c`                                     |
| **Imphash**             | `3d3dd533d1869add465d8fb9bb8b8564`                                                                     |
| **Rich PE header hash** | `34bcd574b2327ef25d43524f6653fe1e`                                                                     |
| **SSDEEP**              | `6144:7lZUVSw6K8BPia4m8kVyPVylI7s17E3v:Aef82+VVc7E3v`                                                 |
| **TLSH**                | `T1C5447C67B34500B6D0F3C1FC8A9351B7F2B3BC29476262CF12B5723A5E76AD1583A612`                             |
| **File Type**           | Win32 EXE                                                                                               |
| **Magic**               | PE32+ executable (GUI) x86-64, for MS Windows                                                          |
| **TrID**                | Win64 Executable (generic) (48.7%) / Win16 NE executable (generic) (23.3%) / OS/2 Executable (generic) (9.3%) / Generic Win/DOS Executable (9.2%) / DOS Executable Generic (9.2%) |
| **DetectItEasy**        | PE64, Compiler: Microsoft Visual C/C++ (19.36.35217), Linker: Microsoft Linker (14.36.35217), Tool: Visual Studio (2022 version 17.6) |
| **Magika**              | PEBIN                                                                                                   |
| **File Size**           | 249.50 KB (255488 bytes)                                                                                |

</details>

#### 🔗 [Abrir análise completa no VirusTotal](https://www.virustotal.com/gui/file/31a6bf13a373f5168776e39eb17e65750368a2c25cca47f0b18698e0b573cafa/details)

---

## 🤝 Como Contribuir

Contribuições são bem-vindas!

* Abra uma *issue* para reportar bugs ou sugerir melhorias.
* Envie um *pull request* se quiser contribuir com código.

<p align="center">
  Desenvolvido com 💙 por <b>Kauã Ferreira</b>!
</p>

