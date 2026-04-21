#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "ScoreManager.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    public ref class LeaderboardForm : public Form
    {
    public:
        LeaderboardForm(void)
        {
            InitializeComponent();
            LoadLeaderboard();
        }

    protected:
        ~LeaderboardForm()
        {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;
        Label^ lblTitle;
        DataGridView^ gridScores;
        Button^ btnClose;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->lblTitle = gcnew Label();
            this->gridScores = gcnew DataGridView();
            this->btnClose = gcnew Button();

            this->SuspendLayout();

            // --- Forma general ---
            this->Text = L"Leaderboard";
            this->StartPosition = FormStartPosition::CenterParent;
            this->BackColor = Color::FromArgb(78, 136, 43);         // verde menú principal
            this->ClientSize = System::Drawing::Size(780, 560);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;

            // --- Título ---
            lblTitle->Text = L" TOP 10  LEADERBOARD ";
            lblTitle->Font = gcnew System::Drawing::Font(L"Consolas", 18, System::Drawing::FontStyle::Bold);
            lblTitle->ForeColor = Color::White;
            lblTitle->AutoSize = true;
            lblTitle->Location = Point(28, 20);

            // --- Separador bajo el título ---
            Panel^ pnlDivider = gcnew Panel();
            pnlDivider->Location = Point(28, 58);
            pnlDivider->Size = System::Drawing::Size(724, 2);
            pnlDivider->BackColor = Color::FromArgb(52, 98, 30);    // verde oscuro del menú
            this->Controls->Add(pnlDivider);

            // --- Grid ---
            gridScores->Location = Point(28, 74);
            gridScores->Size = System::Drawing::Size(724, 418);
            gridScores->BackgroundColor = Color::FromArgb(170, 215, 81);  // verde lima del menú
            gridScores->BorderStyle = BorderStyle::None;
            gridScores->AllowUserToAddRows = false;
            gridScores->AllowUserToDeleteRows = false;
            gridScores->AllowUserToResizeRows = false;
            gridScores->AllowUserToResizeColumns = false;
            gridScores->ReadOnly = true;
            gridScores->RowHeadersVisible = false;
            gridScores->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            gridScores->MultiSelect = false;
            gridScores->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            gridScores->EnableHeadersVisualStyles = false;
            gridScores->ColumnHeadersHeight = 40;
            gridScores->RowTemplate->Height = 36;
            gridScores->GridColor = Color::FromArgb(52, 98, 30);
            gridScores->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::None;

            // Encabezados: verde muy oscuro con texto blanco
            gridScores->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(52, 98, 30);
            gridScores->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            gridScores->ColumnHeadersDefaultCellStyle->Font = gcnew System::Drawing::Font(L"Consolas", 11, System::Drawing::FontStyle::Bold);
            gridScores->ColumnHeadersDefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleCenter;

            // Filas normales: verde lima claro
            gridScores->DefaultCellStyle->BackColor = Color::FromArgb(239, 248, 229);
            gridScores->DefaultCellStyle->ForeColor = Color::FromArgb(35, 35, 35);
            gridScores->DefaultCellStyle->SelectionBackColor = Color::FromArgb(44, 110, 73);  // verde botón leaderboard
            gridScores->DefaultCellStyle->SelectionForeColor = Color::White;
            gridScores->DefaultCellStyle->Font = gcnew System::Drawing::Font(L"Consolas", 11);
            gridScores->DefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleCenter;

            // Filas alternas: verde lima más saturado
            gridScores->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(198, 231, 163);
            gridScores->AlternatingRowsDefaultCellStyle->ForeColor = Color::FromArgb(35, 35, 35);
            gridScores->AlternatingRowsDefaultCellStyle->SelectionBackColor = Color::FromArgb(44, 110, 73);
            gridScores->AlternatingRowsDefaultCellStyle->SelectionForeColor = Color::White;
            gridScores->AlternatingRowsDefaultCellStyle->Font = gcnew System::Drawing::Font(L"Consolas", 11);
            gridScores->AlternatingRowsDefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleCenter;

            gridScores->Columns->Add(L"Rank", L"#");
            gridScores->Columns->Add(L"Username", L"USERNAME");
            gridScores->Columns->Add(L"Score", L"SCORE");
            gridScores->Columns->Add(L"BestScore", L"BEST");
            gridScores->Columns->Add(L"BoardSize", L"BOARD");
            gridScores->Columns->Add(L"GameMode", L"MODE");
            gridScores->Columns[0]->FillWeight = 40;
            gridScores->Columns[1]->FillWeight = 110;
            gridScores->Columns[2]->FillWeight = 70;
            gridScores->Columns[3]->FillWeight = 80;
            gridScores->Columns[4]->FillWeight = 90;
            gridScores->Columns[5]->FillWeight = 110;

            // --- Botón Cerrar ---
            btnClose->Text = L"X  CERRAR";
            btnClose->Font = gcnew System::Drawing::Font(L"Consolas", 11, System::Drawing::FontStyle::Bold);
            btnClose->BackColor = Color::FromArgb(52, 98, 30);      // verde oscuro del menú
            btnClose->ForeColor = Color::White;
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->FlatAppearance->MouseOverBackColor = Color::FromArgb(44, 110, 73);
            btnClose->FlatAppearance->MouseDownBackColor = Color::FromArgb(35, 80, 55);
            btnClose->Size = System::Drawing::Size(140, 40);
            btnClose->Location = Point(612, 508);
            btnClose->Cursor = Cursors::Hand;
            btnClose->Click += gcnew EventHandler(this, &LeaderboardForm::CloseForm);

            this->Controls->Add(lblTitle);
            this->Controls->Add(gridScores);
            this->Controls->Add(btnClose);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void LoadLeaderboard()
        {
            gridScores->Rows->Clear();

            auto topScores = ScoreManager::GetTopScores(10);
            for (int i = 0; i < topScores->Count; i++)
            {
                ScoreEntry^ entry = topScores[i];
                gridScores->Rows->Add(
                    (i + 1).ToString(),
                    entry->Username,
                    entry->Score.ToString(),
                    entry->BestScore.ToString(),
                    entry->BoardSize,
                    entry->GameMode);
            }

            if (topScores->Count == 0)
                gridScores->Rows->Add(L"-", L"Sin registros", L"0", L"0", L"-", L"-");
        }

        void CloseForm(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
    };
}