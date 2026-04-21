#pragma once

#using <System.dll>
#using <System.Core.dll>
#using <System.Windows.Forms.dll>

#include "Snake.h"

namespace MiProyecto {

    using namespace System;
    using namespace System::Collections::Generic;

    public ref class ScoreEntry
    {
    public:
        property String^ Username;
        property int Score;
        property String^ BoardSize;
        property String^ GameMode;
        property int BestScore;

        ScoreEntry()
        {
            Username = String::Empty;
            Score = 0;
            BoardSize = String::Empty;
            GameMode = String::Empty;
            BestScore = 0;
        }
    };

    public ref class ScoreManager abstract sealed
    {
    public:
        static void SaveScore(String^ username, int score, String^ boardSize, String^ gameMode);
        static List<ScoreEntry^>^ LoadScores();
        static List<ScoreEntry^>^ GetTopScores(int n);
        static Dictionary<String^, int>^ GetBestScorePerPlayer();
        static int GetHighestScore();

    private:
        static int CompareByScoreDesc(ScoreEntry^ left, ScoreEntry^ right);
        static String^ GetScoreFilePath();
        static void EnsureFileHasHeader();
        static ScoreEntry^ ParseScoreLine(String^ line);
        static String^ SanitizeCsvField(String^ value);
        static String^ NormalizeUsername(String^ username);
    };
}