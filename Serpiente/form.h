#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

namespace MiProyecto {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

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
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;

        Panel^ panelTop;
        Panel^ panelGame;
        Label^ lblApple;
        Label^ lblAppleCount;
        Label^ lblTrophy;
        Label^ lblScore;
        Label^ lblCoin;
        Label^ lblCoinCount;

        void InitializeComponent(void)
        {
            this->panelTop = (gcnew System::Windows::Forms::Panel());
            this->panelGame = (gcnew System::Windows::Forms::Panel());
            this->lblApple = (gcnew System::Windows::Forms::Label());
            this->lblAppleCount = (gcnew System::Windows::Forms::Label());
            this->lblTrophy = (gcnew System::Windows::Forms::Label());
            this->lblScore = (gcnew System::Windows::Forms::Label());
            this->lblCoin = (gcnew System::Windows::Forms::Label());
            this->lblCoinCount = (gcnew System::Windows::Forms::Label());

            this->SuspendLayout();

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(820, 820);
            this->Name = L"Form1";
            this->Text = L"Snake GUI";
            this->BackColor = System::Drawing::Color::FromArgb(78, 136, 43);
            this->StartPosition = FormStartPosition::CenterScreen;

            // Panel de arriba donde pongo los emojiis
            this->panelTop->BackColor = System::Drawing::Color::FromArgb(73, 123, 40);
            this->panelTop->Location = System::Drawing::Point(0, 0);
            this->panelTop->Name = L"panelTop";
            this->panelTop->Size = System::Drawing::Size(820, 88);
            this->panelTop->TabIndex = 0;

            // Emoji para las mamnzanas
            this->lblApple->AutoSize = true;
            this->lblApple->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 20, FontStyle::Regular);
            this->lblApple->ForeColor = System::Drawing::Color::White;
            this->lblApple->Location = System::Drawing::Point(25, 22);
            this->lblApple->Name = L"lblApple";
            this->lblApple->Size = System::Drawing::Size(50, 46);
            this->lblApple->Text = L"🍎";

            // Conteo de las manzanass
            this->lblAppleCount->AutoSize = true;
            this->lblAppleCount->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, FontStyle::Regular);
            this->lblAppleCount->ForeColor = System::Drawing::Color::White;
            this->lblAppleCount->Location = System::Drawing::Point(100, 25);
            this->lblAppleCount->Name = L"lblAppleCount";
            this->lblAppleCount->Size = System::Drawing::Size(39, 41);
            this->lblAppleCount->Text = L"0";

            // Trofeos
            this->lblTrophy->AutoSize = true;
            this->lblTrophy->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 20, FontStyle::Regular);
            this->lblTrophy->ForeColor = System::Drawing::Color::White;
            this->lblTrophy->Location = System::Drawing::Point(350, 22);
            this->lblTrophy->Name = L"lblTrophy";
            this->lblTrophy->Size = System::Drawing::Size(50, 46);
            this->lblTrophy->Text = L"🏆";

            // Puntaj de los trofeos
            this->lblScore->AutoSize = true;
            this->lblScore->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, FontStyle::Regular);
            this->lblScore->ForeColor = System::Drawing::Color::White;
            this->lblScore->Location = System::Drawing::Point(425, 25);
            this->lblScore->Name = L"lblScore";
            this->lblScore->Size = System::Drawing::Size(39, 41);
            this->lblScore->Text = L"0";

            // Glory coins
            this->lblCoin->AutoSize = true;
            this->lblCoin->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 20, FontStyle::Regular);
            this->lblCoin->ForeColor = System::Drawing::Color::White;
            this->lblCoin->Location = System::Drawing::Point(670, 22);
            this->lblCoin->Name = L"lblCoin";
            this->lblCoin->Size = System::Drawing::Size(50, 46);
            this->lblCoin->Text = L"💰";

            // Conteador de menedas //740
            this->lblCoinCount->AutoSize = true;
            this->lblCoinCount->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, FontStyle::Bold);
            this->lblCoinCount->ForeColor = System::Drawing::Color::White;
            this->lblCoinCount->Location = System::Drawing::Point(940, 25);
            this->lblCoinCount->Name = L"lblCoinCount";
            this->lblCoinCount->Size = System::Drawing::Size(39, 41);
            this->lblCoinCount->Text = L"0";

            // Agregar controles al panel superior
            this->panelTop->Controls->Add(this->lblApple);
            this->panelTop->Controls->Add(this->lblAppleCount);
            this->panelTop->Controls->Add(this->lblTrophy);
            this->panelTop->Controls->Add(this->lblScore);
            this->panelTop->Controls->Add(this->lblCoin);
            this->panelTop->Controls->Add(this->lblCoinCount);

            // Panel centarl de juego
            this->panelGame->BackColor = System::Drawing::Color::FromArgb(170, 215, 81);
            this->panelGame->Location = System::Drawing::Point(30, 120);
            this->panelGame->Name = L"panelGame";
            this->panelGame->Size = System::Drawing::Size(750, 660);
            this->panelGame->TabIndex = 1;
            this->panelGame->BorderStyle = System::Windows::Forms::BorderStyle::None;

            // Agregar nuestros paneles al formss
            this->Controls->Add(this->panelTop);
            this->Controls->Add(this->panelGame);

            this->ResumeLayout(false);
        }
    };
}