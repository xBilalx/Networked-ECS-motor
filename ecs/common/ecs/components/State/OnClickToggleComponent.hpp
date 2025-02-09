#pragma once

#include "../Component.hpp"
#include "../../systems/Network/Serializer.hpp"

// Stocke les input d'une entité (Mais devrait plus stocker les input d'une instance parce ce si il y a plusieurs input sur une meme instance ca fait la même chose)
// Marche seulement avec le Click Left de la souris
struct OnClickToggleComponent : public Component {
    bool isFocus;

    OnClickToggleComponent(bool defautValue = false) : isFocus(defautValue) {}

    void toogle() {
        isFocus = !isFocus;
    }
};
