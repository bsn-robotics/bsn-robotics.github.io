#include "FindWay.h"
#include "Application.h"

namespace
{
    sf::Color deadColour   = {0, 0, 0};
    //sf::Color aliveColour  = sf::Color::Black;
    sf::Color aliveColour  = {255, 255, 255};
    sf::Color unknownColour = {127, 127, 127};
    sf::Color deadColourR   = {0, 32, 0};
    //sf::Color aliveColour  = sf::Color::Black;
    sf::Color aliveColourR  = {0, 255, 0};
    sf::Color unknownColourR = {127, 127, 127};
    sf::Color robotColour = {255, 0, 0};
    sf::Color pathColour = {0, 255, 0};
    sf::Color pathColour2 = {255, 0, 255};
    sf::Color badPathColour = {0, 0, 255};
}

int iteration =0;

const int size_rob = 2;//размер робота в пикселях  // 2  12
const int size_rob_d_2 = 1;

//int y_rob = 26;
//int x_rob = 14;

int y_rob = 10; //10//78  //74   10
int x_rob = 9; //7        //36   9

int orient = 1;
//1 - вверх  2 - вправо  3 - вниз  4 - влево

int y_rob_old = 0;
int x_rob_old = 0;

int place_rob[size_rob][size_rob];

int old_place_mark[100];

const int r = 10;//радиус видимости карты // 10 //72
const int rd2 = 5; // тк нечетное добавляем к результату единицу, если четное то не добалвляем

int y_rad0 = y_rob - (r-size_rob)/2;
int x_rad0 = x_rob - (r-size_rob)/2;

int rad[r][r];


int countw = 0;
int window_y[2048];
int window_x[2048];
int window_c[2048];


int way_y[512]; //100
int way_x[512]; //100
int step = 0;

int countmw = 0;
int mwindow_x[100];
int mwindow_y[100];


const int stackSize = 1024;
int stack_winy[stackSize];
int stack_winx[stackSize];
int stackPointer = 0;

struct Location{
    int x_robot;
    int y_robot;

    int windowY[2048];
    int windowX[2048];
    int windowCheck[2048];
    int countW; //количество окон

    int prevLoc;
};

Location Locations[255]; // локации
int countLocations = 0;
int countSteps = 0;

//карта

//0-неизвестно, 1-препятствие, 2-свободно
const int size_map_x = 46, size_map_y = 47; //40 40 //186*186(320)  46 47

int map[size_map_y][size_map_x]; //реальная карта

int map_rob[size_map_y][size_map_x]; //карта в памяти робота

int screen_buffer[size_map_y][size_map_x];


int number_max_xy = 0;

void create_map(void)
{
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            map[y][x]=0;
        }
    }
}

void create_map_robot(void)
{
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            map_rob[y][x]=0;
        }
    }
}

void print_map(void){
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            if(map[y][x] == 49)std::cout << 8 << " " ;
            else if(map[y][x] == 48)std::cout << " " << " ";
            //else std::cout << map[y][x] << " ";
        }
        std::cout << std::endl;
    }
}

void Application::printMap(void){
//    cellForEach([&](unsigned x, unsigned y)
//                {
//
//                    m_cells[getCellIndex(x, y)] = Cell::Dead;
//                    m_quadBoard.setQuadColour(x, y, deadColour);
//                });

    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            if(map[y][x] == 49) {
                m_cells[getCellIndex(x, y)] = Cell::Dead;
                m_quadBoard.setQuadColour(x, y, deadColour);
            }
            else if(map[y][x] == 48){
                m_cells[getCellIndex(x, y)] = Cell::Alive;
                m_quadBoard.setQuadColour(x, y, aliveColour);
            }
            //else std::cout << map[y][x] << " ";
        }
        //std::cout << std::endl;
    }
}

void print_map_rob(void){
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            if(map_rob[y][x] == 49)std::cout << 8 << " " ;
            else if(map_rob[y][x] == 48)std::cout << " " << " ";
            else std::cout << map_rob[y][x] << " ";
        }
        std::cout << std::endl;
    }
}
void Application::printMapRob(void){
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            if(map_rob[y][x] == 49) {
                m_cells[getCellIndex(x, y)] = Cell::Dead;
                m_quadBoard.setQuadColour(x, y, deadColour);
            }
            else if(map_rob[y][x] == 48){
                m_cells[getCellIndex(x, y)] = Cell::Alive;
                m_quadBoard.setQuadColour(x, y, aliveColour);
            }
            else {
                m_cells[getCellIndex(x, y)] = Cell::Unknown;
                m_quadBoard.setQuadColour(x, y, unknownColour);
            }
            //else std::cout << map[y][x] << " ";
        }
        //std::cout << std::endl;
    }
}

