#include <stdio.h>
#include <iostream>
#include <fstream>


void create_map(void);
void create_map_robot(void);
void print_map(void);
void print_map_rob(void);
void print_screen_buf();
void radar(void);
void map_robot_refresh(void);
void find_window(void);
void fillWindows(void);
void mark_window(void);
void screen_buffer_refresh();
void go_to_coord(int y, int x);
int max_elem(const int* arr, const int size);
int check_window();
void mark_window_xy(int number_max_xy);
void go(void);
void way_to_window(int number_max_xy);
void mark_way();
int depthWay();
void end();
void find_way(int itr);
void OpenMap (void);
