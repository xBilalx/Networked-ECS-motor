#pragma once

#include "../Component.hpp"
#include <string>

#pragma once

#include "../Component.hpp"

struct GameStateComponent : public Component {
    std::size_t currentPlayerEntity;
    bool gameOver;
    std::string winnerText;
    std::size_t player1Entity;
    std::size_t player2Entity;

    GameStateComponent(std::size_t player1, std::size_t player2) 
        : currentPlayerEntity(player1), gameOver(false), winnerText(""), player1Entity(player1), player2Entity(player2) {}

    void switchPlayer() {
        currentPlayerEntity = (currentPlayerEntity == player1Entity) ? player2Entity : player1Entity;
    }

    void endGame(std::size_t winner) {
        gameOver = true;
        winnerText = "🎉 Player " + std::to_string(winner) + " won!";
    }
};