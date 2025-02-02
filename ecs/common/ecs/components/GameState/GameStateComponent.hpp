#pragma once

#include "../Component.hpp"
#include <string>

#pragma once

#include "../Component.hpp"

struct GameStateComponent : public Component {
    int currentPlayer;
    bool gameOver;
    std::string winnerText;
    std::size_t player1Entity;
    std::size_t player2Entity;

    GameStateComponent(std::size_t player1, std::size_t player2) 
        : currentPlayer(1), gameOver(false), winnerText(""), player1Entity(player1), player2Entity(player2) {}

    void switchPlayer() {
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    void endGame(int winner) {
        gameOver = true;
        winnerText = "🎉 Player " + std::to_string(winner) + " won!";
    }
};
