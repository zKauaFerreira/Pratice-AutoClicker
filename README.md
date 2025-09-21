# ✨ AutoClicker C++

O **AutoClicker C++** é um autoclicker de alto desempenho para Windows, criado com o intuito de ser uma ferramenta *prática, leve e configurável* para o dia a dia. Ele roda discretamente na bandeja do sistema (system tray) e não possui uma janela principal, sendo controlado inteiramente por um menu de contexto e uma tecla de atalho global.

---

## 🚀 Como Usar o Aplicativo

Depois de instalar ou executar o `AutoClicker.exe`, um ícone aparecerá na sua bandeja de sistema (próximo ao relógio). Clique com o **botão direito** nesse ícone para abrir o menu de controle.

> **Dica:** A primeira vez que o programa é executado, ele começa **desativado**. Use a tecla de atalho ou o menu para ativá-lo!

### Comportamento Padrão

- **Ativar/Desativar (Toggle):** Use a tecla `End` para ligar ou desligar o autoclicker.
- **Segurar para Clicar (Hold-to-Click):** Com o autoclicker **ativado**, simplesmente segure o `botão esquerdo (M1)` ou o `botão direito (M2)` do mouse para que eles comecem a clicar repetidamente na velocidade definida.
- **Spam Contínuo (Toggle):** Com o autoclicker **ativado**, pressione o botão lateral `M4` do seu mouse para iniciar um spam infinito de cliques esquerdos. Pressione `M4` novamente para parar. O mesmo vale para o botão `M5`, que controla o spam de cliques direitos.

### Menu de Opções

- **✅ Enabled:** Ativa ou desativa globalmente o autoclicker.
- **⚡ Set CPS:** Escolha uma velocidade de cliques (CPS) ou selecione `Custom...` para digitar um valor exato.
- **🔔 Notifications:**
  - `Enable Notifications`: Ativa ou desativa as notificações de ON/OFF.
  - `Silence on Fullscreen`: Impede que as notificações apareçam se você estiver com um aplicativo em tela cheia.
  - `Notification Corner`: Escolhe em qual canto da tela a notificação deve aparecer.
  - `Notification Duration`: Define por quantos segundos a notificação fica visível.
- **⚙️ Other:**
  - `Change Toggle Key`: Abre uma janela para definir uma nova tecla de atalho.
  - `Start with Windows`: Faz o aplicativo iniciar automaticamente com o seu computador.
- **🔄 Reset Settings:** Restaura todas as configurações para os valores padrão.
- **❌ Exit:** Fecha o aplicativo.

---

## 🛠️ Como Compilar (Para Desenvolvedores)

Para compilar o projeto a partir do código-fonte, você precisará dos seguintes pré-requisitos:

### Pré-requisitos

1.  **Visual Studio 2022:** A IDE principal para compilação.
    - **Download:** [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/)
    - **Carga de Trabalho:** Durante a instalação, certifique-se de selecionar a carga de trabalho **"Desenvolvimento para desktop com C++"**. Ela inclui o compilador MSVC, o Windows SDK e outras ferramentas essenciais.

2.  **CMake:** Ferramenta para automatizar o processo de compilação (geralmente incluído na carga de trabalho de C++ do Visual Studio).
    - **Download (Opcional):** [cmake.org/download](https://cmake.org/download/)

### Passos para Compilar

1.  **Clone o repositório:**
    ```sh
    git clone https://github.com/your-username/your-repo-name.git
    ```

2.  **Navegue até a pasta do projeto:**
    ```sh
    cd your-repo-name
    ```

3.  **Crie e acesse a pasta de build:**
    ```sh
    mkdir build
    cd build
    ```

4.  **Gere os arquivos de projeto:**
    ```sh
    cmake ..
    ```

5.  **Compile o projeto:**
    ```sh
    cmake --build . --config Release
    ```

6.  **Encontre o executável:** Após a compilação, o executável final estará em `build\Release\AutoClicker.exe`.

---

## 📂 Arquivo de Configuração (`config.json`)

As configurações são salvas em `%APPDATA%\AutoClicker\config.json`. Você pode editar este arquivo diretamente para ajustes finos.

| Chave Principal   | Chave Aninhada                 | Descrição                                                                 |
| ----------------- | ------------------------------ | ------------------------------------------------------------------------- |
| `enabled`         | -                              | Estado geral do autoclicker (`true`/`false`).                             |
| `cps`             | -                              | Velocidade dos cliques por segundo.                                       |
| `toggleKey`       | -                              | [Virtual-Key Code](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes) da tecla de atalho. Padrão: `35` (End). |
| **`Notifications`** |                                | **Configurações de Notificação**                                          |
|                   | `notificationsEnabled`         | Ativa ou desativa as notificações (`true`/`false`).                       |
|                   | `notifyCorner`                 | Canto da tela: `top-left`, `top-right`, `bottom-left`, `bottom-right`.    |
|                   | `notifyDurationSeconds`        | Duração da notificação em segundos.                                       |
|                   | `silenceNotificationsOnFullscreen` | Suprime notificações em tela cheia (`true`/`false`).                      |
| **`Others`**      |                                | **Outras Configurações**                                                  |
|                   | `holdBehaviorM1`/`M2`          | Ativa o modo "segurar para clicar" para M1/M2 (`true`/`false`).         |
|                   | `m4LimitSeconds`/`m5LimitSeconds` | Duração do spam para M4/M5. Se `0`, é infinito.                         |
|                   | `startWithWindows`             | Inicia com o Windows (`true`/`false`).                                    |
|                   | `killSwitchKey`                | Tecla de emergência para fechar o app. Padrão: `35` (End).                |

---

## 🤝 Como Contribuir

Contribuições são bem-vindas! Sinta-se à vontade para abrir uma *issue* para relatar bugs ou sugerir novas funcionalidades. Se desejar contribuir com código, por favor, abra um *pull request*.

<p align="center">
  Desenvolvido com 💙 por Kauã Ferreira!
</p>
