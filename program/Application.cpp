#include "Application.h"


#include <iostream>
#include <random>
#include <ctime>

namespace
{
    sf::Color deadColour   = {0, 0, 0};
    //sf::Color aliveColour  = sf::Color::Black;
    sf::Color aliveColour  = {0, 255, 0};
    sf::Color unknownColour  = {127, 127, 127};

}

sf::Color& getCellColour(Cell cell)
{
    if(cell == Cell::Alive) return aliveColour;
    else if(cell == Cell::Dead) return  deadColour;
    else if(cell == Cell::Unknown) return  unknownColour;

//    return cell == Cell::Alive ?
//                    aliveColour :
//                    deadColour;


}

Application::Application(const Config& config)
:   CONFIG      (config)
,   m_quadBoard (config)
,   m_window    ({config.windowWidth, config.windowHeight}, "Robot way")
,   m_view      ({0, 0}, {(float)config.windowWidth, (float)config.windowHeight})
,   m_cells     (config.simWidth * config.simHeight)
{
    m_font.loadFromFile         ("font/arial.ttf");
    m_text.setFont              (m_font);
    //m_text.setFillColor         (sf::Color::White);
    //m_text.setOutlineColor      (sf::Color::Black);
    //m_text.setOutlineThickness  (3);
    m_text.setCharacterSize     (20);
    m_text.setPosition          (5, 5);
    m_text.setString            (std::string("\n")
                                 //std::string("Click cell to change it to alive/ dead.\n") +
                                 //std::string("Press \"C\" to clear the cells\n") +
                                 //std::string("Press \"T\" to toggle the grid\n") +
                                 //std::string("Press \"Space\" when you are ready.")
    );

    m_window.setFramerateLimit(5);

   std::mt19937 rng (std::time(nullptr));
//    cellForEach([&](unsigned x, unsigned y)
//    {
//        std::uniform_int_distribution<int> dist(0, 1);
//        auto& cell = m_cells[getCellIndex(x, y)];
//        cell = (Cell)dist(rng);
//        m_quadBoard.addQuad(x, y, getCellColour(cell));
//    });

    cellForEach([&](unsigned x, unsigned y)
                {
                    auto& cell = m_cells[getCellIndex(x, y)];
                    cell = (Cell)2;
                    //m_cells[getCellIndex(x, y)] = Cell::Unknown;
                    m_quadBoard.addQuad(x, y, getCellColour(cell));
                });

    m_view.setCenter(m_window.getSize().x / 2,
                   m_window.getSize().y / 2);
}



void Application::run()
{
    create_map();
    create_map_robot();

    OpenMap();
    int iter = 0;





    std::cout << "Hello, World!" << std::endl;


    //print_map();

    //while(1)
    //{
        //find_way();
   // }

    bool changed = false;
    int offset = 1;
    int generations = 0;
    sf::Clock clock;
    while (m_window.isOpen())
    {
        m_window.clear();
        m_window.setView(m_view);

        switch (m_state)
        {
            case State::LoadMap:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    m_state = State::LoadMapRob;
                }
                printMap();

                break;
            case State::LoadMapRob:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    m_state = State::LoadMap;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                {
                    find_way(iter);
                    iter++;
                }
                printScreenBuf();

            case State::Paused:
                if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                    std::cout << "Generating a new entry" << std::endl;
                    std::cout << "Offset: " << offset << std::endl;
                    std::cout << "Count: " << saves.size() << std::endl;
                    changed = true;
                    if (offset == 1) {
                        m_text.setString("Generation: " + std::to_string(generations++));
                        updateWorld();
                        m_quadBoard.draw(m_window);
                        m_window.setView(m_window.getDefaultView());
                        m_window.draw(m_text);
                        m_window.display();
                    } else {
                        offset--;
                        std::cout << "Going  forward" << std::endl;
                        std::cout << "Offset: " << offset << std::endl;
                        std::cout << "Count: " << saves.size() << std::endl;
                        Save *wanted = &saves[saves.size() - offset];
                        if (wanted != nullptr) {
                            for (CellSave &cell : wanted->cells) {
                                m_quadBoard.setQuadColour(cell.x, cell.y, getCellColour(cell.cell));
                            }
                            m_quadBoard.draw(m_window);
                            m_window.setView(m_window.getDefaultView());
                            m_window.draw(m_text);
                            m_window.display();
                        }
                    }
                } else if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                    changed = true;
                    offset++;
                    std::cout << "Going back" << std::endl;
                    std::cout << "Offset: " << offset << std::endl;
                    std::cout << "Count: " << saves.size() << std::endl;
                    Save *wanted = &saves[saves.size() - offset];

                    if (wanted != nullptr) {
                        for (CellSave &cell : wanted->cells) {

                            m_quadBoard.setQuadColour(cell.x, cell.y, getCellColour(cell.cell));
                        }

                        m_quadBoard.draw(m_window);
                        m_window.setView(m_window.getDefaultView());
                        m_window.draw(m_text);
                        m_window.display();
                    }
                } else if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                    m_state = State::Sim;
                    changed = true;
                    Save *wanted = &saves[saves.size() - 1];
                    if (wanted != nullptr) {
                        offset = 1;
                        for (auto &cell : wanted->cells) // access by reference to avoid copying
                        {
                            m_quadBoard.setQuadColour(cell.x, cell.y, getCellColour(cell.cell));
                        }
                        m_quadBoard.draw(m_window);
                        m_window.setView(m_window.getDefaultView());
                        m_window.draw(m_text);
                        m_window.display();
                    }
                    std::cout << "Changed to continue" << std::endl;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(25));

                    break;
                }

            case State::Creating:
                mouseInput();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    m_state = State::Sim;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                {
                    cellForEach([&](unsigned x, unsigned y)
                    {
                        m_cells[getCellIndex(x, y)] = Cell::Dead;
                        m_quadBoard.setQuadColour(x, y, deadColour);
                    });
                }
                break;

            case State::Sim:
                if (!changed) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                        m_state = State::Paused;
                        changed = true;
                        std::cout << "Changed to paused" << std::endl;
                        break;
                    }
                }
                m_text.setString("Generation: " + std::to_string(generations++));
                updateWorld();
                break;
        }

        if (m_state != State::Paused) {
            m_quadBoard.draw(m_window);
            m_window.setView(m_window.getDefaultView());
            m_window.draw(m_text);

            m_window.display();

        }
        if(saves.size() > 250) {
            saves.erase(saves.begin());

        }
        handleEvents();
        if (changed &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::P) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::N) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            changed = false;
    }
}

