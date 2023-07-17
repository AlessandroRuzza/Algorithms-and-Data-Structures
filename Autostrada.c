#include "Tree.c"
#include "List.c"

typedef Node* Stazione;
typedef Node* Car;

bool AggiungiStazione(Tree* Strada, int dist, int numAuto){
    
    Stazione nuovaStazione = MakeNode(dist);
    int autonomia=0;
    for(int i=0; i<numAuto; i++){
        scanf("%d", &autonomia);
        Car car = MakeCarNode(autonomia);
        //printf("%d %d\n", i, autonomia);
        TreeInsert(&nuovaStazione->carTree, car);
    }
    if(numAuto > 0)
        nuovaStazione->max_car = TreeMax(nuovaStazione->carTree.Root)->dist;
    if(TreeSearch(Strada->Root, dist) != NULL){
        return false;
    }
    else{
        TreeInsert(Strada, nuovaStazione);
        return true;
    }
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
    if(car->dist > stazione->max_car) 
        stazione->max_car = car->dist;
        
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

bool CanReach(Node* from, Node* to){
    int diff = to->dist - from->dist;
    if(diff < 0) diff = -diff;  // diff = absolute value of distance between from and to
    return from->max_car >= diff;
}

List* PercorsoBackward(Tree* Strada, Node* start, Node*  end){
    fprintf(stderr, "TEMP: BACKWARD PATH NOT IMPLEMENTED\n");
    return NULL;
}

List* PercorsoForward(Node* start, Node* end){
    List* percorso;

    if(start == end){  // caso base
        percorso = (List*)malloc(sizeof(List));
        percorso->HEAD = NULL;
        percorso->TAIL = NULL;
    }
    else{   // start.dist < end.dist
        Node* prevTappa = NULL;
        Node* x = end;
        do{
            x = TreePredecessor(x);   
            if(CanReach(x, end))
                prevTappa = x;
        }while(x != start);
        // ora prevTappa è la stazione con la minore dist che può raggiungere end
        if(prevTappa == NULL){
            percorso = NULL; // se non esiste, non esiste percorso
        }
        else{   // se esiste, ripeto ricorsivamente tra prevTappa e end
            percorso = PercorsoForward(start, prevTappa);
        }
    }
    if(percorso != NULL)
        InsertTail(percorso, end);

    return percorso;
}

bool CheckPercorso(List* percorso){
    if(percorso == NULL) return true;

    ListNode* curr = percorso->HEAD;
    while (curr->next != NULL)
    {
        if(!CanReach(curr->s, curr->next->s)) 
            return false;
        curr = curr->next;
    }
    return true;
}

List* PianificaPercorso(Tree* Strada, int start, int end){
    List* percorso;
    Node* startNode = TreeSearch(Strada->Root, start);
    Node* endNode = TreeSearch(Strada->Root, end);
    if(startNode == NULL || endNode == NULL){
        fprintf(stderr, "Errore. Stazioni non inserite per percorso %d %d", start, end);
        return NULL;
    }
    if(start < end){
        percorso = PercorsoForward(startNode, endNode);
    }
    else{
        percorso = PercorsoBackward(Strada, startNode, endNode);
    }

    bool isCorrect = CheckPercorso(percorso);
    if(!isCorrect) printf("Percorso trovato, ma non attuabile! ");

    return percorso;
}
