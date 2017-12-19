#include <iostream>

#include "Application.h"
#include "Config.h"



/**
  * Rules:
  *     The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells,
        each of which is in one of two possible states, alive or dead, or "populated" or "unpopulated".
        Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or
        diagonally adjacent. At each step in time, the following transitions occur:
  *
  *  - Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
  *  - Any live cell with two or three live neighbours lives on to the next generation.
  *  - Any live cell with more than three live neighbours dies, as if by overpopulation.
  *  - Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
  */

int main()
{
    Config config;

    char option = 'Y';
    //std::cout << "Would you like to use default config? [Y/N]: ";
    //std::cin >> option;
    option = std::toupper(option);

    if (option == 'N')
    {
        std::cout << "Enter window width: ";
        std::cin >> config.windowWidth;

        std::cout << "Enter window height: ";
        std::cin >> config.windowHeight;

        std::cout << "Enter cell size: ";
        std::cin >> config.quadSize;

        //Make it so that the cells fit in the window
        config.windowWidth -= config.windowWidth   % config.quadSize;
        config.windowHeight -= config.windowHeight % config.quadSize;

        config.simWidth  =  config.windowWidth  / config.quadSize;
        config.simHeight =  config.windowHeight / config.quadSize;
    }
    else
    {
        config.quadSize     = 16; // 2  //4   16
        config.windowWidth  = 1366;
        config.windowHeight = 768;

        config.simWidth     =  config.windowWidth  / config.quadSize;
        config.simHeight    =  config.windowHeight / config.quadSize;
    }

    Application app(config);
    app.run();
}
