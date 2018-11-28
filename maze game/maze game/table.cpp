#include "stdafx.h"
#include "table.h"
#include "maze game.h"


table::table(int q, int w) {
	this->width = q;
	this->height = w;
	group_count = 0;
	//punem in constructor faza cu initializarea
	   	 }

table::~table() {
	DeleteObject(this);
}
bool table::render(HDC handler) 
	{

	HBRUSH hBrush_wood = CreateSolidBrush(RGB(224,170,95));
	
	//draw empty table
	RECT temp_rect;
	
	for (int i = 1; i < this->width; ++i) {
		for (int j = 1; j < this->height; ++j) {
			temp_rect.left = i*SQUARESIZE;
			temp_rect.top = j* SQUARESIZE;
			temp_rect.right = i* SQUARESIZE + SQUARESIZE-1;
			temp_rect.bottom = j* SQUARESIZE + SQUARESIZE - 1;
			FillRect(handler, &temp_rect, hBrush_wood);
		}
	}

	DeleteObject(hBrush_wood);
	//draw small center points for 19x19 board
	if (this->width == 19 && this->height == 19) {
	
		RECT temp_rect;
		HBRUSH hBrush_ref_point = CreateSolidBrush(REF_POINT);
		SelectObject(handler, hBrush_ref_point);

		Ellipse(handler, 4*SQUARESIZE-REF_POINT_SIZE,4*SQUARESIZE- REF_POINT_SIZE, 4 * SQUARESIZE+ REF_POINT_SIZE, 4 * SQUARESIZE+ REF_POINT_SIZE);
		Ellipse(handler, 4 * SQUARESIZE - REF_POINT_SIZE, 10 * SQUARESIZE - REF_POINT_SIZE, 4 * SQUARESIZE + REF_POINT_SIZE, 10 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 4 * SQUARESIZE - REF_POINT_SIZE, 16 * SQUARESIZE - REF_POINT_SIZE, 4 * SQUARESIZE + REF_POINT_SIZE, 16 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 10 * SQUARESIZE - REF_POINT_SIZE, 4 * SQUARESIZE - REF_POINT_SIZE, 10 * SQUARESIZE + REF_POINT_SIZE, 4 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 10 * SQUARESIZE - REF_POINT_SIZE, 10 * SQUARESIZE - REF_POINT_SIZE, 10 * SQUARESIZE + REF_POINT_SIZE, 10 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 10* SQUARESIZE - REF_POINT_SIZE, 16 * SQUARESIZE - REF_POINT_SIZE, 10 * SQUARESIZE + REF_POINT_SIZE, 16 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 16 * SQUARESIZE - REF_POINT_SIZE, 4 * SQUARESIZE - REF_POINT_SIZE, 16 * SQUARESIZE + REF_POINT_SIZE, 4 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 16 * SQUARESIZE - REF_POINT_SIZE, 10 * SQUARESIZE - REF_POINT_SIZE, 16 * SQUARESIZE + REF_POINT_SIZE, 10 * SQUARESIZE + REF_POINT_SIZE);
		Ellipse(handler, 16 * SQUARESIZE - REF_POINT_SIZE, 16 * SQUARESIZE - REF_POINT_SIZE, 16 * SQUARESIZE + REF_POINT_SIZE, 16 * SQUARESIZE + REF_POINT_SIZE);
		
		
		
		DeleteObject(hBrush_ref_point);

	}


	//draw stones
	/*
	HBRUSH hBrush_white = CreateSolidBrush(WHITE_STONE);
	HBRUSH hBrush_black = CreateSolidBrush(BLACK_STONE);
	
	

	DeleteObject(hBrush_white);
	DeleteObject(hBrush_black);
	*/

	return 0;
	
}

