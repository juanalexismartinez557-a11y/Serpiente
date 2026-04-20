#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Snake.h"
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
        Label^ lblModo;
        Label^ lblTamano;
        ComboBox^ comboModo;
        ComboBox^ comboTamano;
        Button^ btnIniciar;
        Button^ btnTienda;

        // --- Panel de juego ---
        Panel^ panelGame;
        Label^ lblScore;
        Label^ lblAppleCount;
        Label^ lblHighScore;
        Button^ btnMenuDesdeJuego;  // Botón visible solo en Game Over

        // --- Motor del juego ---
        SnakeGame^ game;
        System::Windows::Forms::Timer^ gameTimer;

        // Índices de selección del menú
        int modoSeleccionado;
        int tamanoSeleccionado;

        // =========================================================
        void InitializeComponent(void)
        {
            this->ClientSize = System::Drawing::Size(820, 870);
            this->Text = L"Snake Evolution";
            this->Name = L"Form1";   // Necesario para Application::OpenForms["Form1"]
            this->BackColor = Color::FromArgb(78, 136, 43);
            this->KeyPreview = true;
            this->DoubleBuffered = true;   // Reduce el parpadeo

            // =====================================================
            //  PANEL MENÚ
            // =====================================================
            panelMenu = gcnew Panel();
            panelMenu->BackColor = Color::FromArgb(78, 136, 43);
            panelMenu->Dock = DockStyle::Fill;

            lblTitle = gcnew Label();
            lblTitle->Text = L"🐍 Snake Evolution";
            lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 28, FontStyle::Bold);
            lblTitle->ForeColor = Color::White;
            lblTitle->AutoSize = true;
            lblTitle->Location = System::Drawing::Point(200, 160);

            lblModo = gcnew Label();
            lblModo->Text = L"Modo:";
            lblModo->Font = gcnew System::Drawing::Font(L"Segoe UI", 12);
            lblModo->ForeColor = Color::White;
            lblModo->AutoSize = true;
            lblModo->Location = System::Drawing::Point(260, 310);

            comboModo = gcnew ComboBox();
            comboModo->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            comboModo->Location = System::Drawing::Point(390, 306);
            comboModo->Size = System::Drawing::Size(190, 30);
            comboModo->DropDownStyle = ComboBoxStyle::DropDownList;
            comboModo->Items->Add(L"Obstáculos");           // índice 0 → GameMode::Obstacles
            comboModo->Items->Add(L"Crecimiento Aleatorio"); // índice 1 → GameMode::RandomGrowth
            comboModo->Items->Add(L"Velocidad Creciente");  // índice 2 → GameMode::SpeedIncrease
            comboModo->SelectedIndex = 0;

            lblTamano = gcnew Label();
            lblTamano->Text = L"Tamaño:";
            lblTamano->Font = gcnew System::Drawing::Font(L"Segoe UI", 12);
            lblTamano->ForeColor = Color::White;
            lblTamano->AutoSize = true;
            lblTamano->Location = System::Drawing::Point(260, 370);

            comboTamano = gcnew ComboBox();
            comboTamano->Font = gcnew System::Drawing::Font(L"Segoe UI", 11);
            comboTamano->Location = System::Drawing::Point(390, 366);
            comboTamano->Size = System::Drawing::Size(190, 30);
            comboTamano->DropDownStyle = ComboBoxStyle::DropDownList;
            comboTamano->Items->Add(L"10×10  (Pequeño)");   // índice 0 → BoardSize::Small
            comboTamano->Items->Add(L"25×25  (Mediano)");   // índice 1 → BoardSize::Medium
            comboTamano->Items->Add(L"50×50  (Grande)");    // índice 2 → BoardSize::Large
            comboTamano->SelectedIndex = 1;

            btnIniciar = gcnew Button();
            btnIniciar->Text = L"▶  Iniciar Juego";
            btnIniciar->Font = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            btnIniciar->BackColor = Color::FromArgb(50, 180, 50);
            btnIniciar->ForeColor = Color::White;
            btnIniciar->FlatStyle = FlatStyle::Flat;
            btnIniciar->FlatAppearance->BorderSize = 0;
            btnIniciar->Location = System::Drawing::Point(300, 430);
            btnIniciar->Size = System::Drawing::Size(220, 52);
            btnIniciar->Click += gcnew EventHandler(this, &Form1::IniciarJuego);

            btnTienda = gcnew Button();
            btnTienda->Text = L"🛒  Tienda";
            btnTienda->Font = gcnew System::Drawing::Font(L"Segoe UI", 12);
            btnTienda->BackColor = Color::FromArgb(220, 140, 20);
            btnTienda->ForeColor = Color::White;
            btnTienda->FlatStyle = FlatStyle::Flat;
            btnTienda->FlatAppearance->BorderSize = 0;
            btnTienda->Location = System::Drawing::Point(355, 500);
            btnTienda->Size = System::Drawing::Size(110, 40);
            btnTienda->Click += gcnew EventHandler(this, &Form1::AbrirTienda);

            panelMenu->Controls->Add(lblTitle);
            panelMenu->Controls->Add(lblModo);
            panelMenu->Controls->Add(comboModo);
            panelMenu->Controls->Add(lblTamano);
            panelMenu->Controls->Add(comboTamano);
            panelMenu->Controls->Add(btnIniciar);
            panelMenu->Controls->Add(btnTienda);

            // =====================================================
            //  BARRA SUPERIOR (labels de puntaje)
            // =====================================================
            lblScore = gcnew Label();
            lblScore->Location = System::Drawing::Point(10, 22);
            lblScore->Size = System::Drawing::Size(210, 30);
            lblScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
            lblScore->ForeColor = Color::White;
            lblScore->Text = L"Puntos: 0";
            lblScore->Visible = false;

            lblAppleCount = gcnew Label();
            lblAppleCount->Location = System::Drawing::Point(230, 22);
            lblAppleCount->Size = System::Drawing::Size(220, 30);
            lblAppleCount->Font = gcnew System::Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
            lblAppleCount->ForeColor = Color::White;
            lblAppleCount->Text = L"Manzanas: 0";
            lblAppleCount->Visible = false;

            lblHighScore = gcnew Label();
            lblHighScore->Location = System::Drawing::Point(460, 22);
            lblHighScore->Size = System::Drawing::Size(260, 30);
            lblHighScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
            lblHighScore->ForeColor = Color::Gold;
            lblHighScore->Text = L"Récord: 0";
            lblHighScore->Visible = false;

            // =====================================================
            //  PANEL DE JUEGO
            // =====================================================
            panelGame = gcnew Panel();
            panelGame->BackColor = Color::FromArgb(170, 215, 81);
            panelGame->Location = System::Drawing::Point(10, 60);
            panelGame->Size = System::Drawing::Size(800, 800);
            panelGame->Visible = false;
            panelGame->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame_Paint);

            panelGame->GetType()->GetProperty("DoubleBuffered",
                System::Reflection::BindingFlags::NonPublic |
                System::Reflection::BindingFlags::Instance)
                ->SetValue(panelGame, true, nullptr);

            panelGame->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame_Paint);

            // =====================================================
            //  EVENTOS GLOBALES
            // =====================================================
            this->KeyDown += gcnew KeyEventHandler(this, &Form1::Form1_KeyDown);

            // =====================================================
            //  AGREGAR AL FORM (orden importa: panelMenu al frente)
            // =====================================================
            this->Controls->Add(panelGame);
            this->Controls->Add(lblScore);
            this->Controls->Add(lblAppleCount);
            this->Controls->Add(lblHighScore);
            this->Controls->Add(panelMenu);   // encima de todo en el menú
        }

        // =========================================================
        //  INICIAR JUEGO
        // =========================================================
        void IniciarJuego(Object^ sender, EventArgs^ e)
        {
            modoSeleccionado = comboModo->SelectedIndex;
            tamanoSeleccionado = comboTamano->SelectedIndex;

            InitGame();

            // Ocultar menú, mostrar HUD
            panelMenu->Visible = false;
            panelGame->Visible = true;
            lblScore->Visible = true;
            lblAppleCount->Visible = true;
            lblHighScore->Visible = true;

            panelGame->Focus();  // Para recibir KeyDown sin clic previo
        }

        // =========================================================
        //  INICIALIZAR MOTOR + TIMER
        // =========================================================
        void InitGame()
        {
            // Detener y liberar timer anterior si existe
            if (gameTimer != nullptr) {
                gameTimer->Stop();
                gameTimer = nullptr;
            }

            game = gcnew SnakeGame();

            // --- Aplicar tamaño de tablero ---
            BoardSize bs;
            switch (tamanoSeleccionado) {
            case 0:  bs = BoardSize::Small;  break;
            case 2:  bs = BoardSize::Large;  break;
            default: bs = BoardSize::Medium; break;
            }
            game->SetBoardSize(bs);   // *** CORRECCIÓN: esto faltaba en la versión original ***

            // --- Aplicar modo de juego ---
            GameMode gm;
            switch (modoSeleccionado) {
            case 0:  gm = GameMode::Obstacles;    break;
            case 1:  gm = GameMode::RandomGrowth; break;
            case 2:  gm = GameMode::SpeedIncrease; break;
            default: gm = GameMode::Normal;        break;
            }
            game->SetGameMode(gm);    // *** CORRECCIÓN: esto faltaba en la versión original ***

            // --- Suscribir eventos del juego ---
            game->OnGameOver += gcnew SnakeGame::GameOverDelegate(
                this, &Form1::Game_OnGameOver);
            game->OnScoreChanged += gcnew SnakeGame::ScoreChangedDelegate(
                this, &Form1::Game_OnScoreChanged);
            game->OnSpeedChanged += gcnew SnakeGame::SpeedChangedDelegate(
                this, &Form1::Game_OnSpeedChanged);

            // Actualizar récord en HUD
            lblHighScore->Text = String::Format(L"Récord: {0}", game->HighScore);
            lblScore->Text = L"Puntos: 0";
            lblAppleCount->Text = L"Manzanas: 0";

            // --- Crear y arrancar Timer ---
            gameTimer = gcnew Timer();
            gameTimer->Interval = game->SpeedMs;
            gameTimer->Tick += gcnew EventHandler(this, &Form1::GameTimer_Tick);
            gameTimer->Start();
        }

        // =========================================================
        //  HANDLERS DE EVENTOS DEL JUEGO
        // =========================================================

        // Llamado cuando la serpiente muere
        void Game_OnGameOver(int finalScore)
        {
            gameTimer->Stop();
            // Actualizar récord en HUD (puede haber cambiado)
            lblHighScore->Text = String::Format(L"Récord: {0}", game->HighScore);
            panelGame->Invalidate();
        }

        // Llamado cada vez que cambia el puntaje (al comer manzana)
        void Game_OnScoreChanged(int score, int apples)
        {
            lblScore->Text = String::Format(L"Puntos: {0}", score);
            lblAppleCount->Text = String::Format(L"Manzanas: {0}", apples);
        }

        // Llamado solo en modo SpeedIncrease cuando el intervalo cambia
        void Game_OnSpeedChanged(int newSpeedMs)
        {
            gameTimer->Interval = newSpeedMs;  // *** Actualiza el Timer en tiempo real ***
        }

        // =========================================================
        //  TICK DEL TIMER
        // =========================================================
        void GameTimer_Tick(Object^ sender, EventArgs^ e)
        {
            game->Tick();
            panelGame->Invalidate();
        }

        // =========================================================
        //  PAINT DEL PANEL DE JUEGO
        // =========================================================
        void PanelGame_Paint(Object^ sender, PaintEventArgs^ e)
        {
            if (game != nullptr)
                game->Draw(e->Graphics, panelGame->ClientSize);
        }

        // =========================================================
        //  TECLADO
        // =========================================================
        void Form1_KeyDown(Object^ sender, KeyEventArgs^ e)
        {
            if (game == nullptr) return;

            switch (e->KeyCode)
            {
                // --- Dirección (flechas y WASD) ---
            case Keys::Up:    case Keys::W: game->SetDirection(0, -1);  break;
            case Keys::Down:  case Keys::S: game->SetDirection(0, 1);  break;
            case Keys::Left:  case Keys::A: game->SetDirection(-1, 0);  break;
            case Keys::Right: case Keys::D: game->SetDirection(1, 0);  break;

                // --- Pausa (P o ESC) ---
                // *** CORRECCIÓN: faltaba manejo de pausa en la versión original ***
            case Keys::P:
            case Keys::Escape:
                if (!game->IsGameOver) {
                    game->TogglePause();
                    panelGame->Invalidate();
                }
                break;

                // --- Reiniciar (R) ---
                // *** CORRECCIÓN: faltaba manejo de reinicio en la versión original ***
            case Keys::R:
                if (game->IsGameOver || game->HasStarted) {
                    game->Reset();
                    // Restaurar velocidad del modo (Reset la reinicia en baseSpeedMs)
                    if (gameTimer != nullptr) {
                        gameTimer->Interval = game->SpeedMs;
                        if (!gameTimer->Enabled) gameTimer->Start();
                    }
                    lblScore->Text = L"Puntos: 0";
                    lblAppleCount->Text = L"Manzanas: 0";
                    panelGame->Invalidate();
                }
                break;

                // --- Volver al menú (M) — solo disponible tras Game Over ---
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
            }
        }

        // =========================================================
        //  ABRIR TIENDA
        // =========================================================
        void AbrirTienda(Object^ sender, EventArgs^ e)
        {
            // Pausar el juego si está activo
            if (game != nullptr && game->HasStarted && !game->IsGameOver)
                game->IsPaused = true;

            FormTienda^ tienda = gcnew FormTienda();
            this->Hide();
            tienda->Show();
        }
    };

} // namespace MiProyecto