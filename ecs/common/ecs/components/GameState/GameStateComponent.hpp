#pragma once

#include "../Component.hpp"
#include <string>

#pragma once

#include "../Component.hpp"

struct GameStateComponent : public Component {
    std::size_t currentPlayerEntity;
    bool gameOver;
    bool winner = false;
    std::string winnerText;
    std::size_t player1Entity;
    std::string pathTexture1;
    std::size_t player2Entity;
    std::string pathTexture2;

    GameStateComponent(std::size_t player1, std::string pathTexture1,  std::size_t player2, std::string pathTexture2) 
        : currentPlayerEntity(player1), gameOver(false), winnerText(""), player1Entity(player1), pathTexture1(pathTexture1), player2Entity(player2), pathTexture2(pathTexture2) {}

    GameStateComponent(bool gameOver_, bool winner_) 
    : gameOver(gameOver_), winner(winner_) {}

    void switchPlayer() {
        currentPlayerEntity = (currentPlayerEntity == player1Entity) ? player2Entity : player1Entity;
    }

    void endGame(std::size_t winner) {
        gameOver = true;
        if (player1Entity == currentPlayerEntity) {
            winner = false;
        } else if (currentPlayerEntity == player2Entity) {
            winner = true;
        }
        winnerText = "🎉 Player " + std::to_string(winner) + " won!";
    }
};