#pragma once

#include "../Component.hpp"

struct ArrowComponent : public Component {
    int currentColumn;
    float cellSize;
    int maxColumns;

    ArrowComponent(int startColumn, float cellSize, int maxColumns)
        : currentColumn(startColumn), cellSize(cellSize), maxColumns(maxColumns) {}
};
