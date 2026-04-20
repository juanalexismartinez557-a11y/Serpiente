#pragma once

#include <deque>
#include <vector>
#include <random>
#include <stdexcept>

struct Point { // posicion en el tablero
    int x, y;

    // Comparación para validar colisiones
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum class GameMode { // modos de juegos:
    Normal,        // Sin modificadores especiales
    Obstacles,     // Genera obstáculos en el mapa
    RandomGrowth,  // Crecimiento aleatorio al comer
    SpeedIncrease  // Incrementa velocidad con el puntaje
};

class GameModes {
public:

    GameModes(
        std::deque<Point>& snake, // referencia a la deque que representa la serpiente
        std::vector<Point>& obstacles, //referencia al vector de obstáculos en el mapa
        int& timerInterval, //referencia al intervalo del timer en ms (velocidad)
        int boardWidth, // ancho del tablero en celdas
        int boardHeight, // alto del tablero en celdas
        Point& foodPos //referencia a la posición actual de la comida
    );

    void applyMode(GameMode mode); // se manda llamar al modo de juego

    int handleFoodEaten(GameMode mode); // se llama cuando la serpiente come, indica cuanto debe crecer.

    void updateByScore(GameMode mode, int score); // mide los puntos y que debe pasar si se obtiene esa cantidad de puntos

    bool isPositionValid(Point p) const; // verifica que la posicion no este ocupada (serpiente, comida, obstaculo)

    int getRandomGrowthAmount() const; // devuelve cuanto crecera

    void clearObstacles(); // quita los obstaculos

    int getTimerInterval() const; // devuelve el timer

private:

    // Referencias a los datos del juego (sin copia)
    std::deque<Point>& m_snake;
    std::vector<Point>& m_obstacles;
    int& m_timerInterval;
    int &m_boardWidth;
    int &m_boardHeight;
    Point& m_foodPos;

    // Generador de números aleatorios (Mersenne Twister)
    mutable std::mt19937 m_rng;

    // Control interno para evitar efectos repetidos
    int m_lastObstacleScore;   // Puntaje en el que se generaron obstáculos por última vez
    int m_lastSpeedScore;      // Puntaje en el que se aumentó la velocidad por última vez

    void generateObstacle(); // genera un obstaculo eb un punto valido

    Point randomPoint() const; // genera un punto aleatorio en el tablero.
};