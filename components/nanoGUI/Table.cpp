#include "Table.h"

Table::Table() {
    this->header = "";
}

Table::~Table() = default;

void Table::render() {
    // Render the table
}

void Table::addChildren(GraphicSubstance* child) {
    if(children_count < 12) {
        children[children_count] = child;
        children_count++;
    }
}