void print_screen_buf(){
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            if(screen_buffer[y][x] == 49)std::cout << 8 << " " ;
            else if(screen_buffer[y][x] == 48)std::cout << " " << " ";
            else std::cout << screen_buffer[y][x] << " ";
        }
        std::cout << std::endl;
    }
}
void Application::printScreenBuf(void){
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            if(screen_buffer[y][x] == 49) {
                m_cells[getCellIndex(x, y)] = Cell::Dead;
                m_quadBoard.setQuadColour(x, y, deadColour);
            }
            else if(screen_buffer[y][x] == 48){
                m_cells[getCellIndex(x, y)] = Cell::Alive;
                m_quadBoard.setQuadColour(x, y, aliveColour);
            }
            else if(screen_buffer[y][x] == 5){
                m_cells[getCellIndex(x, y)] = Cell::Robot;
                m_quadBoard.setQuadColour(x, y, robotColour);
            }
            else if(screen_buffer[y][x] == 0){
                m_cells[getCellIndex(x, y)] = Cell::Unknown;
                m_quadBoard.setQuadColour(x, y, unknownColour);
            }
            else if(screen_buffer[y][x] == 7){
                m_cells[getCellIndex(x, y)] = Cell::Path;
                m_quadBoard.setQuadColour(x, y, pathColour);
            }
            else if(screen_buffer[y][x] == 6){
                m_cells[getCellIndex(x, y)] = Cell::BadPath;
                m_quadBoard.setQuadColour(x, y, badPathColour);
            }
            else {
                m_cells[getCellIndex(x, y)] = Cell::Path;
                m_quadBoard.setQuadColour(x, y, pathColour2);
            }
            //else std::cout << map[y][x] << " ";
        }
        //std::cout << std::endl;
    }
}

void radar(void){

    //текущая область видимости карты
    int x_rad = 0, y_rad = 0;

    for(int y=y_rad0; y<y_rad0+r; y++){
        for(int x = x_rad0; x<x_rad0+r; x++){
            rad[y_rad][x_rad]=map[y][x];
            x_rad++;
        }
        y_rad++;
        x_rad = 0;
    }
}

void map_robot_refresh(void){
    int y_rad = 0, x_rad = 0;

    for(int y=y_rad0; y<y_rad0+r; y++){
        for(int x = x_rad0; x<x_rad0+r; x++){
            map_rob[y][x]=rad[y_rad][x_rad];
            x_rad++;
        }
        y_rad++;
        x_rad = 0;
    }
}

void find_window(void){
    int y_win0 = 0, x_win0 = 0;
    countw = 0;
    int flag = 0;

    for(y_win0 = 0; y_win0<r-size_rob+1; y_win0++){

        for(x_win0 = 0; x_win0<r-size_rob+1; x_win0++){

            //if((y_rad0+y_win0 == y_rob)&&(x_rad0+x_win0 == x_rob)) continue; //Если положение окна совпадает с положением робота, то не записывать его

            for(int y_win = 0; y_win<size_rob; y_win++){

                for(int x_win = 0; x_win<size_rob; x_win++){

                    if(rad[y_win0+y_win][x_win0+x_win] == 49) {flag = 1; break;}//если препятствие
                        //Нужно написать условие для 0 - неизвестно
                    else if((y_win == size_rob-1) && (x_win == size_rob-1)){

                        countw++;
                        window_y[countw-1] = y_rad0+y_win0;
                        window_x[countw-1] = x_rad0+x_win0;
                        window_c[countw-1] = 0;
                    }

                }
                if (flag == 1) break;
            }
            flag = 0;
        }
        flag =0;
    }

}
////////////////////////////////////////////////////////////////////////////////
//Stack Functions                                                             //
////////////////////////////////////////////////////////////////////////////////

bool pop(int& x, int& y)
{
    if(stackPointer > 0)
    {
        x = stack_winx[stackPointer];
        y = stack_winy[stackPointer];
        stackPointer--;
        return 1;
    }
    else
    {
        return 0;
    }
}

bool push(int x, int y)
{
    if(stackPointer < stackSize - 1)
    {
        stackPointer++;
        stack_winx[stackPointer] = x;
        stack_winy[stackPointer] = y;
        return 1;
    }
    else
    {
        return 0;
    }
}