bool table::render_stone(HDC handler, int a, int b, COLORREF color) {
	//aici trebuie scris si in array-ul tablei de joc si inainte trebuie verificat dc e liber, dc moare ceva si update la scor

		RECT temp_rect;
		HBRUSH hBrush_stone = CreateSolidBrush(color);
		SelectObject(handler, hBrush_stone);
		int center_x = b * SQUARESIZE;
		int center_y = a * SQUARESIZE;
		int raza = SQUARESIZE / 2;
		if (color==BLACK_STONE) this->table_array[a][b] = 1; // se ocupa celula cu o bila neagra =1 
		if (color == WHITE_STONE) this->table_array[a][b] = 2;// se ocupa celula cu o bila alba = 2
		Ellipse(handler, center_x - raza, center_y - raza, center_x + raza, center_y + raza);
		DeleteObject(hBrush_stone);
		
		
		
		
		return 0; //iesim fara erori


}


bool table::toggle_pos(int a, int b) { 
	if (this->table_array[a][b] != 0) {
		table_array[a][b] = 0;
	}

	else {
		table_array[a][b] = 1;
	}
	return 0; 
}


bool table::clear(HDC handler) {
	RECT temp_rect;
	HBRUSH hBrush_clear = CreateSolidBrush(WHITE_STONE);
	temp_rect.left = 0;
	temp_rect.top = 0;
	temp_rect.right = 600;
	temp_rect.bottom = 600;
	FillRect(handler, &temp_rect, hBrush_clear);

	DeleteObject(hBrush_clear);
	return 0;

}

// intoarce starea curenta a unei celule din tabela indiferent de stadiul jocului. 
// table_array este o reflectie in timp real a tablei de joc
// asta nu arata decat ca celula e ocupata. 
// mai tarziu pot face niste coduri de ex: alb: 0x01, negru 0x02 si nimic 0x00, sau KO: 0xFF <-0xFF arata tot o imposibilitate de 
//mutare in pozitia aia, doar ca celula este goala -> nu e nimic render in ea... 

//IMPORTANT: pentru determinarea grupurilor alive: 
// facem o lista cu grupuri neconectate si numaram libertati 
// statistic sunt maxim 30 grupuri alive sau 60 in seki. 
// la debut, fiecare piesa reprezinta un grup, pe masura ce se conecteaza grupurile se merge-uiesc si alcatuiesc un grup nou
// cu membri noi si cu un nou numar de libertati. 

//cand moare un grup, pozitiile sunt eliberate, scorul updatat, iar grupul eliminat din lista.



int table::get_table_value(int a, int b) {
	return this->table_array[a][b];


}

// 0 pozitia e libera
// 1 pozitia e ocupata de negru
// 2 pozitia e ocupata de alb 


void table::set_table_value(int a, int b) {
	this->table_array[a][b] = 1;


}
void table::clear_table_value(int a, int b) {
	this->table_array[a][b] = 0;

}

// functia asta si tot ce foloseste ea e futai major. 
// si e inca inca in stadiu de explorare
// cred ca determinarea chain-urilor trebuie facut recursiv si la fiecare 
//interatie verificat daca nu am mai trecut pe acolo

