#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

namespace MiProyecto {

    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Drawing;
    using namespace System::IO;

    // =========================================================
    //  CONSTANTES DEL JUEGO  (cambia aqui para modos de juego)
    // =========================================================
    static const int CELL_SIZE = 30;
    static const int GRID_COLS = 25;
    static const int GRID_ROWS = 22;
    static const int SNAKE_SPEED_MS = 150;
    static const int APPLE_GROW_AMOUNT = 3;
    static const int SCORE_PER_APPLE = 10;
    static const int MAX_RANKING = 5;

    // Funcion para obtener el nombre del archivo (evita error C3145 con String^ global)
    static String^ GetScoreFile() { return "snake_scores.csv"; }

    // =========================================================
    //  ESTRUCTURA: Punto en la grid
    // =========================================================
    value struct GridPoint {
        int X, Y;
        GridPoint(int x, int y) : X(x), Y(y) {}
        bool Equals(GridPoint other) { return X == other.X && Y == other.Y; }
    };

    // =========================================================
    //  CLASE: Pared (objeto reutilizable, misma logica para obstaculos futuros)
    // =========================================================
    ref class Wall {
    public:
        GridPoint Position;
        Color     WallColor;

        Wall(GridPoint pos, Color col) : Position(pos), WallColor(col) {}

        void Draw(Graphics^ g) {
            g->FillRectangle(
                gcnew SolidBrush(WallColor),
                Position.X * CELL_SIZE,
                Position.Y * CELL_SIZE,
                CELL_SIZE, CELL_SIZE
            );
            g->DrawRectangle(
                gcnew Pen(Color::FromArgb(60, 35, 10), 1),
                Position.X * CELL_SIZE,
                Position.Y * CELL_SIZE,
                CELL_SIZE - 1, CELL_SIZE - 1
            );
        }

        bool Collides(GridPoint p) { return Position.Equals(p); }
    };

    // =========================================================
    //  CLASE: Manzana
    // =========================================================
    ref class Apple {
    public:
        GridPoint Position;

        Apple() : Position(GridPoint(0, 0)) {}

        void Spawn(List<GridPoint>^ occupied) {
            Random^ rnd = gcnew Random();
            GridPoint candidate(0, 0);
            bool ok = false;
            while (!ok) {
                // Spawnea dentro del area jugable (sin tocar paredes de borde)
                candidate = GridPoint(rnd->Next(1, GRID_COLS - 1), rnd->Next(1, GRID_ROWS - 1));
                ok = true;
                for each (GridPoint p in occupied) {
                    if (p.Equals(candidate)) { ok = false; break; }
                }
            }
            Position = candidate;
        }

        void Draw(Graphics^ g) {
            // Cuerpo
            g->FillEllipse(
                gcnew SolidBrush(Color::FromArgb(220, 50, 50)),
                Position.X * CELL_SIZE + 2,
                Position.Y * CELL_SIZE + 2,
                CELL_SIZE - 4, CELL_SIZE - 4
            );
            // Brillo
            g->FillEllipse(
                gcnew SolidBrush(Color::FromArgb(120, 255, 255, 255)),
                Position.X * CELL_SIZE + 6,
                Position.Y * CELL_SIZE + 4,
                6, 5
            );
        }
    };

