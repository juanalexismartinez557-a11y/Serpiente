#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Snake.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class FormTienda : public System::Windows::Forms::Form
    {
    public:
        FormTienda(int bestScore, SnakeGame^ player1Game, SnakeGame^ player2Game, Action<Color, Color, Color>^ skinSelectedCallback)
        {
            highestUnlockedScore = bestScore;
            game1 = player1Game;
            game2 = player2Game;
            onSkinSelected = skinSelectedCallback;
            InitializeComponent();
            UpdateSkinAvailability();
        }

    protected:
        ~FormTienda() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;

        Panel^ panelTop;
        Panel^ panelStore;
        Button^ btnBackToGame;

        Button^ btnSkin1; Button^ btnSkin2; Button^ btnSkin3;
        Button^ btnSkin4; Button^ btnSkin5; Button^ btnSkin6;

        Label^ lblPrice1; Label^ lblPrice2; Label^ lblPrice3;
        Label^ lblPrice4; Label^ lblPrice5; Label^ lblPrice6;

        Button^ btnBuy1; Button^ btnBuy2; Button^ btnBuy3;
        Button^ btnBuy4; Button^ btnBuy5; Button^ btnBuy6;

        int highestUnlockedScore;
        SnakeGame^ game1;
        SnakeGame^ game2;
        Action<Color, Color, Color>^ onSkinSelected;

        array<int>^ skinRequirements;
        array<Button^>^ buyButtons;

        void InitializeComponent(void)
        {
            this->panelTop = gcnew Panel();
            this->panelStore = gcnew Panel();

            this->btnBackToGame = gcnew Button();

            this->btnSkin1 = gcnew Button(); this->btnSkin2 = gcnew Button();
            this->btnSkin3 = gcnew Button(); this->btnSkin4 = gcnew Button();
            this->btnSkin5 = gcnew Button(); this->btnSkin6 = gcnew Button();

            this->lblPrice1 = gcnew Label(); this->lblPrice2 = gcnew Label();
            this->lblPrice3 = gcnew Label(); this->lblPrice4 = gcnew Label();
            this->lblPrice5 = gcnew Label(); this->lblPrice6 = gcnew Label();

            this->btnBuy1 = gcnew Button(); this->btnBuy2 = gcnew Button();
            this->btnBuy3 = gcnew Button(); this->btnBuy4 = gcnew Button();
            this->btnBuy5 = gcnew Button(); this->btnBuy6 = gcnew Button();

            this->SuspendLayout();

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(820, 820);
            this->Name = L"FormTienda";
            this->Text = L"Logros obtenidos";
            this->BackColor = Color::FromArgb(78, 136, 43);
            this->StartPosition = FormStartPosition::CenterScreen;

            this->panelTop->BackColor = Color::FromArgb(73, 123, 40);
            this->panelTop->Location = System::Drawing::Point(0, 0);
            this->panelTop->Size = System::Drawing::Size(820, 88);

            this->btnBackToGame->BackColor = Color::White;
            this->btnBackToGame->FlatStyle = FlatStyle::Flat;
            this->btnBackToGame->FlatAppearance->BorderSize = 0;
            this->btnBackToGame->Font = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            this->btnBackToGame->ForeColor = Color::FromArgb(73, 123, 40);
            this->btnBackToGame->Location = System::Drawing::Point(250, 20);
            this->btnBackToGame->Size = System::Drawing::Size(330, 55);
            this->btnBackToGame->Text = L"Regresar al juego";
            this->btnBackToGame->Click += gcnew EventHandler(this, &FormTienda::VolverAlJuego);
            this->panelTop->Controls->Add(this->btnBackToGame);

            this->panelStore->BackColor = Color::FromArgb(170, 215, 81);
            this->panelStore->Location = System::Drawing::Point(30, 120);
            this->panelStore->Size = System::Drawing::Size(750, 640);
            this->panelStore->BorderStyle = BorderStyle::None;

            SetupSkinButton(btnSkin1, Color::Black, 60, 45);
            SetupPriceLabel(lblPrice1, L"Score 100", 80, 205);
            SetupBuyButton(btnBuy1, 90, 240);
            btnBuy1->Click += gcnew EventHandler(this, &FormTienda::BuySkin1);

            SetupSkinButton(btnSkin2, Color::Purple, 295, 45);
            SetupPriceLabel(lblPrice2, L"Score 200", 315, 205);
            SetupBuyButton(btnBuy2, 325, 240);
            btnBuy2->Click += gcnew EventHandler(this, &FormTienda::BuySkin2);

            SetupSkinButton(btnSkin3, Color::WhiteSmoke, 530, 45);
            SetupPriceLabel(lblPrice3, L"Score 300", 550, 205);
            SetupBuyButton(btnBuy3, 560, 240);
            btnBuy3->Click += gcnew EventHandler(this, &FormTienda::BuySkin3);

            SetupSkinButton(btnSkin4, Color::Red, 60, 340);
            SetupPriceLabel(lblPrice4, L"Score 400", 80, 500);
            SetupBuyButton(btnBuy4, 90, 535);
            btnBuy4->Click += gcnew EventHandler(this, &FormTienda::BuySkin4);

            SetupSkinButton(btnSkin5, Color::Blue, 295, 340);
            SetupPriceLabel(lblPrice5, L"Score 500", 315, 500);
            SetupBuyButton(btnBuy5, 325, 535);
            btnBuy5->Click += gcnew EventHandler(this, &FormTienda::BuySkin5);

            SetupSkinButton(btnSkin6, Color::Goldenrod, 530, 340);
            SetupPriceLabel(lblPrice6, L"Score 999", 550, 500);
            SetupBuyButton(btnBuy6, 560, 535);
            btnBuy6->Click += gcnew EventHandler(this, &FormTienda::BuySkin6);

            panelStore->Controls->Add(btnSkin1);  panelStore->Controls->Add(lblPrice1); panelStore->Controls->Add(btnBuy1);
            panelStore->Controls->Add(btnSkin2);  panelStore->Controls->Add(lblPrice2); panelStore->Controls->Add(btnBuy2);
            panelStore->Controls->Add(btnSkin3);  panelStore->Controls->Add(lblPrice3); panelStore->Controls->Add(btnBuy3);
            panelStore->Controls->Add(btnSkin4);  panelStore->Controls->Add(lblPrice4); panelStore->Controls->Add(btnBuy4);
            panelStore->Controls->Add(btnSkin5);  panelStore->Controls->Add(lblPrice5); panelStore->Controls->Add(btnBuy5);
            panelStore->Controls->Add(btnSkin6);  panelStore->Controls->Add(lblPrice6); panelStore->Controls->Add(btnBuy6);

            this->Controls->Add(this->panelTop);
            this->Controls->Add(this->panelStore);

            skinRequirements = gcnew array<int>{ 100, 200, 300, 400, 500, 999 };
            buyButtons = gcnew array<Button^>{ btnBuy1, btnBuy2, btnBuy3, btnBuy4, btnBuy5, btnBuy6 };

            this->ResumeLayout(false);
        }

        void SetupSkinButton(Button^ btn, Color bg, int x, int y) {
            btn->BackColor = bg;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Location = System::Drawing::Point(x, y);
            btn->Size = System::Drawing::Size(160, 150);
            btn->UseVisualStyleBackColor = false;
            btn->Enabled = false;
        }

        void SetupPriceLabel(Label^ lbl, String^ text, int x, int y) {
            lbl->AutoSize = true;
            lbl->Font = gcnew System::Drawing::Font(L"Segoe UI", 14);
            lbl->ForeColor = Color::FromArgb(50, 50, 50);
            lbl->Location = System::Drawing::Point(x, y);
            lbl->Text = text;
        }

        void SetupBuyButton(Button^ btn, int x, int y) {
            btn->BackColor = Color::FromArgb(70, 120, 255);
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            btn->ForeColor = Color::White;
            btn->Location = System::Drawing::Point(x, y);
            btn->Size = System::Drawing::Size(100, 38);
            btn->Text = L"Comprar";
            btn->UseVisualStyleBackColor = false;
        }

        void UpdateSkinAvailability()
        {
            for (int i = 0; i < buyButtons->Length; i++) {
                bool unlocked = highestUnlockedScore >= skinRequirements[i];
                buyButtons[i]->Enabled = unlocked;

                if (unlocked) {
                    buyButtons[i]->Text = L"Equipar";
                    buyButtons[i]->BackColor = Color::FromArgb(70, 120, 255);
                }
                else {
                    buyButtons[i]->Text = L"Nopitino";
                    buyButtons[i]->BackColor = Color::Gray;
                }
            }
        }

        void ApplySkinToGames(Color primary, Color secondary, Color head)
        {
            if (onSkinSelected != nullptr)
                onSkinSelected(primary, secondary, head);
        }

        void BuySkin1(Object^ s, EventArgs^ e) {
            if (highestUnlockedScore < 100) return;
            ApplySkinToGames(Color::Black, Color::DimGray, Color::FromArgb(30, 30, 30));
            MessageBox::Show(L"Skin negra seleccionada.");
        }

        void BuySkin2(Object^ s, EventArgs^ e) {
            if (highestUnlockedScore < 200) return;
            ApplySkinToGames(Color::Purple, Color::MediumOrchid, Color::Indigo);
            MessageBox::Show(L"Skin morada seleccionada.");
        }

        void BuySkin3(Object^ s, EventArgs^ e) {
            if (highestUnlockedScore < 300) return;
            ApplySkinToGames(Color::WhiteSmoke, Color::Gainsboro, Color::LightGray);
            MessageBox::Show(L"Skin blanca seleccionada.");
        }

        void BuySkin4(Object^ s, EventArgs^ e) {
            if (highestUnlockedScore < 400) return;
            ApplySkinToGames(Color::Red, Color::Tomato, Color::DarkRed);
            MessageBox::Show(L"Skin roja seleccionada.");
        }

        void BuySkin5(Object^ s, EventArgs^ e) {
            if (highestUnlockedScore < 500) return;
            ApplySkinToGames(Color::Blue, Color::RoyalBlue, Color::Navy);
            MessageBox::Show(L"Skin azul seleccionada.");
        }

        void BuySkin6(Object^ s, EventArgs^ e) {
            if (highestUnlockedScore < 999) return;
            ApplySkinToGames(Color::Goldenrod, Color::Gold, Color::DarkGoldenrod);
            MessageBox::Show(L"Skin dorada seleccionada.");
        }

        void VolverAlJuego(Object^ sender, EventArgs^ e)
        {
            if (game1 != nullptr && game1->HasStarted && !game1->IsGameOver)
                game1->IsPaused = false;

            if (game2 != nullptr && game2->HasStarted && !game2->IsGameOver)
                game2->IsPaused = false;

            Form^ juego = Application::OpenForms[L"Form1"];
            if (juego != nullptr)
                juego->Show();

            this->Close();
        }
    };

} // namespace MiProyecto
