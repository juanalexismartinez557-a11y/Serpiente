/**
 * @file snake_doxygen.h
 * @brief Documentación Doxygen completa del proyecto Snake Evolution
 * @author Equipo de Desarrollo
 * @version 1.0
 * @date 2025
 *
 * @mainpage Snake Evolution — Documentación del Proyecto
 *
 * @section intro_sec Descripción General
 * **Snake Evolution** es un videojuego de serpiente desarrollado en **C++/CLI** con
 * Windows Forms (.NET). Amplía el clásico Snake con modos de juego combinables,
 * tabla de clasificación persistente, tienda de skins desbloqueables y soporte
 * para dos jugadores simultáneos.
 *
 * @section tech_sec Tecnologías
 * - **Lenguaje**: C++/CLI (Mixed-mode: nativo + administrado .NET)
 * - **Framework**: Windows Forms / .NET
 * - **Persistencia**: Archivo CSV (`snake_scores.csv`)
 * - **IDE recomendado**: Visual Studio 2019/2022
 *
 * @section arch_sec Arquitectura de Archivos
 * | Archivo            | Tipo         | Responsabilidad                            |
 * |--------------------|--------------|---------------------------------------------|
 * | `main.cpp`         | Punto de entrada | Inicializa la aplicación WinForms         |
 * | `Snake.h`          | Motor del juego  | Lógica de serpiente, manzana, pared, grid |
 * | `Form.h`           | UI principal     | Menú, bucle de juego, HUD, teclado        |
 * | `ScoreManager.h/cpp` | Persistencia   | Guardar/leer/ordenar puntuaciones         |
 * | `LeaderboardForm.h`| UI secundaria   | Top-10 en DataGridView                    |
 * | `formtienda.h`     | UI secundaria   | Tienda de skins desbloqueables            |
 * | `modos.h / modos.cpp` | Referencia nativa | Diseño original de modos (C++ puro) |
 *
 * @section flow_sec Flujo de la Aplicación
 * @code
 * main() → Application::Run(Form1)
 *   → Menú → [Iniciar] → SnakeGame::Reset() → Timer::Start()
 *     → Timer::Tick → SnakeGame::Tick() → Panel::Invalidate()
 *       → Panel::Paint → SnakeGame::Draw()
 *     → SnakeGame::OnGameOver → ScoreManager::SaveScore()
 *   → [Leaderboard] → LeaderboardForm
 *   → [Tienda]      → FormTienda
 * @endcode
 *
 * @section controls_sec Controles
 * | Tecla          | Acción                              |
 * |----------------|-------------------------------------|
 * | Flechas        | Mover Jugador 1                     |
 * | W / A / S / D  | Mover Jugador 2 (modo 2P)           |
 * | P              | Pausar / Reanudar                   |
 * | R              | Reiniciar partida                   |
 * | M              | Regresar al menú (tras Game Over)   |
 * | L              | Abrir Leaderboard (tras Game Over)  |
 */

// ===========================================================
//  main.cpp
// ===========================================================

/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicación Snake Evolution.
 *
 * Configura el hilo STA requerido por Windows Forms, habilita
 * los estilos visuales del sistema operativo y lanza la ventana
 * principal (Form1).
 */

/**
 * @fn int main(array<String^>^ args)
 * @brief Función principal de la aplicación.
 * @param args Argumentos de línea de comandos (no utilizados).
 * @return 0 al cerrar la aplicación correctamente.
 *
 * Secuencia de inicialización:
 * 1. `Application::EnableVisualStyles()` — activa el tema nativo de Windows.
 * 2. `Application::SetCompatibleTextRenderingDefault(false)` — usa GDI+ para texto.
 * 3. `Application::Run(gcnew Form1())` — inicia el bucle de mensajes.
 */


// ===========================================================
//  Snake.h — Constantes y tipos base
// ===========================================================

/**
 * @file Snake.h
 * @brief Motor del juego Snake: constantes, entidades y clase principal SnakeGame.
 *
 * Contiene todas las primitivas del juego:
 * - Constantes globales de velocidad, tamaño de celda y umbrales.
 * - Los tipos `GridPoint`, `Wall`, `Apple` y `SnakeGame`.
 * - El enum `GameMode` (combinable por flags) y `BoardSize`.
 */