void emptyStack()
{
    //while(pop(x, y));
}

// END Stack Functions                                                        //
////////////////////////////////////////////////////////////////////////////////


int returnWindow(int x, int y){
    for (int i = 0; i < countw; i++){
        if (window_x[i] == x){
            if (window_y[i] == y) return i;
        }
    }
    return -1;
}

bool returnCheckWindow(int x, int y){
    int returnWin = returnWindow(x, y);
    if (returnWin < 0) return false;
    else {
        if( window_c[returnWin] == 1) return false;
        else return true;
    }

}

void fillWindows(void){
    int x1;
    bool spanAbove, spanBelow;

    int x = x_rob;
    int y = y_rob;

    if(!push(x, y)) return;

    while(pop(x, y))
    {
        x1 = x;
        while(x1 >= 0 && returnCheckWindow(x1, y)){  //screenBuffer[y][x1] == oldColor

            x1--;
        }
        x1++;
        spanAbove = spanBelow = 0;
        while(x1 < 186 && returnCheckWindow(x1, y)) //while(x1 < w && screenBuffer[y][x1] == oldColor )
        {
            //screenBuffer[y][x1] = newColor;
            window_c[returnWindow(x1, y)] = 1;
            if(!spanAbove && y > 0 && returnCheckWindow(x1, y-1)) //screenBuffer[y - 1][x1] == oldColor
            {
                if(!push(x1, y - 1)) return;
                spanAbove = 1;
            }
            else if(spanAbove && y > 0 && !returnCheckWindow(x1, y-1)) //screenBuffer[y - 1][x1] != oldColor
            {
                spanAbove = 0;
            }
            if(!spanBelow && y < 185 && returnCheckWindow(x1, y+1)) //screenBuffer[y + 1][x1] == oldColor
            {
                if(!push(x1, y + 1)) return;
                spanBelow = 1;
            }
            else if(spanBelow && y < 185 && !returnCheckWindow(x1, y+1)) //screenBuffer[y + 1][x1] != oldColor
            {
                spanBelow = 0;
            }
            x1++;
        }
    }
}
void mark_window(void){

    for (int i = 0; i<countw; i++){
        if(screen_buffer[window_y[i]][window_x[i]]!=5){
            if (window_c[i] == 1) screen_buffer[window_y[i]][window_x[i]]=7;
            else screen_buffer[window_y[i]][window_x[i]]=6;
        }
    }

}

void screen_buffer_refresh(){
    for (int y=0; y<size_map_y; y++)
    {
        for (int x=0; x<size_map_x; x++)
        {
            screen_buffer[y][x] = map_rob[y][x];
        }
    }
}

void go_to_coord(int y, int x){

    //y_rob = y;
    //x_rob = x;

    y_rad0 = y_rob - (r-size_rob)/2;
    x_rad0 = x_rob - (r-size_rob)/2;

    radar(); //сделаем снимок
    map_robot_refresh(); // обновим карту
    screen_buffer_refresh(); //обновим экранный буфер


//    //восстановим то что было на месте робота !!!!!!!  Можно сделать по другому можно переписывать не с буфера а с изначальной карты
//    for (int yy =  0; yy<  size_rob; yy++){
//        for (int xx = 0; xx< size_rob; xx++){
//            map_rob[y_rob_old +yy][x_rob_old +xx] = place_rob[yy][xx];
//        }
//    }
//
//
//    for (int yy = 0; yy< size_rob; yy++){
//        for (int xx = 0; xx< size_rob; xx++){
//            place_rob[yy][xx] = map[y_rob+yy][x_rob+xx]; //запомним что было на месте робота
//            map_rob[y_rob+yy][x_rob+xx] = 5; //отметим новую позицию робота
//        }
//    }

    //отметим новую позицию робота на экранном буфере

    for (int yy = 0; yy< size_rob; yy++){
        for (int xx = 0; xx< size_rob; xx++){
            screen_buffer[y_rob+yy][x_rob+xx] = 5; //отметим новую позицию робота в экранном буфере
        }
    }



    y_rob_old = y_rob;
    x_rob_old = x_rob;
}

int max_elem(const int* arr, const int size){
    int number_max_x = 0;
    int max = arr[0];

    for (int i  = 1; i  < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
            number_max_x = i;
        }
    }

    return number_max_x;
}

