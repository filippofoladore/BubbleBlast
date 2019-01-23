//BUBBLE BLAST --- FILIPPO FOLADORE --- MAT. 894669
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    //libreria per operare con valori booleani (vero/falso)
#include <time.h>       //libreria per l'uso della funzione di random

        //Definizione costanti
#define RIGHE 5         //numero di righe della matrice di gioco
#define COLONNE 6       //numero di colonne della matrice di gioco
#define MAX_BOLLE 7     //numero di bolle massimo con cui popolare la matrice
#define file_r "risultati.txt"

        //Definizione prototipi
//Funzioni matrici
    void creaGri(int griglia[][COLONNE]);   //fatto
    void stampaGri(int griglia[][COLONNE]); //fatto

//Funzioni grafica gioco
    void regole();                      //fatto
    void spazio();                      //fatto
    void pulisci();                     //fatto
    int difficolta();                   //fatto
    int stampaDiff(int difficolta());   //fatto
    int mosseBonus(int m);              //fatto

//algoritmo mosse da battere
    int soluzione(int griglia[][COLONNE], int r, int c);    //fatto

//gestione esplosione
    void esplosione(int griglia[RIGHE][COLONNE], int rig, int col); //fatto
    void propagaDx(int griglia[RIGHE][COLONNE], int r, int c);      //fatto
    void propagaGiu(int griglia[RIGHE][COLONNE], int r, int c);     //fatto
    void propagaSx(int griglia[RIGHE][COLONNE], int r, int c);      //fatto
    void propagaSu(int griglia[RIGHE][COLONNE], int r, int c);      //fatto

//inserimento valori generati random su matrice
    void inserisciBolle(int griglia[][COLONNE]);                        //fatto
    void copiaGri(void * grigliaDestinazione, void * grigliaOrigine);   //fatto
    bool vittoria(int griglia[][COLONNE]);                              //fatto

