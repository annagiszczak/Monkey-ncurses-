#include <ncurses.h>
#include<iostream>
#include<fstream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

 

//globalne variables
bool koniecGry;
const int szerokosc=40;
const int wysokosc=40;
int len = 1;
int x,y, foodX, foodY, punkty, dlugosc, ogonX[100], ogonY[100];
enum kierunki{STOP=0, LEWO, PRAWO, GORA, DOL};
kierunki kierunek;


void wyswietl_punkty(){
    fstream plik;
    int n=50;
    int p=1;
	plik.open("score.txt", ios::in);
    int *tab=new int[n];
    clear();
    mvprintw(2,szerokosc/2,"Punkty, wyswietlane od najlepszego");
    for(int i=0;i<50;i++){
        plik>>tab[i];
    }
    for(int i=0;i<n;i++)
        for(int j=1;j<n-i;j++)
        if(tab[j-1]<tab[j])
            swap(tab[j-1], tab[j]);

    for(int i=0;i<50;i++){
        if(tab[i]!=0){
            mvprintw(i+3, szerokosc/2,"%d. %d", p, tab[i]);
            p++;
        }
    }
    refresh();
    sleep(10);
    plik.close();
    delete [] tab;
}

void score(){
    fstream fout_n;
	fout_n.open("score.txt", ios::app);
    fout_n<<punkty<<endl;
    fout_n.close();

}
void info(){

    int i=0;
    clear();
    while(i!=6){
        move(wysokosc/2,0);
        attrset (A_BOLD);
        printw ("W- gora \n S- dol \n D-prawo \n A-lewo \n DOBREJ ZABAWY!!!");
        mvprintw (wysokosc/2+7,szerokosc/2,"Malpki to super zwierzatka");
        mvprintw((wysokosc/2)+3,7+i+szerokosc/2,"@");
        refresh();
        sleep(1);
        i++;
    }
}


void poczatek(){
    int i=0;
    clear();
    while(i!=4){   
        move(wysokosc/2,szerokosc/2);
        attrset (A_BOLD);
        printw ("GRA W M@LPIEG@ WEZYKA");
        mvprintw((wysokosc/2)+3,szerokosc/2,"loading");
        mvprintw((wysokosc/2)+3,7+i+szerokosc/2,".");
        refresh();
        sleep(1);
        i++;
    }

}


void koniec(){

    int i=0;
    clear();
    while(i!=30){
        move(wysokosc/2,szerokosc/2);
        attrset (A_BOLD | A_UNDERLINE);
        printw ("END GAME");
        mvaddch(7,i, 'x');
        mvaddch(13,i+5,'o');
        mvaddch(25,szerokosc-i,'O');
        mvaddch(30,2+i,'A');
        refresh();
        usleep(90000);
        i++;
    }
}

void Zasady_poczatkowe(){
    
    koniecGry=false;
    kierunek=STOP;
    x=szerokosc/2;
    y=wysokosc/2;
    foodX=(rand()%(szerokosc-3))+1;
    foodY=(rand()%(wysokosc-3))+1;
    punkty=0;

}
void Rysuj(){

    clear();
    char znaczek=(char)176;
    char glowa='@';
    char ogon='o';
    char jedzenie='!';

    for(int i=0;i<szerokosc-1;i++)
    {
        move(0,i);
        addch(znaczek);

    }
    for(int i=0;i<wysokosc-1;i++)
    {
        move(i,0);
        addch(znaczek);
    }
    for(int i=0;i<szerokosc-1;i++)
    {
        move(wysokosc-2,i);
        addch(znaczek);
    }
    for(int i=0;i<wysokosc-1;i++)
    {
        move(i,szerokosc-2);
        addch(znaczek);
    } 
    move(y,x);
    addch(glowa);
    move(foodY,foodX);
    addch(jedzenie);

    attron(COLOR_PAIR(1));
    move(wysokosc,0);
    printw("Punkty: %d", punkty);
    mvprintw(wysokosc,szerokosc,"Quit: q");
    mvprintw(wysokosc+1,szerokosc,"Info: i");
    mvprintw(wysokosc+2,szerokosc,"Punkty: p");
    attroff(COLOR_PAIR(1));

    for(int i=0; i<dlugosc;i++){
        move(ogonY[i],ogonX[i]);
        addch(ogon);
    }
    
    refresh();
}
void Input(){
    switch(getch()){
    case 'a':
        kierunek = LEWO;
        break;
    case 'd':
        kierunek = PRAWO;
        break;
    case 'w':
        kierunek = GORA;
        break;
    case 's':
        kierunek = DOL;
        break;
    case 'q':
        koniecGry=true;
        break;
    case 'i':
        info();
        break; 
    case 'p':
        wyswietl_punkty();
        break;    
    }   
}

void Logika(){
    
    int poprzednieX=ogonX[0];
    int poprzednieY=ogonY[0];
    int poprzednie2X, poprzednie2Y;
    ogonX[0]=x;
    ogonY[0]=y;
    for(int i=1; i<dlugosc;i++){
        poprzednie2X=ogonX[i];
        poprzednie2Y=ogonY[i];
        ogonX[i]=poprzednieX;
        ogonY[i]=poprzednieY;
        poprzednieX=poprzednie2X;
        poprzednieY=poprzednie2Y;
    }

    switch(kierunek){
        case LEWO:
				x--;
            break;
        case PRAWO:
				x++;
            break;
        case GORA:
				y--;
            break;
        case DOL:
				y++;
            break;
        default:
            break;
    }

    if (x>=szerokosc-1) x=0; else if(x<0) x=wysokosc-1;
    if (y>=wysokosc-1) y=0; else if(y<0) y=szerokosc-1;

    for(int i=0; i<dlugosc;i++){
        if(ogonX[i]==x&& ogonY[i]==y)
            koniecGry=true;
    }
    if(x==foodX && y==foodY){
        punkty+=10;
        foodX=(rand()%(szerokosc-3))+1;
        foodY=(rand()%(wysokosc-3))+1;
        dlugosc++;
    }
}

int main(){

    //ustawienia
    initscr();
    nodelay(stdscr,true);
    keypad(stdscr,true);
    noecho();
    cbreak ();  
    nodelay (stdscr, TRUE);
    curs_set(0);
    srand(time(NULL));
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);

    //start
    poczatek();
    Zasady_poczatkowe();
    while(!koniecGry){
        Rysuj();
		usleep(100000);
        Input();
        Logika();
    }
    score();
    koniec();
    endwin ();
    return 1;
}
