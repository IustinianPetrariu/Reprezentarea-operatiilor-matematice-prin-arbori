/*
****************************
*******Proiect IP-2020******
*Petrariu Florin-Iustinian *
*to do list:ExpresieCorecta() ******
*bugs:-                    *
****************************
*/
#include<windows.h>
#include<mmsystem.h>
#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include <cstring>
#define NMAX 55
using namespace std;
char matrice[NMAX][NMAX],stiva[NMAX][NMAX],postfixat[NMAX][NMAX],sir[NMAX];
char operatori[]="+-*/^";
int numarCuvinte,index,lungime,lenght,afisare_warning,am_desenat,culoareAleasa1,culoareAleasa2;
void deschideProgram();
void MeniuPrincipal();
struct nod
{
    char info[11];
    nod *st;
    nod *dr;
};
nod*arbore;
struct buton
{
    char text[NMAX];
    int x1,y1,x2,y2;
} listaMeniu[NMAX];
char meniu[NMAX][NMAX]= {"START","INSTRUCTIUNI","IESIRE"};
void copiazaExpresie(char sir[NMAX]) /// primeste o expresie corecta si o copiaza in matricea char:matrice
{
    char aux[11];
    int N=strlen(sir);
    int i=0;
    numarCuvinte=0;
    while(i < N)
    {
        int contor=0;

        if(sir[i]=='-')   /// testez daca e minus unar
        {
            if(i==0 || sir[i-1]=='(' )
            {
                strcpy(aux,"minus");
                i++;
            }
            else if(sir[i-1]=='(' && sir[i+1]=='(')
            {
                strcpy(aux,"minus");
                i++;
            }
            else   /// nu este minus unar
            {
                aux[contor]=sir[i];
                contor++;
                aux[contor]=NULL;
                i++;
            }
        }

        else if(sir[i] <='9' && sir[i] >='0') /// daca este un numar de tip: int,float;
        {
            while(sir[i] <='9' && sir[i] >='0' && i < N || sir[i]=='.' || sir[i]==',')
            {
                aux[contor]=sir[i];
                contor++;
                i++;
            }
            aux[contor]=NULL;
        }
        else if(sir[i] <='z' && sir[i] >='a') /// DACA ESTE UN OPERATOR:sin,cos,ln,abs,tg,rad;
        {
            while(sir[i] <='z' && sir[i] >='a' && i < N)
            {
                aux[contor]=sir[i];
                contor++;
                i++;
            }
            aux[contor]=NULL;
        }
        else  /// DACA ESTE OPERATOR:+*/^
        {
            aux[contor]=sir[i];
            contor++;
            aux[contor]=NULL;
            i++;
        }
        strcpy(matrice[numarCuvinte],aux);
        numarCuvinte++;
    }
}
int prioritateOperator(char Operator[5])
{
    if(strlen(Operator) <=1)
    {
        if(Operator[0] =='(' || Operator[0] == ')')
            return 0;
        if(Operator[0] =='+' || Operator[0] =='-')
            return 1;
        if(Operator[0] =='*' || Operator[0] =='/')
            return 2;
        if(Operator[0] =='^')
            return 3;
        if(Operator[0] <='z' && Operator[0] >='a')   /// daca este operand de lungime 1
            return -1;
    }
    if(Operator[0] <='9' && Operator[0] >='0') return -1; /// este valoare numerica;
    return 4; /// daca este una din functiile : sin,cos,tg,abs,ln,rad;
}
void formaPostFixata()
{
    index=0;
    copiazaExpresie(sir);
    int st=0; /// variabila pentru dimensiunea stivei;
    strcpy(stiva[st],"(");
    for(int i = 0 ; i < numarCuvinte ; i++)
    {
        if(matrice[i][0] <='9' && matrice[i][0] >= '0') /// DACA ESTE NUMAR
        {
            strcpy(postfixat[index],matrice[i]);
            index++; /// variabila pentru dimensiunea stivei in forma poloneza postfixata;
        }
        else if(strlen(matrice[i])==1 && matrice[i][0] <='z' && matrice[i][0] >='a') /// DACA ESTE OPERAND DE LUNGIME 1
        {
            strcpy(postfixat[index],matrice[i]);
            index++; /// variabila pentru dimensiunea stivei in forma poloneza postfixata;
        }
        else
        {
            if(strcmp(matrice[i],"(")==0)
            {
                st++;
                strcpy(stiva[st],matrice[i]);
            }
            else if(prioritateOperator(stiva[st]) < prioritateOperator(matrice[i]))
            {
                st++;
                strcpy(stiva[st],matrice[i]);
            }
            else if(prioritateOperator(stiva[st]) >= prioritateOperator(matrice[i]) && strcmp(matrice[i],")")!=NULL)
            {
                while(prioritateOperator(stiva[st]) >= prioritateOperator(matrice[i]))
                {
                    strcpy(postfixat[index],stiva[st]);
                    index++;
                    st--;
                }
                st++;
                strcpy(stiva[st],matrice[i]);
            }
            else if(strcmp(matrice[i],")")==0)
            {
                while(prioritateOperator(stiva[st]) != 0)
                {
                    strcpy(postfixat[index],stiva[st]);
                    index++;
                    st--;
                }
                st--;
            }
        }
    }
    while(st >0)
    {
        strcpy(postfixat[index],stiva[st]);
        index++;
        st--;
    }
    lungime=index-1;
}
void creeazaArbore(nod*&arbore)
{
    if(lungime >= 0)
    {
        char copieElement[11];
        strcpy(copieElement,postfixat[lungime]);
        lungime--;
        if(prioritateOperator(copieElement) == -1 ) /// daca este valoare numerica sau variabila de lungime 1;
        {
            arbore=new nod;
            strcpy(arbore->info,copieElement);
            arbore->st=NULL;
            arbore->dr=NULL;
        }
        else
        {
            arbore=new nod;
            if(strcmp(copieElement,"minus")==0) /// daca este minus unar
                strcpy(arbore->info,"-");
            else strcpy(arbore->info,copieElement);
            if(prioritateOperator(copieElement) == 4) /// e una din functiile unare: minus,sin,cos,tg,abs,rad,ctg
            {
                arbore->st=NULL;
                creeazaArbore(arbore->dr);
            }
            else
            {
                creeazaArbore(arbore->dr);
                creeazaArbore(arbore->st);
            }
        }
    }
}
void deseneazaCerc(nod *arbore,int centrux, int centruy, int raza, int adancime, int lungime,int directie)
{
    char aux[11];
    strcpy(aux,arbore->info);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    if(culoareAleasa2==1)
    {
        setcolor(GREEN);
        setbkcolor(BLACK);
        outtextxy(centrux,centruy,aux);
    }
    else if(culoareAleasa2==2)
    {
        setcolor(RED);
        setbkcolor(BLACK);
        outtextxy(centrux,centruy,aux);
    }
    else if(culoareAleasa2==3)
    {
        setcolor(BLUE);
        setbkcolor(BLACK);
        outtextxy(centrux,centruy,aux);
    }
    else if(culoareAleasa2==4)
    {
        setcolor(WHITE);
        setbkcolor(BLACK);
        outtextxy(centrux,centruy,aux);
    }

    if(culoareAleasa1==1)
    {
        setcolor(GREEN);
        setbkcolor(BLACK);
    }
    else if(culoareAleasa1==2)
    {
        setcolor(RED);
        setbkcolor(BLACK);
    }
    else if(culoareAleasa1==3)
    {
        setcolor(BLUE);
        setbkcolor(BLACK);
    }
    else if(culoareAleasa1==4)
    {
        setcolor(WHITE);
        setbkcolor(BLACK);
    }
    circle(centrux,centruy,raza);
    if(directie)
    {
        if(directie == 1)
        {
            line(centrux,centruy-raza,centrux+2*lungime+raza,centruy-adancime);
        }
        else
        {
            line(centrux,centruy-raza,centrux-2*lungime-raza,centruy-adancime);
        }
    }
}
void deseneazaArbore(nod *arbore, int centrux, int centruy, int raza, int adancime, int lungime, int directie)
{
    if(arbore->st != NULL)
        deseneazaArbore(arbore->st,centrux-raza-lungime,centruy+raza+adancime,raza,adancime,lungime/2,1);

    deseneazaCerc(arbore,centrux,centruy,raza,adancime,lungime,directie);
    if(arbore->dr != NULL)
        deseneazaArbore(arbore->dr,centrux+raza+lungime,centruy+raza+adancime,raza,adancime,lungime/2,2);
}
void scriePeEcran(char s[3], char c, int &deplasare)
{
    sir[lenght]=c;
    lenght++;
    sir[lenght]=NULL;
    s[0]=c;
    s[1]=NULL;
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
    setbkcolor(RED);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy((500+800)/2+deplasare,(210+300)/2,s);
    deplasare+=textwidth(s)+3;
}
void rescriePeEcran(char s[3],char c, int &deplasare)
{
    s[0]=c;
    s[1]=NULL;
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
    setbkcolor(RED);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy((500+800)/2+deplasare,(210+300)/2,s);
    deplasare+=textwidth(s)+3;

}
void afiseazaComenzi()
{
    setbkcolor(BLACK);
    cleardevice();
    readimagefile("arrow.jpg",60,50,110,90);
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setbkcolor(BLACK);
    outtextxy(700,200,"Introduceti expresia algebrica:");
    setfillstyle(SOLID_FILL,RED);
    bar(400,210,1000,300);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
    setbkcolor(RED);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,RED);
    bar(1100,210,1200,300);
    outtextxy((1100+1200)/2,(210+300)/2,"UNDO");
    bar(1250,210,1350,300);
    outtextxy((1250+1350)/2,(210+300)/2,"CLEAR");



}
void afiseazaEroarea(char error[NMAX])
{
    readimagefile("warning.jpg",200,afisare_warning-30,250,afisare_warning+10);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,2);
    settextjustify(LEFT_TEXT,CENTER_TEXT);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(260,afisare_warning,error);
    afisare_warning+=50;
}
int expresieCorecta(char sir[NMAX])
{
    int warning=0;
    afisare_warning=400;
    char functii_permise[11][11]= {"sin","cos","ln","tg","abs","rad","ctg"};
    char aux[11];
    if(lenght == 0)
    {
        afiseazaEroarea("-Expresia trebuie sa fie de lungime >=1");
        return 1;
    }
    if(strchr("+-*/.,?^(",sir[lenght-1])!=NULL)
    {
        warning=1;
        afiseazaEroarea("-Expresia nu se termina corect!.");
    }
    if(sir[lenght-1]<='z' && sir[lenght-1]>='a' && sir[lenght-2] <='z' && sir[lenght-2] >='a')
    {
        warning=1;
        afiseazaEroarea("-Expresia nu se termina corect!.");
    }
    for(int i = 0 ; i < lenght ; i++)   /// daca sunt caractere invalide
    {
        if(sir[i]>'z' )
        {
            warning=1;
            afiseazaEroarea("-Ati introdus simboluri invalide!");
            break;
        }
        else if(sir[i] <'a')
        {
            if((int)sir[i] < 40 || ((int)sir[i] > 57 && (int)sir[i] != 94))
            {
                warning=1;
                afiseazaEroarea("-Ati introdus simboluri invalide!");
                break;
            }
        }
    }
    /*
    for(int i = 0 ; i < lenght ; i++)
    {
        int virgula=0;
        if(sir[i] <='9' && sir[i] >='0')
            while(sir[i] <='9' && sir[i] >='0' && i < lenght) i++;
            if(sir[i]=='.' || sir[i]==',') virgula=1;
            if(sir[i+1] >'9' || sir[i+1] <'0')
            {
                warning=1;
                afiseazaEroarea("-Numarul introdus este invalid!");
                break;
            }
            i++;
         while(sir[i] <='9' && sir[i] >='0' && i < lenght) i++;
         if(sir[i]=='.' || sir[i]==',' && virgula==1)
             {
                warning=1;
                afiseazaEroarea("-Numarul introdus este invalid!");
                break;
             }
    }
    */
    int paranteze=0;
    for(int i = 0 ; i < lenght ; i++)   /// testam parantezarea
    {
        if(sir[i]=='(')
        {
            paranteze++;
            if(sir[i+1]==')')
            {
                paranteze=0;
                warning=1;
                afiseazaEroarea("-Expresie nu este parantezata corect;");
                break;
            }
        }

        else if(sir[i]==')')
        {
            paranteze--;
            if(sir[i+1]=='(')
            {
                paranteze=0;
                warning=1;
                afiseazaEroarea("-Expresie nu este parantezata corect;");
                break;
            }
            else if(paranteze < 0)
            {
                paranteze=0;
                warning=1;
                afiseazaEroarea("-Expresie nu este parantezata corect;");
                break;
            }
        }
    }
    if(paranteze != 0)
    {
        warning=1;
        afiseazaEroarea("-Expresie nu este parantezata corect;");
    }
    for(int i = 0 ; i < lenght ; i++)  /// testam pozitionarea operatorilor
    {
        if(sir[i] <='9' && sir[i]>='0')
        {
            if(sir[i+1]=='(' || sir[i-1]==')')
            {
                warning=1;
                afiseazaEroarea("-Operatorul de inmultire nu este introdus corect.Reciteste restrictiile si precizarile.");
                break;
            }
            if(sir[i+1]>='a' && sir[i+2] <'a')
            {
                warning=1;
                afiseazaEroarea("-Operatorul de inmultire nu este introdus corect.Reciteste restrictiile si precizarile.");
                break;
            }
            if(sir[i-1]>='a' && sir[i-2] <'a')
            {
                warning=1;
                afiseazaEroarea("-Operatorul de inmultire  nu este introdus corect.Reciteste restrictiile si precizarile.");
                break;
            }
        }
        else if(sir[i]<='z' && sir[i] >='a' && sir[i-1]<'a')
        {
            if(sir[i+1]=='(' || sir[i-1]==')')
            {
                warning=1;
                afiseazaEroarea("-Operatorul de inmultire nu este introdus corect.Reciteste restrictiile si precizarile.");
                break;
            }
        }
    }
    for(int i = 0 ; i < lenght ; i++)
        if((int)sir[i] >= 40 && (int)sir[i]<=45)
        {
            if(sir[i]==sir[i+1] && strchr("()",sir[i]) == NULL && strchr("()",sir[i+1])==NULL)
            {
                warning=1;
                afiseazaEroarea("-S-au introdus doi operatori egali pe pozitii consecutive.");
                break;
            }
            if(sir[i-1]=='(' && sir[i+1]==')')
            {
                warning=1;
                afiseazaEroarea("-Operatorul binar introdus trebuie sa fie inclus intre doi operanzi.");
                break;
            }
            if(strchr("+-*/^",sir[i])!= NULL && strchr("+-*/^",sir[i+1])!= NULL)
            {
                warning=1;
                afiseazaEroarea("-S-au introdus 2 operatori consecutivi fara parantezare.");
                break;
            }

        }
    for(int i = 0 ; i < lenght; i++) /// testam corectitudinea functiilor introduse
    {
        int indice=0;
        int functieCorecta=0;
        if(sir[i]<='z' && sir[i] >='a')
        {
            while(sir[i]<='z' && sir[i] >='a' && i < lenght)
            {
                aux[indice]=sir[i],indice++;
                i++;
            }
            aux[indice]=NULL;
            if(indice > 1)
            {
                for(int j = 0 ; j < 7 ; j++)
                {
                    if(strcmp(aux,functii_permise[j])==0)
                        functieCorecta=1;
                }
                if(!functieCorecta)
                {
                    warning=1;
                    afiseazaEroarea("-Functia introdusa nu este recunoscuta.");
                    break;
                }
            }
        }
    }
    return warning;
}
void deseneazaUndo(char mod[NMAX])
{
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
    setbkcolor(RED);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,RED);
    bar(1100,210,1200,300);
    if(strcmp(mod,"apasat")==NULL)
    {
        setcolor(GREEN);
        outtextxy((1100+1200)/2,(210+300)/2+4,"UNDO");
    }
    else
    {
        outtextxy((1100+1200)/2,(210+300)/2,"UNDO");
    }
}
int interiorButon(int x, int y)
{
    if(x <= 1350 && x >= 1250 && y >= 210 && y <= 300)
        return 1;
    return 0;
}
void stergeSubarbore(nod*& arbore)///sterge subarborele cu radacina r, cu exceptia radacinii
{
    if(arbore)
    {
        if (arbore->st != NULL)
            stergeSubarbore(arbore->st);
        delete arbore->st;
        if (arbore->dr != NULL)
        {
            stergeSubarbore(arbore->dr);
            delete arbore->dr;
        }
    }
    else return;
}
void creeazaButon(buton &b,char caractere[NMAX], int x0,int y0, int x1,int y1)
{
    strcpy(b.text,caractere);
    b.x1=x0;
    b.x2=x1;
    b.y1=y0;
    b.y2=y1;

}
void deseneazaButonMeniu(buton b)
{
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,1);
    setcolor(BLACK);
    setbkcolor(WHITE);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setfillstyle(SOLID_FILL,WHITE);
    bar(b.x1,b.y1,b.x2,b.y2);
    outtextxy((b.x1+b.x2)/2,(b.y1+b.y2)/2+2,b.text);
}
int interiorMeniu(int x, int y, buton b)
{
    return x <= b.x2 && x >= b.x1 && y <= b.y2 && y >= b.y1;
}
void afiseazaDinamicButon(buton b,int locatie)
{
    if(locatie == 0)
    {
        setfillstyle(SOLID_FILL,WHITE);
        bar(600,200,950,300);
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,4);
        setcolor(BLACK);
        setbkcolor(WHITE);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        outtextxy((b.x1+b.x2)/2,(b.y1+b.y2)/2+2,b.text);
    }
    if(locatie == 1)
    {
        setfillstyle(SOLID_FILL,WHITE);
        bar(600,301,950,401);
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,4);
        setcolor(BLACK);
        setbkcolor(WHITE);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        outtextxy((b.x1+b.x2)/2,(b.y1+b.y2)/2+2,b.text);

    }
    else if(locatie == 2)
    {
        setfillstyle(SOLID_FILL,WHITE);
        bar(600,402,950,502);
        settextstyle(SIMPLEX_FONT,HORIZ_DIR,4);
        setcolor(BLACK);
        setbkcolor(WHITE);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        outtextxy((b.x1+b.x2)/2,(b.y1+b.y2)/2+2,b.text);

    }
}
void instructiuni()
{
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();
    readimagefile("arrow.jpg",60,50,110,90);
    readimagefile("arborebinar.jpg",550,530,950,780);
    setcolor(WHITE);
    setbkcolor(BLACK);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR,2);
    outtextxy(750,180,"In matematica o expresie algebrica este o expresie construita din constante intregi, variabile si operatiuni algebrice.");
    outtextxy(335,220,"De exemplu, 3x² - 2xy + c este o expresie algebrica.");
    setcolor(RED);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR,3);
    outtextxy(650,260,"Restrictii si precizari asupra programului:");
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR,2);
    outtextxy(560,300,"-> in scrierea expresiei se vor folosi doar caracatere litere mici (a,b..z) si cifre (0,1,..9).");
    outtextxy(580,340,"-> sunt permisi urmatorii operatori: +,-,/,*,^ si functiile: sin,cos,tg,ctg,ln,abs,rad.");
    outtextxy(630,380,"-> sunt permise variabilele de lungime 1 ca fiind literele din alfabet: a,b,..z.");
    outtextxy(655,420,"-> pentru ca arborele sa fie afisat expresia introdusa trebuie sa fie corecta din punct de vedere matematic.");
    outtextxy(655,460,"-> in cazul in care expresia nu este corecta recititi restrictiile si erorile afisate pe ecran.");
    outtextxy(700,500,"->pentru folosirea minusului unar se vor utiliza paranteze corespunzatoare expresiei caruia i se adreseaza minusul.");

    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int x=mousex();
            int y=mousey();
            if(x >= 60 && x <= 110 && y >= 50 && y <= 90)
            {
                clearmouseclick(WM_LBUTTONDOWN);
                PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
                MeniuPrincipal();
                //continua=1;
            }
            else clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}
