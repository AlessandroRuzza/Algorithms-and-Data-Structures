#include "Autostrada.c"

#define AGG_STAZIONE 0
#define DEM_STAZIONE 1
#define AGG_AUTO 2
#define ROTTAMA_AUTO 3
#define PIANIFICA_PERCORSO 4
#define BREAK 5
#define END 6

Car CarCheck4689(Tree* Strada){
    Stazione staz = TreeSearch(Strada->Root, 4689);
    Car car = TreeSearch(staz->carTree.Root, 2170);
    return car;
}
void BreakPoint(Tree* Strada){
    char comment[200];
    scanf("%s", comment);
    fprintf(stderr, "%s\n", comment);
    //CarCheck4689(Strada);
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
    int param1, param2;
    bool result;

    scanf("%s", command);
    commandID = IdentifyCommand(command);

    switch(commandID){
        case AGG_STAZIONE: 
            scanf("%d", &param1);
            scanf("%d", &param2);
            result = AggiungiStazione(Strada, param1, param2);
            if(result) printf("aggiunta\n");
            else printf("non aggiunta\n");
            break;
        
        case DEM_STAZIONE:
            scanf("%d", &param1);
            result = DemolisciStazione(Strada, param1);
            if(result) printf("demolita\n");
            else printf("non demolita\n");
            break;

        case AGG_AUTO:
            scanf("%d", &param1);
            scanf("%d", &param2);
            result = AggiungiAuto(Strada, param1, param2);
            if(result) printf("aggiunta\n");
            else printf("non aggiunta\n");
            break;

        case ROTTAMA_AUTO:
            scanf("%d", &param1);
            scanf("%d", &param2);
            result = RottamaAuto(Strada, param1, param2);
            if(result) printf("rottamata\n");
            else printf("non rottamata\n");
            break;

        case PIANIFICA_PERCORSO:
            List* percorso;
            scanf("%d", &param1);
            scanf("%d", &param2);
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
}

int main(int argc, char const *argv[])
{
    Tree* Strada = (Tree*)malloc(sizeof(Tree));
    Strada->Root = NULL;
    //int i=1;
    while(!feof(stdin)){
        //fprintf(stderr, "LINE %d \n", i++);
        ParseInput(Strada);
    }

    ClearMemoryTree(Strada->Root);
    free(Strada);
    return 0;
}