/**
 * @var CELL_SIZE
 * @brief Tamaño en píxeles de cada celda de la cuadrícula. Valor: 30 px.
 */

/**
 * @var SNAKE_SPEED_MS
 * @brief Intervalo base del temporizador en milisegundos. Valor: 150 ms.
 */

/**
 * @var APPLE_GROW_AMOUNT
 * @brief Número de segmentos que crece la serpiente al comer una manzana (Normal). Valor: 1.
 */

/**
 * @var SCORE_PER_APPLE
 * @brief Puntos que otorga cada manzana consumida. Valor: 10.
 */

/**
 * @var MIN_SPEED_MS
 * @brief Velocidad máxima permitida (intervalo mínimo). Valor: 50 ms.
 */

/**
 * @var OBSTACLE_SCORE_THRESHOLD
 * @brief Puntos entre cada generación de obstáculo en modo Obstacles. Valor: 5.
 */

/**
 * @var SPEED_SCORE_THRESHOLD
 * @brief Puntos entre cada incremento de velocidad en modo SpeedIncrease. Valor: 10.
 */

// ===========================================================
//  namespace MiProyecto
// ===========================================================

/**
 * @namespace MiProyecto
 * @brief Espacio de nombres raíz que agrupa todas las clases del proyecto.
 */
namespace MiProyecto {

    // -------------------------------------------------------
    //  ScoreConfig
    // -------------------------------------------------------

    /**
     * @class ScoreConfig
     * @brief Clase de configuración de rutas de archivos de puntuación.
     *
     * Expone el nombre del archivo CSV mediante una constante literal,
     * evitando "magic strings" dispersas en el código.
     */
    ref class ScoreConfig {
    public:
        /**
         * @brief Nombre del archivo CSV donde se persisten las puntuaciones.
         * @note Valor: "snake_scores.csv"
         */
        literal System::String^ FILE_NAME = "snake_scores.csv";
    };

    // -------------------------------------------------------
    //  GameMode
    // -------------------------------------------------------

    /**
     * @enum GameMode
     * @brief Modos de juego combinables mediante operador OR a nivel de bits.
     *
     * Al declararse con `[FlagsAttribute]`, varios modos pueden estar
     * activos simultáneamente. Ejemplo:
     * @code
     * GameMode modo = GameMode::Obstacles | GameMode::SpeedIncrease;
     * @endcode
     *
     * | Valor          | Bit | Efecto                                              |
     * |----------------|-----|-----------------------------------------------------|
     * | Normal         | 0   | Sin modificadores                                   |
     * | Obstacles      | 1   | Genera obstáculos cada OBSTACLE_SCORE_THRESHOLD pts |
     * | RandomGrowth   | 2   | Crecimiento aleatorio (1–5) al comer una manzana    |
     * | SpeedIncrease  | 4   | Velocidad aumenta cada SPEED_SCORE_THRESHOLD pts    |
     */
    public enum class GameMode {
        Normal        = 0, ///< Sin modificadores especiales.
        Obstacles     = 1, ///< Genera obstáculos dinámicamente.
        RandomGrowth  = 2, ///< Crecimiento aleatorio al comer.
        SpeedIncrease = 4  ///< Incrementa velocidad con el puntaje.
    };

    // -------------------------------------------------------
    //  BoardSize
    // -------------------------------------------------------

    /**
     * @enum BoardSize
     * @brief Tamaños de tablero disponibles en el menú.
     *
     * | Valor  | Celdas    |
     * |--------|-----------|
     * | Small  | 10 × 10   |
     * | Medium | 25 × 25   |
     * | Large  | 50 × 50   |
     */
    enum class BoardSize { Small, Medium, Large };

    // -------------------------------------------------------
    //  GridPoint
    // -------------------------------------------------------

    /**
     * @struct GridPoint
     * @brief Coordenada discreta en la cuadrícula del tablero.
     * @details Estructura de valor (stack-allocated) que representa una posición
     *          (columna, fila) dentro del tablero de juego.
     */
    value struct GridPoint {
        int X; ///< Columna (eje horizontal).
        int Y; ///< Fila    (eje vertical).

        /**
         * @brief Constructor con coordenadas explícitas.
         * @param x Columna.
         * @param y Fila.
         */
        GridPoint(int x, int y);

        /**
         * @brief Comprueba igualdad con otro GridPoint.
         * @param other Punto a comparar.
         * @return true si X e Y coinciden.
         */
        bool Equals(GridPoint other);
    };

