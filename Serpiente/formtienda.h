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
        FormTienda(void) { InitializeComponent(); }

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

            // FORM
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(820, 820);
            this->Name = L"FormTienda";   // *** Necesario para OpenForms ***
            this->Text = L"Tienda Snake";
            this->BackColor = Color::FromArgb(78, 136, 43);
            this->StartPosition = FormStartPosition::CenterScreen;

            // --- PANEL SUPERIOR ---
            this->panelTop->BackColor = Color::FromArgb(73, 123, 40);
            this->panelTop->Location = System::Drawing::Point(0, 0);
            this->panelTop->Size = System::Drawing::Size(820, 88);

            // --- BOTÓN REGRESAR ---
            this->btnBackToGame->BackColor = Color::White;
            this->btnBackToGame->FlatStyle = FlatStyle::Flat;
            this->btnBackToGame->FlatAppearance->BorderSize = 0;
            this->btnBackToGame->Font = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            this->btnBackToGame->ForeColor = Color::FromArgb(73, 123, 40);
            this->btnBackToGame->Location = System::Drawing::Point(25, 20);
            this->btnBackToGame->Size = System::Drawing::Size(230, 45);
            this->btnBackToGame->Text = L"Regresar al juego";
            this->btnBackToGame->Click += gcnew EventHandler(this, &FormTienda::VolverAlJuego);
            this->panelTop->Controls->Add(this->btnBackToGame);

            // --- PANEL TIENDA ---
            this->panelStore->BackColor = Color::FromArgb(170, 215, 81);
            this->panelStore->Location = System::Drawing::Point(30, 120);
            this->panelStore->Size = System::Drawing::Size(750, 640);
            this->panelStore->BorderStyle = BorderStyle::None;

            // Skin 1
            SetupSkinButton(btnSkin1, Color::WhiteSmoke, 60, 45);
            SetupPriceLabel(lblPrice1, L"💰 $5", 105, 205);
            SetupBuyButton(btnBuy1, 90, 240);
            btnBuy1->Click += gcnew EventHandler(this, &FormTienda::BuySkin1);

            // Skin 2
            SetupSkinButton(btnSkin2, Color::LavenderBlush, 295, 45);
            SetupPriceLabel(lblPrice2, L"💰 $10", 340, 205);
            SetupBuyButton(btnBuy2, 325, 240);
            btnBuy2->Click += gcnew EventHandler(this, &FormTienda::BuySkin2);

            // Skin 3
            SetupSkinButton(btnSkin3, Color::Honeydew, 530, 45);
            SetupPriceLabel(lblPrice3, L"💰 $15", 575, 205);
            SetupBuyButton(btnBuy3, 560, 240);
            btnBuy3->Click += gcnew EventHandler(this, &FormTienda::BuySkin3);

            // Skin 4
            SetupSkinButton(btnSkin4, Color::MistyRose, 60, 340);
            SetupPriceLabel(lblPrice4, L"💰 $20", 105, 500);
            SetupBuyButton(btnBuy4, 90, 535);
            btnBuy4->Click += gcnew EventHandler(this, &FormTienda::BuySkin4);

            // Skin 5
            SetupSkinButton(btnSkin5, Color::LightCyan, 295, 340);
            SetupPriceLabel(lblPrice5, L"💰 $25", 340, 500);
            SetupBuyButton(btnBuy5, 325, 535);
            btnBuy5->Click += gcnew EventHandler(this, &FormTienda::BuySkin5);

            // Skin 6
            SetupSkinButton(btnSkin6, Color::LemonChiffon, 530, 340);
            SetupPriceLabel(lblPrice6, L"💰 $30", 575, 500);
            SetupBuyButton(btnBuy6, 560, 535);
            btnBuy6->Click += gcnew EventHandler(this, &FormTienda::BuySkin6);

            // Agregar skins al panel
            panelStore->Controls->Add(btnSkin1);  panelStore->Controls->Add(lblPrice1); panelStore->Controls->Add(btnBuy1);
            panelStore->Controls->Add(btnSkin2);  panelStore->Controls->Add(lblPrice2); panelStore->Controls->Add(btnBuy2);
            panelStore->Controls->Add(btnSkin3);  panelStore->Controls->Add(lblPrice3); panelStore->Controls->Add(btnBuy3);
            panelStore->Controls->Add(btnSkin4);  panelStore->Controls->Add(lblPrice4); panelStore->Controls->Add(btnBuy4);
            panelStore->Controls->Add(btnSkin5);  panelStore->Controls->Add(lblPrice5); panelStore->Controls->Add(btnBuy5);
            panelStore->Controls->Add(btnSkin6);  panelStore->Controls->Add(lblPrice6); panelStore->Controls->Add(btnBuy6);

            this->Controls->Add(this->panelTop);
            this->Controls->Add(this->panelStore);

            this->ResumeLayout(false);
        }

        // ---- Helpers para reducir repetición ----
        void SetupSkinButton(Button^ btn, Color bg, int x, int y) {
            btn->BackColor = bg;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Location = System::Drawing::Point(x, y);
            btn->Size = System::Drawing::Size(160, 150);
            btn->UseVisualStyleBackColor = false;
        }

        void SetupPriceLabel(Label^ lbl, String^ text, int x, int y) {
            lbl->AutoSize = true;
            lbl->Font = gcnew System::Drawing::Font(L"Segoe UI Emoji", 14);
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

        // ---- Handlers de compra (extensibles) ----
        void BuySkin1(Object^ s, EventArgs^ e) { MessageBox::Show(L"¡Skin 1 comprada! (pendiente de implementar)"); }
        void BuySkin2(Object^ s, EventArgs^ e) { MessageBox::Show(L"¡Skin 2 comprada! (pendiente de implementar)"); }
        void BuySkin3(Object^ s, EventArgs^ e) { MessageBox::Show(L"¡Skin 3 comprada! (pendiente de implementar)"); }
        void BuySkin4(Object^ s, EventArgs^ e) { MessageBox::Show(L"¡Skin 4 comprada! (pendiente de implementar)"); }
        void BuySkin5(Object^ s, EventArgs^ e) { MessageBox::Show(L"¡Skin 5 comprada! (pendiente de implementar)"); }
        void BuySkin6(Object^ s, EventArgs^ e) { MessageBox::Show(L"¡Skin 6 comprada! (pendiente de implementar)"); }

        // ---- Regresar al juego ----
        void VolverAlJuego(Object^ sender, EventArgs^ e)
        {
            // Buscar Form1 por nombre (funciona porque Form1::Name = "Form1")
            Form^ juego = Application::OpenForms[L"Form1"];
            if (juego != nullptr)
                juego->Show();
            this->Close();
        }
    };

} // namespace MiProyecto