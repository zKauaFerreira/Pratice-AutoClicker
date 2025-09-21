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
| **MD5**                 | `cda65bb4c0f7c228994eb1943c2f74b6`                                                                       |
| **SHA-1**               | `68178679881e8482698da3a0817870e78d80d12d`                                                               |
| **SHA-256**             | `eb874b9e4765f160c49c5be0077a6d52c8111f496e38f317195283094b42d1f2`                                       |
| **Vhash**               | `025066655d155555105023z60028z17z22z376z475z`                                                            |
| **Authentihash**        | `b9ff4ed5b8277b9380a189bd5eec35bc74bfc47a21bccbd6fed141c86902bfc6`                                       |
| **Imphash**             | `981ddffeef62a167ce08ee932f32e981`                                                                       |
| **Rich PE header hash** | `fc61f43648502f89c0bb1217411dd8e7`                                                                       |
| **SSDEEP**              | `3072:Su2H0sFAAUL9ynC9n1vlgpXctCv8bIPzrihu+iqCrM769QoP17E3j6uF7bd:StH02AAWynC91vl7Keu+Z76/7E3v`          |
| **TLSH**                | `T185347D67B24500B6D0F3C1FC8A9792A7F3B3BC19476262CF12B172394E76AD15D3A612`                               |
| **File Type**           | Win32 EXE (PE32+ executable GUI x86-64)                                                                  |
| **Magic**               | PE32+ executable (GUI) x86-64, for MS Windows                                                            |
| **TrID**                | Win64 Executable 48.7% / Win16 NE 23.3% / OS/2 Executable 9.3% / Generic Win/DOS 9.2% / DOS Generic 9.2% |
| **DetectItEasy**        | PE64, Compiler MSVC 19.36.35217, Linker MS Linker 14.36.35217, Tool Visual Studio 2022                   |
| **Magika**              | PEBIN                                                                                                    |
| **File Size**           | 246.50 KB (252416 bytes)                                                                                 |


</details>
🔗 [Abrir análise completa no VirusTotal](https://www.virustotal.com/gui/file/eb874b9e4765f160c49c5be0077a6d52c8111f496e38f317195283094b42d1f2?nocache=1)

---

## 🤝 Como Contribuir

Contribuições são bem-vindas!

* Abra uma *issue* para reportar bugs ou sugerir melhorias.
* Envie um *pull request* se quiser contribuir com código.

<p align="center">
  Desenvolvido com 💙 por <b>Kauã Ferreira</b>!
</p>