int check_window(){
    //самой правой = x_max от window_x[]

    // выбор самого правого
    int number_max_x = 0;
    number_max_x = max_elem(window_x, countw); //countw должен быть равен размеру выделенного массива
    //самых правых может быть несколько, они имеют одну и ту же координату X

    //выбор самого нижнего из правых
    int max_y = 0;
    int number_max_xy = 0;
    for (int i = number_max_x ; i<countw; i++)
    {
        if(window_x[i] == window_x[number_max_x]) {
            if(window_y[i] > max_y) {
                max_y = window_y[i];
                number_max_xy = i;
            }
        }
    }
    return number_max_xy;
}

void mark_window_xy(int number_max_xy){
    if(countw>0) screen_buffer[window_y[number_max_xy]][window_x[number_max_xy]]=7;
}

void go(void){
    int step = 1;
    //std::cin >> step ;

    if (step == 1) y_rob--;
    if (step == 2) y_rob++;
    if (step == 3) x_rob--;
    if (step == 4) x_rob++;
}

//попытаться построить путь к (самой правой и самой нижней ячейке)

void way_to_window(int number_max_xy){

//    y_rob;
//    x_rob;
//    countw;
//    window_y;
//    window_x;

    //number_max_xy;

    int y_rob_v = y_rob; //виртуальная позиция
    int x_rob_v = x_rob;

    int y_rob_v_old = y_rob_v;
    int x_rob_v_old = x_rob_v;

    int size = 512; //100
    int window_c[512]; //100
    for (int i = 0; i < size; i++){
        window_c[i] = 0;
    }


    int orient = 1; //куда смотрит перед робота ? 1- прямо, 2 - назад, 3 - налево, 4 - направо

    int flag = 0; // Можно ли сдвинуться в ячейку

    int i=0;

    while((((x_rob_v==window_x[number_max_xy])&&(y_rob_v==window_y[number_max_xy]))==0 )){//!!!!!!
        //i++; && (i>10)
        flag = 0;
        if(orient == 1){

            if(flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v+1) == window_x[i]) && (y_rob_v == window_y[i])){ //проверить можно ли сдвинуться на ячейку вправо

                        if(window_c[i]==1) break;

                        flag = 1; //В эту ячейку можно сдвинуться
                        //сдвинуться;

                        x_rob_v = x_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 4;
                        break;
                    }
                }
            }

            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v) == window_x[i]) && ((y_rob_v-1) == window_y[i])){ //проверить можно ли сдвинуться прямо

                        if(window_c[i]==1) break;

                        flag = 2; //В эту ячейку можно сдвинуться
                        y_rob_v = y_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 1;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if((x_rob_v-1) == window_x[i] && (y_rob_v == window_y[i])){ //проверить можно ли сдвинуться влево

                        if(window_c[i]==1) break;

                        flag = 3; //В эту ячейку можно сдвинуться
                        x_rob_v = x_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 3;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v) == window_x[i]) && ((y_rob_v+1) == window_y[i])){ //проверить можно ли сдвинуться назад

                        if(window_c[i]==1) break;

                        flag = 4; //В эту ячейку можно сдвинуться
                        y_rob_v = y_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;

                        orient = 2;
                        break;
                    }
                }
            }
            if (flag == 0){
                //вернуться назад
                step--;

                if ((y_rob_v - way_y[step])>0) orient = 1;
                if ((y_rob_v - way_y[step])<0) orient =2;
                if ((x_rob_v - way_x[step])>0) orient = 3;
                if ((x_rob_v - way_x[step])<0) orient = 4;

                y_rob_v = way_y[step];
                x_rob_v = way_x[step];
            }
        }

        else if(orient == 2){
            if(flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v-1) == window_x[i]) && (y_rob_v == window_y[i])){ //проверить можно ли сдвинуться на ячейку вправо

                        if(window_c[i]==1) break;

                        flag = 1; //В эту ячейку можно сдвинуться
                        //сдвинуться;

                        x_rob_v = x_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 3;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v) == window_x[i]) && ((y_rob_v+1) == window_y[i])){ //проверить можно ли сдвинуться прямо

                        if(window_c[i]==1) break;

                        flag = 2; //В эту ячейку можно сдвинуться
                        y_rob_v = y_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 2;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if((x_rob_v+1) == window_x[i] && (y_rob_v == window_y[i])){ //проверить можно ли сдвинуться влево

                        if(window_c[i]==1) break;

                        flag = 3; //В эту ячейку можно сдвинуться
                        x_rob_v = x_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 4;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v) == window_x[i]) && ((y_rob_v-1) == window_y[i])){ //проверить можно ли сдвинуться назад

                        if(window_c[i]==1) break;

                        flag = 4; //В эту ячейку можно сдвинуться
                        y_rob_v = y_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;

                        orient = 1;
                        break;
                    }
                }
            }
            if (flag == 0){
                //вернуться назад
                step--;

                if ((y_rob_v - way_y[step])>0) orient = 1;
                if ((y_rob_v - way_y[step])<0) orient =2;
                if ((x_rob_v - way_x[step])>0) orient = 3;
                if ((x_rob_v - way_x[step])<0) orient = 4;

                y_rob_v = way_y[step];
                x_rob_v = way_x[step];
            }
        }

        else if(orient == 3){
            if(flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v) == window_x[i]) && (y_rob_v-1 == window_y[i])){ //проверить можно ли сдвинуться на ячейку вправо

                        if(window_c[i]==1) break;

                        flag = 1; //В эту ячейку можно сдвинуться
                        //сдвинуться;

                        y_rob_v = y_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 1;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v-1) == window_x[i]) && ((y_rob_v) == window_y[i])){ //проверить можно ли сдвинуться прямо

                        if(window_c[i]==1) break;

                        flag = 2; //В эту ячейку можно сдвинуться
                        x_rob_v = x_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;
                        window_c[i] = 1;
                        orient = 3;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if((x_rob_v) == window_x[i] && (y_rob_v+1 == window_y[i])){ //проверить можно ли сдвинуться влево

                        if(window_c[i]==1) break;

                        flag = 3; //В эту ячейку можно сдвинуться
                        y_rob_v = y_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 2;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v+1) == window_x[i]) && ((y_rob_v) == window_y[i])){ //проверить можно ли сдвинуться назад

                        if(window_c[i]==1) break;

                        flag = 4; //В эту ячейку можно сдвинуться
                        x_rob_v = x_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;

                        orient = 4;
                        break;
                    }
                }
            }
            if (flag == 0){
                //вернуться назад
                step--;

                if ((y_rob_v - way_y[step])>0) orient = 1;
                if ((y_rob_v - way_y[step])<0) orient =2;
                if ((x_rob_v - way_x[step])>0) orient = 3;
                if ((x_rob_v - way_x[step])<0) orient = 4;

                y_rob_v = way_y[step];
                x_rob_v = way_x[step];
            }
        }

        else if(orient == 4){
            if(flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v) == window_x[i]) && (y_rob_v+1 == window_y[i])){ //проверить можно ли сдвинуться на ячейку вправо

                        if(window_c[i]==1) break;

                        flag = 1; //В эту ячейку можно сдвинуться
                        //сдвинуться;

                        y_rob_v = y_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 2;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if(((x_rob_v+1) == window_x[i]) && ((y_rob_v) == window_y[i])){ //проверить можно ли сдвинуться прямо

                        if(window_c[i]==1) break;

                        flag = 2; //В эту ячейку можно сдвинуться
                        x_rob_v = x_rob_v+1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 4;
                        break;
                    }
                }
            }
            if (flag == 0){
                for (int i = 0; i<countw; i++) //для каждого окна
                {
                    if((x_rob_v) == window_x[i] && (y_rob_v-1 == window_y[i])){ //проверить можно ли сдвинуться влево

                        if(window_c[i]==1) break;

                        flag = 3; //В эту ячейку можно сдвинуться
                        y_rob_v = y_rob_v-1;
                        step++;
                        way_y[step]= y_rob_v;
                        way_x[step]= x_rob_v;

                        window_c[i] = 1;
                        orient = 3;
                        break;}
                }
            }
        }
        if (flag == 0){
            for (int i = 0; i<countw; i++) //для каждого окна
            {
                if(((x_rob_v-1) == window_x[i]) && ((y_rob_v) == window_y[i])){ //проверить можно ли сдвинуться назад

                    if(window_c[i]==1) break;

                    flag = 4; //В эту ячейку можно сдвинуться
                    x_rob_v = x_rob_v-1;
                    step++;
                    way_y[step]= y_rob_v;
                    way_x[step]= x_rob_v;

                    window_c[i] = 1;

                    orient = 2;
                    break;
                }
            }
        }
        if (flag == 0){
            //вернуться назад
            step--;

            if ((y_rob_v - way_y[step])>0) orient = 1;
            if ((y_rob_v - way_y[step])<0) orient =2;
            if ((x_rob_v - way_x[step])>0) orient = 3;
            if ((x_rob_v - way_x[step])<0) orient = 4;

            y_rob_v = way_y[step];
            x_rob_v = way_x[step];
        }
    }

}