bool table::enumerate_groups() {
	
	// start with first row, first column 
	
	int member_count = 0;
	int temp_stone_color; // culoarea grupului pe care il evaluam curent
	bool stanga = FALSE, dreapta = FALSE, sus = FALSE, jos = FALSE;
	
	
	for (int a = 1; a <= 19; a++) {
		for (int b = 1; b <= 19; b++) {
			if (this->table_array[a][b] == 0) {
				//pozitia e goala trecem mai departe


			}
			else {

				temp_stone_color = this->table_array[a][b];
				//aici incepe nebunia 
				//pozitia nu e goala, trebuie sa apelam ceva recursiv si la final Doamne Ajuta, avem 
				//un vector cu grupuri ....

				//daca piesa nu exista in niciun grup 
				if (!this->get_stone_group(a, b)) {

					int group_id_gasit = 0;
					// acum vedem daca se lipeste de vreun grup deja existent 
					// (ca sa o adaugam in acel grup


					//ne uitam in dreapta
					if (this->table_array[a][b + 1] == temp_stone_color) {
						//am gasit o piesa la dreapta cu aceiasi culoare 


						group_id_gasit = this->get_stone_group(a, b + 1);

						//lipim piesa la grupul gasit	
						this->groups[group_id_gasit].member_count++;
						this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][1] = a;
						this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][2] = b;

						dreapta = TRUE;
					}

					//ne uitam in stanga
					if (this->table_array[a][b - 1] == temp_stone_color) {
						//am gasit o piesa la stanga cu aceiasi culoare 

						//acum o lipim doar daca grupul e diferit de ala de mai sus
						if (!this->get_stone_group(a, b - 1) == group_id_gasit && group_id_gasit == 0) {

							group_id_gasit = this->get_stone_group(a, b - 1);

							//lipim piesa la grupul gasit	
							this->groups[group_id_gasit].member_count++;
							this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][1] = a;
							this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][2] = b;

							stanga = TRUE;
						}
					}


					//ne uitam in jos
					if (this->table_array[a + 1][b] == temp_stone_color) {
						//am gasit o piesa in jos cu aceiasi culoare 

						//daca grupul nu este acelasi, cu cel de mai sus, doar atunci adaugam piesa
						if (!this->get_stone_group(a + 1, b) == group_id_gasit && group_id_gasit == 0) {
							//luam grupul nou 
							group_id_gasit = this->get_stone_group(a + 1, b);
							//lipim piesa la grupul gasit	
							this->groups[group_id_gasit].member_count++;
							this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][1] = a;
							this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][2] = b;
							jos = TRUE;
						}
					}

					//ne uitam in sus
					if (this->table_array[a - 1][b] == temp_stone_color) {
						//am gasit o piesa in sus cu aceiasi culoare 

						if (!this->get_stone_group(a - 1, b) == group_id_gasit && group_id_gasit == 0) {
							group_id_gasit = this->get_stone_group(a - 1, b);

							//lipim piesa la grupul gasit	
							this->groups[group_id_gasit].member_count++;
							this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][1] = a;
							this->groups[group_id_gasit].members[this->groups[group_id_gasit].member_count][2] = b;

							sus = TRUE;
						}
					}

					//daca in sfarsit, nu ne lipim de nici un grup, 
					//cream un grup nou si punem membru asta solo in el

					if (!dreapta && !stanga && !jos && !sus) {
						group_count++;

						//punem primul membru
						// la initializarea fiecarui grup, el este alive
						//
						this->groups[group_count].status = ALIVE;
						this->groups[group_count].member_count = 1;
						this->groups[group_count].group_color = temp_stone_color;
						//adaugam piesa curenta la un grup nou
						this->groups[group_count].members[1][1] = a;
						this->groups[group_count].members[1][2] = b;

						stanga = FALSE;
						dreapta = FALSE;
						jos = FALSE;
						sus = FALSE;
					}
				}

			}
		}
		// acum imaginea e destul de clara doar ca grupurile sunt deconectate
		// acum merge-uim grupurile
		// luam fiecare membru comun a cel putin 2 grupuri, eliminam membrul comun si copiem restul membrilor
		//nota bene: aia cu membrii comuni a picat. acum se face prin adiacenta. vezi functia get_similar_adjacent_groups()

		int temp_adjacent_group;

		for (int i = 1; i <= 1000; i++) {

			temp_adjacent_group=this->get_similar_adjacent_group(i);

			if (temp_adjacent_group != 0) {
				this->merge_groups(temp_adjacent_group, i);


			}

			//acum ca le-am merge-uit, le updatam si libetatile 
			//functia asta intoarce si un int cu liberatile daca iti va trebui la ceva

		
	}

		

		

	}

	//facem dump sa vedem ce a iesit
	_RPT1(0, "\n#   cnt adj  lib  status  _____members_____________________________","");
	for (int i = 1; i < 20; i++) {
		_RPT1(0, "\n#%d ", i);
		_RPT1(0, "   %d ", groups[i].member_count);
		_RPT1(0, " %d ", this->get_similar_adjacent_group(i));
		_RPT1(0, " %d ", groups[i].liberties);
		_RPT1(0, "   %s ", this->groups[i].status==1?"ACTIVE":"DEFUNCT");


		for (int j = 1; j <= groups[i].member_count; j++) {
			_RPT1(0, "%d,", groups[i].members[j][1]);
			_RPT1(0, "%d;", groups[i].members[j][2]);

		}


	}

	return 0;
}