void Joaca()
{
    int apasat=0;
    do
    {
        if(interiorMeniu(mousex(),mousey(),listaMeniu[0]))
        {
            afiseazaDinamicButon(listaMeniu[0],0);
            while(interiorMeniu(mousex(),mousey(),listaMeniu[0]))
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
                    clearmouseclick(WM_LBUTTONDOWN);
                    deschideProgram();
                }
            deseneazaButonMeniu(listaMeniu[0]);
        }
        if(interiorMeniu(mousex(),mousey(),listaMeniu[1]))
        {
            afiseazaDinamicButon(listaMeniu[1],1);
            while(interiorMeniu(mousex(),mousey(),listaMeniu[1]))
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
                    clearmouseclick(WM_LBUTTONDOWN);
                    instructiuni();
                }
            deseneazaButonMeniu(listaMeniu[1]);
        }
        if(interiorMeniu(mousex(),mousey(),listaMeniu[2]))
        {
            afiseazaDinamicButon(listaMeniu[2],2);
            while(interiorMeniu(mousex(),mousey(),listaMeniu[2]))
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
                    clearmouseclick(WM_LBUTTONDOWN);
                    apasat=1;
                }
            deseneazaButonMeniu(listaMeniu[2]);
        }
        else if(ismouseclick(WM_LBUTTONDOWN))
            clearmouseclick(WM_LBUTTONDOWN);
    }
    while(!apasat);
}
void MeniuPrincipal()
{
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();
    int x1=600;
    int y1=200;
    int lungime=350;
    int latime=100;
    for(int i = 0 ; i < 3; i++)
    {
        setfillstyle(SOLID_FILL,WHITE);
        bar(x1,y1,x1+lungime,y1+latime);
        creeazaButon(listaMeniu[i],meniu[i],x1,y1,x1+lungime,y1+latime);
        deseneazaButonMeniu(listaMeniu[i]);
        y1=y1+latime+1;
    }
    Joaca();
}
void configureaza()
{
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();
    readimagefile("arrow.jpg",60,50,110,90);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(200,200,"Alege culoarea arborelui:");
    outtextxy(120,500,"Alege culoarea valorilor expresie:");
    setfillstyle(SOLID_FILL,GREEN);
    bar(600,150,700,250);
    setfillstyle(SOLID_FILL,RED);
    bar(800,150,900,250);
    setfillstyle(SOLID_FILL,BLUE);
    bar(1000,150,1100,250);
    setfillstyle(SOLID_FILL,WHITE);
    bar(1200,150,1300,250);


    setfillstyle(SOLID_FILL,GREEN);
    bar(600,450,700,550);
    setfillstyle(SOLID_FILL,RED);
    bar(800,450,900,550);
    setfillstyle(SOLID_FILL,BLUE);
    bar(1000,450,1100,550);
    setfillstyle(SOLID_FILL,WHITE);
    bar(1200,450,1300,550);

    int doubleClick=0,ales1=0,ales2=0;
    while(doubleClick < 2)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int x=mousex();
            int y=mousey();

            if(x <= 700 && x>=600 && y >= 150 && y <= 250 && !ales1)
                culoareAleasa1=1,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN),ales1=1,doubleClick++,outtextxy(300,350,"Ati ales culoarea verde.");
            else if(x <= 900 && x >= 800 && y >= 150 && y <= 250 && !ales1)
                culoareAleasa1=2,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN),ales1=1,doubleClick++,outtextxy(300,350,"Ati ales culoarea rosie.");
            else if(x <= 1100 && x >= 1000 && y >= 150 && y <= 250 && !ales1)
                culoareAleasa1=3,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN),ales1=1,doubleClick++,outtextxy(300,350,"Ati ales culoarea albastra.");
            else if(x <= 1300 && x >= 1200 && y >= 150 && y <= 250 && !ales1)
                culoareAleasa1=4,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN),ales1=1,doubleClick++,outtextxy(300,350,"Ati ales culoarea alba.");

            else if(x <= 700 && x >= 600 && y >= 450 && y <= 550 && !ales2)
                culoareAleasa2=1,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN && !ales2),ales2=1,doubleClick++,outtextxy(300,600,"Ati ales culoarea verde."),delay(900);
            else if(x <= 900 && x >= 800 && y >= 450 && y <= 550 && !ales2)
                culoareAleasa2=2,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN && !ales2),ales2=1,doubleClick++,outtextxy(300,600,"Ati ales culoarea rosie."),delay(900);
            else if(x <= 1100 && x >= 1000 && y >= 450 && y <= 550 && !ales2)
                culoareAleasa2=3,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN),ales2=1,doubleClick++,outtextxy(300,600,"Ati ales culoarea albastra."),delay(900);
            else if(x <= 1300 && x >= 1200 && y >= 450 && y <= 550 && !ales2)
                culoareAleasa2=4,PlaySound(TEXT("click.wav"),NULL,SND_SYNC),clearmouseclick(WM_LBUTTONDOWN),ales2=1,doubleClick++,outtextxy(300,600,"Ati ales culoarea alba."),delay(900);
            else if(x <= 110 && x >= 60 && y >= 50 && y <= 90)
                clearmouseclick(WM_LBUTTONDOWN),PlaySound(TEXT("click.wav"),NULL,SND_SYNC),deschideProgram();
            else clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}
