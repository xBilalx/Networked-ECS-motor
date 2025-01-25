#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <typeindex>
#include <iostream>
#include "./components/Component.hpp"

class Entity {
public:
    Entity() : id(nextId++) {}
    
    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        auto& ref = *component;
        components[std::type_index(typeid(T))] = std::move(component);
        return ref;
    }

    template<typename T>
    T* getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        return it != components.end() ? 
            dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    uint64_t getId() const { return id; }

private:
    static inline uint64_t nextId = 0;
    uint64_t id;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};

class EntityManager {
public:
    // Entity& createEntity() {
    //     entities.push_back(std::make_unique<Entity>());
    //     return *entities.back();
    // }
    std::size_t createEntityTest() {
        entitiesNbr++;
        entities1[entitiesNbr];
        return entitiesNbr;
    }
    std::size_t createEntityTestWithId(std::size_t id) {
        entities1[id];
        return entitiesNbr;
    }

    template<typename T, typename... Args>
    T& addComponentTest(std::size_t entity, Args&&... args) {
        // auto it = entities1.find(entity);
        // if ((it == entities1.end())) {
        //     std::cout << "Il y a pas" << std::endl;
        // }
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        auto& ref = *component;
        entities1[entity][std::type_index(typeid(T))] = std::move(component);
        return ref;
        // it = entities1.find(entity);
        // if ((it == entities1.end())) {
        //     std::cout << "Il y a pas " << std::endl;
        // }
    }

    template<typename T>
    T* getComponentTest(std::size_t entity) {
        auto itEnt = entities1.find(entity);
        if ((itEnt == entities1.end())) {
            // std::cout << "Il y a pas d'entité\n" << std::endl;
            return nullptr;
        }
        auto it = entities1[entity].find(std::type_index(typeid(T)));
        if (it == entities1[entity].end()) {
            // std::cout << "Le composant existe pas\n" << std::endl;
        }
        return it != entities1[entity].end() ? 
            dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    bool checkIfEntityExistTest(std::size_t id) {
        auto itEnt = entities1.find(id);
        return (itEnt != entities1.end());
    }

    // bool checkIfEntityExist(uint64_t id) {
    //     for (auto it = entities.begin(); it != entities.end(); it++) {
    //         if ((*it)->getId() == id)
    //             return true;
    //     }
    //     return false;
    // } 

    // std::unique_ptr<Entity>& findEntity(uint64_t id) {
    //     for (auto it = entities.begin(); it != entities.end(); it++) {
    //         if ((*it)->getId() == id)
    //             return (*it);
    //     }
    // }

    // void destroyEntity(uint64_t id) {
    //     entities.erase(
    //         std::remove_if(entities.begin(), entities.end(),
    //             [id](const auto& e) { return e->getId() == id; }),
    //         entities.end()
    //     );
    // }

    // std::vector<std::unique_ptr<Entity>> entities;
    std::unordered_map<std::size_t, std::unordered_map<std::type_index, std::unique_ptr<Component>>> entities1;
    std::size_t entitiesNbr = -1;
};