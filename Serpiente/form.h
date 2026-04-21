#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Snake.h"
#include "ScoreManager.h"
#include "LeaderboardForm.h"
#include "formtienda.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void) { InitializeComponent(); }

    protected:
        ~Form1() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;

        // --- Panel de menú principal ---
        Panel^ panelMenu;
        Label^ lblTitle;
        Label^ lblModo;       // Etiqueta "Modos:"
        CheckBox^ chkObstacles;
        CheckBox^ chkRandomGrowth;
        CheckBox^ chkSpeedIncrease;
        Label^ lblTamano;
        ComboBox^ comboTamano;
        Button^ btnIniciar;
        Button^ btnLeaderboard;
        Button^ btnSalir;
        Button^ btnTienda;

        // --- Panel de juego ---
        Panel^ panelGame;
        Label^ lblScore;
        Label^ lblAppleCount;
        Label^ lblHighScore;
        Button^ btnMenuDesdeJuego;

        // --- Motor del juego ---
        SnakeGame^ game;
        Timer^ gameTimer;

        int tamanoSeleccionado;

        // =========================================================
        void InitializeComponent(void)
        {
            Rectangle wa = Screen::PrimaryScreen->WorkingArea;
            int initW = Math::Min(820, (int)(wa.Width * 0.90));
            int initH = Math::Min(870, (int)(wa.Height * 0.90));
            this->ClientSize = System::Drawing::Size(initW, initH);
            this->MinimumSize = System::Drawing::Size(500, 450);
            this->Text = L"Snake Evolution";
            this->Name = L"Form1";
            this->BackColor = Color::FromArgb(78, 136, 43);
            this->KeyPreview = true;
            this->DoubleBuffered = true;
            this->Resize += gcnew EventHandler(this, &Form1::Form1_Resize);

            // =====================================================
            //  PANEL MENÚ
            // =====================================================
            panelMenu = gcnew Panel();
            panelMenu->BackColor = Color::FromArgb(78, 136, 43);
            panelMenu->Dock = DockStyle::Fill;

            lblTitle = gcnew Label();
            lblTitle->Text = L"Snake Evolution";
            lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 28, FontStyle::Bold);
            lblTitle->ForeColor = Color::White;
            lblTitle->AutoSize = true;

            // --- Etiqueta "Modos:" ---
            lblModo = gcnew Label();
            lblModo->Text = L"Modos:";
            lblModo->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblModo->ForeColor = Color::White;
            lblModo->AutoSize = true;

            // --- CheckBoxes de modos ---
            chkObstacles = gcnew CheckBox();
            chkObstacles->Text = L"Obstáculos";
            chkObstacles->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            chkObstacles->ForeColor = Color::White;
            chkObstacles->AutoSize = true;

            chkRandomGrowth = gcnew CheckBox();
            chkRandomGrowth->Text = L"Crecimiento Aleatorio";
            chkRandomGrowth->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            chkRandomGrowth->ForeColor = Color::White;
            chkRandomGrowth->AutoSize = true;

            chkSpeedIncrease = gcnew CheckBox();
            chkSpeedIncrease->Text = L"Velocidad Creciente";
            chkSpeedIncrease->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            chkSpeedIncrease->ForeColor = Color::White;
            chkSpeedIncrease->AutoSize = true;

            // --- Tamaño ---
            lblTamano = gcnew Label();
            lblTamano->Text = L"Tamaño:";
            lblTamano->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblTamano->ForeColor = Color::White;
            lblTamano->AutoSize = true;

            comboTamano = gcnew ComboBox();
            comboTamano->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            comboTamano->Size = System::Drawing::Size(190, 30);
            comboTamano->DropDownStyle = ComboBoxStyle::DropDownList;
            comboTamano->Items->Add(L"10x10  (Pequeño)");
            comboTamano->Items->Add(L"25x25  (Mediano)");
            comboTamano->Items->Add(L"50x50  (Grande)");
            comboTamano->SelectedIndex = 1;

            btnIniciar = gcnew Button();
            btnIniciar->Text = L"Iniciar Juego";
            btnIniciar->Font = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            btnIniciar->BackColor = Color::FromArgb(50, 180, 50);
            btnIniciar->ForeColor = Color::White;
            btnIniciar->FlatStyle = FlatStyle::Flat;
            btnIniciar->FlatAppearance->BorderSize = 0;
            btnIniciar->Size = System::Drawing::Size(220, 52);
            btnIniciar->Click += gcnew EventHandler(this, &Form1::IniciarJuego);

            btnLeaderboard = gcnew Button();
            btnLeaderboard->Text = L"Leaderboard";
            btnLeaderboard->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            btnLeaderboard->BackColor = Color::FromArgb(44, 110, 73);
            btnLeaderboard->ForeColor = Color::White;
            btnLeaderboard->FlatStyle = FlatStyle::Flat;
            btnLeaderboard->FlatAppearance->BorderSize = 0;
            btnLeaderboard->Size = System::Drawing::Size(220, 44);
            btnLeaderboard->Click += gcnew EventHandler(this, &Form1::AbrirLeaderboard);

            btnTienda = gcnew Button();
            btnTienda->Text = L"Tienda";
            btnTienda->Font = gcnew System::Drawing::Font(L"Segoe UI", 12);
            btnTienda->BackColor = Color::FromArgb(220, 140, 20);
            btnTienda->ForeColor = Color::White;
            btnTienda->FlatStyle = FlatStyle::Flat;
            btnTienda->FlatAppearance->BorderSize = 0;
            btnTienda->Size = System::Drawing::Size(110, 40);
            btnTienda->Click += gcnew EventHandler(this, &Form1::AbrirTienda);

            btnSalir = gcnew Button();
            btnSalir->Text = L"Salir";
            btnSalir->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            btnSalir->BackColor = Color::FromArgb(130, 45, 45);
            btnSalir->ForeColor = Color::White;
            btnSalir->FlatStyle = FlatStyle::Flat;
            btnSalir->FlatAppearance->BorderSize = 0;
            btnSalir->Size = System::Drawing::Size(110, 40);
            btnSalir->Click += gcnew EventHandler(this, &Form1::SalirAplicacion);

            panelMenu->Controls->Add(lblTitle);
            panelMenu->Controls->Add(lblModo);
            panelMenu->Controls->Add(chkObstacles);
            panelMenu->Controls->Add(chkRandomGrowth);
            panelMenu->Controls->Add(chkSpeedIncrease);
            panelMenu->Controls->Add(lblTamano);
            panelMenu->Controls->Add(comboTamano);
            panelMenu->Controls->Add(btnIniciar);
            panelMenu->Controls->Add(btnLeaderboard);
            panelMenu->Controls->Add(btnTienda);
            panelMenu->Controls->Add(btnSalir);

            // =====================================================
            //  HUD
            // =====================================================
            lblScore = gcnew Label();
            lblScore->Location = System::Drawing::Point(10, 22);
            lblScore->Size = System::Drawing::Size(210, 30);
            lblScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
            lblScore->ForeColor = Color::White;
            lblScore->Text = L"Puntos: 0";
            lblScore->Anchor = AnchorStyles::Top | AnchorStyles::Left;
            lblScore->Visible = false;

            lblAppleCount = gcnew Label();
            lblAppleCount->Location = System::Drawing::Point(230, 22);
            lblAppleCount->Size = System::Drawing::Size(220, 30);
            lblAppleCount->Font = gcnew System::Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
            lblAppleCount->ForeColor = Color::White;
            lblAppleCount->Text = L"Manzanas: 0";
            lblAppleCount->Anchor = AnchorStyles::Top | AnchorStyles::Left;
            lblAppleCount->Visible = false;

            lblHighScore = gcnew Label();
            lblHighScore->Location = System::Drawing::Point(460, 22);
            lblHighScore->Size = System::Drawing::Size(260, 30);
            lblHighScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
            lblHighScore->ForeColor = Color::Gold;
            lblHighScore->Text = L"Record: 0";
            lblHighScore->Anchor = AnchorStyles::Top | AnchorStyles::Left;
            lblHighScore->Visible = false;

            // =====================================================
            //  PANEL DE JUEGO
            // =====================================================
            panelGame = gcnew Panel();
            panelGame->BackColor = Color::FromArgb(170, 215, 81);
            panelGame->Location = System::Drawing::Point(10, 60);
            panelGame->Visible = false;
            panelGame->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame_Paint);

            panelGame->GetType()->GetProperty("DoubleBuffered",
                System::Reflection::BindingFlags::NonPublic |
                System::Reflection::BindingFlags::Instance)
                ->SetValue(panelGame, true, nullptr);

            this->KeyDown += gcnew KeyEventHandler(this, &Form1::Form1_KeyDown);

            this->Controls->Add(panelGame);
            this->Controls->Add(lblScore);
            this->Controls->Add(lblAppleCount);
            this->Controls->Add(lblHighScore);
            this->Controls->Add(panelMenu);

            CenterMenuControls();
        }

        // =========================================================
        //  INICIAR JUEGO
        // =========================================================
        void IniciarJuego(Object^ sender, EventArgs^ e)
        {
            tamanoSeleccionado = comboTamano->SelectedIndex;

            // Combinar flags según checkboxes marcados
            int flags = 0;
            if (chkObstacles->Checked)     flags |= static_cast<int>(GameMode::Obstacles);
            if (chkRandomGrowth->Checked)  flags |= static_cast<int>(GameMode::RandomGrowth);
            if (chkSpeedIncrease->Checked) flags |= static_cast<int>(GameMode::SpeedIncrease);
            // Si no se marcó nada → Normal (0)

            InitGame(static_cast<GameMode>(flags));

            panelMenu->Visible = false;
            lblScore->Visible = true;
            lblAppleCount->Visible = true;
            lblHighScore->Visible = true;

            Form1_Resize(nullptr, nullptr);
            panelGame->Visible = true;
            panelGame->Focus();
        }

        // =========================================================
        //  INICIALIZAR MOTOR + TIMER
        // =========================================================
        void InitGame(GameMode combinedMode)
        {
            if (gameTimer != nullptr) { gameTimer->Stop(); gameTimer = nullptr; }

            game = gcnew SnakeGame();

            BoardSize bs;
            switch (tamanoSeleccionado) {
            case 0:  bs = BoardSize::Small;  break;
            case 2:  bs = BoardSize::Large;  break;
            default: bs = BoardSize::Medium; break;
            }
            game->SetBoardSize(bs);
            game->SetGameMode(combinedMode);   // pasa la combinación de flags

            game->OnGameOver += gcnew SnakeGame::GameOverDelegate(this, &Form1::Game_OnGameOver);
            game->OnScoreChanged += gcnew SnakeGame::ScoreChangedDelegate(this, &Form1::Game_OnScoreChanged);
            game->OnSpeedChanged += gcnew SnakeGame::SpeedChangedDelegate(this, &Form1::Game_OnSpeedChanged);

            lblHighScore->Text = String::Format(L"Record: {0}", game->HighScore);
            lblScore->Text = L"Puntos: 0";
            lblAppleCount->Text = L"Manzanas: 0";

            gameTimer = gcnew Timer();
            gameTimer->Interval = game->SpeedMs;
            gameTimer->Tick += gcnew EventHandler(this, &Form1::GameTimer_Tick);
            gameTimer->Start();
        }

        // =========================================================
        //  RESIZE
        // =========================================================
        void Form1_Resize(Object^ sender, EventArgs^ e)
        {
            if (panelGame != nullptr) {
                int sideMargin = 10, topMargin = 55, bottomMargin = 10;
                int newWidth = Math::Max(100, this->ClientSize.Width - sideMargin * 2);
                int newHeight = Math::Max(100, this->ClientSize.Height - topMargin - bottomMargin);
                panelGame->Location = System::Drawing::Point(sideMargin, topMargin);
                panelGame->Size = System::Drawing::Size(newWidth, newHeight);
            }
            CenterMenuControls();
        }

        // =========================================================
        //  EVENTOS DEL JUEGO
        // =========================================================
        void Game_OnGameOver(int finalScore)
        {
            gameTimer->Stop();
            String^ username = PromptUsername();
            SaveScore(username, finalScore);
            lblHighScore->Text = String::Format(L"Record: {0}", game->HighScore);
            panelGame->Invalidate();
        }

        void Game_OnScoreChanged(int score, int apples)
        {
            lblScore->Text = String::Format(L"Puntos: {0}", score);
            lblAppleCount->Text = String::Format(L"Manzanas: {0}", apples);
        }

        void Game_OnSpeedChanged(int newSpeedMs)
        {
            gameTimer->Interval = newSpeedMs;
        }

        // =========================================================
        //  CENTRAR CONTROLES DEL MENÚ
        // =========================================================
        void CenterMenuControls()
        {
            if (panelMenu == nullptr) return;
            int cw = panelMenu->ClientSize.Width;
            int ch = panelMenu->ClientSize.Height;
            int centerX = cw / 2;

            lblTitle->Location = System::Drawing::Point(
                (cw - lblTitle->Width) / 2, (int)(ch * 0.10));

            // Etiqueta "Modos:" centrada
            lblModo->Location = System::Drawing::Point(
                (cw - lblModo->Width) / 2, (int)(ch * 0.28));

            // Los 3 checkboxes apilados bajo la etiqueta, alineados a la izquierda del centro
            int chkX = centerX - 110;   // margen izquierdo fijo para alinear los 3
            int chkY0 = lblModo->Bottom + 8;
            chkObstacles->Location = System::Drawing::Point(chkX, chkY0);
            chkRandomGrowth->Location = System::Drawing::Point(chkX, chkY0 + 28);
            chkSpeedIncrease->Location = System::Drawing::Point(chkX, chkY0 + 56);

            // Tamaño: label a la izquierda del centro, combo a la derecha
            int labelRightX = centerX - 10;
            int controlLeftX = centerX + 10;

            lblTamano->Location = System::Drawing::Point(
                labelRightX - lblTamano->Width, (int)(ch * 0.55));
            comboTamano->Location = System::Drawing::Point(
                controlLeftX,
                lblTamano->Top + (lblTamano->Height - comboTamano->Height) / 2);

            btnIniciar->Location = System::Drawing::Point(
                (cw - btnIniciar->Width) / 2, (int)(ch * 0.65));

            btnLeaderboard->Location = System::Drawing::Point(
                (cw - btnLeaderboard->Width) / 2, btnIniciar->Bottom + 12);

            btnTienda->Location = System::Drawing::Point(
                centerX - btnTienda->Width - 12, btnLeaderboard->Bottom + 14);

            btnSalir->Location = System::Drawing::Point(
                centerX + 12, btnLeaderboard->Bottom + 14);
        }

        // =========================================================
        //  TICK / PAINT / TECLADO
        // =========================================================
        void GameTimer_Tick(Object^ sender, EventArgs^ e)
        {
            game->Tick();
            panelGame->Invalidate();
        }

        void PanelGame_Paint(Object^ sender, PaintEventArgs^ e)
        {
            if (game != nullptr)
                game->Draw(e->Graphics, panelGame->ClientSize);
        }

        void Form1_KeyDown(Object^ sender, KeyEventArgs^ e)
        {
            if (game == nullptr) return;
            switch (e->KeyCode)
            {
            case Keys::Up:    case Keys::W: game->SetDirection(0, -1); break;
            case Keys::Down:  case Keys::S: game->SetDirection(0, 1); break;
            case Keys::Left:  case Keys::A: game->SetDirection(-1, 0); break;
            case Keys::Right: case Keys::D: game->SetDirection(1, 0); break;

            case Keys::P:
            case Keys::Escape:
                if (!game->IsGameOver) { game->TogglePause(); panelGame->Invalidate(); }
                break;

            case Keys::R:
                if (game->IsGameOver || game->HasStarted) {
                    game->Reset();
                    if (gameTimer != nullptr) {
                        gameTimer->Interval = game->SpeedMs;
                        if (!gameTimer->Enabled) gameTimer->Start();
                    }
                    lblScore->Text = L"Puntos: 0";
                    lblAppleCount->Text = L"Manzanas: 0";
                    panelGame->Invalidate();
                }
                break;

            case Keys::M:
                if (game->IsGameOver) {
                    if (gameTimer != nullptr) gameTimer->Stop();
                    panelGame->Visible = false;
                    lblScore->Visible = false;
                    lblAppleCount->Visible = false;
                    lblHighScore->Visible = false;
                    panelMenu->Visible = true;
                }
                break;

            case Keys::L:
                if (game->IsGameOver) {
                    LeaderboardForm^ lb = gcnew LeaderboardForm();
                    lb->ShowDialog(this);
                }
                break;
            }
        }

        // =========================================================
        //  NAVEGACIÓN
        // =========================================================
        void AbrirTienda(Object^ sender, EventArgs^ e)
        {
            if (game != nullptr && game->HasStarted && !game->IsGameOver)
                game->IsPaused = true;
            FormTienda^ tienda = gcnew FormTienda();
            this->Hide();
            tienda->Show();
        }

        void AbrirLeaderboard(Object^ sender, EventArgs^ e)
        {
            LeaderboardForm^ lb = gcnew LeaderboardForm();
            lb->ShowDialog(this);
        }

        void SalirAplicacion(Object^ sender, EventArgs^ e) { this->Close(); }

        // =========================================================
        //  GUARDAR PUNTUACIÓN
        // =========================================================
        String^ PromptUsername()
        {
            Form^ prompt = gcnew Form();
            prompt->Text = L"Guardar puntaje";
            prompt->StartPosition = FormStartPosition::CenterParent;
            prompt->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            prompt->ClientSize = System::Drawing::Size(340, 150);
            prompt->MinimizeBox = false; prompt->MaximizeBox = false;
            prompt->BackColor = Color::WhiteSmoke;

            Label^ lblPrompt = gcnew Label();
            lblPrompt->Text = L"Ingresa tu username:";
            lblPrompt->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            lblPrompt->Location = System::Drawing::Point(18, 18);
            lblPrompt->Size = System::Drawing::Size(280, 24);

            TextBox^ txtUsername = gcnew TextBox();
            txtUsername->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            txtUsername->Location = System::Drawing::Point(22, 52);
            txtUsername->Size = System::Drawing::Size(292, 27);
            txtUsername->MaxLength = 24;
            txtUsername->Text = L"Jugador";

            Button^ btnOk = gcnew Button();
            btnOk->Text = L"Guardar";
            btnOk->BackColor = Color::FromArgb(50, 180, 50);
            btnOk->ForeColor = Color::White;
            btnOk->FlatStyle = FlatStyle::Flat;
            btnOk->FlatAppearance->BorderSize = 0;
            btnOk->Location = System::Drawing::Point(146, 98);
            btnOk->Size = System::Drawing::Size(80, 32);
            btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;

            Button^ btnCancel = gcnew Button();
            btnCancel->Text = L"Omitir";
            btnCancel->Location = System::Drawing::Point(234, 98);
            btnCancel->Size = System::Drawing::Size(80, 32);
            btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

            prompt->AcceptButton = btnOk;
            prompt->CancelButton = btnCancel;
            prompt->Controls->Add(lblPrompt);
            prompt->Controls->Add(txtUsername);
            prompt->Controls->Add(btnOk);
            prompt->Controls->Add(btnCancel);

            String^ username = L"Jugador";
            if (prompt->ShowDialog(this) == System::Windows::Forms::DialogResult::OK &&
                !String::IsNullOrWhiteSpace(txtUsername->Text))
                username = txtUsername->Text->Trim();

            delete prompt;
            return username;
        }

        void SaveScore(String^ username, int score)
        {
            if (game == nullptr) return;
            String^ boardSize = GetBoardSizeLabel(game->CurrentCols, game->CurrentRows);
            String^ gameMode = GetGameModeLabel(game->CurrentMode);
            ScoreManager::SaveScore(username, score, boardSize, gameMode);
            game->HighScore = ScoreManager::GetHighestScore();
            lblHighScore->Text = String::Format(L"Record: {0}", ScoreManager::GetHighestScore());
        }

        String^ GetBoardSizeLabel(int cols, int rows)
        {
            return String::Format(L"{0}x{1}", cols, rows);
        }

        // Genera una etiqueta legible con todos los modos activos
        String^ GetGameModeLabel(GameMode mode)
        {
            if (mode == GameMode::Normal) return L"Normal";

            System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
            if ((static_cast<int>(mode) & static_cast<int>(GameMode::Obstacles)) != 0) {
                if (sb->Length > 0) sb->Append(L"+");
                sb->Append(L"Obstaculos");
            }
            if ((static_cast<int>(mode) & static_cast<int>(GameMode::RandomGrowth)) != 0) {
                if (sb->Length > 0) sb->Append(L"+");
                sb->Append(L"RandomGrowth");
            }
            if ((static_cast<int>(mode) & static_cast<int>(GameMode::SpeedIncrease)) != 0) {
                if (sb->Length > 0) sb->Append(L"+");
                sb->Append(L"SpeedIncrease");
            }
            return sb->ToString();
        }
    };

} // namespace MiProyecto