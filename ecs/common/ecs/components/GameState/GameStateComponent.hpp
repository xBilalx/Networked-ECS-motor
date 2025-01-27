#pragma once

#include "../Component.hpp"

// Composant générique pour représenter l'état global d'un jeu
class GameStateComponent : public Component {
public:
    GameStateComponent()
        : isGameOver(false), currentPlayerId(0) {}

    // Indique si le jeu est terminé
    bool getGameOver() const { return isGameOver; }
    void setGameOver(bool gameOver) { isGameOver = gameOver; }

    // Obtenir et définir le joueur actuel
    int getCurrentPlayerId() const { return currentPlayerId; }
    void setCurrentPlayerId(int playerId) { currentPlayerId = playerId; }

private:
    bool isGameOver;          // Indique si le jeu est terminé
    int currentPlayerId;      // Identifiant du joueur dont c'est le tour
};
