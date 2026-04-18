#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Snake.h"
#include "formtienda.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;

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

    private:
        System::ComponentModel::Container^ components;

        // --- UI ---
        System::Windows::Forms::Panel^ panelTop;
        System::Windows::Forms::Panel^ panelGame;
        System::Windows::Forms::Label^ lblApple;
        System::Windows::Forms::Label^ lblAppleCount;
        System::Windows::Forms::Label^ lblTrophy;
        System::Windows::Forms::Label^ lblScore;
        System::Windows::Forms::Label^ lblCoin;
        System::Windows::Forms::Label^ lblCoinCount;
        System::Windows::Forms::Label^ lblHighScore;
        System::Windows::Forms::Label^ lblBoardSize;
        System::Windows::Forms::ComboBox^ cboBoardSize;

        // --- Juego ---
        SnakeGame^ game;
        System::Windows::Forms::Timer^ gameTimer;

        void InitializeComponent(void)
        {
            this->panelTop = gcnew System::Windows::Forms::Panel();
            this->panelGame = gcnew System::Windows::Forms::Panel();
            this->lblApple = gcnew System::Windows::Forms::Label();
            this->lblAppleCount = gcnew System::Windows::Forms::Label();
            this->lblTrophy = gcnew System::Windows::Forms::Label();
            this->lblScore = gcnew System::Windows::Forms::Label();
            this->lblCoin = gcnew System::Windows::Forms::Label();
            this->lblCoinCount = gcnew System::Windows::Forms::Label();
            this->lblHighScore = gcnew System::Windows::Forms::Label();
            this->lblBoardSize = gcnew System::Windows::Forms::Label();
            this->cboBoardSize = gcnew System::Windows::Forms::ComboBox();

            this->SuspendLayout();

            // ---- Formulario ----
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(820, 820);
            this->Name = L"Form1";
            this->Text = L"Snake Evolution";
            this->BackColor = System::Drawing::Color::FromArgb(78, 136, 43);
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->KeyPreview = true;

            // ---- Panel superior ----
            this->panelTop->BackColor = System::Drawing::Color::FromArgb(73, 123, 40);
            this->panelTop->Location = System::Drawing::Point(0, 0);
            this->panelTop->Size = System::Drawing::Size(820, 88);
            this->panelTop->TabIndex = 0;

            // Emoji manzana
            this->lblApple->AutoSize = true;
            this->lblApple->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 20, System::Drawing::FontStyle::Regular);
            this->lblApple->ForeColor = System::Drawing::Color::White;
            this->lblApple->Location = System::Drawing::Point(25, 22);
            this->lblApple->Text = L"\U0001F34E"; // 🍎

            // Contador manzanas
            this->lblAppleCount->AutoSize = true;
            this->lblAppleCount->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular);
            this->lblAppleCount->ForeColor = System::Drawing::Color::White;
            this->lblAppleCount->Location = System::Drawing::Point(100, 25);
            this->lblAppleCount->Text = L"0";

            // Emoji trofeo
            this->lblTrophy->AutoSize = true;
            this->lblTrophy->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 20, System::Drawing::FontStyle::Regular);
            this->lblTrophy->ForeColor = System::Drawing::Color::White;
            this->lblTrophy->Location = System::Drawing::Point(200, 22);
            this->lblTrophy->Text = L"\U0001F3C6"; // 🏆

            // Puntaje actual
            this->lblScore->AutoSize = true;
            this->lblScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular);
            this->lblScore->ForeColor = System::Drawing::Color::White;
            this->lblScore->Location = System::Drawing::Point(275, 25);
            this->lblScore->Text = L"0";

            // Emoji moneda
            this->lblCoin->AutoSize = true;
            this->lblCoin->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 20, System::Drawing::FontStyle::Regular);
            this->lblCoin->ForeColor = System::Drawing::Color::White;
            this->lblCoin->Location = System::Drawing::Point(375, 22);
            this->lblCoin->Text = L"\U0001F4B0"; // 💰
            this->lblCoin->Cursor = System::Windows::Forms::Cursors::Hand;
            this->lblCoin->Click += gcnew System::EventHandler(this, &Form1::AbrirTienda);

            // Numero al lado de la bolsa de monedas
            this->lblCoinCount->AutoSize = true;
            this->lblCoinCount->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold);
            this->lblCoinCount->ForeColor = System::Drawing::Color::White;
            this->lblCoinCount->Location = System::Drawing::Point(450, 25);
            this->lblCoinCount->Text = L"0";

            // Etiqueta BEST
            this->lblHighScore->AutoSize = true;
            this->lblHighScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Regular);
            this->lblHighScore->ForeColor = System::Drawing::Color::FromArgb(255, 230, 100);
            this->lblHighScore->Location = System::Drawing::Point(700, 8);
            this->lblHighScore->Text = L"BEST";

            // Etiqueta de tamano de tablero
            this->lblBoardSize->AutoSize = true;
            this->lblBoardSize->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold);
            this->lblBoardSize->ForeColor = System::Drawing::Color::White;
            this->lblBoardSize->Location = System::Drawing::Point(520, 12);
            this->lblBoardSize->Text = L"TABLERO";

            // Selector de tamano de tablero
            this->cboBoardSize->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->cboBoardSize->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->cboBoardSize->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Regular);
            this->cboBoardSize->FormattingEnabled = true;
            this->cboBoardSize->Items->AddRange(gcnew cli::array< System::Object^  > { L"Small (10x10)", L"Medium (25x25)", L"Large (50x50)" });
            this->cboBoardSize->Location = System::Drawing::Point(523, 34);
            this->cboBoardSize->Name = L"cboBoardSize";
            this->cboBoardSize->Size = System::Drawing::Size(155, 31);
            this->cboBoardSize->TabIndex = 2;
            this->cboBoardSize->SelectedIndexChanged += gcnew System::EventHandler(
                this, &Form1::CboBoardSize_SelectedIndexChanged);

            // Agregar al panel superior
            this->panelTop->Controls->Add(this->lblApple);
            this->panelTop->Controls->Add(this->lblAppleCount);
            this->panelTop->Controls->Add(this->lblTrophy);
            this->panelTop->Controls->Add(this->lblScore);
            this->panelTop->Controls->Add(this->lblCoin);
            this->panelTop->Controls->Add(this->lblCoinCount);
            this->panelTop->Controls->Add(this->lblHighScore);
            this->panelTop->Controls->Add(this->lblBoardSize);
            this->panelTop->Controls->Add(this->cboBoardSize);

            // ---- Panel de juego ----
            this->panelGame->BackColor = System::Drawing::Color::FromArgb(170, 215, 81);
            this->panelGame->Location = System::Drawing::Point(30, 120);
            this->panelGame->Size = System::Drawing::Size(750, 660);
            this->panelGame->TabIndex = 1;
            this->panelGame->BorderStyle = System::Windows::Forms::BorderStyle::None;

            // Evento Paint del panel
            this->panelGame->Paint += gcnew System::Windows::Forms::PaintEventHandler(
                this, &Form1::PanelGame_Paint);
            this->panelGame->Resize += gcnew System::EventHandler(
                this, &Form1::PanelGame_Resize);

            // Eventos del Form
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(
                this, &Form1::Form1_KeyDown);

            this->Controls->Add(this->panelTop);
            this->Controls->Add(this->panelGame);

            this->ResumeLayout(false);
        }

        void InitGame() {
            game = gcnew SnakeGame(GetSelectedBoardSize());
            game->WallPassThrough = false;
            game->OnGameOver += gcnew SnakeGame::GameOverDelegate(this, &Form1::Game_OnGameOver);
            game->OnScoreChanged += gcnew SnakeGame::ScoreChangedDelegate(this, &Form1::Game_OnScoreChanged);

            cboBoardSize->SelectedIndex = 1;
            cboBoardSize->Enabled = true;
            lblCoinCount->Text = game->HighScore.ToString();
            UpdateUI(0, 0);

            gameTimer = gcnew System::Windows::Forms::Timer();
            gameTimer->Interval = game->SpeedMs;
            gameTimer->Tick += gcnew System::EventHandler(this, &Form1::GameTimer_Tick);
            gameTimer->Start();
            panelGame->Invalidate();
        }

        void GameTimer_Tick(Object^ sender, EventArgs^ e) {
            if (game == nullptr) return;
            game->Tick();
            panelGame->Invalidate();
        }

        void PanelGame_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            if (game == nullptr) return;
            game->Draw(e->Graphics, panelGame->ClientSize);
        }

        void PanelGame_Resize(Object^ sender, EventArgs^ e) {
            panelGame->Invalidate();
        }

        BoardSize GetSelectedBoardSize() {
            switch (cboBoardSize->SelectedIndex) {
            case 0: return BoardSize::Small;
            case 2: return BoardSize::Large;
            case 1:
            default:
                return BoardSize::Medium;
            }
        }

        void CboBoardSize_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
            if (cboBoardSize->SelectedIndex < 0 || game == nullptr) return;

            if (game->HasStarted && !game->IsGameOver) {
                cboBoardSize->Enabled = false;
                return;
            }

            game->SetBoardSize(GetSelectedBoardSize());
            if (gameTimer != nullptr) {
                gameTimer->Interval = game->SpeedMs;
            }
            lblCoinCount->Text = game->HighScore.ToString();
            UpdateUI(0, 0);
            panelGame->Invalidate();
            this->ActiveControl = nullptr;
        }

        void Form1_KeyDown(Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
            switch (e->KeyCode) {
            case System::Windows::Forms::Keys::Up:
            case System::Windows::Forms::Keys::W:
                cboBoardSize->Enabled = false;
                game->SetDirection(0, -1); break;

            case System::Windows::Forms::Keys::Down:
            case System::Windows::Forms::Keys::S:
                cboBoardSize->Enabled = false;
                game->SetDirection(0, 1); break;

            case System::Windows::Forms::Keys::Left:
            case System::Windows::Forms::Keys::A:
                cboBoardSize->Enabled = false;
                game->SetDirection(-1, 0); break;

            case System::Windows::Forms::Keys::Right:
            case System::Windows::Forms::Keys::D:
                cboBoardSize->Enabled = false;
                game->SetDirection(1, 0); break;

            case System::Windows::Forms::Keys::P:
            case System::Windows::Forms::Keys::Escape:
                if (game != nullptr) {
                    game->TogglePause();
                }
                break;

            case System::Windows::Forms::Keys::R:
                if (game != nullptr && (game->IsGameOver || game->IsPaused)) {
                    game->Reset();
                    gameTimer->Interval = game->SpeedMs;
                    cboBoardSize->Enabled = true;
                    lblCoinCount->Text = game->HighScore.ToString();
                    UpdateUI(0, 0);
                    panelGame->Invalidate();
                }
                break;
            }
            e->Handled = true;
        }

        void Game_OnGameOver(int finalScore) {
            cboBoardSize->Enabled = true;
            lblCoinCount->Text = game->HighScore.ToString();
        }

        void Game_OnScoreChanged(int score, int apples) {
            UpdateUI(score, apples);
        }

        void UpdateUI(int score, int apples) {
            lblScore->Text = score.ToString();
            lblAppleCount->Text = apples.ToString();
        }

        void AbrirTienda(System::Object^ sender, System::EventArgs^ e) {
            FormTienda^ tienda = gcnew FormTienda();
            tienda->Show();
            this->Hide();
        }
    };

} // namespace MiProyecto