void mark_way(){
    for (int i = 0; i<step; i++){
        screen_buffer[way_y[i]][way_x[i]]=i;
    }

    step = 0;//!!!!!!!!!!!!!!
}

//void makeLocation(){
//
//    Locations[countLocations].x_robot = x_rob;
//    Locations[countLocations].y_robot = y_rob;
//    Locations[countLocations].orient = orient;
//    Locations[countLocations].windowX[1] = ;
//
//    countw++;
//    window_y[countw-1] = y_rad0+y_win0;
//    window_x[countw-1] = x_rad0+x_win0;
//}
//


int depthWay() {

    int targetWindow = 0;
    if (countSteps == 0) Locations[countSteps].prevLoc = -1;
    if (countSteps == -1) return -1;
    iteration++;
    Locations[countSteps].x_robot = x_rob;
    Locations[countSteps].y_robot = y_rob;

    for(int i=0; i < countw; i++){
        Locations[countSteps].windowX[i] = window_x[i];   //запоминаем окна по пути
        Locations[countSteps].windowY[i] = window_y[i];
        Locations[countSteps].windowCheck[i] = 0; // пока заполняем нулями, значит что клетка не лежит на границе
    }
    Locations[countSteps].countW = countw ; //количество окон в локации

    std::cout << countLocations;





    //int size = 512; //100
    //int window_c[512]; //100
    //for (int i = 0; i < size; i++) {
    //   window_c[i] = 0;
    //}
    int prevLoc = 0;
    int flagWas = 0;
    int flag = 0; // Можно ли сдвинуться в ячейку
                for (int i = 0; i < countw; i++) //для каждого окна в локации
                {

                    if (((x_rob + rd2-size_rob_d_2) == window_x[i])&&(window_c[i] == 1)){
                        //проверить есть ли окна на границе справа r-size_rob

                        //проверить пренадлежит ли это окно другой локации или мы уже там были ?!!!!!!!!

                        //if (window_c[i] == 1) break;

                    //если клетка лежит на границе то помечаем ее единицей
                        Locations[countSteps].windowCheck[i] = 1;

                        for (int j = 0; j < countLocations+1; j++){
                            if (((Locations[j].x_robot == window_x[i])&&(Locations[j].y_robot == window_y[i]))){
                               flagWas = 1; //мы уже там были
                            }

                            for (int k = 0; k < Locations[j].countW; k++){
                                if (((Locations[j].windowX[k] == window_x[i])&&(Locations[j].windowY[k] == window_y[i])&&(Locations[j].windowCheck[k]==0))){
                                    flagWas = 1; //мы уже там были
                                }
                            }
                        }

                        if ((flagWas==0)&&(flag == 0)){
                            flag = 1; //В эту ячейку можно сдвинуться
                            //сдвинуться;

                            x_rob = window_x[i];
                            y_rob = window_y[i];

                            prevLoc = countSteps;
                            countSteps = countLocations+1;
                            Locations[countSteps].prevLoc = prevLoc;
                            countLocations++;
                            flagWas = 0;
                            targetWindow = i;
                        }
                        else {
                            flagWas = 0;
                        }
                    }
                }

                for (int i = 0; i < countw; i++) //для каждого окна
                {
                    if (((y_rob + rd2-size_rob_d_2) == window_y[i])&&(window_c[i] == 1)){
                        //проверить есть ли окна на границе снизу r-size_rob

                        //проверить пренадлежит ли это окно другой локации !!!!!!!!

                        //if (window_c[i] == 1) break;
                        //если клетка лежит на границе то помечаем ее единицей
                        Locations[countSteps].windowCheck[i] = 1;

                        for (int j = 0; j < countLocations+1; j++){
                            if (((Locations[j].x_robot == window_x[i])&&(Locations[j].y_robot == window_y[i]))){
                                flagWas = 1; //мы уже там были
                            }

                            for (int k = 0; k < Locations[j].countW; k++){
                                if (((Locations[j].windowX[k] == window_x[i])&&(Locations[j].windowY[k] == window_y[i])&&(Locations[j].windowCheck[k]==0))){
                                    flagWas = 1; //мы уже там были
                                }
                            }
                        }

                        if ((flagWas==0)&&(flag == 0)){
                            flag = 1; //В эту ячейку можно сдвинуться
                            //сдвинуться;

                            x_rob = window_x[i];
                            y_rob = window_y[i];

                            prevLoc = countSteps;
                            countSteps = countLocations+1;
                            Locations[countSteps].prevLoc = prevLoc;
                            countLocations++;
                            flagWas = 0;
                            targetWindow = i;
                        }
                        else {
                            flagWas = 0;
                        }
                    }
                }


                for (int i = 0; i < countw; i++) //для каждого окна
                {
                    if (((x_rob - rd2+size_rob_d_2)== window_x[i])&&(window_c[i] == 1)){
                        //проверить есть ли окна на границе слева r-size_rob

                        //проверить пренадлежит ли это окно другой локации !!!!!!!!

                        //if (window_c[i] == 1) break;
                        //если клетка лежит на границе то помечаем ее единицей
                        Locations[countSteps].windowCheck[i] = 1;

                        for (int j = 0; j < countLocations+1; j++){
                            if (((Locations[j].x_robot == window_x[i])&&(Locations[j].y_robot == window_y[i]))){
                                flagWas = 1; //мы уже там были
                            }

                            for (int k = 0; k < Locations[j].countW; k++){
                                if (((Locations[j].windowX[k] == window_x[i])&&(Locations[j].windowY[k] == window_y[i])&&(Locations[j].windowCheck[k]==0))){
                                    flagWas = 1; //мы уже там были
                                }
                            }
                        }

                        if ((flagWas==0)&&(flag == 0)){
                            flag = 1; //В эту ячейку можно сдвинуться
                            //сдвинуться;

                            x_rob = window_x[i];
                            y_rob = window_y[i];

                            prevLoc = countSteps;
                            countSteps = countLocations+1;
                            Locations[countSteps].prevLoc = prevLoc;
                            countLocations++;
                            flagWas = 0;
                            targetWindow = i;
                        }
                        else {
                            flagWas = 0;
                        }
                    }
                }


                for (int i = 0; i < countw; i++) //для каждого окна
                {
                    if (((y_rob - rd2+size_rob_d_2) == window_y[i])&&(window_c[i] == 1)){
                        //проверить есть ли окна на границе сверху r-size_rob

                        //проверить пренадлежит ли это окно другой локации !!!!!!!!

                        //if (window_c[i] == 1) break;
                        //если клетка лежит на границе то помечаем ее единицей
                        Locations[countSteps].windowCheck[i] = 1;

                        for (int j = 0; j < countLocations+1; j++){
                            if (((Locations[j].x_robot == window_x[i])&&(Locations[j].y_robot == window_y[i]))){
                                flagWas = 1; //мы уже там были
                            }

                            for (int k = 0; k < Locations[j].countW; k++){
                                if (((Locations[j].windowX[k] == window_x[i])&&(Locations[j].windowY[k] == window_y[i])&&(Locations[j].windowCheck[k]==0))){
                                    flagWas = 1; //мы уже там были
                                }
                            }
                        }

                        if ((flagWas==0)&&(flag == 0)){
                            flag = 1; //В эту ячейку можно сдвинуться
                            //сдвинуться;

                            x_rob = window_x[i];
                            y_rob = window_y[i];

                            prevLoc = countSteps;
                            countSteps = countLocations+1;
                            Locations[countSteps].prevLoc = prevLoc;
                            countLocations++;
                            flagWas = 0;
                            targetWindow = i;
                        }
                        else {
                            flagWas = 0;
                        }
                    }
                }


            if (flag == 0) {

                //вернуться назад
                countSteps = Locations[countSteps].prevLoc;
                countLocations++;

                y_rob = Locations[countSteps].y_robot;
                x_rob = Locations[countSteps].x_robot;
            }
    return targetWindow;
}
void end(){
    for (int yy = 0; yy< 20; yy++){
        for (int xx = 0; xx< 20; xx++){
            screen_buffer[y_rob+yy][x_rob+xx] = 5; //отметим новую позицию робота в экранном буфере
        }
    }
}

