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

    public ref class FormTienda : public System::Windows::Forms::Form
    {
    public:
        FormTienda(void)
        {
            InitializeComponent();
        }

    protected:
        ~FormTienda()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;

        Panel^ panelTop;
        Panel^ panelStore;

        Button^ btnBackToGame;

        Button^ btnSkin1;
        Button^ btnSkin2;
        Button^ btnSkin3;
        Button^ btnSkin4;
        Button^ btnSkin5;
        Button^ btnSkin6;

        Label^ lblPrice1;
        Label^ lblPrice2;
        Label^ lblPrice3;
        Label^ lblPrice4;
        Label^ lblPrice5;
        Label^ lblPrice6;

        Button^ btnBuy1;
        Button^ btnBuy2;
        Button^ btnBuy3;
        Button^ btnBuy4;
        Button^ btnBuy5;
        Button^ btnBuy6;

        void InitializeComponent(void)
        {
            this->panelTop = (gcnew System::Windows::Forms::Panel());
            this->panelStore = (gcnew System::Windows::Forms::Panel());

            this->btnBackToGame = (gcnew System::Windows::Forms::Button());

            this->btnSkin1 = (gcnew System::Windows::Forms::Button());
            this->btnSkin2 = (gcnew System::Windows::Forms::Button());
            this->btnSkin3 = (gcnew System::Windows::Forms::Button());
            this->btnSkin4 = (gcnew System::Windows::Forms::Button());
            this->btnSkin5 = (gcnew System::Windows::Forms::Button());
            this->btnSkin6 = (gcnew System::Windows::Forms::Button());

            this->lblPrice1 = (gcnew System::Windows::Forms::Label());
            this->lblPrice2 = (gcnew System::Windows::Forms::Label());
            this->lblPrice3 = (gcnew System::Windows::Forms::Label());
            this->lblPrice4 = (gcnew System::Windows::Forms::Label());
            this->lblPrice5 = (gcnew System::Windows::Forms::Label());
            this->lblPrice6 = (gcnew System::Windows::Forms::Label());

            this->btnBuy1 = (gcnew System::Windows::Forms::Button());
            this->btnBuy2 = (gcnew System::Windows::Forms::Button());
            this->btnBuy3 = (gcnew System::Windows::Forms::Button());
            this->btnBuy4 = (gcnew System::Windows::Forms::Button());
            this->btnBuy5 = (gcnew System::Windows::Forms::Button());
            this->btnBuy6 = (gcnew System::Windows::Forms::Button());

            this->SuspendLayout();

            // FORM
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(820, 820);
            this->Name = L"FormTienda";
            this->Text = L"Tienda Snake";
            this->BackColor = System::Drawing::Color::FromArgb(78, 136, 43);
            this->StartPosition = FormStartPosition::CenterScreen;

            // PANEL SUPERIOR
            this->panelTop->BackColor = System::Drawing::Color::FromArgb(73, 123, 40);
            this->panelTop->Location = System::Drawing::Point(0, 0);
            this->panelTop->Name = L"panelTop";
            this->panelTop->Size = System::Drawing::Size(820, 88);
            this->panelTop->TabIndex = 0;

            // BOTON REGRESAR
            this->btnBackToGame->BackColor = System::Drawing::Color::White;
            this->btnBackToGame->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBackToGame->FlatAppearance->BorderSize = 0;
            this->btnBackToGame->Font = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            this->btnBackToGame->ForeColor = System::Drawing::Color::FromArgb(73, 123, 40);
            this->btnBackToGame->Location = System::Drawing::Point(25, 20);
            this->btnBackToGame->Name = L"btnBackToGame";
            this->btnBackToGame->Size = System::Drawing::Size(230, 45);
            this->btnBackToGame->TabIndex = 0;
            this->btnBackToGame->Text = L"Regresar al juego";
            this->btnBackToGame->UseVisualStyleBackColor = false;
            this->btnBackToGame->Click += gcnew System::EventHandler(this, &FormTienda::VolverAlJuego);

            this->panelTop->Controls->Add(this->btnBackToGame);

            // PANEL TIENDA
            this->panelStore->BackColor = System::Drawing::Color::FromArgb(170, 215, 81);
            this->panelStore->Location = System::Drawing::Point(30, 120);
            this->panelStore->Name = L"panelStore";
            this->panelStore->Size = System::Drawing::Size(750, 640);
            this->panelStore->TabIndex = 1;
            this->panelStore->BorderStyle = System::Windows::Forms::BorderStyle::None;

            // btnSkin1
            this->btnSkin1->BackColor = System::Drawing::Color::WhiteSmoke;
            this->btnSkin1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSkin1->FlatAppearance->BorderSize = 0;
            this->btnSkin1->Location = System::Drawing::Point(60, 45);
            this->btnSkin1->Name = L"btnSkin1";
            this->btnSkin1->Size = System::Drawing::Size(160, 150);
            this->btnSkin1->TabIndex = 1;
            this->btnSkin1->UseVisualStyleBackColor = false;

            this->lblPrice1->AutoSize = true;
            this->lblPrice1->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
            this->lblPrice1->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
            this->lblPrice1->Location = System::Drawing::Point(105, 205);
            this->lblPrice1->Name = L"lblPrice1";
            this->lblPrice1->Text = L"💰 $5";

            this->btnBuy1->BackColor = System::Drawing::Color::FromArgb(70, 120, 255);
            this->btnBuy1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBuy1->FlatAppearance->BorderSize = 0;
            this->btnBuy1->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            this->btnBuy1->ForeColor = System::Drawing::Color::White;
            this->btnBuy1->Location = System::Drawing::Point(90, 240);
            this->btnBuy1->Name = L"btnBuy1";
            this->btnBuy1->Size = System::Drawing::Size(100, 38);
            this->btnBuy1->TabIndex = 2;
            this->btnBuy1->Text = L"Comprar";
            this->btnBuy1->UseVisualStyleBackColor = false;

            // btnSkin2
            this->btnSkin2->BackColor = System::Drawing::Color::LavenderBlush;
            this->btnSkin2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSkin2->FlatAppearance->BorderSize = 0;
            this->btnSkin2->Location = System::Drawing::Point(295, 45);
            this->btnSkin2->Name = L"btnSkin2";
            this->btnSkin2->Size = System::Drawing::Size(160, 150);
            this->btnSkin2->TabIndex = 3;
            this->btnSkin2->UseVisualStyleBackColor = false;

            this->lblPrice2->AutoSize = true;
            this->lblPrice2->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
            this->lblPrice2->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
            this->lblPrice2->Location = System::Drawing::Point(340, 205);
            this->lblPrice2->Name = L"lblPrice2";
            this->lblPrice2->Text = L"💰 $10";

            this->btnBuy2->BackColor = System::Drawing::Color::FromArgb(70, 120, 255);
            this->btnBuy2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBuy2->FlatAppearance->BorderSize = 0;
            this->btnBuy2->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            this->btnBuy2->ForeColor = System::Drawing::Color::White;
            this->btnBuy2->Location = System::Drawing::Point(325, 240);
            this->btnBuy2->Name = L"btnBuy2";
            this->btnBuy2->Size = System::Drawing::Size(100, 38);
            this->btnBuy2->TabIndex = 4;
            this->btnBuy2->Text = L"Comprar";
            this->btnBuy2->UseVisualStyleBackColor = false;

            // btnSkin3
            this->btnSkin3->BackColor = System::Drawing::Color::Honeydew;
            this->btnSkin3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSkin3->FlatAppearance->BorderSize = 0;
            this->btnSkin3->Location = System::Drawing::Point(530, 45);
            this->btnSkin3->Name = L"btnSkin3";
            this->btnSkin3->Size = System::Drawing::Size(160, 150);
            this->btnSkin3->TabIndex = 5;
            this->btnSkin3->UseVisualStyleBackColor = false;

            this->lblPrice3->AutoSize = true;
            this->lblPrice3->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
            this->lblPrice3->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
            this->lblPrice3->Location = System::Drawing::Point(575, 205);
            this->lblPrice3->Name = L"lblPrice3";
            this->lblPrice3->Text = L"💰 $15";

            this->btnBuy3->BackColor = System::Drawing::Color::FromArgb(70, 120, 255);
            this->btnBuy3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBuy3->FlatAppearance->BorderSize = 0;
            this->btnBuy3->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            this->btnBuy3->ForeColor = System::Drawing::Color::White;
            this->btnBuy3->Location = System::Drawing::Point(560, 240);
            this->btnBuy3->Name = L"btnBuy3";
            this->btnBuy3->Size = System::Drawing::Size(100, 38);
            this->btnBuy3->TabIndex = 6;
            this->btnBuy3->Text = L"Comprar";
            this->btnBuy3->UseVisualStyleBackColor = false;

            // btnSkin4
            this->btnSkin4->BackColor = System::Drawing::Color::MistyRose;
            this->btnSkin4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSkin4->FlatAppearance->BorderSize = 0;
            this->btnSkin4->Location = System::Drawing::Point(60, 340);
            this->btnSkin4->Name = L"btnSkin4";
            this->btnSkin4->Size = System::Drawing::Size(160, 150);
            this->btnSkin4->TabIndex = 7;
            this->btnSkin4->UseVisualStyleBackColor = false;

            this->lblPrice4->AutoSize = true;
            this->lblPrice4->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
            this->lblPrice4->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
            this->lblPrice4->Location = System::Drawing::Point(105, 500);
            this->lblPrice4->Name = L"lblPrice4";
            this->lblPrice4->Text = L"💰 $20";

            this->btnBuy4->BackColor = System::Drawing::Color::FromArgb(70, 120, 255);
            this->btnBuy4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBuy4->FlatAppearance->BorderSize = 0;
            this->btnBuy4->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            this->btnBuy4->ForeColor = System::Drawing::Color::White;
            this->btnBuy4->Location = System::Drawing::Point(90, 535);
            this->btnBuy4->Name = L"btnBuy4";
            this->btnBuy4->Size = System::Drawing::Size(100, 38);
            this->btnBuy4->TabIndex = 8;
            this->btnBuy4->Text = L"Comprar";
            this->btnBuy4->UseVisualStyleBackColor = false;

            // btnSkin5
            this->btnSkin5->BackColor = System::Drawing::Color::LightCyan;
            this->btnSkin5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSkin5->FlatAppearance->BorderSize = 0;
            this->btnSkin5->Location = System::Drawing::Point(295, 340);
            this->btnSkin5->Name = L"btnSkin5";
            this->btnSkin5->Size = System::Drawing::Size(160, 150);
            this->btnSkin5->TabIndex = 9;
            this->btnSkin5->UseVisualStyleBackColor = false;

            this->lblPrice5->AutoSize = true;
            this->lblPrice5->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
            this->lblPrice5->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
            this->lblPrice5->Location = System::Drawing::Point(340, 500);
            this->lblPrice5->Name = L"lblPrice5";
            this->lblPrice5->Text = L"💰 $25";

            this->btnBuy5->BackColor = System::Drawing::Color::FromArgb(70, 120, 255);
            this->btnBuy5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBuy5->FlatAppearance->BorderSize = 0;
            this->btnBuy5->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            this->btnBuy5->ForeColor = System::Drawing::Color::White;
            this->btnBuy5->Location = System::Drawing::Point(325, 535);
            this->btnBuy5->Name = L"btnBuy5";
            this->btnBuy5->Size = System::Drawing::Size(100, 38);
            this->btnBuy5->TabIndex = 10;
            this->btnBuy5->Text = L"Comprar";
            this->btnBuy5->UseVisualStyleBackColor = false;

            // btnSkin6
            this->btnSkin6->BackColor = System::Drawing::Color::LemonChiffon;
            this->btnSkin6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSkin6->FlatAppearance->BorderSize = 0;
            this->btnSkin6->Location = System::Drawing::Point(530, 340);
            this->btnSkin6->Name = L"btnSkin6";
            this->btnSkin6->Size = System::Drawing::Size(160, 150);
            this->btnSkin6->TabIndex = 11;
            this->btnSkin6->UseVisualStyleBackColor = false;

            this->lblPrice6->AutoSize = true;
            this->lblPrice6->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
            this->lblPrice6->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
            this->lblPrice6->Location = System::Drawing::Point(575, 500);
            this->lblPrice6->Name = L"lblPrice6";
            this->lblPrice6->Text = L"💰 $30";

            this->btnBuy6->BackColor = System::Drawing::Color::FromArgb(70, 120, 255);
            this->btnBuy6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBuy6->FlatAppearance->BorderSize = 0;
            this->btnBuy6->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            this->btnBuy6->ForeColor = System::Drawing::Color::White;
            this->btnBuy6->Location = System::Drawing::Point(560, 535);
            this->btnBuy6->Name = L"btnBuy6";
            this->btnBuy6->Size = System::Drawing::Size(100, 38);
            this->btnBuy6->TabIndex = 12;
            this->btnBuy6->Text = L"Comprar";
            this->btnBuy6->UseVisualStyleBackColor = false;

            // AGREGAR TODO AL PANEL STORE
            this->panelStore->Controls->Add(this->btnSkin1);
            this->panelStore->Controls->Add(this->lblPrice1);
            this->panelStore->Controls->Add(this->btnBuy1);

            this->panelStore->Controls->Add(this->btnSkin2);
            this->panelStore->Controls->Add(this->lblPrice2);
            this->panelStore->Controls->Add(this->btnBuy2);

            this->panelStore->Controls->Add(this->btnSkin3);
            this->panelStore->Controls->Add(this->lblPrice3);
            this->panelStore->Controls->Add(this->btnBuy3);

            this->panelStore->Controls->Add(this->btnSkin4);
            this->panelStore->Controls->Add(this->lblPrice4);
            this->panelStore->Controls->Add(this->btnBuy4);

            this->panelStore->Controls->Add(this->btnSkin5);
            this->panelStore->Controls->Add(this->lblPrice5);
            this->panelStore->Controls->Add(this->btnBuy5);

            this->panelStore->Controls->Add(this->btnSkin6);
            this->panelStore->Controls->Add(this->lblPrice6);
            this->panelStore->Controls->Add(this->btnBuy6);

            // AGREGAR AL FORM
            this->Controls->Add(this->panelTop);
            this->Controls->Add(this->panelStore);

            this->ResumeLayout(false);
        }

        System::Void VolverAlJuego(System::Object^ sender, System::EventArgs^ e)
        {
            Form^ juego = Application::OpenForms["Form1"];
            if (juego != nullptr)
            {
                juego->Show();
            }
            this->Close();
        }
    };
}