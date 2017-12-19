#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <random>

class Random
{
    public:
        Random();

        int getIntInRange(int low, int high);

    private:
        std::mt19937 m_randNumGenerator;
};

#endif // RANDOM_H_INCLUDED
