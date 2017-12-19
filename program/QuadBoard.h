#ifndef QuadBoard_H_INCLUDED
#define QuadBoard_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "Config.h"

class QuadBoard
{
    public:
        QuadBoard(const Config& config);

        void addQuad        (unsigned x, unsigned y, sf::Color& colour);
        void setQuadColour  (unsigned x, unsigned y, sf::Color& colour);

        void draw(sf::RenderWindow& window);

    private:
        unsigned getQuadIndex(unsigned x, unsigned y) const;
        void makeGrid();

        std::vector<sf::Vertex>     m_grid;
        std::vector<sf::Vertex>     m_pixels;

        const Config CONFIG;
};

#endif // QuadBoard_H_INCLUDED