void Application::updateWorld()
{
    std::vector<Cell> newCells(CONFIG.simWidth * CONFIG.simHeight);
    Save save = Save();
    cellForEach([&](unsigned x, unsigned y)
    {
        unsigned count = 0;
        for (int nX = -1; nX <= 1; nX++)    //check neighbours
        for (int nY = -1; nY <= 1; nY++)
        {
            int newX = nX + x;
            int newY = nY + y;

            if (newX == -1 || newX == (int)CONFIG.simWidth ||
                newY == -1 || newY == (int)CONFIG.simHeight || //out of bounds
                (nX == 0 && nY == 0)) //Cell itself
            {
                continue;
            }
            auto cell = m_cells[getCellIndex(newX, newY)];
            if (cell == Cell::Alive)
                count++;
        }

        auto cell           = m_cells[getCellIndex(x, y)];
        auto& updateCell    = newCells[getCellIndex(x, y)];
        updateCell = cell;
        switch (cell)
        {
            case Cell::Alive:
                if (count < 2 || count > 3)
                {
                    updateCell = Cell::Dead;
                }
                break;

            case Cell::Dead:
                if (count == 3)
                {
                    updateCell = Cell::Alive;
                }
                break;
        }

        m_quadBoard.setQuadColour(x, y, getCellColour(updateCell));
        CellSave cellSave = CellSave();
        cellSave.x = x;
        cellSave.y = y;
        cellSave.cell = updateCell;
        save.cells.push_back(cellSave);
    });
    m_cells = std::move(newCells);
    saves.push_back(save);
}


void Application::handleEvents()
{
    sf::Event e;

    while (m_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

unsigned Application::getCellIndex(unsigned x, unsigned y) const
{
    return y * CONFIG.simWidth + x;
}

void Application::mouseInput()
{
    static sf::Clock delay;
    if (delay.getElapsedTime().asSeconds() > 0.2)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            delay.restart();
            auto mousePosition = sf::Mouse::getPosition(m_window);
            auto adjustedMousePosition = m_window.mapCoordsToPixel({(float)mousePosition.x,
                                                                    (float)mousePosition.y});
            auto x = adjustedMousePosition.x;
            auto y = adjustedMousePosition.y;

            if (x < 0 || x > (int)m_window.getSize().x ||
                y < 0 || y > (int)m_window.getSize().y)
            {
                return;
            }

            //Convert mouse/ screen coordinates to cell coordinates
            int newX = x / CONFIG.quadSize;
            int newY = y / CONFIG.quadSize;

            //Switch cell type
            auto& cell = m_cells[getCellIndex(newX, newY)];
            cell =  cell == Cell::Alive ?
                        Cell::Dead :
                        Cell::Alive;

            //Set new colour
            m_quadBoard.setQuadColour(newX, newY, getCellColour(cell));
        }
    }
}