int table::get_stone_group(int a, int b) {
	for (int i = 1; i <= 1000 /*group count aici */; i++) {
		for (int j = 1; j <= groups[i].member_count; j++) {

			if (groups[i].members[j][1] == a && groups[i].members[j][2] == b) return i;

		}
	}
	return 0; //intorce 0 daca nu a gasit nimic
}

int table::get_similar_adjacent_group(int group_id) {
	//luam fiecare membru din grupul care ni s-a pasat
	//si vedem daca avem ceva pe langa noi
	//daca nu, e libertate //o numaram cu alta functie
	//daca da, si e din acelasi grup nu ne intoarcem pe noi insine
	//daca da dar e din alt grup si aceiasi culoare, inseamna ca am dat de un grup care trebuie lipit 

	int a;
	int b;
	int temp_group;
	int temp_color;
	//luam fiecare membru din grupul care ni s-a dat
	for (int i = 1; i <= this->groups[group_id].member_count; i++) {
		a = this->groups[group_id].members[i][1];
		b = this->groups[group_id].members[i][2];
		
		
		//ne uitam dupa un grup in stanga
		temp_group=this->get_stone_group(a - 1, b);
		temp_color = this->get_group_color(temp_group);
		if (temp_group != group_id && temp_group != 0 &&temp_color==this->get_group_color(group_id)) return temp_group;

		//ne uitam dupa un grup in dreapta
		temp_group = this->get_stone_group(a + 1, b);
		temp_color = this->get_group_color(temp_group);
		if (temp_group != group_id && temp_group != 0 && temp_color == this->get_group_color(group_id)) return temp_group;

		//ne uitam dupa un grup in jos
		temp_group = this->get_stone_group(a, b+1);
		temp_color = this->get_group_color(temp_group);
		if (temp_group != group_id && temp_group != 0 && temp_color == this->get_group_color(group_id)) return temp_group;
		
		//ne uitam dupa un grup in sus
		temp_group = this->get_stone_group(a, b - 1);
		temp_color = this->get_group_color(temp_group);
		if (temp_group != group_id && temp_group != 0 && temp_color == this->get_group_color(group_id)) return temp_group;


		}

	return 0;
}

//asta nu e suficient testata / pare ca merge
void table::merge_groups(int grup_sursa, int grup_destinatie) {
	int temp_a;
	int temp_b;
	for (int i = 1; i <= this->groups[grup_sursa].member_count; i++) {
		temp_a = this->groups[grup_sursa].members[i][1];
		temp_b = this->groups[grup_sursa].members[i][2];
		this->groups[grup_destinatie].member_count++;
		this->groups[grup_destinatie].members[this->groups[grup_destinatie].member_count][1] = temp_a;
		this->groups[grup_destinatie].members[this->groups[grup_destinatie].member_count][2] = temp_b;

	}
	//am copiat membrii
	//acum distrugem si invalidam sursa
	this->groups[grup_sursa].member_count = 0;
	this->groups[grup_sursa].status = DEAD;


}


int  table::get_group_color(int group_id) {
	return groups[group_id].group_color;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//asta nu functioneaza clar pentru ca nu tine cont de libertati comune obtinute din intersectia altora
//functia intoarce o tona de duplicate, care vor trebui eliminate intr-un episod viitor
int table::update_group_liberties(int group_id) {
	int temp_liberties = 0;
	int temp_a;
	int temp_b;
	for (int i = 1; i <= groups[group_id].member_count; i++) {
		temp_a = this->groups[group_id].members[i][1];
		temp_b = this->groups[group_id].members[i][1];

		//sus
		if (this->table_array[temp_a - 1][temp_b] == 0)  temp_liberties++;
		if (this->table_array[temp_a + 1][temp_b] == 0)  temp_liberties++;
		if (this->table_array[temp_a][temp_b + 1] == 0)  temp_liberties++;
		if (this->table_array[temp_a][temp_b - 1] == 0) temp_liberties++;

		groups[group_id].liberties = temp_liberties;
		return  temp_liberties;

	}

}