#ifndef GRAPHICSUBSTANCE_HPP
#define GRAPHICSUBSTANCE_HPP

class GraphicSubstance
{
public:
    virtual void render() = 0;
    virtual void addChildren() = 0;
    virtual void removeChildren() = 0;
};

#endif // GRAPHICSUBSTANCE_HPP