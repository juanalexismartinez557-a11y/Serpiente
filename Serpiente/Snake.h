#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

namespace MiProyecto {

    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Drawing;

    // =========================================================
    //  CONSTANTES DEL JUEGO
    // =========================================================
    static const int CELL_SIZE = 30;
    static const int SNAKE_SPEED_MS = 150;
    static const int APPLE_GROW_AMOUNT = 1;
    static const int SCORE_PER_APPLE = 10;
    static const int MAX_RANKING = 5;
    static const int MIN_SPEED_MS = 50;   // Velocidad máxima (intervalo mínimo)
    static const int OBSTACLE_SCORE_THRESHOLD = 5;   // Cada cuántos puntos aparece obstáculo
    static const int SPEED_SCORE_THRESHOLD = 10;   // Cada cuántos puntos aumenta velocidad

    ref class ScoreConfig {
    public:
        literal System::String^ FILE_NAME = "snake_scores.csv";
    };

    // =========================================================
    //  MODOS DE JUEGO
    //  Equivalente al enum GameMode en modos.h, pero en C++/CLI
    //  para ser compatible con SnakeGame (ref class administrada).
    //  La lógica de cada modo está implementada directamente en
    //  SnakeGame::UpdateByScore() y SnakeGame::Tick(), replicando
    //  el comportamiento de GameModes::updateByScore() y
    //  GameModes::handleFoodEaten() de modos.cpp.
    // =========================================================
    public enum class GameMode {
        Normal,         // Sin modificadores
        Obstacles,      // Genera obstáculos cada OBSTACLE_SCORE_THRESHOLD puntos
        RandomGrowth,   // Crecimiento aleatorio (1-5) al comer
        SpeedIncrease   // Aumenta velocidad cada SPEED_SCORE_THRESHOLD puntos
    };

    enum class BoardSize { Small, Medium, Large };

    // =========================================================
    //  ESTRUCTURA: Punto en la grid
    // =========================================================
    value struct GridPoint {
        int X, Y;
        GridPoint(int x, int y) : X(x), Y(y) {}
        bool Equals(GridPoint other) { return X == other.X && Y == other.Y; }
    };