    // -------------------------------------------------------
    //  Wall
    // -------------------------------------------------------

    /**
     * @class Wall
     * @brief Representa una celda bloqueada en el tablero (borde o obstáculo).
     *
     * Las paredes de borde se crean en `SnakeGame::BuildBorderWalls()` y los
     * obstáculos dinámicos en `SnakeGame::GenerateObstacle()`.
     */
    ref class Wall {
    public:
        GridPoint Position; ///< Posición en la cuadrícula.
        Color     WallColor;///< Color de relleno.
        bool      IsBorder; ///< true si pertenece al borde del tablero.

        /** @brief Dibuja la celda de pared en el contexto gráfico dado. */
        void Draw(Graphics^ g);

        /**
         * @brief Verifica si la posición p colisiona con esta pared.
         * @param p Punto a verificar.
         * @return true si p coincide con Position.
         */
        bool Collides(GridPoint p);
    };

    // -------------------------------------------------------
    //  Apple
    // -------------------------------------------------------

    /**
     * @class Apple
     * @brief Manzana que la serpiente debe consumir para crecer y ganar puntos.
     *
     * Se regenera en una posición libre del tablero cada vez que es comida.
     */
    ref class Apple {
    public:
        GridPoint Position; ///< Posición actual de la manzana.

        /**
         * @brief Genera una nueva posición libre para la manzana.
         * @param rnd          Instancia Random compartida.
         * @param cols         Número de columnas del tablero.
         * @param rows         Número de filas del tablero.
         * @param occupiedCells Lista de celdas ya ocupadas (serpiente + paredes).
         * @note Realiza hasta 100 intentos aleatorios. Si fracasa, posiciona
         *       en una celda aleatoria sin validar.
         */
        void Generate(Random^ rnd, int cols, int rows, List<GridPoint>^ occupiedCells);

        /** @brief Dibuja la manzana como una elipse roja con brillo especular. */
        void Draw(Graphics^ g);
    };

    // -------------------------------------------------------
    //  SnakeGame
    // -------------------------------------------------------

