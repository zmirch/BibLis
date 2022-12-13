#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <winbgim.h>
#define tip int
#define EROARE 10000
#define MAX_INPUT_LEN 80
/*
char inputbuf[MAX_INPUT_LEN];
int input_pos = 0;
*/
bool eLista = 0, eStiva = 0, eCoada = 0, eDubla = 0;
bool fillBackground = 1;
using namespace std;
#define MAX 20
#define FUNDAL CYAN
int oldv,olda;
int height=GetSystemMetrics(SM_CYSCREEN); // inaltimea ecranului utilizatorului
int width=GetSystemMetrics(SM_CXSCREEN); // latimea ecranului utilizatorului
void refreshdisplay()
{
    oldv=getvisualpage();
    setvisualpage(oldv);
}
struct punct
{
    int x, y;
};
struct dreptunghi
{
    punct SS, DJ; // punctele de stanga sus, si dreapta jos
};
bool apartine(punct P, dreptunghi D)
{
    return D.SS.x<=P.x && P.x<=D.DJ.x && D.SS.y<=P.y && P.y<=D.DJ.y;
    // x stanga sus de dreptunghi <= x mouse, si x mouse <= x dreapta jos de dreptunghi
    //AND
    //y stanga sus de dreptunghi <= y mouse, si y mouse <= y dreapta jos de dreptunghi
}
struct buton
{
    dreptunghi D;
    int culoare;
    char text[20];
};
buton B_init[10], B_stiva[10], B_coada[10];
int nrButoaneInitial = 5, nrButoaneStiva = 5;
struct nod
{
    int info;
    nod * urm;
};
///Functii legate de coada
struct coada
{
    nod* prim;
    nod* ultim;
    int nrElemente;
};
bool esteVida(coada C)
{
    return C.nrElemente==0;
}
void initializeaza(coada& C)
{
    C.prim=NULL;
    C.ultim=NULL;
    C.nrElemente=0;
}
int peek(coada C)// ia informatia din primul nod al cozii, fara a-l elimina
{
    if (!esteVida(C))
        return C.prim->info;
    else
    {
        cerr<<"Eroare";
        return 0;
    }
}
int dequeue(coada &C)// scoate primul element din coada, din capul cozii
{
    if (!esteVida(C))
    {
        int element=peek(C);
        nod* primul;
        primul=C.prim;
        C.prim=C.prim->urm;
        delete primul;
        C.nrElemente--;
        return element;
    }
    else
    {
        cout<<"EROARE: coada goala. "<<endl;
        return 0;
    }
}
void goleste(coada &C)
{
    int element;
    while (!esteVida(C))
        element=dequeue(C);
}
void enqueue(coada &C, int element)// adauga un element la coada, dupa ultimul element (FIFO)
{

    nod * nod_nou;
    if (esteVida(C))
    {
        C.nrElemente=1;
        C.prim=new nod;
        C.prim->info=element;
        C.prim->urm=NULL;
        C.ultim=C.prim;
    }
    else
    {
        C.nrElemente++;
        nod_nou = new nod;
        nod_nou->info=element;
        nod_nou->urm=NULL;
        C.ultim->urm=nod_nou;
        C.ultim=nod_nou;
    }
}
void afiseaza(coada C)
{
    nod* nod_curent;
    nod_curent=C.prim;
    while (nod_curent!=NULL)
    {
        if (nod_curent->urm != NULL)
            cout<<nod_curent->info<<", ";
        else
            cout << nod_curent -> info;
        nod_curent=nod_curent->urm;
    }
    cout << endl;
}
void creareAleatoare(coada &C)
{
    int n=3+rand()%4;
    for (int i=1; i<=n; i++)
        enqueue(C,rand()%20-rand()%20);
}
/// Functii legate de stiva
struct stiva
{
    nod* varf;
    int nrElemente;
};
bool esteVida(stiva S)
{
    return S.nrElemente == 0;
}
void initializeaza(stiva& S)
{
    S.varf = NULL;
    S.nrElemente = 0;
}
int pop(stiva &S) // scoate elementul din varful stivei
{
    if (!esteVida(S))
    {
        int element;
        nod *varf_nou;
        element = S.varf -> info;
        varf_nou = S.varf -> urm;
        delete S.varf;
        S.varf = varf_nou;
        S.nrElemente--;
        return element;
    }
    else
    {
        cerr << "EROARE: stiva goala. ";
        return 0;
    }
}
void goleste(stiva &S)
{
    int element;
    while(!esteVida(S))
        element = pop(S);
}
void push(stiva &S, int element) // adauga un element la stiva, in varf (LIFO)
{
    nod *nod_nou;
    if (esteVida(S))
    {
        S.nrElemente = 1;
        S.varf = new nod;
        S.varf -> info = element;
        S.varf -> urm = NULL;
    }
    else
    {
        S.nrElemente++;
        nod_nou = new nod;
        nod_nou -> info = element;
        nod_nou -> urm = S.varf;
        S.varf = nod_nou;
    }
}
void afiseaza(stiva S)
{
    nod* nod_curent;
    nod_curent = S.varf;
    while (nod_curent != NULL)
    {
        if (nod_curent->urm != NULL)
            cout << nod_curent -> info<<",";
        else
            cout << nod_curent -> info;
        nod_curent = nod_curent->urm;
    }
    cout << endl;
}
void creareAleatoare(stiva &S)
{
    int n=3+rand()%4;
    for (int i=1; i<=n; i++)
        push(S,rand()%20-rand()%20);
}
///functii legate de liste dublu inlantuite
struct nodDub
{
    int info;
    nodDub * prec;
    nodDub * urm;
};