int main()
{
    //definizione variabili
    int griglia[RIGHE][COLONNE];        //matrice originaria
    int grigliaMod[RIGHE][COLONNE];     //matrice modificata al termine di ogni mossa
    int contComp = 0;                   //contatore mosse da battere
    int contGioc = 0;                   //contatore mosse effettuate dal giocatore
    bool end;                           //condizione di vittoria
    int giocR;                          //riga inserita dal giocatore
    int giocC;                          //colonna inserita dal giocatore
    int bonus = 0;                          //variabile per impostare la difficoltà
    char continua = "";                 //variabile per far continuare il gioco
    int livello = 1;                    //contatore di livelli completati
    bool fineGioco;                     //condizione per far finire il gioco per uscita/sconfitta
    bool sconfitta;                     //condizione della sconfitta per esaurimento mosse
    FILE *file_risultati;
    char risultati[128];
    bool vuoto;
    char nome_giocatore[30];
    time_t giorno;
    time(&giorno);
    struct tm* gioOut;
    gioOut = localtime(&giorno);
    char giornoMeseAnno[12];
    strftime(giornoMeseAnno,12,"%d-%m-%Y",gioOut);
    time_t ora;
    time(&ora);
    struct tm* oraOut;
    oraOut = localtime(&ora);
    char oraMinSec[12];
    strftime(oraMinSec, 12, "%H:%M:%S", oraOut);


    regole();   //stampa regole gioco
    impostaNome(nome_giocatore);
    bonus = difficolta();    //seleziona la difficoltà di gioco
    pulisci();
    attendi();

    do
    {
        contComp=0; contGioc=0;
        end = false;
        fineGioco = false;
        creaGri(griglia);
        inserisciBolle(griglia);
        pulisci();
        printf("Livello: %d", livello);
        spazio();
        copiaGri(grigliaMod, griglia); //copia la griglia di gioco nella griglia che verrà modificata
        stampaGri(griglia); //stampa la griglia di gioco
        contComp = soluzione(griglia, RIGHE, COLONNE);  //contatore delle mosse da battere
        printf("Mosse calcolate dall'algoritmo: %d\t\t", contComp); //stampa mosse da battere
        contComp += mosseBonus(bonus); //aggiunta mosse bonus
        printf("Mosse disponibili all'utente: %d (+%d)", contComp, mosseBonus(bonus)); //stampa mosse disponibili
        spazio();
        do
        {
            //inserimento riga + controllo validità input
            printf("Inserisci riga della bolla da colpire! (Ricorda: la prima riga e' la numero zero) --> ");
            scanf("%d", &giocR);
            if (giocR >= 0 && giocR < RIGHE){}else{
                printf("Errore! Hai inserito un numero fuori dalla griglia, sprecherai la mossa!\n");
            }

            //inserimento colonna + controllo validità input
            printf("Inserisci la colonna della bolla da colpire! (Ricorda: la prima colonna e' la numero zero) --> ");
            scanf("%d", &giocC);
            if (giocC >= 0 && giocC < COLONNE){}else{
                printf("Errore! Hai inserito un numero fuori dalla griglia, sprecherai la mossa!\n");
            }
            spazio();
            //stampa valori inseriti
            printf("Hai inserito: %d e %d\n\n", giocR, giocC);
            contGioc++;

            if (grigliaMod[giocR][giocC] == 1){ //caso: colpito un 1 --> esplosione
                printf("Hai colpito una bolla in procinto di esplodere! Pronti all'esplosione.\nBOOOM!\n");
                grigliaMod[giocR][giocC]--;
                esplosione(grigliaMod, giocR, giocC);
            } else if (grigliaMod[giocR][giocC] == 2){ //caso: colpito un 2 --> -1
                printf("Hai colpito una bolla gonfia a meta'! Ora e' in procinto di esplodere.\n");
                grigliaMod[giocR][giocC]--;
            } else if (grigliaMod[giocR][giocC] == 3){  //caso: colpito un 3 --> -1
                printf("Hai colpito una bolla sgonfia! Si gonfia a meta'.\n");
                grigliaMod[giocR][giocC]--;
            }else{printf("Bolla mancata!\n");} //caso: colpito uno 0

            stampaGri(grigliaMod);
            spazio();
            //stampa contatori mosse
            printf("Il tuo numero di mosse: %d\t Mosse da battere: %d\t Mosse rimanenti: %d\n\n", contGioc, contComp, contComp-contGioc);

            end = vittoria(grigliaMod);

        } while (end != true && contComp-contGioc > 0);
        if (end == true)
        {
                livello++;
            do
            {
                pulisci();
                printf("Hai terminato il livello %d, continua cosi'!\n", livello-1);
                printf("Vuoi continuare? Premi y per continuare oppure n per terminare. ");
                scanf("%c", &continua);
                if (continua == 'y')
                {attendi(); fineGioco = false; break;}
                else if (continua == 'n')
                {fineGioco = true;}
            }while (fineGioco != true);
        } else {printf("Hai perso!\n"); fineGioco = true; sconfitta = true;}
    } while (fineGioco != true);



    file_risultati = fopen(file_r, "a+");
    if (file_risultati == NULL)
        {
        printf("Impossibile aprire il file!\n");
        exit(1);
        } else
        {
            fprintf(file_risultati, "Utente: %s\tData: %s - %s\t Livelli completati: %d\n", nome_giocatore, giornoMeseAnno, oraMinSec, livello-1);
        }
    fclose(file_risultati);
}



