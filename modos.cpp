#include "modos.h"
#include <algorithm> // para std::max
#include <stdexcept>
#include <chrono>


GameModes::GameModes(
    std::deque<Point>& snake,
    std::vector<Point>& obstacles,
    int& timerInterval,
    int boardWidth,
    int boardHeight,
    Point& foodPos
)
    : m_snake(snake),
    m_obstacles(obstacles),
    m_timerInterval(timerInterval),
    m_boardWidth(boardWidth),
    m_boardHeight(boardHeight),
    m_foodPos(foodPos),
    m_lastObstacleScore(-1),
    m_lastSpeedScore(-1)
{
    // random 
    auto seed = static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()
        );
    m_rng.seed(seed);
}

void GameModes::applyMode(GameMode mode) {
    switch (mode) {
    case GameMode::Normal:
        // Sin modificadores
        break;

    case GameMode::Obstacles:
        // La generación de obstáculos se maneja en updateByScore
        break;

    case GameMode::RandomGrowth:
        // El crecimiento se maneja en handleFoodEaten
        break;

    case GameMode::SpeedIncrease:
        // El aumento de velocidad se maneja en updateByScore
        break;
    }
}

int GameModes::handleFoodEaten(GameMode mode) {
    switch (mode) {
    case GameMode::RandomGrowth:
        // Retorna entre 1 y 5 segmentos extra
        return getRandomGrowthAmount();

    case GameMode::Normal:
    case GameMode::Obstacles:
    case GameMode::SpeedIncrease:
    default:
        // Crecimiento normal: 0 extra (solo crece 1 por defecto)
        return 0;
    }
}

void GameModes::updateByScore(GameMode mode, int score) {

    // Modo Obstacles: genera un nuevo obstáculo cada 5 puntos
    if (mode == GameMode::Obstacles) {
        // Umbral configurable: un obstáculo cada 5 puntos
        const int OBSTACLE_THRESHOLD = 5;

        // Calcula en qué "bloque de 5" estamos
        int currentBlock = score / OBSTACLE_THRESHOLD;
        int lastBlock = m_lastObstacleScore / OBSTACLE_THRESHOLD;

        if (currentBlock > lastBlock || m_lastObstacleScore < 0) {
            generateObstacle();
            m_lastObstacleScore = score;
        }
    }

    // Modo SpeedIncrease: aumenta velocidad cada 10 puntos
    // El intervalo del timer BAJA un 10% (más rápido).
    // Intervalo mínimo: 50 ms para evitar velocidad imposible.
    if (mode == GameMode::SpeedIncrease) {
        const int SPEED_THRESHOLD = 10;
        const int MIN_INTERVAL = 50;  // ms mínimos entre ticks

        int currentBlock = score / SPEED_THRESHOLD;
        int lastBlock = m_lastSpeedScore / SPEED_THRESHOLD;

        if (currentBlock > lastBlock || m_lastSpeedScore < 0) {
            // Reducir 10% del intervalo actual
            int newInterval = static_cast<int>(m_timerInterval * 0.90);

            // Respetar el mínimo para que el juego siga siendo jugable
            m_timerInterval = std::max(newInterval, MIN_INTERVAL);
            m_lastSpeedScore = score;
        }
    }
}

bool GameModes::isPositionValid(Point p) const {

    // Verificar contra cada segmento de la serpiente
    for (const auto& seg : m_snake) {
        if (seg == p) return false;
    }

    // Verificar contra cada obstáculo existente
    for (const auto& obs : m_obstacles) {
        if (obs == p) return false;
    }

    // Verificar contra la posición de la comida
    if (m_foodPos == p) return false;

    return true;
}

int GameModes::getRandomGrowthAmount() const {
    std::uniform_int_distribution<int> dist(1, 5);
    return dist(m_rng);
}

//  clearObstacles — Elimina todos los obstáculos del mapa
void GameModes::clearObstacles() {
    m_obstacles.clear();
    m_lastObstacleScore = -1;
}

//  getTimerInterval — Retorna el intervalo actual del timer
int GameModes::getTimerInterval() const {
    return m_timerInterval;
}

//  generateObstacle 
void GameModes::generateObstacle() {
    const int MAX_ATTEMPTS = 100;//  Intenta hasta 100 veces encontrar una celda libre para colocar un nuevo obstáculo. Si no lo logra, no hace nada

    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        Point candidate = randomPoint();

        if (isPositionValid(candidate)) {
            m_obstacles.push_back(candidate);
            return; // Obstáculo colocado exitosamente
        }
    }
    // Si no se encontró posición válida, se ignora silenciosamente
}

//  randomPoint 
//  Genera una posición aleatoria dentro de los límites del
//  tablero (excluyendo el borde para no bloquear el camino).
// ============================================================
Point GameModes::randomPoint() const {
    // Margen de 1 celda en cada borde para no generar obstáculos
    // pegados a las paredes (opcional: ajustar a 0 si no hay borde)
    const int MARGIN = 1;

    std::uniform_int_distribution<int> distX(MARGIN, m_boardWidth - 1 - MARGIN);
    std::uniform_int_distribution<int> distY(MARGIN, m_boardHeight - 1 - MARGIN);

    return { distX(m_rng), distY(m_rng) };
}