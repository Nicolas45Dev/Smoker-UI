#ifndef TABLE_H
#define TABLE_H

#include <array>
#include "GraphicSubstance.hpp"
#include <stdint.h>
#include <string>

typedef struct {
    GraphicSubstance* child_rigth;
    GraphicSubstance* child_left;
    GraphicSubstance* root;
} ChiildrenTree;

class Table {
private:
    std::string header;
    std::array<GraphicSubstance*, 20> children;
    uint32_t children_count = 0;
public:
    Table();
    ~Table();
    void render();
    void addChildren(GraphicSubstance* child);
    void removeChildren(uint32_t child_id);
    void setHeader(std::string header);
};

#endif //TABLE_H