//Definizione funzioni
    void regole(){
        //Funzione che stampa le regole del gioco
        printf("\tB U B B L E\t B L A S T");
        printf("\nBenvenuto!\nQuesto e' Bubble Blast!\n");
        printf("Lo scopo del gioco e' semplice: fai esplodere tutte le bolle prima di terminare i tentativi.\n");
        printf("Appena partira' il gioco vedrai una matrice con dei numeri al suo interno.\n");
        printf("I tipi di bolle che puoi trovare sono 3:\n \t 1) Bolla in procinto di esplodere;\n\t 2) Bolla gonfia a meta';\n\t 3) Bolla sgonfia.\n");
        printf("Quando colpirai una bolla sgonfia (3) la gonfierai a meta',\nquando colpirai una bolla gonfia a meta' la renderai in procinto di esplodere \nmentre quando colpirai una bolla in procinto di esplodere (1) la farai esplodere.\n");
        printf("Facendo esplodere una bolla di tipo 1 creerai un'esplosione che coinvolgera' le bolle nelle quattro direzioni rispetto a quella da te colpita!\n");
        printf("Ma attenzione! Le matrici partono da zero quindi se vuoi selezionare la prima colonna o la prima riga dovrai inserire 0 e non 1!\n");
        printf("Vinci quando fai esplodere tutte le bolle presenti sulla griglia, perdi se non riesci nei tentativi possibili.\n\nINIZIAMO!\n\n");
}

    void spazio(){
    //funzione che separa le varie mosse per l'ordine sulla console
    int i;
    printf("\n");
    for (i=0; i<30; i++)
        {
        printf("***");
    }
    printf("\n");
}

    void attendi(){
        //funzione che dopo aver pulito lo schermo fa comparire un messaggio di attesa
        printf("\nAttendi il carimento della griglia...\n\n");
}

    void pulisci(){
    //funzione che pulisce lo schermo per un maggior ordine
     system("@cls||clear");
}

    void creaGri(int griglia[RIGHE][COLONNE]){
    //funzione che inizializza la matrice a tutti zeri
    int i, j;
    for (i=0; i<RIGHE; i++){
        for (j=0; j<COLONNE; j++){
            griglia[i][j] = 0;
        }
    }
}

    void stampaGri(int griglia[RIGHE][COLONNE]){
    //funzione che stampa la matrice a schermo
    int i, j;
    printf("\n");
    for (i=0; i<RIGHE; i++){
        for (j=0; j<COLONNE; j++){
            printf("\t");
            printf("%d", griglia[i][j]);
        }
        printf("\n\n");
    }
}

    void inserisciBolle(int griglia[RIGHE][COLONNE]){
        //funzione che inserisce le bolle create tramite le funzione di generazione
        int bolle_max = MAX_BOLLE; //contatore che indica il numero massimo di bolle da inserire nella matrice
        do
        {
            int insRig; //variabile che contiene la riga generata dalla funzione creaRigaBolla
            int insCol; //variabile che contiene la colonna generata dalla funzione creaColonnaBolla
            int bolla;
            srand((unsigned)time(NULL));
            insRig = rand() % RIGHE;
            srand((unsigned)time(NULL));
            insCol = rand () % COLONNE;
            srand((unsigned)time(NULL));
            bolla = rand()%3+1;
            if (griglia[insRig][insCol] == 0){ //se l'elemento trovato è zero (quindi vuoto)
            griglia[insRig][insCol] = bolla; //inserisce nell'elemento dato dalle coordinate create una bolla con stato random
            bolle_max--; //diminuisce il contatore del numero massimo di bolle
            }
        } while (bolle_max != 0); //ripeti finchè le bolle disponibili non sono esaurite
}

    void copiaGri(void * grigliaDestinazione, void * grigliaOrigine){
        //funzione che alla fine di ogni mossa copia la matrice
        //memcpy(destinazione, origine, elementi da copiare, byte da trasferire) (tutorialspoint.com)
        memcpy(grigliaDestinazione, grigliaOrigine, RIGHE*COLONNE*sizeof(int));
        //non usando sizeof(int) nella visualizzazione viene errato, usandolo i valori copiati sono uguali
}

    int difficolta(){
    //funzione che regola la difficoltà del gioco diminuendo le mosse a disposizione
    int d;
    printf("Scegli la difficolta' di gioco.\nUna maggiore difficolta' prevede meno mosse a disposizione per far scoppiare tutte le bolle!\n");
    do
        {
        printf("Inserisci un numero da 1 a 3: \n\t 1) Facile; \n\t 2) Medio; \n\t 3) Difficile.\n");
        scanf("%d", &d);
        } while (d <= 0 || d>4);
        spazio();
        return d;
}

    int soluzione(int griglia[RIGHE][COLONNE], int r, int c){
        int mosse = 0; //contatore che contiene le mosse da battere
        bool finita;    //operatore usato come condizione nel ciclo per sapere quando fermarsi

        do
            {
                finita = true;
                for (r=0; r<RIGHE; r++){
                    for (c=0; c<COLONNE; c++)
                        if (griglia[r][c] == 1){
                            griglia[r][c]--;
                            esplosione(griglia, r, c);
                            mosse++;
                            finita = false;
                        }
                        else if (griglia[r][c] == 2 || griglia[r][c] == 3){
                            griglia[r][c]--;
                            mosse++;
                            finita = false;
                        }
            }
     }while (finita == false);
        return mosse;
 }

    bool vittoria(int griglia[RIGHE][COLONNE]){
        //funzione usata nel main per dare un valore a 'Vittoria' per decidere se il giocatore ha vinto o no
        //vinto = tutti zeri, non vinto = almeno uno diverso da zero
        int i, j;
        bool trovato = true; //inizializza l'operatore a vero
        for (i=0; i<RIGHE; i++){
            for(j=0; j<COLONNE; j++){
                if (griglia[i][j] != 0){trovato = false;} //cerca un qualsiasi elemento maggiore di zero quindi ritorna falso
            }
        }
        return trovato;
}

    int mosseBonus(int m){
        //imposta le mosse a seconda del valore di difficolta()
        int aggiungi;
        switch (m)
        {
            case 1 : aggiungi = 3; break;  //caso facile
            case 2 : aggiungi = 2; break;  //caso medio
            case 3 : aggiungi = 0; break;  //casio difficile
        }
        return aggiungi;
}

    void esplosione(int griglia[RIGHE][COLONNE], int rig, int col){
        //funzione che innesca l'esplosione sulle 4 coordinate rispetto alla cella selezionata
        esplosioneSu(griglia, rig, col);
        esplosioneGiu(griglia, rig, col);
        esplosioneDx(griglia, rig, col);
        esplosioneSx(griglia, rig, col);

}

    void impostaNome(char nome[30]){
        printf("Dammi il tuo nome! ");
        scanf("%s", nome);
        printf("Ciao %s! Benvenuto.\n", nome);
        return nome[30];
}

    void esplosioneSu (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoSu = false;

    if (rig > 0){
        rig--;
    do
        {
        if (griglia[rig][col] != 0) {bordoSu = true;
        if (griglia[rig][col] == 1) {griglia[rig][col]--; esplosione(griglia, rig++, col);}
        else {griglia[rig][col]--;}
        }else rig--;
        if (rig < 0)
           { bordoSu = true;}
        } while (bordoSu != true);
     } else return;
}

    void esplosioneGiu (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoGiu = false;

    if (rig < RIGHE-1){
        rig++;
    do
        {
        if (griglia[rig][col] != 0){bordoGiu = true;
        if (griglia[rig][col] == 1){griglia[rig][col]--; esplosione(griglia, rig--, col);}
        else {griglia[rig][col]--;}
        }else rig++;
        if (rig == RIGHE){ bordoGiu = true;}
        } while (bordoGiu != true);
     }else  return;
}

    void esplosioneDx (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoDestra = false;

    if (rig < COLONNE-1){    //se la cella non è nel bordo destro
        col++;              //fai partire la ricerca di un elemento diverso da 0 aumentano l'indice di colonna
    do
        {
        if (griglia[rig][col] != 0) {bordoDestra = true; //se l'elemento non è 0 controlliamo se è 1 o 2 o 3
        if (griglia[rig][col] == 1) {griglia[rig][col]--; esplosione(griglia, rig, col--);} //caso colpito un 1
        else {griglia[rig][col]--;} //caso colpito 2/3
        }else col++; //se non è ne 1 ne 2 ne 3 aumenta l'indice di colonna per vedere la prossima colonna
        if (col == COLONNE)    { bordoDestra = true;} //se la cella dopo aver aumentato il contatore è sul bordo destro
        } while (bordoDestra != true);  //esci da ciclo di ricerca
     }else  return; //se la cella è nel bordo destro esce dalla funzione
}

    void esplosioneSx (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoSinistra = false;

     if (rig > 0){
        col--;
    do
        {
        if (griglia[rig][col] != 0) {bordoSinistra = true;
        if (griglia[rig][col] == 1) {griglia[rig][col]--; esplosione(griglia, rig, col++);}
        else {griglia[rig][col]--;}
       } else col--;
        if (col < 0)    {bordoSinistra = true;}
        } while (bordoSinistra != true);
     }else  return;
}