    // =========================================================
    //  CLASE: Pared / Obstáculo
    // =========================================================
    ref class Wall {
    public:
        GridPoint Position;
        Color     WallColor;
        bool      IsBorder;  // true = pared de borde, false = obstáculo interior

        Wall(GridPoint pos, Color col, bool isBorder)
            : Position(pos), WallColor(col), IsBorder(isBorder) {
        }

        // Sobrecarga para compatibilidad con código existente
        Wall(GridPoint pos, Color col) : Position(pos), WallColor(col), IsBorder(false) {}

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

        void Generate(Random^ rnd, int cols, int rows, List<GridPoint>^ occupiedCells) {
            int minX = 1, maxX = cols - 1;
            int minY = 1, maxY = rows - 1;
            if (maxX <= minX) maxX = minX + 1;
            if (maxY <= minY) maxY = minY + 1;

            for (int attempts = 0; attempts < 100; attempts++) {
                GridPoint newPos(rnd->Next(minX, maxX), rnd->Next(minY, maxY));
                bool isOccupied = false;
                for each (GridPoint p in occupiedCells) {
                    if (p.Equals(newPos)) { isOccupied = true; break; }
                }
                if (!isOccupied) { Position = newPos; return; }
            }
            // Fallback si el tablero está casi lleno
            Position = GridPoint(rnd->Next(minX, maxX), rnd->Next(minY, maxY));
        }

        void Draw(Graphics^ g) {
            g->FillEllipse(
                gcnew SolidBrush(Color::FromArgb(220, 50, 50)),
                Position.X * CELL_SIZE + 2,
                Position.Y * CELL_SIZE + 2,
                CELL_SIZE - 4, CELL_SIZE - 4
            );
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
        // --- Configuración ---
        bool WallPassThrough;
        int  SpeedMs;       // Intervalo actual del timer (Form1 lo usa para el Timer)
        int  GrowAmount;    // Segmentos que crece al comer (modo Normal/Obstacles)

        // --- Estado público ---
        bool HasStarted;
        bool IsGameOver;
        bool IsPaused;
        int  Score;
        int  ApplesEaten;
        int  HighScore;
        int  CurrentCols;
        int  CurrentRows;
        GameMode CurrentMode;

        // --- Eventos ---
        delegate void GameOverDelegate(int finalScore);
        event GameOverDelegate^ OnGameOver;

        delegate void ScoreChangedDelegate(int score, int apples);
        event ScoreChangedDelegate^ OnScoreChanged;

        // NUEVO: notifica a Form1 cuando el SpeedIncrease cambia el intervalo
        delegate void SpeedChangedDelegate(int newSpeedMs);
        event SpeedChangedDelegate^ OnSpeedChanged;

    private:
        List<GridPoint>^ snakeBody;
        int dx, dy, pendingDx, pendingDy, growPending;
        Apple^ apple;
        List<Wall^>^ walls;
        Random^ rnd;

        int baseSpeedMs;        // Velocidad base del modo (para Reset)
        int lastObstacleScore;  // Último puntaje donde se generó obstáculo
        int lastSpeedScore;     // Último puntaje donde aumentó la velocidad

    public:

        SnakeGame() {
            // *** CORRECCIÓN CRÍTICA: inicializar dimensiones ANTES de llamar Reset() ***
            CurrentCols = 25;
            CurrentRows = 25;

            WallPassThrough = false;
            SpeedMs = SNAKE_SPEED_MS;
            baseSpeedMs = SNAKE_SPEED_MS;
            GrowAmount = APPLE_GROW_AMOUNT;
            CurrentMode = GameMode::Normal;
            lastObstacleScore = -1;
            lastSpeedScore = -1;

            rnd = gcnew Random();
            walls = gcnew List<Wall^>();
            apple = gcnew Apple();
            snakeBody = gcnew List<GridPoint>();

            LoadHighScore();
            Reset();
        }

        // Cambia el tamaño del tablero y reinicia (solo antes de empezar)
        void SetBoardSize(BoardSize size) {
            switch (size) {
            case BoardSize::Small:  CurrentCols = 10; CurrentRows = 10; break;
            case BoardSize::Medium: CurrentCols = 25; CurrentRows = 25; break;
            case BoardSize::Large:  CurrentCols = 50; CurrentRows = 50; break;
            }
            if (CurrentCols < 5) CurrentCols = 5;
            if (CurrentRows < 5) CurrentRows = 5;
            Reset();
        }

        // Establece el modo de juego y ajusta parámetros iniciales del modo
        void SetGameMode(GameMode mode) {
            CurrentMode = mode;
            switch (mode) {
            case GameMode::SpeedIncrease:
                // Empieza un poco más rápido que el modo Normal
                SpeedMs = 120; baseSpeedMs = 120;
                break;
            case GameMode::Obstacles:
                WallPassThrough = false; // Obstáculos requieren colisión con paredes
                SpeedMs = SNAKE_SPEED_MS; baseSpeedMs = SNAKE_SPEED_MS;
                break;
            default:
                SpeedMs = SNAKE_SPEED_MS; baseSpeedMs = SNAKE_SPEED_MS;
                break;
            }
        }

        // Reinicia la partida sin destruir el objeto
        void Reset() {
            snakeBody->Clear();
            walls->Clear();

            IsGameOver = false;
            IsPaused = false;
            HasStarted = false;
            Score = 0;
            ApplesEaten = 0;
            growPending = 0;

            // Restaurar velocidad base del modo
            SpeedMs = baseSpeedMs;
            lastObstacleScore = -1;
            lastSpeedScore = -1;

            // Serpiente inicial de 3 segmentos en el centro, moviéndose a la derecha
            int midRow = CurrentRows / 2;
            int midCol = CurrentCols / 2;
            snakeBody->Add(GridPoint(midCol, midRow));
            snakeBody->Add(GridPoint(midCol - 1, midRow));
            snakeBody->Add(GridPoint(midCol - 2, midRow));

            dx = 1; dy = 0;
            pendingDx = 1; pendingDy = 0;

            BuildBorderWalls();
            apple->Generate(rnd, CurrentCols, CurrentRows, GetOccupiedCells());
        }

        // Buffer de dirección (evita giro de 180°)
        void SetDirection(int newDx, int newDy) {
            if (newDx == -dx && newDy == -dy) return;
            pendingDx = newDx;
            pendingDy = newDy;
        }

        // Tick principal — llamado por el Timer de Form1
        void Tick() {
            if (IsGameOver || IsPaused) return;
            HasStarted = true;
            dx = pendingDx; dy = pendingDy;

            GridPoint head = snakeBody[0];
            GridPoint newHead(head.X + dx, head.Y + dy);

            // Lógica de paredes del borde
            if (WallPassThrough) {
                if (newHead.X <= 0)             newHead.X = CurrentCols - 2;
                else if (newHead.X >= CurrentCols - 1) newHead.X = 1;
                if (newHead.Y <= 0)             newHead.Y = CurrentRows - 2;
                else if (newHead.Y >= CurrentRows - 1) newHead.Y = 1;
            }
            else {
                for each (Wall ^ w in walls) {
                    if (w->Collides(newHead)) { TriggerGameOver(); return; }
                }
            }

            // Colisión con cuerpo propio
            for (int i = 0; i < snakeBody->Count - 1; i++) {
                if (snakeBody[i].Equals(newHead)) { TriggerGameOver(); return; }
            }

            // Mover serpiente
            snakeBody->Insert(0, newHead);
            if (growPending > 0) growPending--;
            else snakeBody->RemoveAt(snakeBody->Count - 1);

            // ¿Comió manzana?
            if (newHead.Equals(apple->Position)) {
                ApplesEaten++;
                Score += SCORE_PER_APPLE;

                // Crecimiento según modo
                // Equivalente a GameModes::handleFoodEaten() en modos.cpp
                int grow = GrowAmount;
                if (CurrentMode == GameMode::RandomGrowth)
                    grow = rnd->Next(1, 6); // Aleatorio 1-5 (igual que getRandomGrowthAmount)

                growPending += grow;
                apple->Generate(rnd, CurrentCols, CurrentRows, GetOccupiedCells());
                OnScoreChanged(Score, ApplesEaten);

                // Efectos por puntaje según modo
                UpdateByScore();
            }
        }

        // Dibuja todo el estado del juego
        void Draw(Graphics^ g, System::Drawing::Size clientSize) {
            System::Drawing::Size boardPixels(CurrentCols * CELL_SIZE, CurrentRows * CELL_SIZE);

            float scaleX = (float)clientSize.Width / boardPixels.Width;
            float scaleY = (float)clientSize.Height / boardPixels.Height;
            float scale = Math::Min(scaleX, scaleY);

            float offsetX = (clientSize.Width - boardPixels.Width * scale) / 2.0f;
            float offsetY = (clientSize.Height - boardPixels.Height * scale) / 2.0f;

            System::Drawing::Drawing2D::Matrix^ originalTransform = g->Transform;
            g->TranslateTransform(offsetX, offsetY);
            g->ScaleTransform(scale, scale);

            // Fondo verde
            g->FillRectangle(
                gcnew SolidBrush(Color::FromArgb(170, 215, 81)),
                0, 0, boardPixels.Width, boardPixels.Height
            );

            // Paredes y obstáculos
            for each (Wall ^ w in walls) { w->Draw(g); }

            // Manzana
            apple->Draw(g);

            // Serpiente
            for (int i = 0; i < snakeBody->Count; i++) {
                GridPoint p = snakeBody[i];
                Color segColor;
                if (i == 0)          segColor = Color::FromArgb(34, 100, 14);
                else if (i % 2 == 0)      segColor = Color::FromArgb(74, 160, 44);
                else                      segColor = Color::FromArgb(90, 180, 55);

                int px = p.X * CELL_SIZE, py = p.Y * CELL_SIZE;
                g->FillRectangle(gcnew SolidBrush(segColor),
                    px + 1, py + 1, CELL_SIZE - 2, CELL_SIZE - 2);
                if (i == 0) DrawEyes(g, px, py);
            }

            // Restaurar transformación para overlays
            g->Transform = originalTransform;

            if (IsGameOver) DrawGameOverOverlay(g, clientSize);
            else if (IsPaused)   DrawPauseOverlay(g, clientSize);
        }

        List<int>^ SaveAndGetRanking(int finalScore) {
            List<int>^ scores = LoadScores();
            scores->Add(finalScore);
            scores->Sort();
            scores->Reverse();

            System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(
                ScoreConfig::FILE_NAME, false);
            for (int i = 0; i < Math::Min(scores->Count, MAX_RANKING); i++)
                sw->WriteLine(scores[i]);
            sw->Close();

            if (scores->Count > 0) HighScore = scores[0];
            return scores;
        }

        List<int>^ LoadScores() {
            List<int>^ scores = gcnew List<int>();
            if (!System::IO::File::Exists(ScoreConfig::FILE_NAME)) return scores;
            try {
                System::IO::StreamReader^ sr =
                    gcnew System::IO::StreamReader(ScoreConfig::FILE_NAME);
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

        // Agrega obstáculo manualmente (para uso externo si se desea)
        void AddWall(int gridX, int gridY) {
            walls->Add(gcnew Wall(GridPoint(gridX, gridY),
                Color::FromArgb(80, 50, 20), false));
        }

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

        // -------------------------------------------------------
        //  UpdateByScore — replica GameModes::updateByScore()
        //  de modos.cpp usando tipos C++/CLI compatibles con
        //  la ref class SnakeGame.
        // -------------------------------------------------------
        void UpdateByScore() {

            // Modo Obstacles: un obstáculo cada OBSTACLE_SCORE_THRESHOLD puntos
            if (CurrentMode == GameMode::Obstacles) {
                int currentBlock = Score / OBSTACLE_SCORE_THRESHOLD;
                int lastBlock = (lastObstacleScore >= 0)
                    ? lastObstacleScore / OBSTACLE_SCORE_THRESHOLD
                    : -1;
                if (currentBlock > lastBlock) {
                    GenerateObstacle();
                    lastObstacleScore = Score;
                }
            }

            // Modo SpeedIncrease: –10% de intervalo cada SPEED_SCORE_THRESHOLD puntos
            if (CurrentMode == GameMode::SpeedIncrease) {
                int currentBlock = Score / SPEED_SCORE_THRESHOLD;
                int lastBlock = (lastSpeedScore >= 0)
                    ? lastSpeedScore / SPEED_SCORE_THRESHOLD
                    : -1;
                if (currentBlock > lastBlock) {
                    int prevSpeed = SpeedMs;
                    int newSpeed = (int)(SpeedMs * 0.90);
                    SpeedMs = Math::Max(newSpeed, MIN_SPEED_MS);
                    lastSpeedScore = Score;
                    // Notificar al Form1 para que actualice el Timer
                    if (SpeedMs != prevSpeed)
                        OnSpeedChanged(SpeedMs);
                }
            }
        }

        // -------------------------------------------------------
        //  GenerateObstacle — replica GameModes::generateObstacle()
        // -------------------------------------------------------
        void GenerateObstacle() {
            List<GridPoint>^ occupied = GetOccupiedCells();
            for (int attempt = 0; attempt < 100; attempt++) {
                GridPoint candidate(
                    rnd->Next(1, CurrentCols - 1),
                    rnd->Next(1, CurrentRows - 1)
                );
                bool valid = true;
                for each (GridPoint p in occupied) {
                    if (p.Equals(candidate)) { valid = false; break; }
                }
                if (valid) {
                    walls->Add(gcnew Wall(candidate,
                        Color::FromArgb(80, 50, 20), false));
                    return;
                }
            }
            // Si no hay lugar libre, no se genera (igual que modos.cpp)
        }

        void BuildBorderWalls() {
            Color wallCol = Color::FromArgb(101, 67, 33);
            for (int x = 0; x < CurrentCols; x++) {
                walls->Add(gcnew Wall(GridPoint(x, 0), wallCol, true));
                walls->Add(gcnew Wall(GridPoint(x, CurrentRows - 1), wallCol, true));
            }
            for (int y = 1; y < CurrentRows - 1; y++) {
                walls->Add(gcnew Wall(GridPoint(0, y), wallCol, true));
                walls->Add(gcnew Wall(GridPoint(CurrentCols - 1, y), wallCol, true));
            }
        }

        void DrawEyes(Graphics^ g, int px, int py) {
            int ex1, ey1, ex2, ey2;
            if (dx == 1 && dy == 0) {
                ex1 = px + CELL_SIZE - 8; ey1 = py + 4;
                ex2 = px + CELL_SIZE - 8; ey2 = py + CELL_SIZE - 10;
            }
            else if (dx == -1 && dy == 0) {
                ex1 = px + 2; ey1 = py + 4;
                ex2 = px + 2; ey2 = py + CELL_SIZE - 10;
            }
            else if (dx == 0 && dy == -1) {
                ex1 = px + 4;           ey1 = py + 2;
                ex2 = px + CELL_SIZE - 10; ey2 = py + 2;
            }
            else {
                ex1 = px + 4;           ey1 = py + CELL_SIZE - 8;
                ex2 = px + CELL_SIZE - 10; ey2 = py + CELL_SIZE - 8;
            }
            g->FillEllipse(gcnew SolidBrush(Color::White), ex1, ey1, 6, 6);
            g->FillEllipse(gcnew SolidBrush(Color::White), ex2, ey2, 6, 6);
            g->FillEllipse(gcnew SolidBrush(Color::Black), ex1 + 1, ey1 + 1, 4, 4);
            g->FillEllipse(gcnew SolidBrush(Color::Black), ex2 + 1, ey2 + 1, 4, 4);
        }

        void DrawGameOverOverlay(Graphics^ g, System::Drawing::Size clientSize) {
            g->FillRectangle(
                gcnew SolidBrush(Color::FromArgb(160, 0, 0, 0)),
                0, 0, clientSize.Width, clientSize.Height
            );
            System::Drawing::Font^ bigFont =
                gcnew System::Drawing::Font(L"Segoe UI", 32, System::Drawing::FontStyle::Bold);
            System::Drawing::Font^ smFont =
                gcnew System::Drawing::Font(L"Segoe UI", 14);
            StringFormat^ center = gcnew StringFormat();
            center->Alignment = StringAlignment::Center;
            center->LineAlignment = StringAlignment::Center;

            int cx = clientSize.Width / 2, cy = clientSize.Height / 2;
            g->DrawString(L"GAME OVER", bigFont,
                gcnew SolidBrush(Color::Gold), (float)cx, (float)(cy - 50), center);
            g->DrawString(String::Format(L"Puntaje: {0}", Score), smFont,
                gcnew SolidBrush(Color::White), (float)cx, (float)(cy + 10), center);
            g->DrawString(L"R = Reiniciar   M = Menú", smFont,
                gcnew SolidBrush(Color::White), (float)cx, (float)(cy + 50), center);

            delete bigFont; delete smFont; delete center;
        }

        void DrawPauseOverlay(Graphics^ g, System::Drawing::Size clientSize) {
            g->FillRectangle(
                gcnew SolidBrush(Color::FromArgb(120, 0, 0, 0)),
                0, 0, clientSize.Width, clientSize.Height
            );
            System::Drawing::Font^ bigFont =
                gcnew System::Drawing::Font(L"Segoe UI", 36, System::Drawing::FontStyle::Bold);
            System::Drawing::Font^ smFont =
                gcnew System::Drawing::Font(L"Segoe UI", 14);
            StringFormat^ center = gcnew StringFormat();
            center->Alignment = StringAlignment::Center;
            center->LineAlignment = StringAlignment::Center;

            int cx = clientSize.Width / 2, cy = clientSize.Height / 2;
            g->DrawString(L"PAUSA", bigFont,
                gcnew SolidBrush(Color::White), (float)cx, (float)(cy - 30), center);
            g->DrawString(L"P / ESC = Continuar", smFont,
                gcnew SolidBrush(Color::LightGray), (float)cx, (float)(cy + 20), center);

            delete bigFont; delete smFont; delete center;
        }
    };

} // namespace MiProyecto