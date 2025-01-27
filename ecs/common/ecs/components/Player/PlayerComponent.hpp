#pragma once

#include "../Component.hpp"
#include <string>

// Composant générique pour représenter un joueur
class PlayerComponent : public Component {
public:
    PlayerComponent(int id, const std::string& name)
        : playerId(id), playerName(name), score(0), isTurn(false) {}

    // Obtenir l'ID du joueur
    int getId() const { return playerId; }

    // Obtenir le nom du joueur
    const std::string& getName() const { return playerName; }

    // Obtenir et mettre à jour le score du joueur
    int getScore() const { return score; }
    void addScore(int points) { score += points; }

    // Gérer le tour du joueur
    bool getTurn() const { return isTurn; }
    void setTurn(bool turn) { isTurn = turn; }

private:
    int playerId;               // Identifiant unique du joueur
    std::string playerName;     // Nom du joueur
    int score;                  // Score actuel
    bool isTurn;                // Indique si c'est le tour du joueur
};
