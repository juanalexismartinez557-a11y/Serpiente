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
    static const int   CELL_SIZE = 30;   // px por celda
    static const int   GRID_COLS = 25;   // columnas  (750 / 30)
    static const int   GRID_ROWS = 22;   // filas     (660 / 30)

    static const int   SNAKE_SPEED_MS = 150;  // ms entre ticks  (menor = mas rapido)
    static const int   APPLE_GROW_AMOUNT = 3;   // cuantos segmentos crece al comer

    static const int   SCORE_PER_APPLE = 10;   // puntos por manzana
    static const int   MAX_RANKING = 5;    // top 5 en el ranking

    static const String^ SCORE_FILE = "snake_scores.csv"; // archivo de puntuaciones

    // =========================================================
    //  ESTRUCTURA: Punto en la grid
    // =========================================================
    value struct GridPoint {
        int X, Y;
        GridPoint(int x, int y) : X(x), Y(y) {}
        bool Equals(GridPoint other) { return X == other.X && Y == other.Y; }
    };

    // =========================================================
    //  CLASE: Pared (objeto reutilizable para obstaculos)
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
            // borde oscuro para que se vea solido
            g->DrawRectangle(
                gcnew Pen(Color::FromArgb(80, 50, 20), 1),
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
                candidate = GridPoint(rnd->Next(0, GRID_COLS), rnd->Next(0, GRID_ROWS));
                ok = true;
                for each (GridPoint p in occupied) {
                    if (p.Equals(candidate)) { ok = false; break; }
                }
            }
            Position = candidate;
        }

        void Draw(Graphics^ g) {
            // cuerpo rojo
            g->FillEllipse(
                gcnew SolidBrush(Color::FromArgb(220, 50, 50)),
                Position.X * CELL_SIZE + 2,
                Position.Y * CELL_SIZE + 2,
                CELL_SIZE - 4, CELL_SIZE - 4
            );
            // brillo
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
        // ----- Variables de configuracion (puedes cambiarlas desde fuera) -----
        bool WallPassThrough;   // true = serpiente atraviesa paredes del borde
        int  SpeedMs;           // velocidad del juego en ms
        int  GrowAmount;        // cuanto crece al comer una manzana

        // ----- Estado del juego -----
        bool IsGameOver;
        bool IsPaused;
        int  Score;
        int  ApplesEaten;
        int  HighScore;         // mejor puntaje del ranking guardado

        // ----- Eventos que el Form puede escuchar -----
        delegate void GameOverDelegate(int finalScore);
        event GameOverDelegate^ OnGameOver;

        delegate void ScoreChangedDelegate(int score, int apples);
        event ScoreChangedDelegate^ OnScoreChanged;

    private:
        List<GridPoint>^ snakeBody;
        int               dx, dy;           // direccion actual
        int               pendingDx, pendingDy; // siguiente direccion (buffer de input)
        int               growPending;      // segmentos que faltan por agregar

        Apple^ apple;
        List<Wall^>^ walls;            // lista de paredes (bordes + futuros obstaculos)

        Random^ rnd;

    public:
        SnakeGame() {
            // Configuracion por defecto
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

        // ----- Inicializar / reiniciar partida -----
        void Reset() {
            snakeBody->Clear();
            walls->Clear();

            IsGameOver = false;
            IsPaused = false;
            Score = 0;
            ApplesEaten = 0;
            growPending = 0;

            // Serpiente inicial: 3 segmentos en el centro, moviendose a la derecha
            int midRow = GRID_ROWS / 2;
            int midCol = GRID_COLS / 2;
            snakeBody->Add(GridPoint(midCol, midRow));
            snakeBody->Add(GridPoint(midCol - 1, midRow));
            snakeBody->Add(GridPoint(midCol - 2, midRow));

            dx = 1; dy = 0;
            pendingDx = 1; pendingDy = 0;

            // Construir paredes de borde
            BuildBorderWalls();

            // Spawnear primera manzana
            apple->Spawn(GetOccupiedCells());
        }

        // ----- Construir paredes del borde del tablero -----
        void BuildBorderWalls() {
            Color wallCol = Color::FromArgb(101, 67, 33); // cafe oscuro

            // Fila superior e inferior
            for (int x = 0; x < GRID_COLS; x++) {
                walls->Add(gcnew Wall(GridPoint(x, 0), wallCol));
                walls->Add(gcnew Wall(GridPoint(x, GRID_ROWS - 1), wallCol));
            }
            // Columna izquierda y derecha (sin esquinas ya agregadas)
            for (int y = 1; y < GRID_ROWS - 1; y++) {
                walls->Add(gcnew Wall(GridPoint(0, y), wallCol));
                walls->Add(gcnew Wall(GridPoint(GRID_COLS - 1, y), wallCol));
            }
        }

        // ----- Agregar una pared extra (para modo obstaculos futuro) -----
        void AddWall(int gridX, int gridY) {
            walls->Add(gcnew Wall(GridPoint(gridX, gridY), Color::FromArgb(80, 50, 20)));
        }

        // ----- Procesar input del teclado -----
        void SetDirection(int newDx, int newDy) {
            // No permitir giro de 180 grados
            if (newDx == -dx && newDy == -dy) return;
            pendingDx = newDx;
            pendingDy = newDy;
        }

        // ----- Tick del juego (llamado por el Timer) -----
        void Tick() {
            if (IsGameOver || IsPaused) return;

            // Aplicar direccion pendiente
            dx = pendingDx;
            dy = pendingDy;

            // Calcular nueva cabeza
            GridPoint head = snakeBody[0];
            GridPoint newHead(head.X + dx, head.Y + dy);

            // ---- Logica de paredes (atravesar o chocar) ----
            if (WallPassThrough) {
                // Wrap-around: si sale del area de juego (ignorando la capa de paredes)
                // el area jugable es [1, GRID_COLS-2] x [1, GRID_ROWS-2]
                if (newHead.X <= 0)             newHead.X = GRID_COLS - 2;
                else if (newHead.X >= GRID_COLS - 1) newHead.X = 1;
                if (newHead.Y <= 0)             newHead.Y = GRID_ROWS - 2;
                else if (newHead.Y >= GRID_ROWS - 1) newHead.Y = 1;
            }
            else {
                // Verificar colision con paredes
                for each (Wall ^ w in walls) {
                    if (w->Collides(newHead)) {
                        TriggerGameOver();
                        return;
                    }
                }
            }

            // ---- Colision con el propio cuerpo ----
            for (int i = 0; i < snakeBody->Count - 1; i++) {
                if (snakeBody[i].Equals(newHead)) {
                    TriggerGameOver();
                    return;
                }
            }

            // ---- Mover serpiente ----
            snakeBody->Insert(0, newHead);

            if (growPending > 0) {
                growPending--;  // no eliminar cola: crece
            }
            else {
                snakeBody->RemoveAt(snakeBody->Count - 1);
            }

            // ---- Colision con manzana ----
            if (newHead.Equals(apple->Position)) {
                ApplesEaten++;
                Score += SCORE_PER_APPLE;
                growPending += GrowAmount;
                apple->Spawn(GetOccupiedCells());
                OnScoreChanged(Score, ApplesEaten);
            }
        }

        // ----- Dibujar todo en el Graphics del panel -----
        void Draw(Graphics^ g) {
            // Fondo del area jugable (ya lo pone el panel, pero por si acaso)
            // Paredes
            for each (Wall ^ w in walls) {
                w->Draw(g);
            }

            // Manzana
            apple->Draw(g);

            // Serpiente
            for (int i = 0; i < snakeBody->Count; i++) {
                GridPoint p = snakeBody[i];
                Color segColor;

                if (i == 0) {
                    // Cabeza: verde oscuro
                    segColor = Color::FromArgb(34, 100, 14);
                }
                else if (i % 2 == 0) {
                    segColor = Color::FromArgb(74, 160, 44);
                }
                else {
                    segColor = Color::FromArgb(90, 180, 55);
                }

                int px = p.X * CELL_SIZE;
                int py = p.Y * CELL_SIZE;

                // Cuerpo redondeado
                g->FillRectangle(
                    gcnew SolidBrush(segColor),
                    px + 1, py + 1, CELL_SIZE - 2, CELL_SIZE - 2
                );

                // Ojos en la cabeza
                if (i == 0) {
                    DrawEyes(g, px, py);
                }
            }

            // Overlay de Game Over
            if (IsGameOver) {
                DrawGameOverOverlay(g);
            }

            // Overlay de Pausa
            if (IsPaused && !IsGameOver) {
                DrawPauseOverlay(g);
            }
        }

        // ----- Guardar puntaje y retornar ranking -----
        List<int>^ SaveAndGetRanking(int finalScore) {
            List<int>^ scores = LoadScores();
            scores->Add(finalScore);
            scores->Sort();
            scores->Reverse(); // mayor primero

            // Guardar top MAX_RANKING
            StreamWriter^ sw = gcnew StreamWriter(SCORE_FILE, false);
            for (int i = 0; i < Math::Min(scores->Count, MAX_RANKING); i++) {
                sw->WriteLine(scores[i]);
            }
            sw->Close();

            // Actualizar high score en memoria
            if (scores->Count > 0) HighScore = scores[0];

            return scores;
        }

        // ----- Cargar ranking desde archivo -----
        List<int>^ LoadScores() {
            List<int>^ scores = gcnew List<int>();
            if (!File::Exists(SCORE_FILE)) return scores;
            try {
                StreamReader^ sr = gcnew StreamReader(SCORE_FILE);
                String^ line;
                while ((line = sr->ReadLine()) != nullptr) {
                    int val;
                    if (int::TryParse(line->Trim(), val)) {
                        scores->Add(val);
                    }
                }
                sr->Close();
            }
            catch (...) {}
            return scores;
        }

        void TogglePause() {
            if (!IsGameOver) IsPaused = !IsPaused;
        }

        int GetSnakeLength() { return snakeBody->Count; }

    private:
        // Retorna todas las celdas ocupadas (cuerpo + paredes) para spawn seguro
        List<GridPoint>^ GetOccupiedCells() {
            List<GridPoint>^ occupied = gcnew List<GridPoint>();
            for each (GridPoint p in snakeBody) occupied->Add(p);
            for each (Wall ^ w in walls)        occupied->Add(w->Position);
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
            Color eyeWhite = Color::White;
            Color eyePupil = Color::Black;

            // Posicion de ojos segun direccion
            int ex1, ey1, ex2, ey2;
            if (dx == 1 && dy == 0) { // derecha
                ex1 = px + CELL_SIZE - 8; ey1 = py + 4;
                ex2 = px + CELL_SIZE - 8; ey2 = py + CELL_SIZE - 10;
            }
            else if (dx == -1 && dy == 0) { // izquierda
                ex1 = px + 2; ey1 = py + 4;
                ex2 = px + 2; ey2 = py + CELL_SIZE - 10;
            }
            else if (dx == 0 && dy == -1) { // arriba
                ex1 = px + 4;              ey1 = py + 2;
                ex2 = px + CELL_SIZE - 10; ey2 = py + 2;
            }
            else { // abajo
                ex1 = px + 4;              ey1 = py + CELL_SIZE - 8;
                ex2 = px + CELL_SIZE - 10; ey2 = py + CELL_SIZE - 8;
            }

            g->FillEllipse(gcnew SolidBrush(eyeWhite), ex1, ey1, 6, 6);
            g->FillEllipse(gcnew SolidBrush(eyeWhite), ex2, ey2, 6, 6);
            g->FillEllipse(gcnew SolidBrush(eyePupil), ex1 + 1, ey1 + 1, 4, 4);
            g->FillEllipse(gcnew SolidBrush(eyePupil), ex2 + 1, ey2 + 1, 4, 4);
        }

        void DrawGameOverOverlay(Graphics^ g) {
            // Fondo semitransparente
            g->FillRectangle(
                gcnew SolidBrush(Color::FromArgb(160, 0, 0, 0)),
                0, 0, GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE
            );

            // Texto GAME OVER
            Font^ bigFont = gcnew Font(L"Segoe UI", 32, FontStyle::Bold);
            Font^ smFont = gcnew Font(L"Segoe UI", 14, FontStyle::Regular);
            SolidBrush^ white = gcnew SolidBrush(Color::White);
            SolidBrush^ gold = gcnew SolidBrush(Color::Gold);

            StringFormat^ center = gcnew StringFormat();
            center->Alignment = StringAlignment::Center;

            int cx = (GRID_COLS * CELL_SIZE) / 2;
            int cy = (GRID_ROWS * CELL_SIZE) / 2;

            g->DrawString(L"GAME OVER", bigFont, gold,
                RectangleF((float)(cx - 200), (float)(cy - 80), 400, 60), center);

            g->DrawString(
                String::Format(L"Puntaje: {0}", Score),
                smFont, white,
                RectangleF((float)(cx - 150), (float)(cy - 10), 300, 35), center
            );
            g->DrawString(
                L"Presiona R para reiniciar",
                smFont, white,
                RectangleF((float)(cx - 150), (float)(cy + 30), 300, 35), center
            );
        }

        void DrawPauseOverlay(Graphics^ g) {
            g->FillRectangle(
                gcnew SolidBrush(Color::FromArgb(120, 0, 0, 0)),
                0, 0, GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE
            );
            Font^ bigFont = gcnew Font(L"Segoe UI", 36, FontStyle::Bold);
            StringFormat^ center = gcnew StringFormat();
            center->Alignment = StringAlignment::Center;
            int cx = (GRID_COLS * CELL_SIZE) / 2;
            int cy = (GRID_ROWS * CELL_SIZE) / 2;
            g->DrawString(L"PAUSA", bigFont, gcnew SolidBrush(Color::White),
                RectangleF((float)(cx - 150), (float)(cy - 30), 300, 60), center);
        }
    };

} // namespace MiProyecto