void find_way(int itr){


    int iter = itr;
    //int number_max_xy = 0;

    //(iter == 0) go();
    //m1:
    //if(iter == 1) go_to_coord(window_y[number_max_xy], window_x[number_max_xy]);
    if(iter > 0) go_to_coord(y_rob, x_rob);
    else if(iter == 0 )go_to_coord(y_rob, x_rob);

    find_window();
    fillWindows();
    mark_window();
    //number_max_xy = check_window(); //выбор окна
    number_max_xy = depthWay();

    if (number_max_xy == -1) end();
    //makeLocation();

    mark_window_xy(number_max_xy);
    //print_screen_buf();

    //std::cin >> iter;

    //way_to_window(number_max_xy);
    //mark_way();

    print_screen_buf();
//!!!!!!!!!!!ЗАпомнить где были !!
    //mwindow_y[countmw] = window_y[number_max_xy];
    //mwindow_x[countmw] = window_x[number_max_xy];
    //countmw++;

    //std::cin >> iter;

    //goto m1;

//    go_to_coordinte();
//    print_way();
}

void OpenMap (void){
    FILE *fp;
    char name[]="map.txt";
    fp = fopen(name, "r");
    if(fp != NULL) { // открыть файл удалось?
        size_t count = 0;
        for (int y=0; y<size_map_y; y++)
        {
            for (int x=0; x<size_map_x; x++)
            {

                fread(&map[y][x],1, 1, fp);

            }
        }
    } else printf("Не удалось открыть файл");
    fclose(fp);


//    uint8_t c; // буфер промежуточного хранения считываемого из файла текста
//    std::ifstream fin("map_in", std::ios_base::in | std::ios_base::binary); // открыли файл для чтения
//    //std::ofstream fout("map_out", std::ios_base::out | std::ios_base::binary);
//
//    int i = 1;
//    int countSkip2 = 56;
//    int countSkip1 = 44;
//    uint8_t swap = 0x00;
//
//    int x_1=0, x_2=0, x_3=0;
//    int y_1=0, y_2=0, y_3=0;
//
//    int countRazriad = 0;
//    int flagSecond = 0;
//
//    while(!fin.eof())
//    {
//        //45 элемент
//
//        c = fin.get();
//        if((i>countSkip1)){
//
//            if (c==0x0a) break;
//
//            if((c != 0x20) && (flagSecond==0)){
//                if(countRazriad == 0){
//                    x_1 = c; //старший разряд числа
//                    countRazriad++;
//                }
//                else if(countRazriad == 1){
//                    x_2 = c;
//                    countRazriad++;
//                }
//                else if(countRazriad == 2){
//                    x_3 = c;
//                    countRazriad++;
//                }
//            }
//            else {
//                flagSecond = 1;
//                countRazriad = 0;
//            }
//
//            if((c != 0x20) && (flagSecond==1)){
//                if(countRazriad == 0){
//                    y_1 = c; //старший разряд числа
//                    countRazriad++;
//                }
//                else if(countRazriad == 1){
//                    y_2 = c;
//                    countRazriad++;
//                }
//                else if(countRazriad == 2){
//                    y_3 = c;
//                    countRazriad++;
//                }
//            }
//        }
//
//        i++;
//        std::cout << i << std::endl;
//    }
//
//    while(!fin.eof())
//    {
//
//        c = fin.get();
//        if (c == 0xcd) break;
//    }
//
//    for (int y=0; y<size_map_y; y++)
//    {
//            for (int x=0; x<size_map_x; x++)
//            {
//                c = fin.get();
//                if(c == 0xcd) map[y][x] = 49; //препятствие
//                else if(c == 0x00) map[y][x] = 49;
//                else if(c == 0xfe) map[y][x] = 48; //свободно
//
//            }
//    }
//
//    std::cout << i << std::endl;
//    fin.close();
}


