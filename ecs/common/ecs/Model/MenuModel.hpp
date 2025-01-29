#pragma once

#include "../components/Box/HoverComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"
#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Menu/MenuComponent.hpp"
#include "../Model/ButtonModel.hpp"
#include "../entitiesManager.hpp"
#include <vector>
#include <functional>
#include <string>

class MenuModel {
public:
    MenuModel(Scene& scene, const std::string& backgroundTexture) {
        menuEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(menuEntity, 0, 0); // Position du menu (coin supérieur gauche)
        scene.addComponent<RenderComponent>(menuEntity, backgroundTexture, true); // Chargement de l'image de fond
    }

    std::size_t getEntity() const {
        return menuEntity;
    }

private:
    std::size_t menuEntity;
};