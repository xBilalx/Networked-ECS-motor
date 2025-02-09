#pragma once

class Scene;

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void update(Scene& scene, float dt) = 0;
};
