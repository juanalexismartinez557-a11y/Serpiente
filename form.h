#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include <cstdlib>
#include "Snake.h"
#include "formtienda.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::Windows::Forms;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            InitializeComponent();
        }

    protected:
        ~Form1()
        {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;

        // --- UI Juego ---
        Panel^ panelTop;
        Panel^ panelGame;
        Label^ lblScore;
        Label^ lblAppleCount;
        Label^ lblCoinCount;

        // --- MENU ---
        ComboBox^ comboModo;
        ComboBox^ comboTamano;
        Button^ btnIniciar;
        Label^ lblModo;
        Label^ lblTamano;

        // --- Juego ---
        SnakeGame^ game;
        Timer^ gameTimer;

        int modoSeleccionado;
        int tamanoSeleccionado;

        void InitializeComponent(void)
        {
            this->ClientSize = System::Drawing::Size(820, 820);
            this->Text = L"Snake Evolution";
            this->BackColor = System::Drawing::Color::FromArgb(78, 136, 43);
            this->KeyPreview = true;

            // -------- MENU --------
            lblModo = gcnew Label();
            lblModo->Text = "Modo:";
            lblModo->Location = System::Drawing::Point(300, 300);
            lblModo->ForeColor = System::Drawing::Color::White;

            comboModo = gcnew ComboBox();
            comboModo->Location = System::Drawing::Point(380, 300);
            comboModo->Items->Add("Obstáculos");
            comboModo->Items->Add("Aleatorio");
            comboModo->Items->Add("Velocidad");

            lblTamano = gcnew Label();
            lblTamano->Text = "Tamaño:";
            lblTamano->Location = System::Drawing::Point(300, 350);
            lblTamano->ForeColor = System::Drawing::Color::White;

            comboTamano = gcnew ComboBox();
            comboTamano->Location = System::Drawing::Point(380, 350);
            comboTamano->Items->Add("10x10");
            comboTamano->Items->Add("25x25");
            comboTamano->Items->Add("50x50");

            btnIniciar = gcnew Button();
            btnIniciar->Text = "Iniciar Juego";
            btnIniciar->Location = System::Drawing::Point(350, 400);
            btnIniciar->Click += gcnew EventHandler(this, &Form1::IniciarJuego);

            // -------- PANEL JUEGO --------
            panelGame = gcnew Panel();
            panelGame->BackColor = System::Drawing::Color::FromArgb(170, 215, 81);
            panelGame->Location = System::Drawing::Point(30, 120);
            panelGame->Size = System::Drawing::Size(750, 660);
            panelGame->Paint += gcnew PaintEventHandler(this, &Form1::PanelGame_Paint);
            panelGame->Visible = false;

            // -------- LABELS --------
            lblScore = gcnew Label();
            lblScore->Location = System::Drawing::Point(30, 30);
            lblScore->ForeColor = System::Drawing::Color::White;

            lblAppleCount = gcnew Label();
            lblAppleCount->Location = System::Drawing::Point(150, 30);
            lblAppleCount->ForeColor = System::Drawing::Color::White;

            lblCoinCount = gcnew Label();
            lblCoinCount->Location = System::Drawing::Point(300, 30);
            lblCoinCount->ForeColor = System::Drawing::Color::White;

            // -------- EVENTOS --------
            this->KeyDown += gcnew KeyEventHandler(this, &Form1::Form1_KeyDown);

            // -------- ADD --------
            this->Controls->Add(lblModo);
            this->Controls->Add(comboModo);
            this->Controls->Add(lblTamano);
            this->Controls->Add(comboTamano);
            this->Controls->Add(btnIniciar);

            this->Controls->Add(panelGame);
            this->Controls->Add(lblScore);
            this->Controls->Add(lblAppleCount);
            this->Controls->Add(lblCoinCount);
        }

        void IniciarJuego(System::Object^ sender, System::EventArgs^ e)
        {
            if (comboModo->SelectedIndex == -1 || comboTamano->SelectedIndex == -1)
            {
                MessageBox::Show("Selecciona modo y tamaño");
                return;
            }

            modoSeleccionado = comboModo->SelectedIndex;
            tamanoSeleccionado = comboTamano->SelectedIndex;

            InitGame();

            // Ocultar menú
            comboModo->Visible = false;
            comboTamano->Visible = false;
            btnIniciar->Visible = false;
            lblModo->Visible = false;
            lblTamano->Visible = false;

            panelGame->Visible = true;
        }

        void InitGame()
        {
            game = gcnew SnakeGame();

            // --- MODOS ---
            if (modoSeleccionado == 0)
                game->WallPassThrough = false;

            else if (modoSeleccionado == 1)
                game->GrowAmount = rand() % 5 + 1;

            else if (modoSeleccionado == 2)
                game->SpeedMs = 80;

            // --- TIMER ---
            gameTimer = gcnew Timer();
            gameTimer->Interval = game->SpeedMs;
            gameTimer->Tick += gcnew EventHandler(this, &Form1::GameTimer_Tick);
            gameTimer->Start();
        }

        void GameTimer_Tick(Object^ sender, EventArgs^ e)
        {
            game->Tick();
            panelGame->Invalidate();
        }

        void PanelGame_Paint(Object^ sender, PaintEventArgs^ e)
        {
            game->Draw(e->Graphics);
        }

        void Form1_KeyDown(Object^ sender, KeyEventArgs^ e)
        {
            if (game == nullptr) return;

            switch (e->KeyCode)
            {
            case Keys::Up:    game->SetDirection(0, -1); break;
            case Keys::Down:  game->SetDirection(0, 1); break;
            case Keys::Left:  game->SetDirection(-1, 0); break;
            case Keys::Right: game->SetDirection(1, 0); break;
            }
        }
    };
}