    /**
     * @class SnakeGame
     * @brief Motor principal del juego Snake.
     *
     * Encapsula la lógica completa de una instancia de juego:
     * movimiento de la serpiente, detección de colisiones, sistema de modos
     * combinables, renderizado escalado y gestión de puntuaciones.
     *
     * Se instancia una vez por jugador (`game1`, `game2` en Form1).
     *
     * @par Eventos
     * | Evento           | Cuándo se dispara                          |
     * |------------------|--------------------------------------------|
     * | OnGameOver       | Al colisionar con pared o con sí misma     |
     * | OnScoreChanged   | Al consumir una manzana                    |
     * | OnSpeedChanged   | Al incrementar velocidad (modo SpeedIncrease)|
     */
    ref class SnakeGame {
    public:
        // ── Propiedades públicas ─────────────────────────────────

        bool WallPassThrough; ///< Si true, la serpiente atraviesa bordes.
        int  SpeedMs;         ///< Intervalo actual del temporizador (ms).
        int  GrowAmount;      ///< Segmentos a crecer en modo Normal.

        bool HasStarted;  ///< true tras el primer movimiento.
        bool IsGameOver;  ///< true cuando la partida ha terminado.
        bool IsPaused;    ///< true cuando la partida está en pausa.
        int  Score;       ///< Puntaje acumulado en la partida actual.
        int  ApplesEaten; ///< Manzanas consumidas en la partida.
        int  HighScore;   ///< Máxima puntuación histórica cargada del CSV.
        int  CurrentCols; ///< Columnas activas del tablero.
        int  CurrentRows; ///< Filas activas del tablero.
        GameMode CurrentMode; ///< Modo(s) de juego activo(s) (flags combinados).

        Color SnakePrimaryColor;   ///< Color de los segmentos pares.
        Color SnakeSecondaryColor; ///< Color de los segmentos impares.
        Color SnakeHeadColor;      ///< Color de la cabeza.

        // ── Eventos ─────────────────────────────────────────────

        /** @brief Delegado emitido cuando la partida termina. */
        event GameOverDelegate^ OnGameOver;

        /** @brief Delegado emitido cuando cambia la puntuación. */
        event ScoreChangedDelegate^ OnScoreChanged;

        /** @brief Delegado emitido cuando cambia la velocidad. */
        event SpeedChangedDelegate^ OnSpeedChanged;

        // ── Métodos públicos ─────────────────────────────────────

        /** @brief Constructor: inicializa estado y carga el HighScore del CSV. */
        SnakeGame();

        /**
         * @brief Cambia los colores de la serpiente (skin).
         * @param primary   Color de segmentos pares.
         * @param secondary Color de segmentos impares.
         * @param head      Color de la cabeza.
         */
        void SetSnakeSkin(Color primary, Color secondary, Color head);

        /**
         * @brief Configura el tamaño del tablero y reinicia la partida.
         * @param size Enum BoardSize (Small/Medium/Large).
         */
        void SetBoardSize(BoardSize size);

        /**
         * @brief Aplica un modo de juego (o combinación de flags).
         * @param mode Combinación de valores GameMode.
         * @note Ajusta SpeedMs base si SpeedIncrease está activo.
         */
        void SetGameMode(GameMode mode);

        /**
         * @brief Reinicia el estado completo del juego.
         *
         * Limpia serpiente, paredes y obstáculos. Posiciona la serpiente
         * en el centro del tablero y genera la primera manzana.
         */
        void Reset();

        /**
         * @brief Encola un cambio de dirección.
         * @param newDx Componente X de la nueva dirección (-1, 0 o 1).
         * @param newDy Componente Y de la nueva dirección (-1, 0 o 1).
         * @note No permite inversión de 180° inmediata.
         */
        void SetDirection(int newDx, int newDy);

        /**
         * @brief Avanza un tick lógico del juego.
         *
         * Secuencia por tick:
         * 1. Aplica la dirección pendiente.
         * 2. Calcula la nueva posición de la cabeza.
         * 3. Verifica colisión con paredes o cuerpo propio.
         * 4. Verifica colisión con la manzana.
         * 5. Llama a `UpdateByScore()` si se comió una manzana.
         */
        void Tick();

        /**
         * @brief Renderiza el estado completo del juego.
         * @param g          Contexto GDI+ del evento Paint.
         * @param clientSize Tamaño en píxeles del panel contenedor.
         *
         * Aplica una transformación de escala uniforme para que el tablero
         * ocupe el máximo espacio disponible sin distorsión.
         * Dibuja: fondo, paredes, manzana, serpiente, ojos de la cabeza.
         * Si la partida terminó o está pausada, superpone el overlay correspondiente.
         */
        void Draw(Graphics^ g, System::Drawing::Size clientSize);

        /** @brief Alterna el estado de pausa. */
        void TogglePause();

        /** @brief Retorna la longitud actual de la serpiente. */
        int GetSnakeLength();

        /**
         * @brief Agrega un obstáculo manual en la posición (gridX, gridY).
         * @param gridX Columna de la cuadrícula.
         * @param gridY Fila de la cuadrícula.
         */
        void AddWall(int gridX, int gridY);

    private:
        /**
         * @brief Genera una celda de obstáculo aleatoria y libre.
         * @details Realiza hasta 100 intentos para encontrar una posición
         *          no ocupada por la serpiente, las paredes existentes o la manzana.
         */
        void GenerateObstacle();

        /**
         * @brief Construye las paredes de borde del tablero.
         *
         * Crea Wall con `IsBorder = true` en todo el perímetro
         * (fila 0, fila N-1, columna 0, columna N-1).
         */
        void BuildBorderWalls();

        /**
         * @brief Actualiza obstáculos y velocidad según el puntaje actual.
         *
         * Llamado tras cada manzana consumida. Comprueba si el puntaje
         * cruzó un umbral nuevo para cada modo activo:
         * - Obstacles → `GenerateObstacle()`
         * - SpeedIncrease → `SpeedMs *= 0.90` (mín. MIN_SPEED_MS) + evento
         */
        void UpdateByScore();

        /**
         * @brief Activa el estado de Game Over y emite el evento OnGameOver.
         * @note También llama a `SaveAndGetRanking()` para actualizar el CSV.
         */
        void TriggerGameOver();

        /** @brief Retorna la lista unificada de celdas ocupadas (cuerpo + paredes). */
        List<GridPoint>^ GetOccupiedCells();

        /**
         * @brief Dibuja los ojos de la cabeza de la serpiente.
         * @param g  Contexto gráfico.
         * @param px Coordenada X en píxeles de la celda de la cabeza.
         * @param py Coordenada Y en píxeles de la celda de la cabeza.
         */
        void DrawEyes(Graphics^ g, int px, int py);

        /** @brief Dibuja el overlay semitransparente de Game Over. */
        void DrawGameOverOverlay(Graphics^ g, System::Drawing::Size clientSize);

        /** @brief Dibuja el overlay semitransparente de Pausa. */
        void DrawPauseOverlay(Graphics^ g, System::Drawing::Size clientSize);

        /**
         * @brief Guarda la puntuación final y devuelve el ranking ordenado.
         * @param finalScore Puntaje al finalizar la partida.
         * @return Lista de puntuaciones ordenada de mayor a menor.
         */
        List<int>^ SaveAndGetRanking(int finalScore);

        /**
         * @brief Lee las puntuaciones del CSV y devuelve solo los valores enteros.
         * @return Lista de enteros extraídos del CSV (compatible con formato antiguo).
         */
        List<int>^ LoadScores();

        /** @brief Carga el HighScore desde el CSV al iniciar la instancia. */
        void LoadHighScore();

        /** @brief Comprueba si el flag `flag` está activo en CurrentMode. */
        bool HasMode(GameMode flag);
    };

