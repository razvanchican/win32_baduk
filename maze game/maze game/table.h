#pragma once



#include "stdafx.h"
#include "maze game.h"
#include "game_vector.h"
#include <crtdbg.h>

class table {

public:
	 table(int q, int w);
	~table();
	bool render(HDC handler);
	bool toggle_pos(int a, int b);
	bool init_empty_table();
	bool render_stone(HDC handler, int a, int b, COLORREF color);

	bool clear(HDC handler);
	int get_table_value(int a, int b);
	void set_table_value(int a, int b);
	void clear_table_value(int a, int b);
	bool enumerate_groups();   //creaza grupurile 
	
	//intoarce primul grup din care face parte o piesa
	int get_stone_group(int a, int b);
	int get_similar_adjacent_group(int group_id); //gaseste un grup adiacent de aceaisi culoare care trebuie lipit
	void merge_groups(int grup_sursa, int grup_destinatie); //sursa o baga in destinatie, si distruge sursa
	int get_group_color(int group_id); //intoarce culoarea unui grup dat;
	int update_group_liberties(int group_id); //numara libertatile grupului dat, updateaza libertatile in campurile grupului si apoi intorace numarul de libertati
private:
	int group_count;
	int table_array[MAX_TABLE_SIZE][MAX_TABLE_SIZE];
	int width;
	int height;
	group groups[255]; //maxim 255 grupuri / cu mult mai mult decat necesar
	DWORD click_pos;
protected:
};