void deschideProgram()
{
    afiseazaComenzi();
    char c;
    char s[3];
    int deplasare=0;
    int warning=0;
    int enter=0;
    int gata=1;
    lenght=0;
    do
    {
        if(kbhit())
        {
            c=getch();
            if((int)c != 13 && (int)c != 8) /// daca nu e avertisment,scrie pe ecran
            {
                scriePeEcran(s,c,deplasare);
                enter=0;
            }
            else
            {
                if((int)c==8) ///daca e backspace
                {
                    deseneazaUndo("apasat");
                    delay(100);
                    deseneazaUndo("relaxat");
                    if(lenght > 0)
                    {
                        enter=0;
                        lenght--;
                        sir[lenght]=NULL;
                        afiseazaComenzi();
                        deplasare=0;
                        for(int i = 0 ; i < lenght; i++)
                            rescriePeEcran(s,sir[i],deplasare);
                    }
                    else if(warning == 1)
                    {
                        warning=0;
                        afiseazaComenzi();
                    }
                }
                else
                {
                    if((int)c==13 && !enter)/// daca e enter
                    {
                        afiseazaComenzi();
                        deplasare=0;
                        for(int i = 0 ; i < lenght; i++)
                            rescriePeEcran(s,sir[i],deplasare);
                        warning=expresieCorecta(sir),enter++;
                    }
                    else if(enter==1)
                        afiseazaEroarea("-Trebuie sa corectati greselile!"),enter=2;
                }
            }
        }
        else
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                if(interiorButon(mousex(),mousey()))
                {
                    PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
                    clearmouseclick(WM_LBUTTONDOWN);
                    afiseazaComenzi();
                    deplasare=0;
                    sir[0]=NULL;
                    lenght=0;
                }
                else if(mousex()>=60 && mousex()<=110 && mousey()>=50 && mousey()<=90 )
                {
                    PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
                    clearmouseclick(WM_LBUTTONDOWN);
                    MeniuPrincipal();
                }
                else clearmouseclick(WM_LBUTTONDOWN);
            }
        }
    }
    while((int)c!=13 || warning == 1);
    sir[lenght]=NULL;
    formaPostFixata();
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,2);
    settextjustify(LEFT_TEXT,CENTER_TEXT);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(505,500,"Expresia introdusa este corecta!");
    delay(1500);
    configureaza();
    creeazaArbore(arbore);
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();
    deseneazaArbore(arbore,770,70,25,60,300,0);
    stergeSubarbore(arbore);
    readimagefile("arrow.jpg",60,50,110,90);
    int continuaDesen=0;
    while(continuaDesen==0)
    {
        if(kbhit())
        {
            char d;
            d=getch();
            continuaDesen=0;
        }
        else if(ismouseclick(WM_LBUTTONDOWN))
        {
            int x=mousex();
            int y=mousey();
            PlaySound(TEXT("click.wav"),NULL,SND_SYNC);
            clearmouseclick(WM_LBUTTONDOWN);
            if(x>=60 && x <= 110 && y >= 50 && y <= 90)
            {
                sir[0]=NULL;
                lenght=0;
                continuaDesen=1;
            }
        }
    }
    if(continuaDesen==1)
        deschideProgram();
}
void Loading()
{
    readimagefile("expresii.jpg",550,400,950,650);
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    setbkcolor(BLACK);
    outtextxy(750,250,"ARBORI BINARI DIN EXPRESII ALGEBRICE");
    line(350,270,1200,270);
    line(350,320,1200,320);
    line(350,270,350,320);
    line(1200,270,1200,320);
    delay(2000);
    bar(352,272,450,319);
    delay(250);
    bar(450,272,670,319);
    delay(250);
    bar(670,272,1198,319);
    delay(2000);

}
int main()
{
    initwindow(1540,800);
    //Loading();
    MeniuPrincipal();
    //getch();
    closegraph();
    return 0;
}