    // ===========================================================
    //  Form.h — Form1 (Formulario principal)
    // ===========================================================

    /**
     * @file Form.h
     * @brief Formulario principal de Snake Evolution.
     *
     * Aloja el menú de inicio, los paneles de juego, el HUD de puntuación
     * y gestiona el ciclo de vida de las instancias SnakeGame.
     */

    /**
     * @class Form1
     * @brief Ventana principal de la aplicación.
     *
     * Responsabilidades:
     * - Mostrar el menú de configuración (modo, tamaño, jugadores).
     * - Crear y coordinar hasta dos instancias de SnakeGame.
     * - Manejar el Timer global que avanza los ticks del juego.
     * - Capturar eventos de teclado y distribuirlos a cada juego.
     * - Guardar puntuaciones al final de cada partida.
     * - Abrir formularios secundarios (Leaderboard, Tienda).
     */
    public ref class Form1 : public System::Windows::Forms::Form {
    public:
        /**
         * @brief Aplica una skin seleccionada desde la Tienda a ambos juegos activos.
         * @param primary   Color principal de segmentos.
         * @param secondary Color secundario de segmentos.
         * @param head      Color de la cabeza.
         */
        void SetSelectedSkin(Color primary, Color secondary, Color head);

    private:
        /**
         * @brief Manejador del botón "Iniciar Juego".
         *
         * Lee la configuración del menú, crea las instancias SnakeGame,
         * suscribe los eventos y arranca el Timer global.
         */
        void IniciarJuego(Object^ sender, EventArgs^ e);

        /**
         * @brief Crea y configura una instancia SnakeGame con los parámetros actuales.
         * @param mode Modos de juego a aplicar.
         * @return Instancia configurada lista para jugar.
         */
        SnakeGame^ CreateGame(GameMode mode);

        /**
         * @brief Tick del Timer global: avanza ambos juegos y solicita repintado.
         *
         * Detiene el Timer si ambos jugadores están en Game Over.
         */
        void GameTimer_Tick(Object^ sender, EventArgs^ e);

        /**
         * @brief Manejador de teclado global.
         *
         * Distribuye flechas → J1, WASD → J2 y procesa teclas globales:
         * P (pausa), R (reinicio), M (menú), L (leaderboard).
         */
        void Form1_KeyDown(Object^ sender, KeyEventArgs^ e);

        /**
         * @brief Comprueba si ambos jugadores han terminado y muestra el resultado.
         *
         * En modo 2P muestra el ganador, luego pide username a cada jugador
         * y guarda los puntajes vía ScoreManager.
         */
        void CheckBothDead();

        /**
         * @brief Muestra un diálogo modal para que el jugador ingrese su nombre.
         * @param playerLabel Etiqueta del jugador ("Jugador 1" o "Jugador 2").
         * @return El nombre ingresado, o el valor por defecto si se omitió.
         */
        String^ PromptUsernameFor(String^ playerLabel);

        /**
         * @brief Guarda la puntuación en el CSV usando ScoreManager.
         * @param username Nombre del jugador.
         * @param score    Puntaje final.
         * @param g        Instancia SnakeGame para leer tablero y modo.
         */
        void SaveScore(String^ username, int score, SnakeGame^ g);

        /**
         * @brief Convierte un valor GameMode en su etiqueta textual para el CSV.
         * @param mode Modo activo.
         * @return Cadena como "Normal", "Obstaculos+SpeedIncrease", etc.
         */
        String^ GetGameModeLabel(GameMode mode);

        /**
         * @brief Abre el formulario de Tienda y pausa los juegos activos.
         */
        void AbrirTienda(Object^ sender, EventArgs^ e);

        /**
         * @brief Abre el formulario de Leaderboard como diálogo modal.
         */
        void AbrirLeaderboard(Object^ sender, EventArgs^ e);

        /**
         * @brief Reposiciona controles del menú al centro del panel al redimensionar.
         */
        void CenterMenuControls();

        /**
         * @brief Redistribuye los paneles de juego al redimensionar la ventana.
         *
         * En modo 2P divide el ancho disponible en dos mitades iguales.
         */
        void Form1_Resize(Object^ sender, EventArgs^ e);
    };


