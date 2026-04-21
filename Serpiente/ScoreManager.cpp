#include "ScoreManager.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Linq;
using namespace System::Windows::Forms;

namespace MiProyecto {

    String^ ScoreManager::GetScoreFilePath()
    {
        return ScoreConfig::FILE_NAME;
    }
    int ScoreManager::CompareByScoreDesc(ScoreEntry^ left, ScoreEntry^ right)
    {
        if (Object::ReferenceEquals(left, nullptr) && Object::ReferenceEquals(right, nullptr))
            return 0;
        if (Object::ReferenceEquals(left, nullptr))
            return 1;
        if (Object::ReferenceEquals(right, nullptr))
            return -1;

        return right->Score.CompareTo(left->Score);
    }
    String^ ScoreManager::NormalizeUsername(String^ username)
    {
        if (String::IsNullOrWhiteSpace(username))
            return "Jugador";

        String^ cleanName = username->Trim();
        return cleanName->Length > 24 ? cleanName->Substring(0, 24) : cleanName;
    }

    String^ ScoreManager::SanitizeCsvField(String^ value)
    {
        if (String::IsNullOrWhiteSpace(value))
            return "N/A";

        String^ cleanValue = value->Trim()->Replace(",", " ");
        return cleanValue->Replace(Environment::NewLine, " ");
    }

    void ScoreManager::EnsureFileHasHeader()
    {
        String^ path = GetScoreFilePath();
        if (!File::Exists(path))
        {
            File::WriteAllText(path, "Username,Score,BoardSize,GameMode" + Environment::NewLine);
            return;
        }

        try
        {
            array<String^>^ lines = File::ReadAllLines(path);
            if (lines->Length == 0)
            {
                File::WriteAllText(path, "Username,Score,BoardSize,GameMode" + Environment::NewLine);
                return;
            }

            if (!String::Equals(lines[0]->Trim(), "Username,Score,BoardSize,GameMode", StringComparison::OrdinalIgnoreCase))
            {
                List<String^>^ updatedLines = gcnew List<String^>();
                updatedLines->Add("Username,Score,BoardSize,GameMode");
                updatedLines->AddRange(lines);
                File::WriteAllLines(path, updatedLines->ToArray());
            }
        }
        catch (...)
        {
            // Mantener manejo mínimo de errores y evitar romper el flujo del juego.
        }
    }

    ScoreEntry^ ScoreManager::ParseScoreLine(String^ line)
    {
        if (String::IsNullOrWhiteSpace(line))
            return nullptr;

        String^ trimmed = line->Trim();
        if (String::Equals(trimmed, "Username,Score,BoardSize,GameMode", StringComparison::OrdinalIgnoreCase))
            return nullptr;

        int legacyScore = 0;
        if (Int32::TryParse(trimmed, legacyScore))
        {
            ScoreEntry^ legacyEntry = gcnew ScoreEntry();
            legacyEntry->Username = "Legacy";
            legacyEntry->Score = legacyScore;
            legacyEntry->BoardSize = "N/A";
            legacyEntry->GameMode = "N/A";
            return legacyEntry;
        }

        array<String^>^ parts = trimmed->Split(',');
        if (parts->Length < 4)
            return nullptr;

        int score = 0;
        if (!Int32::TryParse(parts[1]->Trim(), score))
            return nullptr;

        ScoreEntry^ entry = gcnew ScoreEntry();
        entry->Username = NormalizeUsername(parts[0]);
        entry->Score = score;
        entry->BoardSize = SanitizeCsvField(parts[2]);
        entry->GameMode = SanitizeCsvField(parts[3]);
        return entry;
    }

    void ScoreManager::SaveScore(String^ username, int score, String^ boardSize, String^ gameMode)
    {
        try
        {
            EnsureFileHasHeader();

            String^ record = String::Format(
                "{0},{1},{2},{3}",
                SanitizeCsvField(NormalizeUsername(username)),
                score,
                SanitizeCsvField(boardSize),
                SanitizeCsvField(gameMode));

            File::AppendAllText(GetScoreFilePath(), record + Environment::NewLine);
        }
        catch (...)
        {
            // Error básico silencioso para no afectar el cierre de la partida.
        }
    }

    List<ScoreEntry^>^ ScoreManager::LoadScores()
    {
        List<ScoreEntry^>^ scores = gcnew List<ScoreEntry^>();

        try
        {
            String^ path = GetScoreFilePath();
            if (!File::Exists(path))
                return scores;

            for each (String ^ line in File::ReadAllLines(path))
            {
                ScoreEntry^ entry = ParseScoreLine(line);
                if (entry != nullptr)
                    scores->Add(entry);
            }

            Dictionary<String^, int>^ bestByPlayer = GetBestScorePerPlayer();
            for each (ScoreEntry ^ entry in scores)
            {
                if (bestByPlayer->ContainsKey(entry->Username))
                    entry->BestScore = bestByPlayer[entry->Username];
            }
        }
        catch (...)
        {
            scores->Clear();
        }

        return scores;
    }

    Dictionary<String^, int>^ ScoreManager::GetBestScorePerPlayer()
    {
        Dictionary<String^, int>^ bestByPlayer = gcnew Dictionary<String^, int>(StringComparer::OrdinalIgnoreCase);

        try
        {
            String^ path = GetScoreFilePath();
            if (!File::Exists(path))
                return bestByPlayer;

            for each (String ^ line in File::ReadAllLines(path))
            {
                ScoreEntry^ entry = ParseScoreLine(line);
                if (entry == nullptr)
                    continue;

                if (!bestByPlayer->ContainsKey(entry->Username) || entry->Score > bestByPlayer[entry->Username])
                    bestByPlayer[entry->Username] = entry->Score;
            }
        }
        catch (...)
        {
            bestByPlayer->Clear();
        }

        return bestByPlayer;
    }

    List<ScoreEntry^>^ ScoreManager::GetTopScores(int n)
    {
        List<ScoreEntry^>^ scores = LoadScores();
        scores->Sort(gcnew Comparison<ScoreEntry^>(&ScoreManager::CompareByScoreDesc));

        if (n <= 0 || scores->Count <= n)
            return scores;

        return gcnew List<ScoreEntry^>(scores->GetRange(0, n));
    }

    int ScoreManager::GetHighestScore()
    {
        int bestScore = 0;
        List<ScoreEntry^>^ scores = LoadScores();
        for each (ScoreEntry ^ entry in scores)
        {
            if (entry->Score > bestScore)
                bestScore = entry->Score;
        }

        return bestScore;
    }
}   