    // =========================================================
    //  CLASE PRINCIPAL: Motor del juego Snake
    // =========================================================
    ref class SnakeGame {
    public:
        // --- Configuracion (modificable desde Form para modos de juego) ---
        bool WallPassThrough;   // true = la serpiente atraviesa las paredes del borde
        int  SpeedMs;           // ms entre ticks (menor = mas rapido)
        int  GrowAmount;        // segmentos que crece al comer una manzana

        // --- Estado ---
        bool IsGameOver;
        bool IsPaused;
        int  Score;
        int  ApplesEaten;
        int  HighScore;

        // --- Eventos ---
        delegate void GameOverDelegate(int finalScore);
        event GameOverDelegate^ OnGameOver;

        delegate void ScoreChangedDelegate(int score, int apples);
        event ScoreChangedDelegate^ OnScoreChanged;

    private:
        List<GridPoint>^ snakeBody;
        int dx, dy;
        int pendingDx, pendingDy;
        int growPending;

        Apple^ apple;
        List<Wall^>^ walls;
        Random^ rnd;

    public:
        SnakeGame() {
            WallPassThrough = false;
            SpeedMs = SNAKE_SPEED_MS;
            GrowAmount = APPLE_GROW_AMOUNT;

            rnd = gcnew Random();
            walls = gcnew List<Wall^>();
            apple = gcnew Apple();
            snakeBody = gcnew List<GridPoint>();

            LoadHighScore();
            Reset();
        }

        // Reinicia la partida sin destruir el objeto
        void Reset() {
            snakeBody->Clear();
            walls->Clear();

            IsGameOver = false;
            IsPaused = false;
            Score = 0;
            ApplesEaten = 0;
            growPending = 0;

            // Serpiente inicial de 3 segmentos en el centro, moviendose a la derecha
            int midRow = GRID_ROWS / 2;
            int midCol = GRID_COLS / 2;
            snakeBody->Add(GridPoint(midCol, midRow));
            snakeBody->Add(GridPoint(midCol - 1, midRow));
            snakeBody->Add(GridPoint(midCol - 2, midRow));

            dx = 1; dy = 0;
            pendingDx = 1; pendingDy = 0;

            BuildBorderWalls();
            apple->Spawn(GetOccupiedCells());
        }

        // Construye las paredes del borde como objetos Wall
        void BuildBorderWalls() {
            Color wallCol = Color::FromArgb(101, 67, 33);
            for (int x = 0; x < GRID_COLS; x++) {
                walls->Add(gcnew Wall(GridPoint(x, 0), wallCol));
                walls->Add(gcnew Wall(GridPoint(x, GRID_ROWS - 1), wallCol));
            }
            for (int y = 1; y < GRID_ROWS - 1; y++) {
                walls->Add(gcnew Wall(GridPoint(0, y), wallCol));
                walls->Add(gcnew Wall(GridPoint(GRID_COLS - 1, y), wallCol));
            }
        }

        // Agrega una pared extra (para modo obstaculos)
        void AddWall(int gridX, int gridY) {
            walls->Add(gcnew Wall(GridPoint(gridX, gridY), Color::FromArgb(80, 50, 20)));
        }

        // Buffer de direccion (evita giro de 180 grados)
        void SetDirection(int newDx, int newDy) {
            if (newDx == -dx && newDy == -dy) return;
            pendingDx = newDx;
            pendingDy = newDy;
        }

        // Tick principal llamado por el Timer del Form
        void Tick() {
            if (IsGameOver || IsPaused) return;

            dx = pendingDx;
            dy = pendingDy;

            GridPoint head = snakeBody[0];
            GridPoint newHead(head.X + dx, head.Y + dy);

            // Logica de paredes
            if (WallPassThrough) {
                if (newHead.X <= 0)             newHead.X = GRID_COLS - 2;
                else if (newHead.X >= GRID_COLS - 1) newHead.X = 1;
                if (newHead.Y <= 0)             newHead.Y = GRID_ROWS - 2;
                else if (newHead.Y >= GRID_ROWS - 1) newHead.Y = 1;
            }
            else {
                for each (Wall ^ w in walls) {
                    if (w->Collides(newHead)) { TriggerGameOver(); return; }
                }
            }

            // Colision con cuerpo propio
            for (int i = 0; i < snakeBody->Count - 1; i++) {
                if (snakeBody[i].Equals(newHead)) { TriggerGameOver(); return; }
            }

            // Mover
            snakeBody->Insert(0, newHead);
            if (growPending > 0) {
                growPending--;
            }
            else {
                snakeBody->RemoveAt(snakeBody->Count - 1);
            }

            // Comer manzana
            if (newHead.Equals(apple->Position)) {
                ApplesEaten++;
                Score += SCORE_PER_APPLE;
                growPending += GrowAmount;
                apple->Spawn(GetOccupiedCells());
                OnScoreChanged(Score, ApplesEaten);
            }
        }

        // Dibuja todo el estado del juego
        void Draw(Graphics^ g) {
            for each (Wall ^ w in walls)  w->Draw(g);
            apple->Draw(g);

            for (int i = 0; i < snakeBody->Count; i++) {
                GridPoint p = snakeBody[i];
                Color segColor;
                if (i == 0)     segColor = Color::FromArgb(34, 100, 14);
                else if (i % 2 == 0) segColor = Color::FromArgb(74, 160, 44);
                else                 segColor = Color::FromArgb(90, 180, 55);

                int px = p.X * CELL_SIZE;
                int py = p.Y * CELL_SIZE;
                g->FillRectangle(gcnew SolidBrush(segColor), px + 1, py + 1, CELL_SIZE - 2, CELL_SIZE - 2);
                if (i == 0) DrawEyes(g, px, py);
            }

            if (IsGameOver)              DrawGameOverOverlay(g);
            if (IsPaused && !IsGameOver) DrawPauseOverlay(g);
        }

        // Guarda el puntaje y devuelve el ranking actualizado
        List<int>^ SaveAndGetRanking(int finalScore) {
            List<int>^ scores = LoadScores();
            scores->Add(finalScore);
            scores->Sort();
            scores->Reverse();

            StreamWriter^ sw = gcnew StreamWriter(GetScoreFile(), false);
            for (int i = 0; i < Math::Min(scores->Count, MAX_RANKING); i++)
                sw->WriteLine(scores[i]);
            sw->Close();

            if (scores->Count > 0) HighScore = scores[0];
            return scores;
        }

        // Carga los puntajes guardados desde el CSV
        List<int>^ LoadScores() {
            List<int>^ scores = gcnew List<int>();
            if (!File::Exists(GetScoreFile())) return scores;
            try {
                StreamReader^ sr = gcnew StreamReader(GetScoreFile());
                String^ line;
                while ((line = sr->ReadLine()) != nullptr) {
                    int val;
                    if (int::TryParse(line->Trim(), val))
                        scores->Add(val);
                }
                sr->Close();
            }
            catch (...) {}
            return scores;
        }

        void TogglePause() { if (!IsGameOver) IsPaused = !IsPaused; }
        int  GetSnakeLength() { return snakeBody->Count; }

    private:
        List<GridPoint>^ GetOccupiedCells() {
            List<GridPoint>^ occupied = gcnew List<GridPoint>();
            for each (GridPoint p in snakeBody) occupied->Add(p);
            for each (Wall ^ w in walls)         occupied->Add(w->Position);
            return occupied;
        }

        void TriggerGameOver() {
            IsGameOver = true;
            SaveAndGetRanking(Score);
            OnGameOver(Score);
        }

        void LoadHighScore() {
            List<int>^ scores = LoadScores();
            HighScore = (scores->Count > 0) ? scores[0] : 0;
        }

        void DrawEyes(Graphics^ g, int px, int py) {
            int ex1, ey1, ex2, ey2;
            if (dx == 1 && dy == 0) { ex1 = px + CELL_SIZE - 8; ey1 = py + 4;              ex2 = px + CELL_SIZE - 8;  ey2 = py + CELL_SIZE - 10; }
            else if (dx == -1 && dy == 0) { ex1 = px + 2;           ey1 = py + 4;              ex2 = px + 2;            ey2 = py + CELL_SIZE - 10; }
            else if (dx == 0 && dy == -1) { ex1 = px + 4;           ey1 = py + 2;              ex2 = px + CELL_SIZE - 10; ey2 = py + 2; }
            else { ex1 = px + 4;           ey1 = py + CELL_SIZE - 8;    ex2 = px + CELL_SIZE - 10; ey2 = py + CELL_SIZE - 8; }

            g->FillEllipse(gcnew SolidBrush(Color::White), ex1, ey1, 6, 6);
            g->FillEllipse(gcnew SolidBrush(Color::White), ex2, ey2, 6, 6);
            g->FillEllipse(gcnew SolidBrush(Color::Black), ex1 + 1, ey1 + 1, 4, 4);
            g->FillEllipse(gcnew SolidBrush(Color::Black), ex2 + 1, ey2 + 1, 4, 4);
        }

        void DrawGameOverOverlay(Graphics^ g) {
            g->FillRectangle(gcnew SolidBrush(Color::FromArgb(160, 0, 0, 0)),
                0, 0, GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE);

            System::Drawing::Font^ bigFont = gcnew System::Drawing::Font(L"Segoe UI", 32, System::Drawing::FontStyle::Bold);
            System::Drawing::Font^ smFont = gcnew System::Drawing::Font(L"Segoe UI", 14, System::Drawing::FontStyle::Regular);
            StringFormat^ center = gcnew StringFormat();
            center->Alignment = StringAlignment::Center;

            int cx = (GRID_COLS * CELL_SIZE) / 2;
            int cy = (GRID_ROWS * CELL_SIZE) / 2;

            g->DrawString(L"GAME OVER", bigFont, gcnew SolidBrush(Color::Gold),
                RectangleF((float)(cx - 200), (float)(cy - 80), 400, 60), center);
            g->DrawString(String::Format(L"Puntaje: {0}", Score), smFont, gcnew SolidBrush(Color::White),
                RectangleF((float)(cx - 150), (float)(cy - 10), 300, 35), center);
            g->DrawString(L"Presiona R para reiniciar", smFont, gcnew SolidBrush(Color::White),
                RectangleF((float)(cx - 150), (float)(cy + 30), 300, 35), center);
        }

        void DrawPauseOverlay(Graphics^ g) {
            g->FillRectangle(gcnew SolidBrush(Color::FromArgb(120, 0, 0, 0)),
                0, 0, GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE);
            System::Drawing::Font^ bigFont = gcnew System::Drawing::Font(L"Segoe UI", 36, System::Drawing::FontStyle::Bold);
            StringFormat^ center = gcnew StringFormat();
            center->Alignment = StringAlignment::Center;
            int cx = (GRID_COLS * CELL_SIZE) / 2;
            int cy = (GRID_ROWS * CELL_SIZE) / 2;
            g->DrawString(L"PAUSA", bigFont, gcnew SolidBrush(Color::White),
                RectangleF((float)(cx - 150), (float)(cy - 30), 300, 60), center);
        }
    };

} // namespace MiProyecto