#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include <SFML/Graphics.hpp>

// Pour une futur classe
class ISystem
{
    virtual void update(Scene& scene, float dt) = 0;
};

class RenderSystem
{
    public:
        void createWindow(unsigned int modeWidth,unsigned int modeHeight, std::string windowName) {
            window.create(sf::VideoMode(modeWidth, modeHeight), windowName);
            window.setFramerateLimit(120);
        }
        // bool createWindowWithEntity(Scene& em) {
        //     for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
        //         std::cout << "_________-->"<< it->first << " a " << it->second.size()  << " composants" << std::endl;
        //         auto win = it->second.find(std::type_index(typeid(WindowComponent)));
        //         if (win == it->second.end()) {
        //             continue;
        //         }
        //         WindowComponent *winCmpnt = dynamic_cast<WindowComponent*>(win->second.get());
        //         if (winCmpnt) {
        //             window.create(sf::VideoMode(winCmpnt->modeWidth, winCmpnt->modeHeight), winCmpnt->WindowName);
        //             return true;
        //         }
        //     }
        //     return false;
        // }
        sf::RenderWindow& getWindow() {
            return window;
        };
        void update(Scene& scene) {
            window.clear();
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
                RenderComponent* render = scene.getComponent<RenderComponent>(it->first);
                
                if (position && render) {
                    render->sprite.setPosition(position->position.x, position->position.y);
                    window.draw(render->sprite);
                    continue;
                }
                if (render) {
                    render->sprite.setPosition(0, 0);
                    window.draw(render->sprite);
                    continue; 
                }
            }
            window.display();
        }
    protected:

    private:
        sf::RenderWindow window; // Best way if the window variable would be in WindowComponent ?
};