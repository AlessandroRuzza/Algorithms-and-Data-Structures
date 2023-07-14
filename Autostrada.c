#include "Tree.c"

typedef Node* Stazione;
typedef Node* Car;

bool AggiungiStazione(Tree* Strada, int dist, int numAuto){
    if(TreeSearch(Strada->Root, dist) != NULL){
        return false;
    }
    Stazione nuovaStazione = MakeNode(dist);
    int autonomia=0;
    for(int i=0; i<numAuto; i++){
        scanf("%d", &autonomia);
        Car car = MakeCarNode(autonomia);
        printf("%d %d\n", i, autonomia);
        TreeInsert(&nuovaStazione->carTree, car);
    }
    nuovaStazione->max_car = TreeMax(nuovaStazione->carTree.Root)->dist;
    TreeInsert(Strada, nuovaStazione);
    return true;
}

bool DemolisciStazione(Tree* Strada, int dist){
    Stazione stazione = TreeSearch(Strada->Root, dist);
    if(stazione == NULL)
        return false;

    TreeDelete(Strada, stazione);
    return true;
}

bool AggiungiAuto(Tree* Strada, int dist, int autonomia){
    Stazione stazione = TreeSearch(Strada->Root, dist);
    if(stazione == NULL)
        return false;

    Car car = MakeCarNode(autonomia);
    TreeInsert(&stazione->carTree, car);
    return true;
}

bool RottamaAuto(Tree* Strada, int dist, int autonomia){
    Stazione stazione = TreeSearch(Strada->Root, dist);
    if(stazione == NULL)
        return false;

    Car car = TreeSearch(stazione->carTree.Root, autonomia);
    if(car == NULL)
        return false;
    
    TreeDelete(&stazione->carTree, car);
    return true;
}

bool PianificaPercorso(Tree* Strada, int start, int end){
    fprintf(stderr, "TEMP: TO IMPLEMENT");
    return false;
}