//int main(int, char**){
//    if (SDL_Init(SDL_INIT_VIDEO) != 0){
//        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
//        return 1;
//    }
//
//
//
//    if (SDL_Init(SDL_INIT_VIDEO) != 0){
//        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
//        return 1;
//    }
//
//
//
//    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
//    if (win == nullptr){
//        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//
//
//    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    if (ren == nullptr){
//        SDL_DestroyWindow(win);
//        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//
//    std::string imagePath = "hello.bmp";
//    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
//    if (bmp == nullptr){
//        SDL_DestroyRenderer(ren);
//        SDL_DestroyWindow(win);
//        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//
//
//    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
//    SDL_FreeSurface(bmp);
//    if (tex == nullptr){
//        SDL_DestroyRenderer(ren);
//        SDL_DestroyWindow(win);
//        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//
//    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
//    for (int i = 0; i < 3; ++i){
//        //First clear the renderer
//        SDL_RenderClear(ren);
//        //Draw the texture
//        SDL_RenderCopy(ren, tex, NULL, NULL);
//        //Update the screen
//        SDL_RenderPresent(ren);
//        //Take a quick break after all that hard work
//        SDL_Delay(1000);
//    }
//
//
//
//    SDL_DestroyTexture(tex);
//    SDL_DestroyRenderer(ren);
//    SDL_DestroyWindow(win);
//    SDL_Quit();
//    return 0;
//}

//https://habrahabr.ru/post/200074/