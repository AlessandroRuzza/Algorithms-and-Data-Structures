#include "Autostrada.c"

#define AGG_STAZIONE 0
#define DEM_STAZIONE 1
#define AGG_AUTO 2
#define ROTTAMA_AUTO 3
#define PIANIFICA_PERCORSO 4

int IdentifyCommand(char command[25]){
    if(command[0] == 'a' && command[9] == 's')  return AGG_STAZIONE;
    if(command[0] == 'd')                       return DEM_STAZIONE;
    if(command[0] == 'a' && command[9] == 'a')  return AGG_AUTO;
    if(command[0] == 'r')                       return ROTTAMA_AUTO;
    if(command[0] == 'p')                       return PIANIFICA_PERCORSO;

    return -1; // se non c'è match con nessun comando
}
void ParseInput(Tree* Strada){
    char command[25];
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
            scanf("%d", &param1);
            scanf("%d", &param2);
            result = PianificaPercorso(Strada, param1, param2);
            if(!result) printf("nessun percorso\n");
            break;

        default: fprintf(stderr, "Comando non identificato!");
    }
}

int main(int argc, char const *argv[])
{
    Tree* Strada = (Tree*)malloc(sizeof(Tree));
    Strada->Root = NULL;
    int i=0;
    
    while(!feof(stdin)){
        printf("LINE %d\n", i++);
        ParseInput(Strada);
    }

    return 0;
}