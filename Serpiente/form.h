#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Snake.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            InitializeComponent();
            InitGame();
        }

    protected:
        ~Form1()
        {
            if (components) delete components;
        }

        // =========================================================
        //  MIEMBROS PRIVADOS
        // =========================================================
    private:
        System::ComponentModel::Container^ components;

        // --- UI ---
        Panel^ panelTop;
        Panel^ panelGame;
        Label^ lblApple;
        Label^ lblAppleCount;
        Label^ lblTrophy;
        Label^ lblScore;
        Label^ lblCoin;
        Label^ lblCoinCount;
        Label^ lblHighScore;   // label extra para el high score

        // --- Juego ---
        SnakeGame^ game;
        System::Windows::Forms::Timer^ gameTimer;

        // =========================================================
        //  INICIALIZAR COMPONENTES (UI)
        // =========================================================
        void InitializeComponent(void)
        {
            this->panelTop = gcnew Panel();
            this->panelGame = gcnew Panel();
            this->lblApple = gcnew Label();
            this->lblAppleCount = gcnew Label();
            this->lblTrophy = gcnew Label();
            this->lblScore = gcnew Label();
            this->lblCoin = gcnew Label();
            this->lblCoinCount = gcnew Label();
            this->lblHighScore = gcnew Label();

            this->SuspendLayout();

            // ---- Formulario ----
            this->AutoScaleDimensions = SizeF(8, 16);
            this->AutoScaleMode = AutoScaleMode::Font;
            this->ClientSize = Size(820, 820);
            this->Name = L"Form1";
            this->Text = L"Snake Evolution";
            this->BackColor = Color::FromArgb(78, 136, 43);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->KeyPreview = true;   // Recibir teclas aunque el panel tenga foco

            // ---- Panel superior ----
            this->panelTop->BackColor = Color::FromArgb(73, 123, 40);
            this->panelTop->Location = Point(0, 0);
            this->panelTop->Size = Size(820, 88);
            this->panelTop->TabIndex = 0;

            // Manzana emoji
            this->lblApple->AutoSize = true;
            this->lblApple->Font = gcnew Font(L"Segoe UI Emoji", 20, FontStyle::Regular);
            this->lblApple->ForeColor = Color::White;
            this->lblApple->Location = Point(25, 22);
            this->lblApple->Text = L"🍎";

            // Contador manzanas
            this->lblAppleCount->AutoSize = true;
            this->lblAppleCount->Font = gcnew Font(L"Segoe UI", 18, FontStyle::Regular);
            this->lblAppleCount->ForeColor = Color::White;
            this->lblAppleCount->Location = Point(78, 25);
            this->lblAppleCount->Text = L"0";

            // Trofeo emoji
            this->lblTrophy->AutoSize = true;
            this->lblTrophy->Font = gcnew Font(L"Segoe UI Emoji", 20, FontStyle::Regular);
            this->lblTrophy->ForeColor = Color::White;
            this->lblTrophy->Location = Point(320, 22);
            this->lblTrophy->Text = L"🏆";

            // Puntaje actual
            this->lblScore->AutoSize = true;
            this->lblScore->Font = gcnew Font(L"Segoe UI", 18, FontStyle::Regular);
            this->lblScore->ForeColor = Color::White;
            this->lblScore->Location = Point(375, 25);
            this->lblScore->Text = L"0";

            // Moneda emoji (high score)
            this->lblCoin->AutoSize = true;
            this->lblCoin->Font = gcnew Font(L"Segoe UI Emoji", 20, FontStyle::Regular);
            this->lblCoin->ForeColor = Color::White;
            this->lblCoin->Location = Point(600, 22);
            this->lblCoin->Text = L"💰";

            // High score guardado
            this->lblHighScore->AutoSize = true;
            this->lblHighScore->Font = gcnew Font(L"Segoe UI", 14, FontStyle::Regular);
            this->lblHighScore->ForeColor = Color::FromArgb(255, 230, 100);
            this->lblHighScore->Location = Point(653, 10);
            this->lblHighScore->Text = L"BEST";

            // Contador monedas (lblCoinCount = high score numerico)
            this->lblCoinCount->AutoSize = true;
            this->lblCoinCount->Font = gcnew Font(L"Segoe UI", 18, FontStyle::Bold);
            this->lblCoinCount->ForeColor = Color::White;
            this->lblCoinCount->Location = Point(653, 35);
            this->lblCoinCount->Text = L"0";

            // Agregar a panel superior
            this->panelTop->Controls->Add(this->lblApple);
            this->panelTop->Controls->Add(this->lblAppleCount);
            this->panelTop->Controls->Add(this->lblTrophy);
            this->panelTop->Controls->Add(this->lblScore);
            this->panelTop->Controls->Add(this->lblCoin);
            this->panelTop->Controls->Add(this->lblHighScore);
            this->panelTop->Controls->Add(this->lblCoinCount);

            // ---- Panel de juego ----
            this->panelGame->BackColor = Color::FromArgb(170, 215, 81);
            this->panelGame->Location = Point(30, 120);
            this->panelGame->Size = Size(750, 660);
            this->panelGame->TabIndex = 1;
            this->panelGame->BorderStyle = BorderStyle::None;

            // Conectar evento Paint del panel
            this->panelGame->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame_Paint);

            // Eventos del formulario
            this->KeyDown += gcnew KeyEventHandler(this, &Form1::Form1_KeyDown);

            // Agregar paneles al form
            this->Controls->Add(this->panelTop);
            this->Controls->Add(this->panelGame);

            this->ResumeLayout(false);
        }

        // =========================================================
        //  INICIALIZAR JUEGO
        // =========================================================
        void InitGame() {
            game = gcnew SnakeGame();

            // Ejemplo: para activar atravesar paredes cambia a true
            game->WallPassThrough = false;

            // Conectar eventos del juego
            game->OnGameOver += gcnew SnakeGame::GameOverDelegate(this, &Form1::Game_OnGameOver);
            game->OnScoreChanged += gcnew SnakeGame::ScoreChangedDelegate(this, &Form1::Game_OnScoreChanged);

            // Actualizar high score en UI
            lblCoinCount->Text = game->HighScore.ToString();

            // Timer del juego
            gameTimer = gcnew System::Windows::Forms::Timer();
            gameTimer->Interval = game->SpeedMs;
            gameTimer->Tick += gcnew EventHandler(this, &Form1::GameTimer_Tick);
            gameTimer->Start();
        }

        // =========================================================
        //  EVENTO: Tick del timer -> avanzar juego + redibujar
        // =========================================================
        void GameTimer_Tick(Object^ sender, EventArgs^ e) {
            game->Tick();
            panelGame->Invalidate(); // fuerza repaint
        }

        // =========================================================
        //  EVENTO: Paint del panel de juego
        // =========================================================
        void PanelGame_Paint(Object^ sender, PaintEventArgs^ e) {
            game->Draw(e->Graphics);
        }

        // =========================================================
        //  EVENTO: Teclado
        // =========================================================
        void Form1_KeyDown(Object^ sender, KeyEventArgs^ e) {
            switch (e->KeyCode) {
                // Movimiento
            case Keys::Up:    case Keys::W: game->SetDirection(0, -1);  break;
            case Keys::Down:  case Keys::S: game->SetDirection(0, 1);  break;
            case Keys::Left:  case Keys::A: game->SetDirection(-1, 0);  break;
            case Keys::Right: case Keys::D: game->SetDirection(1, 0);  break;

                // Pausa
            case Keys::P:
            case Keys::Escape:
                game->TogglePause();
                break;

                // Reiniciar (solo cuando game over o en pausa)
            case Keys::R:
                if (game->IsGameOver || game->IsPaused) {
                    game->Reset();
                    gameTimer->Interval = game->SpeedMs;
                    UpdateUI(0, 0);
                }
                break;
            }
            e->Handled = true;
        }

        // =========================================================
        //  EVENTOS DEL JUEGO
        // =========================================================
        void Game_OnGameOver(int finalScore) {
            // Mostrar ranking en consola de debug (o podrias abrir un Form nuevo)
            List<int>^ ranking = game->LoadScores();
            System::Diagnostics::Debug::WriteLine("=== RANKING ===");
            for (int i = 0; i < ranking->Count; i++) {
                System::Diagnostics::Debug::WriteLine(
                    String::Format("{0}. {1}", i + 1, ranking[i])
                );
            }

            // Actualizar high score en UI
            lblCoinCount->Text = game->HighScore.ToString();
        }

        void Game_OnScoreChanged(int score, int apples) {
            UpdateUI(score, apples);
        }

        // =========================================================
        //  ACTUALIZAR LABELS DE LA UI
        // =========================================================
        void UpdateUI(int score, int apples) {
            lblScore->Text = score.ToString();
            lblAppleCount->Text = apples.ToString();
        }
    };

} // namespace MiProyecto