    // ===========================================================
    //  ScoreManager.h / ScoreManager.cpp
    // ===========================================================

    /**
     * @file ScoreManager.h
     * @brief Declaración de ScoreEntry y ScoreManager.
     *
     * @file ScoreManager.cpp
     * @brief Implementación de ScoreManager: E/S de CSV, parsing y ordenamiento.
     */

    /**
     * @class ScoreEntry
     * @brief Modelo de datos para una entrada del leaderboard.
     *
     * Cada línea del CSV se deserializa en un ScoreEntry.
     */
    public ref class ScoreEntry {
    public:
        String^ Username;  ///< Nombre del jugador (máx. 24 caracteres).
        int     Score;     ///< Puntuación de esta partida.
        String^ BoardSize; ///< Dimensiones del tablero (ej. "25x25").
        String^ GameMode;  ///< Modo(s) jugados (ej. "Obstaculos+SpeedIncrease").
        int     BestScore; ///< Mejor puntuación histórica del jugador (calculado).

        /** @brief Constructor: inicializa todos los campos a sus valores por defecto. */
        ScoreEntry();
    };

    /**
     * @class ScoreManager
     * @brief Clase estática sellada para persistencia de puntuaciones en CSV.
     *
     * No puede instanciarse (`abstract sealed`). Todos sus métodos son estáticos.
     *
     * @par Formato del CSV
     * @code
     * Username,Score,BoardSize,GameMode
     * Jugador1,150,25x25,Normal
     * Jugador2,300,50x50,Obstaculos+SpeedIncrease
     * @endcode
     *
     * @par Compatibilidad Legacy
     * Si una línea contiene únicamente un entero (formato antiguo), se crea
     * un ScoreEntry con Username = "Legacy" y campos BoardSize/GameMode = "N/A".
     */
    public ref class ScoreManager abstract sealed {
    public:
        /**
         * @brief Agrega una puntuación al archivo CSV.
         * @param username  Nombre del jugador.
         * @param score     Puntaje final.
         * @param boardSize Tamaño del tablero como cadena (ej. "25x25").
         * @param gameMode  Modo de juego como cadena.
         * @note Si el archivo no existe, lo crea con cabecera. Error silencioso.
         */
        static void SaveScore(String^ username, int score, String^ boardSize, String^ gameMode);

        /**
         * @brief Carga todas las entradas del CSV y completa BestScore por jugador.
         * @return Lista de ScoreEntry con todos los registros (sin ordenar).
         */
        static List<ScoreEntry^>^ LoadScores();

        /**
         * @brief Retorna las N mejores puntuaciones ordenadas descendentemente.
         * @param n Número de entradas a retornar (0 o negativo → todas).
         * @return Lista de hasta N ScoreEntry ordenados por Score desc.
         */
        static List<ScoreEntry^>^ GetTopScores(int n);

        /**
         * @brief Calcula el mejor puntaje por jugador.
         * @return Diccionario [Username → MejorPuntaje] (case-insensitive).
         */
        static Dictionary<String^, int>^ GetBestScorePerPlayer();

        /**
         * @brief Retorna la puntuación más alta de todos los registros.
         * @return Entero con el puntaje máximo, o 0 si no hay registros.
         */
        static int GetHighestScore();

    private:
        /**
         * @brief Comparador descendente por Score para `List::Sort`.
         * @param left  Entrada izquierda.
         * @param right Entrada derecha.
         * @return Negativo si left > right, 0 si iguales, positivo si left < right.
         */
        static int CompareByScoreDesc(ScoreEntry^ left, ScoreEntry^ right);

        /** @brief Retorna la ruta del archivo CSV (ScoreConfig::FILE_NAME). */
        static String^ GetScoreFilePath();

        /**
         * @brief Garantiza que el CSV existe y tiene cabecera válida.
         *
         * Si no existe, lo crea. Si existe sin cabecera, la inserta al inicio.
         */
        static void EnsureFileHasHeader();

        /**
         * @brief Parsea una línea del CSV en un ScoreEntry.
         * @param line Línea de texto a parsear.
         * @return ScoreEntry válido, o nullptr si la línea es inválida/cabecera.
         * @note Soporta formato legacy (solo entero) y formato completo (4 campos).
         */
        static ScoreEntry^ ParseScoreLine(String^ line);

        /**
         * @brief Sanitiza un campo CSV eliminando comas y saltos de línea.
         * @param value Campo a sanitizar.
         * @return Cadena limpia, o "N/A" si estaba vacía.
         */
        static String^ SanitizeCsvField(String^ value);

        /**
         * @brief Normaliza un nombre de usuario.
         * @param username Nombre a normalizar.
         * @return Nombre recortado a 24 caracteres, o "Jugador" si estaba vacío.
         */
        static String^ NormalizeUsername(String^ username);
    };