struct listaDubla
{
    nodDub* stanga;
    nodDub* prim;
    nodDub* curent;
    nodDub* ultim;
    nodDub* dreapta;
    int nrElemente;
};
bool esteVida(listaDubla L)
{
    return L.nrElemente==0;
}
void initializeaza(listaDubla& L)
{
    L.stanga=new nodDub;
    L.dreapta= new nodDub;
    L.stanga->info=0;
    L.dreapta->info=0;
    L.stanga->urm=L.dreapta;
    L.stanga->prec=NULL;
    L.dreapta->prec=L.stanga;
    L.dreapta->urm=NULL;
    L.curent=NULL;
    L.prim=NULL;
    L.ultim=NULL;
    L.nrElemente=0;
}
int scoatePrimul(listaDubla &L)
{
    int element;
    if (!esteVida(L))
        if (L.nrElemente==1)
        {
            delete L.prim;
            L.stanga->urm=L.dreapta;
            L.dreapta->prec=L.stanga;
            L.prim=NULL;
            L.ultim=NULL;
            L.nrElemente--;
        }
        else
        {
            element=L.prim->info;
            L.nrElemente--;
            L.stanga->urm=L.prim->urm;
            delete L.prim;
            L.prim=L.prim->urm;
        }
    else
    {
        cerr<<"EROARE. listaDubla e vida.";
        return 0;
    }
}
int scoateUltimul(listaDubla &L)
{
    int element;
    if (!esteVida(L))
        if (L.nrElemente == 1)
        {
            delete L.ultim;
            L.dreapta->prec=L.stanga;
            L.stanga->urm = L.dreapta;
            L.ultim = NULL;
            L.prim = NULL;
            L.nrElemente--;
        }
        else
        {
            element = L.ultim->info;
            L.nrElemente--;
            L.dreapta->prec = L.ultim->prec;
            delete L.ultim;
            L.ultim = L.ultim->prec;
        }
    else
    {
        cerr<<"EROARE. listaDubla e vida.";
        return 0;
    }
    return element;
}
void goleste(listaDubla &L)
{
    int element;
    while (!esteVida(L)) element=scoatePrimul(L);
}
void adaugaInFata(listaDubla &L, int element)
{
    nodDub * nod_nou;
    nod_nou = new nodDub;
    nod_nou->info=element;
    if (L.nrElemente>0)
    {
        nod_nou->urm=L.prim;
        L.prim->prec=nod_nou;
        nod_nou->prec=L.stanga;
        L.stanga->urm=nod_nou;
        L.prim=nod_nou;
        L.curent=L.prim;
    }
    else
    {
        nod_nou->urm=L.dreapta;
        nod_nou->prec=L.stanga;
        L.stanga->urm=nod_nou;
        L.dreapta->prec=nod_nou;
        L.prim=nod_nou;
        L.ultim=L.prim;
        L.curent=L.prim;
    }
    L.nrElemente++;
}
void adaugaLaSpate(listaDubla &L, int element)
{
    nodDub * nod_nou;
    nod_nou = new nodDub;
    nod_nou->info=element;
    if (L.nrElemente>0)
    {
        nod_nou->prec=L.ultim;
        L.ultim->urm=nod_nou;
        nod_nou->urm=L.dreapta;
        L.dreapta->prec=nod_nou;
        L.ultim=nod_nou;
        L.curent=L.ultim;
    }
    else
    {
        nod_nou->urm=L.dreapta;
        nod_nou->prec=L.stanga;
        L.stanga->urm=nod_nou;
        L.dreapta->prec=nod_nou;
        L.prim=nod_nou;
        L.ultim=L.prim;
        L.curent=L.prim;
    }
    L.nrElemente++;
}
void adaugaDupaCurent(listaDubla &L, int element)
{
    nodDub * nod_nou;
    nod_nou = new nodDub;
    nod_nou->info=element;
    nod_nou->prec=L.curent;
    nod_nou->urm=L.curent->urm;
    L.curent->urm->prec=nod_nou;
    L.curent->urm=nod_nou;
    L.curent=nod_nou;
    L.nrElemente++;
}
int scoateCurent(listaDubla &L)
{
    if (L.curent == L.prim)
    {
        int element;
        if (!esteVida(L))
            if (L.nrElemente==1)
            {
                delete L.prim;
                L.stanga->urm=L.dreapta;
                L.dreapta->prec=L.stanga;
                L.prim=NULL;
                L.ultim=NULL;
                L.nrElemente--;
            }
            else
            {
                element=L.prim->info;
                L.nrElemente--;
                L.stanga->urm=L.prim->urm;
                delete L.prim;
                L.prim=L.prim->urm;
            }
        else
        {
            cerr<<"EROARE. listaDubla e vida.";
            return 0;
        }
    }
    else if (L.curent == L.ultim)
    {
        nodDub * aux;
        aux = new nodDub;
        aux = L.curent;
        L.curent = L.curent->prec;
        L.stanga = L.curent->prec;
        L.dreapta = L.curent->urm;
        L.ultim = L.ultim->prec;
        delete aux;
        L.nrElemente--;
    }
    else
    {
        L.curent->prec->urm = L.curent->urm;
        L.curent->urm->prec = L.curent->prec;
        L.curent = L.curent->urm;
        L.nrElemente--;
    }
    // TODO: de implementat aceasta functie
}
void creareAleatoare(listaDubla &L)
{
    int n=3+rand()%4;
    int i;
    for (i=1; i<=n; i++)
        adaugaLaSpate(L,rand()%20-rand()%20);
}
void afiseaza(listaDubla L)
{
    if (!esteVida(L))
    {
        nodDub * nod_curent;
        nod_curent=L.prim;
        while (nod_curent!=L.ultim->urm)//(nod_curent!=L.dreapta)
        {
            cout<<nod_curent->info<<",";
            nod_curent=nod_curent->urm;
        }
        cout<<endl;
    }
}
/// Functii legate de liste simplu inlantuite
struct listaSimpla
{
    nod* prim;
    nod* ultim;
    int nrElemente;
};
bool listaVida(listaSimpla L)
{
    return L.nrElemente == 0;
}
void initializareListaSimpla(listaSimpla &L)
{
    L.prim=NULL;
    L.ultim=NULL;
    L.nrElemente=0;
}
void adaugareLaInceput(listaSimpla &L, int element)
{
    nod *nod_nou;
    if(!listaVida(L))
    {
        L.nrElemente++;
        nod_nou=new nod;
        nod_nou->info=element;
        nod_nou->urm=L.prim;
        L.prim=nod_nou;
    }
    else
    {
        L.nrElemente=1;
        nod* aux = new nod;
        aux->info = element;
        aux->urm = NULL;
        L.prim=aux;
        L.ultim=L.prim;
    }
}
void adaugareLaFinal(listaSimpla &L, int element)
{
    nod *nod_nou;
    if(!listaVida(L))
    {
        L.nrElemente++;
        nod_nou=new nod;
        nod_nou->info=element;
        nod_nou->urm=NULL;
        L.ultim->urm=nod_nou;
        L.ultim=nod_nou;
    }
    else
    {
        L.nrElemente=1;
        L.ultim=new nod;
        L.ultim->info=element;
        L.ultim->urm=NULL;
        L.prim=L.ultim;
    }
}
void sumaLista(listaSimpla L)
{
    int s=0;
    nod * p;
    p=L.prim;
    if(!listaVida(L))
    {
        while(p)
        {
            s=s+p->info;
            p=p->urm;
        }
        cout<<"\nSuma este "<<s<<endl;
    }
    else
        cout<<"\nLista este vida. Suma este 0.\n";
}
void afisareLista(listaSimpla L)
{
    nod *p;
    if(listaVida(L))
        cout<<"\nLista este vida.\n";
    else
    {
        p=L.prim;
        while(p)
        {
            cout<<p->info<<" ";
            p=p->urm;
        }
    }
}
void stergerePozitie(listaSimpla &L, int pozitie)
{
    if (listaVida(L))
    {
        cout << "\nLista vida. \n";
    }
    else if ((pozitie > L.nrElemente) || (pozitie < 0))
    {
        cout << "\nPozitia data nu exista. \n";
    }
    else
    {
        if(pozitie == 0)
        {
            nod *aux = L.prim;
            L.prim = L.prim->urm;
            delete aux;
            //cout<<"AICI";
            L.nrElemente--;
        }
        else if (pozitie == L.nrElemente - 1)
        {
            nod *p;
            p=L.prim;
            while(p->urm->urm)
            {
                p = p->urm;
            }
            p->urm = NULL;
            L.ultim = p;
        }
        else
        {
            nod *p;
            p=L.prim;
            for (int i = 1; i < pozitie; i++) // pana la pozitie, neinclusiv
            {
                p = p->urm;
            }
            p->urm = p->urm->urm;
            L.nrElemente--;
            while (p)
            {
                p = p->urm;
            }
            L.ultim = p;
            p = L.prim;
        }
    }
}
void golireLista(listaSimpla &L)
{
    L.prim->urm=NULL;
    L.prim = NULL;
    L.nrElemente = 0;
}
void deseneazaMeniulInitial()
{
    setcolor(WHITE);
    readimagefile("imagine3.jpg",0,0,width,height);
    int i;
    for (i=1; i<=nrButoaneInitial; i++)
    {
        B_init[i].D.SS.x=(GetSystemMetrics(SM_CXSCREEN)/3*2)-50;
        B_init[i].D.DJ.x=(GetSystemMetrics(SM_CXSCREEN)/3*2)+250;
        B_init[i].D.SS.y=(GetSystemMetrics(SM_CYSCREEN)/nrButoaneInitial*i)-150;
        B_init[i].D.DJ.y=(GetSystemMetrics(SM_CYSCREEN)/nrButoaneInitial*i)-50;
        switch(i)
        {
        case 1:
            strcpy(B_init[i].text,"Coada");
            break;
        case 2:
            strcpy(B_init[i].text,"Stiva");
            break;
        case 3:
            strcpy(B_init[i].text,"Lista Simplu Inlantuita");
            break;
        case 4:
            strcpy(B_init[i].text,"Lista Dublu Inlantuita");
            break;
        case 5:
            strcpy(B_init[i].text,"IESIRE");
            break;
        }
        rectangle(B_init[i].D.SS.x, B_init[i].D.SS.y,B_init[i].D.DJ.x,B_init[i].D.DJ.y);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        settextstyle(EUROPEAN_FONT,0, 2);
        outtextxy(B_init[i].D.SS.x +150,B_init[i].D.SS.y+55,B_init[i].text);
    }
}
int butonAles(buton B[])
{
    int i;
    punct p;
    while(true)
    {
        if(mousex()>=B[1].D.SS.x and mousex()<=B[1].D.DJ.x and mousey()>=B[1].D.SS.y and mousey()<=B[1].D.DJ.y)
        {
            setcolor(RED);
            rectangle(B[1].D.SS.x, B[1].D.SS.y,B[1].D.DJ.x,B[1].D.DJ.y);
            refreshdisplay();
        }
        else if(mousex()>=B[2].D.SS.x and mousex()<=B[2].D.DJ.x and mousey()>=B[2].D.SS.y and mousey()<=B[2].D.DJ.y)
        {
            setcolor(RED);
            rectangle(B[2].D.SS.x, B[2].D.SS.y,B[2].D.DJ.x,B[2].D.DJ.y);
            refreshdisplay();
        }
        else if(mousex()>=B[3].D.SS.x and mousex()<=B[3].D.DJ.x and mousey()>=B[3].D.SS.y and mousey()<=B[3].D.DJ.y)
        {
            setcolor(RED);
            rectangle(B[3].D.SS.x, B[3].D.SS.y,B[3].D.DJ.x,B[3].D.DJ.y);
            refreshdisplay();
        }
        else if(mousex()>=B[4].D.SS.x and mousex()<=B[4].D.DJ.x and mousey()>=B[4].D.SS.y and mousey()<=B[4].D.DJ.y)
        {
            setcolor(RED);
            rectangle(B[4].D.SS.x, B[4].D.SS.y,B[4].D.DJ.x,B[4].D.DJ.y);
            refreshdisplay();
        }
        else if(mousex()>=B[5].D.SS.x and mousex()<=B[5].D.DJ.x and mousey()>=B[5].D.SS.y and mousey()<=B[5].D.DJ.y)
        {
            setcolor(RED);
            rectangle(B[5].D.SS.x, B[5].D.SS.y,B[5].D.DJ.x,B[5].D.DJ.y);
            refreshdisplay();
        }
        else
        {
            setcolor(WHITE);
            setlinestyle(0,0,5);
            rectangle(B[1].D.SS.x, B[1].D.SS.y,B[1].D.DJ.x,B[1].D.DJ.y);
            rectangle(B[2].D.SS.x, B[2].D.SS.y,B[2].D.DJ.x,B[2].D.DJ.y);
            rectangle(B[3].D.SS.x, B[3].D.SS.y,B[3].D.DJ.x,B[3].D.DJ.y);
            rectangle(B[4].D.SS.x, B[4].D.SS.y,B[4].D.DJ.x,B[4].D.DJ.y);
            rectangle(B[5].D.SS.x, B[5].D.SS.y,B[5].D.DJ.x,B[5].D.DJ.y);
            refreshdisplay();
        }
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            p.x=mousex();
            p.y=mousey();
            for (i=1; i<=nrButoaneInitial; i++)
                if (apartine(p,B[i].D))
                    return i;
        }
    }
}
void  deseneazaMeniuFunctii()
{
    if (fillBackground)
    {
        setcolor(BLACK);
    rectangle(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
    setfillstyle(1,BLACK);
    bar(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
    }

    setcolor(WHITE);
    int i;
    punct p;
    for(i=1; i<=5; i++)
    {
        B_stiva[i].D.SS.x=(GetSystemMetrics(SM_CXSCREEN)/5*(i-1));
        B_stiva[i].D.DJ.x=(GetSystemMetrics(SM_CXSCREEN)/5*(i-1) + 230);
        B_stiva[i].D.SS.y=(GetSystemMetrics(SM_CYSCREEN) - 120);
        B_stiva[i].D.DJ.y=(GetSystemMetrics(SM_CYSCREEN) - 30);
        if(eStiva)
        {
            switch(i)
            {
            case 1:
                strcpy(B_stiva[i].text,"Adaugare");
                break;
            case 2:
                strcpy(B_stiva[i].text,"Eliminare");
                break;
            case 3:
                strcpy(B_stiva[i].text,"Creare Aleatoare");
                break;
            case 4:
                strcpy(B_stiva[i].text,"Golire");
                break;
            case 5:
                strcpy(B_stiva[i].text,"IESIRE");
                break;
            }
        }
        else if(eCoada)
            {
                switch(i)
                {
                case 1:
                    strcpy(B_stiva[i].text,"Adaugare");
                    break;
                case 2:
                    strcpy(B_stiva[i].text,"Eliminare");
                    break;
                case 3:
                    strcpy(B_stiva[i].text,"Creare Aleatoare");
                    break;
                case 4:
                    strcpy(B_stiva[i].text,"Golire");
                    break;
                case 5:
                    strcpy(B_stiva[i].text,"IESIRE");
                    break;
                }
            }
        else if(eLista)
            {
                switch(i)
                {
                case 1:
                    strcpy(B_stiva[i].text,"Adaugare Inceput");
                    break;
                case 2:
                    strcpy(B_stiva[i].text,"Adaugare Final");
                    break;
                case 3:
                    strcpy(B_stiva[i].text,"Elimina al X-lea");
                    break;
                case 4:
                    strcpy(B_stiva[i].text,"Golire");
                    break;
                case 5:
                    strcpy(B_stiva[i].text,"IESIRE");
                    break;
                }
            }
        else if(eDubla)
            {
                switch(i)
                {
                case 1:
                    strcpy(B_stiva[i].text,"Adaugare Inceput");
                    break;
                case 2:
                    strcpy(B_stiva[i].text,"Adaugare Final");
                    break;
                case 3:
                    strcpy(B_stiva[i].text,"Elimina Curent");
                    break;
                case 4:
                    strcpy(B_stiva[i].text,"Golire");
                    break;
                case 5:
                    strcpy(B_stiva[i].text,"IESIRE");
                    break;
                }
            }
            rectangle(B_stiva[i].D.SS.x, B_stiva[i].D.SS.y,B_stiva[i].D.DJ.x,B_stiva[i].D.DJ.y);
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(EUROPEAN_FONT,0, 2);
            outtextxy(B_stiva[i].D.SS.x +120,B_stiva[i].D.SS.y+55,B_stiva[i].text);
        }
    }
void deseneazaStiva(stiva S)
{
    setcolor(WHITE);
    int nr=0,ok;
    if(esteVida(S))
        outtextxy(width/2,height/2,"STIVA ESTE GOALA");
    else
    {
        nod* p;
        p=S.varf;
        while(p!=NULL)
        {
            int c=p->info, poz = 0, nrc = 0; //nrc = nr de cifre
            char valoare[100];
            while(c)
            {
                c /= 10;
                nrc++;
            }
            c=p->info;
            if (c > 0)
            {
                for(int i = nrc-1; i>=0; i--)
                {
                    valoare[i] = c%10 + 48; c /= 10;
                }
                valoare[nrc] = '\0';
            }
            else if (c<0)
            {
                c = abs(c);
                for(int i = nrc; i>0; i--)
                {

                    valoare[i] = c%10 + 48; c /= 10;
                }
                valoare[0] = '-';
                valoare[nrc+1] = '\0';
            }
            else
            {
                valoare[0] = '0';
                valoare[1] = '\0';
            }


            int lungime= textwidth(valoare);
            int inaltime= textheight(valoare);
            setlinestyle(0,0,3);
            if (p == S.varf)
            {
                setcolor(RED);
                rectangle(width/2-width/8,height/10*8-50*(S.nrElemente - nr)-25,width/2+width/8,height/10*8-50*(S.nrElemente - nr)+25);
                setcolor(WHITE);
            }
            else
                rectangle(width/2-width/8,height/10*8-50*(S.nrElemente - nr)-25,width/2+width/8,height/10*8-50*(S.nrElemente - nr)+25);
            settextstyle(EUROPEAN_FONT,0, 2);
            outtextxy(width/2,height/10*8-50*(S.nrElemente - nr),valoare);
            p=p->urm;
            nr++;
            settextstyle(EUROPEAN_FONT, 0, 2);
        }
    }
}
void animatieStivaAdaugare(stiva S)
{
    fillBackground = 0;
    nod * p = new nod();
    p = S.varf;
    int c=p->info, poz = 0, nrc = 0; //nrc = nr de cifre
            char valoare[100];
            while(c)
            {
                c /= 10;
                nrc++;
            }
            c=p->info;
            if (c > 0)
            {
                for(int i = nrc-1; i>=0; i--)
                {
                    valoare[i] = c%10 + 48; c /= 10;
                }
                valoare[nrc] = '\0';
            }
            else if (c<0)
            {
                c = abs(c);
                for(int i = nrc; i>0; i--)
                {

                    valoare[i] = c%10 + 48; c /= 10;
                }
                valoare[0] = '-';
                valoare[nrc+1] = '\0';
            }
            else
            {
                valoare[0] = '0';
                valoare[1] = '\0';
            }

    int x = width/2-width/8, nr = 0, y = 0;
    while(y < height/10*7-50*(S.nrElemente - nr)-25)
    {

        //refreshdisplay();
        deseneazaMeniuFunctii();
            setcolor(BLACK);
            setfillstyle(1,BLACK);
            bar(x-50,y,x+2*width/8 + 20,y+50);

            setcolor(RED);

            rectangle(x,y,x+2*width/8,y+50);
            setcolor(WHITE);
            //settextstyle(EUROPEAN_FONT,0, 2);
            outtextxy(x + width/8,y+25,valoare);
            //deseneazaStiva(S);
            y+=10;
            setcolor(BLACK);
            setfillstyle(1,BLACK);
            bar(x - 50, 0, x+2*width/8 + 20, y-10);
            setcolor(WHITE);
    }
    fillBackground = 1;
}
void animatieStivaEliminare(stiva S)
{
    fillBackground = 0;
    nod * p = new nod();
    p = S.varf;
    int c=p->info, poz = 0, nrc = 0; //nrc = nr de cifre
            char valoare[100];
            while(c)
            {
                c /= 10;
                nrc++;
            }
            c=p->info;
            if (c > 0)
            {
                for(int i = nrc-1; i>=0; i--)
                {
                    valoare[i] = c%10 + 48; c /= 10;
                }
                valoare[nrc] = '\0';
            }
            else if (c<0)
            {
                c = abs(c);
                for(int i = nrc; i>0; i--)
                {

                    valoare[i] = c%10 + 48; c /= 10;
                }
                valoare[0] = '-';
                valoare[nrc+1] = '\0';
            }
            else
            {
                valoare[0] = '0';
                valoare[1] = '\0';
            }

    int x = width/2-width/8, nr = 0, y = height/10*8-50*(S.nrElemente - nr)-25;
    while(x < width)
    {

        //refreshdisplay();
        deseneazaMeniuFunctii();
            setcolor(BLACK);
            setfillstyle(1,BLACK);
            bar(x-20,y-20,x+2*width/8 + 30,y+60);

            setcolor(RED);

            rectangle(x,y,x+2*width/8,y+50);
            setcolor(WHITE);
            //settextstyle(EUROPEAN_FONT,0, 2);
            outtextxy(x + width/8,y+25,valoare);
            //deseneazaStiva(S);
            x+=10;
            setcolor(BLACK);
            setfillstyle(1,BLACK);
            bar(x - 50, 0, x+2*width/8 + 20, y-10);
            setcolor(WHITE);
    }
    fillBackground = 1;
}
int main()
{
    initwindow(width,height);
    setlinestyle(0,0,1);
    deseneazaMeniulInitial();
    int comandaButon, butonul_apasat;
    do
    {
        butonul_apasat=butonAles(B_init);
        if (butonul_apasat!=0)
        {
            comandaButon=butonul_apasat;
            //cout<<"Comanda "<<comandaButon<<endl;
            if(butonul_apasat==2)
            {
                eStiva = 1;
                break;
            }
            else if (butonul_apasat == 1)
            {
                eCoada = 1;
                break;
            }
            else if (butonul_apasat == 3)
            {
                eLista = 1;
                break;
            }
            else if (butonul_apasat == 4)
            {
                eDubla = 1;
                break;
            }
        }
    }
    while (comandaButon!=5);
    if(eStiva)
    {
        stiva S;
        tip element;
        initializeaza(S);
        do
        {
            deseneazaMeniuFunctii();
            deseneazaStiva(S);
            butonul_apasat=butonAles(B_stiva);
            if (butonul_apasat!=0)
            {
                setcolor(WHITE);
                comandaButon=butonul_apasat;
                //cout<<"Comanda "<<comandaButon<<endl; // pentru testare in consola
                char*mesajAdaugare = "Element de adaugat: ";
                char*mesajEliminare = "Element de eliminat: ";
                //char*mesajInstructiune = "(apasati enter pentru a finisa introducerea input-ului)";
                int val;
                int c, posXText = 300, posYText = 30;
                int the_end = 0;
                char inputbuf[MAX_INPUT_LEN] = " ";
                int input_pos = 0;
                nod * aux = new nod();
                switch (butonul_apasat)
                {
                case 1:
                    outtextxy(140, 30, mesajAdaugare);
                    //outtextxy(450, 90, mesajInstructiune);
                    do
                    {
                        bar(250, 0, width, 60);
                       outtextxy (posXText,posYText, inputbuf);
                       //posXText+=7;
                       c = getch();
                       switch (c)
                       {
                            case 8: /* backspace */
                              if (input_pos)
                              {
                                  posXText-=7;
                                 input_pos--;
                                 inputbuf[input_pos] = 0;
                              }
                              break;
                            case 13: /* return */
                              the_end = 1;
                              break;
                            case 27: /* Escape = Abort */
                              inputbuf[0] = 0;
                              the_end = 1;
                              break;
                            default:
                              if (input_pos < MAX_INPUT_LEN-1 && c >= ' ' && c <= '~')
                              {
                                 if (((c>=48) && (c<=57))||(c == 45))
                                 {
                                     posXText+=7;
                                    inputbuf[input_pos] = c;
                                    input_pos++;
                                    inputbuf[input_pos] = 0;
                                 }
                              }
                       }
                    } while (!the_end);

                    aux->info = atoi(inputbuf);
                    aux->urm = S.varf;
                    S.varf = aux;
                    S.nrElemente++;
                    strcpy(inputbuf, "");
                    afiseaza(S);
                    animatieStivaAdaugare(S);
                    deseneazaStiva(S);
                    break;
                case 2:
                    animatieStivaEliminare(S);
                    pop(S);
                    afiseaza(S);
                    break;
                case 3:
                    creareAleatoare(S);
                    afiseaza(S);
                    break;
                case 4:
                    goleste(S);
                    afiseaza(S);
                default:
                    break;
                }
            }
        }
        while (comandaButon!=5);
    }
    else if (eCoada)
    {
        coada C;
        tip element;
        initializeaza(C);
        do
        {
            deseneazaMeniuFunctii();
            butonul_apasat=butonAles(B_stiva);
            deseneazaMeniuFunctii();
            if (butonul_apasat!=0)
            {
                setcolor(WHITE);

                //cout<<"Comanda "<<comandaButon<<endl; // pentru testare in consola
                char*mesajAdaugare = "Element de adaugat: ";
                //char*mesajEliminare = "Element de eliminat: ";
                //char*mesajInstructiune = "(apasati enter pentru a finisa introducerea input-ului)";
                int val;
                int c, posXText = 300, posYText = 30;
                int the_end = 0;
                char inputbuf[MAX_INPUT_LEN] = " ";
                int input_pos = 0;
                nod * aux = new nod();
                comandaButon=butonul_apasat;
                //cout<<"Comanda "<<comandaButon<<endl; // pentru testare in consola
                switch (butonul_apasat)
                {
                    case 1:
                    outtextxy(140, 30, mesajAdaugare);
                    //outtextxy(450, 90, mesajInstructiune);
                    do
                    {
                        bar(250, 0, width, 60);
                       outtextxy (posXText,posYText, inputbuf);
                       //posXText+=7;
                       c = getch();
                       switch (c)
                       {
                            case 8: /* backspace */
                              if (input_pos)
                              {
                                  posXText-=7;
                                 input_pos--;
                                 inputbuf[input_pos] = 0;
                              }
                              break;
                            case 13: /* return */
                              the_end = 1;
                              break;
                            case 27: /* Escape = Abort */
                              inputbuf[0] = 0;
                              the_end = 1;
                              break;
                            default:
                              if (input_pos < MAX_INPUT_LEN-1 && c >= ' ' && c <= '~')
                              {
                                 if (((c>=48) && (c<=57))||(c == 45))
                                 {
                                     posXText+=7;
                                    inputbuf[input_pos] = c;
                                    input_pos++;
                                    inputbuf[input_pos] = 0;
                                 }
                              }
                       }
                    } while (!the_end);

                    enqueue(C, atoi(inputbuf));

                    strcpy(inputbuf, "");
                    afiseaza(C);

                    break;
                case 2:
                    dequeue(C);
                    afiseaza(C);
                    break;
                case 3:
                    creareAleatoare(C);
                    afiseaza(C);
                    break;
                case 4:
                    goleste(C);
                    afiseaza(C);
                    break;
                }
            }
        }
        while (comandaButon!=5);
    }
    else if (eLista)
    {
        listaSimpla L;
        int element;
        initializareListaSimpla(L);
        do
        {
            deseneazaMeniuFunctii();
            butonul_apasat=butonAles(B_stiva);

            if (butonul_apasat!=0)
            {
                comandaButon=butonul_apasat;
                //cout<<"Comanda "<<comandaButon<<endl; // pentru testare in consola
                switch (butonul_apasat)
                {
                case 4:
                    golireLista(L);
                    afisareLista(L);
                    break;
                }
            }
        }
        while (comandaButon!=5);
    }
    else if (eDubla)
    {
        listaDubla L;
        int element;
        initializeaza(L);
        do
        {
            deseneazaMeniuFunctii();
            butonul_apasat=butonAles(B_stiva);

            if (butonul_apasat!=0)
            {
                comandaButon=butonul_apasat;
                //cout<<"Comanda "<<comandaButon<<endl; // pentru testare in consola
                switch(butonul_apasat)
                {
                case 4:
                    goleste(L);
                    afiseaza(L);
                    break;
                }
            }
        }
        while (comandaButon!=5);
    }
    //getch();
    closegraph();
    return 0;
}
