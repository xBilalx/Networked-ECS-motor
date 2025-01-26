#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <typeindex>
#include <iostream>
#include "./components/Component.hpp"

class sceneManager;

class Scene {
public:
    Scene(sceneManager* SceneManager) : SceneManager(SceneManager) {};
    std::size_t createEntity() {
        entitiesNbr++;
        entities1[entitiesNbr];
        return entitiesNbr;
    }
    std::size_t createEntityWithId(std::size_t id) {
        entities1[id];
        return entitiesNbr;
    }

    template<typename T, typename... Args>
    T& addComponent(std::size_t entity, Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        auto& ref = *component;
        entities1[entity][std::type_index(typeid(T))] = std::move(component);
        return ref;
    }

    template<typename T>
    T* getComponent(std::size_t entity) {
        auto itEnt = entities1.find(entity);
        if ((itEnt == entities1.end())) {
            return nullptr;
        }
        auto it = entities1[entity].find(std::type_index(typeid(T)));
        if (it == entities1[entity].end()) {
        }
        return it != entities1[entity].end() ? 
            dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    bool checkIfEntityExist(std::size_t id) {
        auto itEnt = entities1.find(id);
        return (itEnt != entities1.end());
    }

    std::unordered_map<std::size_t, std::unordered_map<std::type_index, std::unique_ptr<Component>>> entities1;
    std::size_t entitiesNbr = -1;
    sceneManager *SceneManager;
};