    // ===========================================================
    //  LeaderboardForm.h
    // ===========================================================

    /**
     * @file LeaderboardForm.h
     * @brief Formulario de Leaderboard — Top 10 de puntuaciones.
     */

    /**
     * @class LeaderboardForm
     * @brief Ventana modal que muestra las 10 mejores puntuaciones.
     *
     * Columnas del DataGridView:
     * | # | USERNAME | SCORE | BEST | BOARD | MODE |
     *
     * Se abre desde Form1 al pulsar el botón "Leaderboard" o la tecla L.
     */
    public ref class LeaderboardForm : public Form {
    public:
        /** @brief Constructor: inicializa la UI y carga el leaderboard. */
        LeaderboardForm();

    private:
        /**
         * @brief Llama a ScoreManager::GetTopScores(10) y puebla el DataGridView.
         *
         * Si no hay registros, muestra la fila "Sin registros".
         */
        void LoadLeaderboard();

        /** @brief Manejador del botón "X CERRAR" → cierra el formulario. */
        void CloseForm(Object^ sender, EventArgs^ e);
    };


    // ===========================================================
    //  formtienda.h
    // ===========================================================

    /**
     * @file formtienda.h
     * @brief Formulario de Tienda — Desbloqueo y equipamiento de skins.
     */

    /**
     * @class FormTienda
     * @brief Formulario que permite desbloquear y equipar skins para la serpiente.
     *
     * Muestra 6 skins con su color representativo y el puntaje mínimo requerido.
     * Las skins bloqueadas muestran "Nopitino" en gris; las disponibles muestran
     * "Equipar" en azul.
     *
     * @par Tabla de Skins
     * | # | Color       | Score mínimo |
     * |---|-------------|--------------|
     * | 1 | Negro       | 100          |
     * | 2 | Morado      | 200          |
     * | 3 | Blanco      | 300          |
     * | 4 | Rojo        | 400          |
     * | 5 | Azul        | 500          |
     * | 6 | Dorado      | 999          |
     *
     * Al equipar, llama al callback `Action<Color, Color, Color>^` que
     * invoca `Form1::SetSelectedSkin()`.
     */
    public ref class FormTienda : public System::Windows::Forms::Form {
    public:
        /**
         * @brief Constructor.
         * @param bestScore           Puntuación máxima para determinar skins disponibles.
         * @param player1Game         Referencia al juego del jugador 1 (puede ser null).
         * @param player2Game         Referencia al juego del jugador 2 (puede ser null).
         * @param skinSelectedCallback Callback invocado al equipar una skin.
         */
        FormTienda(int bestScore, SnakeGame^ player1Game, SnakeGame^ player2Game,
                   Action<Color, Color, Color>^ skinSelectedCallback);

    private:
        /**
         * @brief Recorre los botones de compra y habilita/deshabilita según bestScore.
         *
         * Usa `skinRequirements[]` para comparar con `highestUnlockedScore`.
         */
        void UpdateSkinAvailability();

        /**
         * @brief Aplica los colores de la skin a los juegos activos vía callback.
         * @param primary   Color principal.
         * @param secondary Color secundario.
         * @param head      Color de cabeza.
         */
        void ApplySkinToGames(Color primary, Color secondary, Color head);

        /**
         * @brief Reanuda los juegos pausados y regresa a Form1.
         *
         * Recupera la instancia de Form1 mediante `Application::OpenForms["Form1"]`.
         */
        void VolverAlJuego(Object^ sender, EventArgs^ e);
    };


