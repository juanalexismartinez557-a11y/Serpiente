#pragma once
// ============================================================
//  modos.h  —  Gestor de modos de juego (C++ nativo)
//
//  NOTA DE INTEGRACIÓN:
//  Esta clase usa std::deque / std::vector (C++ nativo) y no
//  puede conectarse directamente a SnakeGame (C++/CLI ref class)
//  sin un adaptador. Su lógica ha sido reimplementada dentro de
//  SnakeGame usando tipos administrados (List<GridPoint>^), en
//  los métodos UpdateByScore() y GenerateObstacle().
//
//  Este archivo se mantiene tal cual porque:
//   - Es correcto y compila de forma independiente.
//   - Sirve como referencia de diseño del sistema de modos.
//   - Puede usarse en futuras versiones nativas del proyecto.
// ============================================================

#include <deque>
#include <vector>
#include <random>
#include <stdexcept>

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum class GameMode {
    Normal,        // Sin modificadores especiales
    Obstacles,     // Genera obstáculos en el mapa
    RandomGrowth,  // Crecimiento aleatorio al comer
    SpeedIncrease  // Incrementa velocidad con el puntaje
};

class GameModes {
public:
    GameModes(
        std::deque<Point>& snake,
        std::vector<Point>& obstacles,
        int& timerInterval,
        int boardWidth,
        int boardHeight,
        Point& foodPos
    );

    void applyMode(GameMode mode);
    int  handleFoodEaten(GameMode mode);
    void updateByScore(GameMode mode, int score);
    bool isPositionValid(Point p) const;
    int  getRandomGrowthAmount() const;
    void clearObstacles();
    int  getTimerInterval() const;

private:
    std::deque<Point>& m_snake;
    std::vector<Point>& m_obstacles;
    int& m_timerInterval;
    int                 m_boardWidth;
    int                 m_boardHeight;
    Point& m_foodPos;

    mutable std::mt19937 m_rng;

    int m_lastObstacleScore;
    int m_lastSpeedScore;

    void  generateObstacle();
    Point randomPoint() const;
};