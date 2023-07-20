#ifndef RELEASE
    #include "Autostrada.c"
#endif

#define AGG_STAZIONE 0
#define DEM_STAZIONE 1
#define AGG_AUTO 2
#define ROTTAMA_AUTO 3
#define PIANIFICA_PERCORSO 4
#define BREAK 5
#define END 6

Car CarCheck(Tree* Strada, int s, int c){
    Stazione staz = TreeSearch(Strada->Root, s);
    Car car = TreeSearch(staz->carTree.Root, c);
    return car;
}
void BreakPoint(Tree* Strada){
    char comment[200];
    int dump;
    dump=scanf("%s", comment);
    fprintf(stderr, "%s\n", comment);
    dump=dump;
    //CarCheck(Strada, 4689, 2110);
}

int IdentifyCommand(char command[25]){
    if(command[0] == 'a' && command[9] == 's')  return AGG_STAZIONE;
    if(command[0] == 'd')                       return DEM_STAZIONE;
    if(command[0] == 'a' && command[9] == 'a')  return AGG_AUTO;
    if(command[0] == 'r')                       return ROTTAMA_AUTO;
    if(command[0] == 'p')                       return PIANIFICA_PERCORSO;
    if(command[0] == 'b')                       return BREAK;
    if(command[0] == 0)                         return END;

    return -1; // se non c'è match con nessun comando
}
void ParseInput(Tree* Strada){
    char command[25] = {0};
    int commandID = -1;
    int param1, param2, dump=0;
    bool result; 
    List* percorso;

    dump=scanf("%s", command);
    commandID = IdentifyCommand(command);

    switch(commandID){
        case AGG_STAZIONE: 
            dump=scanf("%d", &param1);
            dump=scanf("%d", &param2);
            result = AggiungiStazione(Strada, param1, param2);
            if(result) printf("aggiunta\n");
            else printf("non aggiunta\n");
            break;
        
        case DEM_STAZIONE:
            dump=scanf("%d", &param1);
            result = DemolisciStazione(Strada, param1);
            if(result) printf("demolita\n");
            else printf("non demolita\n");
            break;

        case AGG_AUTO:
            dump=scanf("%d", &param1);
            dump=scanf("%d", &param2);
            result = AggiungiAuto(Strada, param1, param2);
            if(result) printf("aggiunta\n");
            else printf("non aggiunta\n");
            break;

        case ROTTAMA_AUTO:
            dump=scanf("%d", &param1);
            dump=scanf("%d", &param2);
            result = RottamaAuto(Strada, param1, param2);
            if(result) printf("rottamata\n");
            else printf("non rottamata\n");
            break;

        case PIANIFICA_PERCORSO:
            dump=scanf("%d", &param1);
            dump=scanf("%d", &param2);
            percorso = PianificaPercorso(Strada, param1, param2);
            if(percorso == NULL) printf("nessun percorso\n");
            else{ PrintList(percorso); ClearMemoryList(percorso); }
            break;

        case BREAK:
            BreakPoint(Strada);
            break;
        case END:
            break;

        default: fprintf(stderr, "Comando non identificato!\n");
    }
    dump=dump;
}

int main(int argc, char const *argv[])
{
    Tree* Strada = (Tree*)malloc(sizeof(Tree));
    Strada->Root = NULL;
    while(!feof(stdin)){
        ParseInput(Strada);
    }

    ClearMemoryTree(Strada->Root);
    free(Strada);
    return 0;
}