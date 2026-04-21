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

        // ── Menú ──────────────────────────────────────────────
        Panel^ panelMenu;
        Label^ lblTitle;
        Label^ lblModo;
        CheckBox^ chkObstacles;
        CheckBox^ chkRandomGrowth;
        CheckBox^ chkSpeedIncrease;
        Label^ lblTamano;
        ComboBox^ comboTamano;
        Label^ lblNumJugadores;
        ComboBox^ comboJugadores;   // 1 o 2 jugadores
        Button^ btnIniciar;
        Button^ btnLeaderboard;
        Button^ btnSalir;
        Button^ btnTienda;

        // ── Paneles de juego ──────────────────────────────────
        Panel^ panelGame1;   // Jugador 1 (flechas)
        Panel^ panelGame2;   // Jugador 2 (WASD)  — null en modo 1P

        // ── HUD ───────────────────────────────────────────────
        Label^ lblScore1;
        Label^ lblScore2;
        Label^ lblHighScore;
        Label^ lblP1Tag;     // "J1" encima del panel
        Label^ lblP2Tag;     // "J2" encima del panel

        // ── Motor ─────────────────────────────────────────────
        SnakeGame^ game1;
        SnakeGame^ game2;    // null en modo 1P
        Timer^ gameTimer;

        int  tamanoSeleccionado;
        bool twoPlayerMode;

        // =========================================================
        void InitializeComponent(void)
        {
            Rectangle wa = Screen::PrimaryScreen->WorkingArea;
            int initW = Math::Min(900, (int)(wa.Width * 0.92));
            int initH = Math::Min(870, (int)(wa.Height * 0.92));
            this->ClientSize = System::Drawing::Size(initW, initH);
            this->MinimumSize = System::Drawing::Size(600, 500);
            this->Text = L"Snake Evolution";
            this->Name = L"Form1";
            this->BackColor = Color::FromArgb(78, 136, 43);
            this->KeyPreview = true;
            this->DoubleBuffered = true;
            this->Resize += gcnew EventHandler(this, &Form1::Form1_Resize);

            // ── Panel menú ────────────────────────────────────
            panelMenu = gcnew Panel();
            panelMenu->BackColor = Color::FromArgb(78, 136, 43);
            panelMenu->Dock = DockStyle::Fill;

            lblTitle = gcnew Label();
            lblTitle->Text = L"Snake Evolution";
            lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 28, FontStyle::Bold);
            lblTitle->ForeColor = Color::White;
            lblTitle->AutoSize = true;

            lblModo = gcnew Label();
            lblModo->Text = L"Modos:";
            lblModo->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblModo->ForeColor = Color::White;
            lblModo->AutoSize = true;

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

            lblNumJugadores = gcnew Label();
            lblNumJugadores->Text = L"Jugadores:";
            lblNumJugadores->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblNumJugadores->ForeColor = Color::White;
            lblNumJugadores->AutoSize = true;

            comboJugadores = gcnew ComboBox();
            comboJugadores->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            comboJugadores->Size = System::Drawing::Size(190, 30);
            comboJugadores->DropDownStyle = ComboBoxStyle::DropDownList;
            comboJugadores->Items->Add(L"1 Jugador");
            comboJugadores->Items->Add(L"2 Jugadores");
            comboJugadores->SelectedIndex = 0;

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
            panelMenu->Controls->Add(lblNumJugadores);
            panelMenu->Controls->Add(comboJugadores);
            panelMenu->Controls->Add(btnIniciar);
            panelMenu->Controls->Add(btnLeaderboard);
            panelMenu->Controls->Add(btnTienda);
            panelMenu->Controls->Add(btnSalir);

            // ── HUD ───────────────────────────────────────────
            lblP1Tag = gcnew Label();
            lblP1Tag->Text = L"🎮 J1: Flechas";
            lblP1Tag->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            lblP1Tag->ForeColor = Color::LightCyan;
            lblP1Tag->AutoSize = true;
            lblP1Tag->Visible = false;

            lblP2Tag = gcnew Label();
            lblP2Tag->Text = L"⌨ J2: WASD";
            lblP2Tag->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            lblP2Tag->ForeColor = Color::LightYellow;
            lblP2Tag->AutoSize = true;
            lblP2Tag->Visible = false;

            lblScore1 = gcnew Label();
            lblScore1->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblScore1->ForeColor = Color::LightCyan;
            lblScore1->Text = L"J1: 0 pts";
            lblScore1->AutoSize = true;
            lblScore1->Visible = false;

            lblScore2 = gcnew Label();
            lblScore2->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblScore2->ForeColor = Color::LightYellow;
            lblScore2->Text = L"J2: 0 pts";
            lblScore2->AutoSize = true;
            lblScore2->Visible = false;

            lblHighScore = gcnew Label();
            lblHighScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblHighScore->ForeColor = Color::Gold;
            lblHighScore->Text = L"Record: 0";
            lblHighScore->AutoSize = true;
            lblHighScore->Visible = false;

            // ── Paneles de juego ──────────────────────────────
            panelGame1 = gcnew Panel();
            panelGame1->BackColor = Color::FromArgb(170, 215, 81);
            panelGame1->Visible = false;
            panelGame1->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame1_Paint);
            EnableDoubleBuffer(panelGame1);

            panelGame2 = gcnew Panel();
            panelGame2->BackColor = Color::FromArgb(170, 215, 81);
            panelGame2->Visible = false;
            panelGame2->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame2_Paint);
            EnableDoubleBuffer(panelGame2);

            // ── Eventos globales ──────────────────────────────
            this->KeyDown += gcnew KeyEventHandler(this, &Form1::Form1_KeyDown);

            // ── Agregar al form ───────────────────────────────
            this->Controls->Add(panelGame1);
            this->Controls->Add(panelGame2);
            this->Controls->Add(lblP1Tag);
            this->Controls->Add(lblP2Tag);
            this->Controls->Add(lblScore1);
            this->Controls->Add(lblScore2);
            this->Controls->Add(lblHighScore);
            this->Controls->Add(panelMenu);   // encima de todo

            CenterMenuControls();
        }

        // ── Activa doble buffer en un panel por reflexión ────
        void EnableDoubleBuffer(Panel^ p) {
            p->GetType()->GetProperty("DoubleBuffered",
                System::Reflection::BindingFlags::NonPublic |
                System::Reflection::BindingFlags::Instance)
                ->SetValue(p, true, nullptr);
        }

        // =========================================================
        //  INICIAR JUEGO
        // =========================================================
        void IniciarJuego(Object^ sender, EventArgs^ e)
        {
            tamanoSeleccionado = comboTamano->SelectedIndex;
            twoPlayerMode = (comboJugadores->SelectedIndex == 1);

            // Construir flags de modo
            int flags = 0;
            if (chkObstacles->Checked)     flags |= static_cast<int>(GameMode::Obstacles);
            if (chkRandomGrowth->Checked)  flags |= static_cast<int>(GameMode::RandomGrowth);
            if (chkSpeedIncrease->Checked) flags |= static_cast<int>(GameMode::SpeedIncrease);
            GameMode combinedMode = static_cast<GameMode>(flags);

            // Detener timer anterior
            if (gameTimer != nullptr) { gameTimer->Stop(); gameTimer = nullptr; }

            // ── Juego 1 ──────────────────────────────────────
            game1 = CreateGame(combinedMode);
            game1->OnGameOver += gcnew SnakeGame::GameOverDelegate(this, &Form1::Game1_OnGameOver);
            game1->OnScoreChanged += gcnew SnakeGame::ScoreChangedDelegate(this, &Form1::Game1_OnScoreChanged);
            game1->OnSpeedChanged += gcnew SnakeGame::SpeedChangedDelegate(this, &Form1::Game_OnSpeedChanged);

            // ── Juego 2 (solo en 2P) ─────────────────────────
            if (twoPlayerMode) {
                game2 = CreateGame(combinedMode);
                game2->OnGameOver += gcnew SnakeGame::GameOverDelegate(this, &Form1::Game2_OnGameOver);
                game2->OnScoreChanged += gcnew SnakeGame::ScoreChangedDelegate(this, &Form1::Game2_OnScoreChanged);
                game2->OnSpeedChanged += gcnew SnakeGame::SpeedChangedDelegate(this, &Form1::Game_OnSpeedChanged);
            }
            else {
                game2 = nullptr;
            }

            // ── HUD inicial ───────────────────────────────────
            lblScore1->Text = L"J1: 0 pts";
            lblScore2->Text = L"J2: 0 pts";
            lblHighScore->Text = String::Format(L"Record: {0}", game1->HighScore);

            // Mostrar/ocultar etiquetas según modo
            lblP1Tag->Visible = true;
            lblP2Tag->Visible = twoPlayerMode;
            lblScore1->Visible = true;
            lblScore2->Visible = twoPlayerMode;
            lblHighScore->Visible = true;
            panelMenu->Visible = false;

            // ── Timer compartido ──────────────────────────────
            gameTimer = gcnew Timer();
            gameTimer->Interval = game1->SpeedMs;
            gameTimer->Tick += gcnew EventHandler(this, &Form1::GameTimer_Tick);
            gameTimer->Start();

            // Ajustar paneles y mostrar
            Form1_Resize(nullptr, nullptr);
            panelGame1->Visible = true;
            panelGame2->Visible = twoPlayerMode;
            panelGame1->Focus();
        }

        // ── Factoría de juego ─────────────────────────────────
        SnakeGame^ CreateGame(GameMode mode)
        {
            SnakeGame^ g = gcnew SnakeGame();
            BoardSize bs;
            switch (tamanoSeleccionado) {
            case 0:  bs = BoardSize::Small;  break;
            case 2:  bs = BoardSize::Large;  break;
            default: bs = BoardSize::Medium; break;
            }
            g->SetBoardSize(bs);
            g->SetGameMode(mode);
            return g;
        }

        // =========================================================
        //  TIMER
        // =========================================================
        void GameTimer_Tick(Object^ sender, EventArgs^ e)
        {
            // Tick independiente: si uno ya murió, no sigue avanzando
            if (game1 != nullptr && !game1->IsGameOver)
                game1->Tick();

            if (game2 != nullptr && !game2->IsGameOver)
                game2->Tick();

            panelGame1->Invalidate();
            if (twoPlayerMode) panelGame2->Invalidate();

            // Detener el timer solo cuando AMBOS hayan terminado
            bool g1Done = (game1 == nullptr || game1->IsGameOver);
            bool g2Done = !twoPlayerMode || (game2 == nullptr || game2->IsGameOver);
            if (g1Done && g2Done) gameTimer->Stop();
        }

        // =========================================================
        //  PAINT
        // =========================================================
        void PanelGame1_Paint(Object^ sender, PaintEventArgs^ e)
        {
            if (game1 != nullptr)
                game1->Draw(e->Graphics, panelGame1->ClientSize);
        }

        void PanelGame2_Paint(Object^ sender, PaintEventArgs^ e)
        {
            if (game2 != nullptr)
                game2->Draw(e->Graphics, panelGame2->ClientSize);
        }

        // =========================================================
        //  EVENTOS DE PUNTAJE
        // =========================================================
        void Game1_OnScoreChanged(int score, int apples)
        {
            lblScore1->Text = String::Format(L"J1: {0} pts  🍎{1}", score, apples);
        }

        void Game2_OnScoreChanged(int score, int apples)
        {
            lblScore2->Text = String::Format(L"J2: {0} pts  🍎{1}", score, apples);
        }

        void Game_OnSpeedChanged(int newSpeedMs)
        {
            // Ambos juegos comparten el mismo timer; usa el más rápido
            if (gameTimer != nullptr && newSpeedMs < gameTimer->Interval)
                gameTimer->Interval = newSpeedMs;
        }

        // =========================================================
        //  GAME OVER POR JUGADOR
        // =========================================================
        void Game1_OnGameOver(int finalScore)
        {
            panelGame1->Invalidate();   // muestra overlay "GAME OVER" en J1
            CheckBothDead();
        }

        void Game2_OnGameOver(int finalScore)
        {
            panelGame2->Invalidate();   // muestra overlay "GAME OVER" en J2
            CheckBothDead();
        }

        // Cuando ambos murieron → guardar y mostrar ganador
        void CheckBothDead()
        {
            bool g1Dead = (game1 == nullptr || game1->IsGameOver);
            bool g2Dead = !twoPlayerMode || (game2 == nullptr || game2->IsGameOver);

            if (!g1Dead || !g2Dead) return;   // todavía hay alguien vivo

            gameTimer->Stop();

            if (twoPlayerMode) {
                // Determinar ganador
                int s1 = (game1 != nullptr) ? game1->Score : 0;
                int s2 = (game2 != nullptr) ? game2->Score : 0;

                String^ winnerMsg;
                if (s1 > s2) winnerMsg = String::Format(L"🏆 ¡Ganó el Jugador 1!  ({0} pts)", s1);
                else if (s2 > s1) winnerMsg = String::Format(L"🏆 ¡Ganó el Jugador 2!  ({0} pts)", s2);
                else              winnerMsg = L"🤝 ¡Empate!";

                MessageBox::Show(winnerMsg, L"Resultado final",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }

            // Guardar puntaje del jugador 1
            if (game1 != nullptr) {
                String^ user1 = PromptUsernameFor(L"Jugador 1");
                SaveScore(user1, game1->Score, game1);
            }

            // Guardar puntaje del jugador 2 (si aplica)
            if (twoPlayerMode && game2 != nullptr) {
                String^ user2 = PromptUsernameFor(L"Jugador 2");
                SaveScore(user2, game2->Score, game2);
            }

            // Actualizar récord en HUD
            lblHighScore->Text = String::Format(L"Record: {0}", ScoreManager::GetHighestScore());

            // Refrescar overlays finales con el hint "M = Menú"
            panelGame1->Invalidate();
            if (twoPlayerMode) panelGame2->Invalidate();
        }

        // =========================================================
        //  RESIZE — divide el espacio entre los dos paneles
        // =========================================================
        void Form1_Resize(Object^ sender, EventArgs^ e)
        {
            int sideMargin = 8;
            int topMargin = 50;
            int bottomMargin = 8;
            int gap = 6;   // separación entre los dos paneles

            int totalW = this->ClientSize.Width - sideMargin * 2;
            int totalH = this->ClientSize.Height - topMargin - bottomMargin;

            if (twoPlayerMode) {
                // Mitad izquierda / mitad derecha
                int halfW = (totalW - gap) / 2;

                panelGame1->Location = System::Drawing::Point(sideMargin, topMargin);
                panelGame1->Size = System::Drawing::Size(halfW, totalH);

                panelGame2->Location = System::Drawing::Point(sideMargin + halfW + gap, topMargin);
                panelGame2->Size = System::Drawing::Size(halfW, totalH);

                // Etiquetas de jugador encima de cada panel
                lblP1Tag->Location = System::Drawing::Point(
                    sideMargin, topMargin - lblP1Tag->Height - 4);
                lblScore1->Location = System::Drawing::Point(
                    sideMargin + lblP1Tag->Width + 8, topMargin - lblScore1->Height - 4);

                lblP2Tag->Location = System::Drawing::Point(
                    sideMargin + halfW + gap, topMargin - lblP2Tag->Height - 4);
                lblScore2->Location = System::Drawing::Point(
                    sideMargin + halfW + gap + lblP2Tag->Width + 8, topMargin - lblScore2->Height - 4);

                // Record centrado arriba
                lblHighScore->Location = System::Drawing::Point(
                    (this->ClientSize.Width - lblHighScore->Width) / 2,
                    topMargin - lblHighScore->Height - 4);
            }
            else {
                // Un solo panel ocupa todo el espacio
                panelGame1->Location = System::Drawing::Point(sideMargin, topMargin);
                panelGame1->Size = System::Drawing::Size(totalW, totalH);

                lblP1Tag->Location = System::Drawing::Point(sideMargin, topMargin - lblP1Tag->Height - 4);
                lblScore1->Location = System::Drawing::Point(
                    sideMargin + lblP1Tag->Width + 8, topMargin - lblScore1->Height - 4);
                lblHighScore->Location = System::Drawing::Point(
                    this->ClientSize.Width - lblHighScore->Width - sideMargin,
                    topMargin - lblHighScore->Height - 4);
            }

            CenterMenuControls();
        }

        // =========================================================
        //  TECLADO
        // =========================================================
        void Form1_KeyDown(Object^ sender, KeyEventArgs^ e)
        {
            // ── Jugador 1: flechas ────────────────────────────
            if (game1 != nullptr && !game1->IsGameOver) {
                switch (e->KeyCode) {
                case Keys::Up:    game1->SetDirection(0, -1); break;
                case Keys::Down:  game1->SetDirection(0, 1); break;
                case Keys::Left:  game1->SetDirection(-1, 0); break;
                case Keys::Right: game1->SetDirection(1, 0); break;
                default: break;
                }
            }

            // ── Jugador 2: WASD ───────────────────────────────
            if (twoPlayerMode && game2 != nullptr && !game2->IsGameOver) {
                switch (e->KeyCode) {
                case Keys::W: game2->SetDirection(0, -1); break;
                case Keys::S: game2->SetDirection(0, 1); break;
                case Keys::A: game2->SetDirection(-1, 0); break;
                case Keys::D: game2->SetDirection(1, 0); break;
                default: break;
                }
            }

            // ── Pausa (P) — pausa/reanuda ambos ──────────────
            if (e->KeyCode == Keys::P) {
                bool isPaused = (game1 != nullptr && game1->IsPaused);
                if (game1 != nullptr && !game1->IsGameOver) game1->TogglePause();
                if (game2 != nullptr && !game2->IsGameOver) game2->TogglePause();
                panelGame1->Invalidate();
                if (twoPlayerMode) panelGame2->Invalidate();
            }

            // ── Reiniciar (R) ─────────────────────────────────
            if (e->KeyCode == Keys::R) {
                bool anyOver = (game1 != nullptr && (game1->IsGameOver || game1->HasStarted));
                if (anyOver) {
                    if (game1 != nullptr) { game1->Reset(); }
                    if (game2 != nullptr) { game2->Reset(); }
                    if (gameTimer != nullptr) {
                        gameTimer->Interval = game1->SpeedMs;
                        if (!gameTimer->Enabled) gameTimer->Start();
                    }
                    lblScore1->Text = L"J1: 0 pts";
                    lblScore2->Text = L"J2: 0 pts";
                    panelGame1->Invalidate();
                    if (twoPlayerMode) panelGame2->Invalidate();
                }
            }

            // ── Volver al menú (M) — solo si ambos terminaron ─
            if (e->KeyCode == Keys::M) {
                bool g1Done = (game1 == nullptr || game1->IsGameOver);
                bool g2Done = !twoPlayerMode || (game2 == nullptr || game2->IsGameOver);
                if (g1Done && g2Done) {
                    if (gameTimer != nullptr) gameTimer->Stop();
                    panelGame1->Visible = false;
                    panelGame2->Visible = false;
                    lblP1Tag->Visible = false;
                    lblP2Tag->Visible = false;
                    lblScore1->Visible = false;
                    lblScore2->Visible = false;
                    lblHighScore->Visible = false;
                    panelMenu->Visible = true;
                }
            }

            // ── Leaderboard (L) ───────────────────────────────
            if (e->KeyCode == Keys::L) {
                bool g1Done = (game1 == nullptr || game1->IsGameOver);
                bool g2Done = !twoPlayerMode || (game2 == nullptr || game2->IsGameOver);
                if (g1Done && g2Done) {
                    LeaderboardForm^ lb = gcnew LeaderboardForm();
                    lb->ShowDialog(this);
                }
            }
        }

        // =========================================================
        //  CENTRAR CONTROLES DEL MENÚ
        // =========================================================
        void CenterMenuControls()
        {
            if (panelMenu == nullptr) return;
            int cw = panelMenu->ClientSize.Width;
            int ch = panelMenu->ClientSize.Height;
            int cx = cw / 2;

            lblTitle->Location = System::Drawing::Point(
                (cw - lblTitle->Width) / 2, (int)(ch * 0.08));

            // Modos
            lblModo->Location = System::Drawing::Point(
                (cw - lblModo->Width) / 2, (int)(ch * 0.26));
            int chkX = cx - 120;
            int chkY0 = lblModo->Bottom + 6;
            chkObstacles->Location = System::Drawing::Point(chkX, chkY0);
            chkRandomGrowth->Location = System::Drawing::Point(chkX, chkY0 + 26);
            chkSpeedIncrease->Location = System::Drawing::Point(chkX, chkY0 + 52);

            // Tamaño (label | combo)
            int lx = cx - 10, rx = cx + 10;
            lblTamano->Location = System::Drawing::Point(lx - lblTamano->Width, (int)(ch * 0.51));
            comboTamano->Location = System::Drawing::Point(rx,
                lblTamano->Top + (lblTamano->Height - comboTamano->Height) / 2);

            // Jugadores (label | combo)
            lblNumJugadores->Location = System::Drawing::Point(lx - lblNumJugadores->Width, (int)(ch * 0.60));
            comboJugadores->Location = System::Drawing::Point(rx,
                lblNumJugadores->Top + (lblNumJugadores->Height - comboJugadores->Height) / 2);

            btnIniciar->Location = System::Drawing::Point(
                (cw - btnIniciar->Width) / 2, (int)(ch * 0.70));
            btnLeaderboard->Location = System::Drawing::Point(
                (cw - btnLeaderboard->Width) / 2, btnIniciar->Bottom + 10);
            btnTienda->Location = System::Drawing::Point(
                cx - btnTienda->Width - 10, btnLeaderboard->Bottom + 12);
            btnSalir->Location = System::Drawing::Point(
                cx + 10, btnLeaderboard->Bottom + 12);
        }

        // =========================================================
        //  TIENDA / LEADERBOARD / SALIR
        // =========================================================
        void AbrirTienda(Object^ sender, EventArgs^ e)
        {
            if (game1 != nullptr && game1->HasStarted && !game1->IsGameOver) game1->IsPaused = true;
            if (game2 != nullptr && game2->HasStarted && !game2->IsGameOver) game2->IsPaused = true;
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
        //  PROMPT USERNAME
        // =========================================================
        String^ PromptUsernameFor(String^ playerLabel)
        {
            Form^ prompt = gcnew Form();
            prompt->Text = String::Format(L"Guardar puntaje — {0}", playerLabel);
            prompt->StartPosition = FormStartPosition::CenterParent;
            prompt->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            prompt->ClientSize = System::Drawing::Size(340, 150);
            prompt->MinimizeBox = false; prompt->MaximizeBox = false;
            prompt->BackColor = Color::WhiteSmoke;

            Label^ lbl = gcnew Label();
            lbl->Text = String::Format(L"Username para {0}:", playerLabel);
            lbl->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            lbl->Location = System::Drawing::Point(18, 18);
            lbl->Size = System::Drawing::Size(300, 24);

            TextBox^ txt = gcnew TextBox();
            txt->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            txt->Location = System::Drawing::Point(22, 52);
            txt->Size = System::Drawing::Size(292, 27);
            txt->MaxLength = 24;
            txt->Text = playerLabel;

            Button^ ok = gcnew Button();
            ok->Text = L"Guardar";
            ok->BackColor = Color::FromArgb(50, 180, 50);
            ok->ForeColor = Color::White;
            ok->FlatStyle = FlatStyle::Flat;
            ok->FlatAppearance->BorderSize = 0;
            ok->Location = System::Drawing::Point(146, 98);
            ok->Size = System::Drawing::Size(80, 32);
            ok->DialogResult = System::Windows::Forms::DialogResult::OK;

            Button^ cancel = gcnew Button();
            cancel->Text = L"Omitir";
            cancel->Location = System::Drawing::Point(234, 98);
            cancel->Size = System::Drawing::Size(80, 32);
            cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

            prompt->AcceptButton = ok;
            prompt->CancelButton = cancel;
            prompt->Controls->Add(lbl);
            prompt->Controls->Add(txt);
            prompt->Controls->Add(ok);
            prompt->Controls->Add(cancel);

            String^ result = playerLabel;
            if (prompt->ShowDialog(this) == System::Windows::Forms::DialogResult::OK &&
                !String::IsNullOrWhiteSpace(txt->Text))
                result = txt->Text->Trim();

            delete prompt;
            return result;
        }

        // =========================================================
        //  GUARDAR PUNTAJE
        // =========================================================
        void SaveScore(String^ username, int score, SnakeGame^ g)
        {
            if (g == nullptr) return;
            String^ boardSize = String::Format(L"{0}x{1}", g->CurrentCols, g->CurrentRows);
            String^ gameMode = GetGameModeLabel(g->CurrentMode);
            ScoreManager::SaveScore(username, score, boardSize, gameMode);
            g->HighScore = ScoreManager::GetHighestScore();
        }

        String^ GetGameModeLabel(GameMode mode)
        {
            if (mode == GameMode::Normal) return L"Normal";
            System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
            if ((static_cast<int>(mode) & static_cast<int>(GameMode::Obstacles)) != 0) {
                if (sb->Length > 0) sb->Append(L"+"); sb->Append(L"Obstaculos");
            }
            if ((static_cast<int>(mode) & static_cast<int>(GameMode::RandomGrowth)) != 0) {
                if (sb->Length > 0) sb->Append(L"+"); sb->Append(L"RandomGrowth");
            }
            if ((static_cast<int>(mode) & static_cast<int>(GameMode::SpeedIncrease)) != 0) {
                if (sb->Length > 0) sb->Append(L"+"); sb->Append(L"SpeedIncrease");
            }
            return sb->ToString();
        }
    };

} // namespace MiProyecto