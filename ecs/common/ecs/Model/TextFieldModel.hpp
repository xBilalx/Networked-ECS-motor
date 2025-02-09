#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Text/TextComponent.hpp"
#include "../components/Text/TextFieldComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"
#include "../components/Score/ScoreComponent.hpp"
#include "../components/State/OnClickToggleComponent.hpp"

#include "../entitiesManager.hpp"

class TextFieldModel
{
public:
    TextFieldModel(Scene &scene, float x, float y, float width, float height, const std::string &defautText = "Enter text")
    {
        entity = scene.createEntity();
        scene.addComponent<PositionComponent>(entity, x, y);
        scene.addComponent<RectangleComponent>(entity, x, y, width, height);
        scene.addComponent<OnClickToggleComponent>(entity);
        scene.addComponent<TextFieldComponent>(entity, x, y, width, height, defautText);
        scene.addComponent<RenderComponent>(entity, 1);
    }
    std::size_t getEntity() const
    {
        return entity;
    }

private:
    std::size_t entity;
};