    // ===========================================================
    //  modos.h / modos.cpp (Referencia C++ nativo)
    // ===========================================================

    /**
     * @file modos.h
     * @brief Gestor de modos de juego en C++ nativo (archivo de referencia).
     * @file modos.cpp
     * @brief Implementación de GameModes en C++ nativo (archivo de referencia).
     *
     * @warning Este archivo **no se integra directamente** con SnakeGame (C++/CLI)
     *          porque usa `std::deque` y `std::vector` (tipos nativos) que son
     *          incompatibles con las ref classes administradas.
     *
     * Su lógica fue reimplementada dentro de SnakeGame con tipos administrados
     * (`List<GridPoint>^`) en los métodos:
     * - SnakeGame::UpdateByScore()
     * - SnakeGame::GenerateObstacle()
     *
     * @note Se conserva como referencia de diseño y puede usarse en futuras
     *       versiones nativas del proyecto.
     */

    /**
     * @struct Point
     * @brief Coordenada 2D para uso en contexto C++ nativo.
     * @details Equivalente nativo de GridPoint para su uso con std::deque.
     */
    struct Point {
        int x; ///< Componente horizontal.
        int y; ///< Componente vertical.
        /** @brief Operador de igualdad. */
        bool operator==(const Point& other) const;
    };

    /**
     * @class GameModes
     * @brief Gestor de modos de juego en C++ nativo (sin .NET).
     *
     * Opera directamente sobre referencias a las estructuras de datos del juego.
     */
    class GameModes {
    public:
        /**
         * @brief Constructor que vincula el gestor con las estructuras del juego.
         * @param snake        Referencia al deque que representa el cuerpo.
         * @param obstacles    Referencia al vector de obstáculos.
         * @param timerInterval Referencia al intervalo del timer (ms).
         * @param boardWidth   Ancho del tablero en celdas.
         * @param boardHeight  Alto del tablero en celdas.
         * @param foodPos      Referencia a la posición actual de la comida.
         */
        GameModes(std::deque<Point>& snake, std::vector<Point>& obstacles,
                  int& timerInterval, int boardWidth, int boardHeight, Point& foodPos);

        /**
         * @brief Aplica la configuración inicial del modo indicado.
         * @param mode Modo a aplicar.
         */
        void applyMode(GameMode mode);

        /**
         * @brief Calcula cuánto crece la serpiente al comer.
         * @param mode Modo activo.
         * @return Cantidad de segmentos a agregar.
         */
        int handleFoodEaten(GameMode mode);

        /**
         * @brief Evalúa si el puntaje cruza un umbral y aplica efectos.
         * @param mode  Modo activo.
         * @param score Puntaje actual.
         */
        void updateByScore(GameMode mode, int score);

        /**
         * @brief Verifica si una posición es válida (no ocupada).
         * @param p Punto a verificar.
         * @return true si la posición está libre.
         */
        bool isPositionValid(Point p) const;

        /** @brief Cantidad aleatoria de crecimiento (modo RandomGrowth). */
        int getRandomGrowthAmount() const;

        /** @brief Elimina todos los obstáculos del tablero. */
        void clearObstacles();

        /** @brief Retorna el intervalo actual del timer. */
        int getTimerInterval() const;

    private:
        /** @brief Genera un obstáculo en una posición aleatoria libre. */
        void generateObstacle();

        /** @brief Genera un Point aleatorio dentro de los límites del tablero. */
        Point randomPoint() const;
    };

} // namespace MiProyecto
