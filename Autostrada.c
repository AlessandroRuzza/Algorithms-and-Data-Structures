#include "Tree.c"
#include "List.c"

typedef Node* Stazione;
typedef Node* Car;

bool AggiungiStazione(Tree* Strada, int dist, int numAuto){
    int autonomia=0;
    if(TreeSearch(Strada->Root, dist) != NULL){
        for(int i=0; i<numAuto; i++){
            scanf("%d", &autonomia);
        }
        return false;
    }
    Stazione nuovaStazione = MakeNode(dist);
    for(int i=0; i<numAuto; i++){
        scanf("%d", &autonomia);
        Car car = MakeCarNode(autonomia);
        //printf("%d %d\n", i, autonomia);
        TreeInsert(&nuovaStazione->carTree, car);
    }
    if(numAuto > 0)
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

bool CanReach(Stazione from, Stazione to){
    int diff = to->dist - from->dist;
    if(diff < 0) diff = -diff;  // diff = absolute value of distance between from and to
    return from->max_car >= diff;
}

List* PercorsoNaiveBackward(Stazione start, Stazione end){
    List* percorso = NULL;

    if(start == end){  // caso base
        percorso = MakeEmptyList(percorso);
    }
    else{   // start.dist > end.dist
        Stazione prevTappa = NULL;
        Stazione x = end;
        do{
            x = TreeSuccessor(x);   
            if(CanReach(x, end))
                prevTappa = x;
        }while(x != start);
        // ora prevTappa è la stazione con la maggiore dist che può raggiungere end
        if(prevTappa != NULL){  // se esiste, ripeto ricorsivamente tra prevTappa e end
            percorso = PercorsoNaiveBackward(start, prevTappa);
        }
    }
    if(percorso != NULL)
        InsertTail(percorso, end);

    return percorso;
}
List* PercorsoMinDistBackward(Stazione start, Stazione end, int length){
    List* percorso = NULL;
    if(length == 1 && start == end){  
        percorso = MakeEmptyList();
        InsertHead(percorso, end);
    }
    else if(length > 1){ 
        Stazione x = start;
        do{
            x = TreePredecessor(x);
            if(CanReach(start, x))
                percorso = PercorsoMinDistBackward(x, end, length-1);
        }while(x != end && CanReach(start, x));
        if(percorso != NULL)
            InsertHead(percorso, start);
    }

    return percorso;
}

List* PercorsoBackward(Stazione start, Stazione end){
    List* percorso = PercorsoNaiveBackward(start, end);
    if(percorso == NULL) return NULL;
    //else, esiste almeno un percorso
    int minLength = percorso->length;
    ClearMemoryList(percorso);
    percorso = NULL;

    percorso = AI_PercorsoMinDistBackward(start, end, minLength);

    fprintf(stderr, "Using NAIVE PercorsoBackward!\n");
    return percorso;
}

List* PercorsoForward(Stazione start, Stazione end){
    List* percorso = NULL;

    if(start == end){  // caso base
        percorso = MakeEmptyList();
    }
    else{   // start.dist < end.dist
        Stazione prevTappa = NULL;
        Stazione x = end;
        do{
            x = TreePredecessor(x);   
            if(CanReach(x, end))
                prevTappa = x;
        }while(x != start);
        // ora prevTappa è la stazione con la minore dist che può raggiungere end
        if(prevTappa != NULL)   // se esiste, ripeto ricorsivamente tra prevTappa e end
            percorso = PercorsoForward(start, prevTappa);
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
    Stazione startNode = TreeSearch(Strada->Root, start);
    Stazione endNode = TreeSearch(Strada->Root, end);
    if(startNode == NULL || endNode == NULL){
        fprintf(stderr, "Errore. Stazioni non inserite per percorso %d %d", start, end);
        return NULL;
    }
    if(start < end){
        percorso = PercorsoForward(startNode, endNode);
    }
    else{ // start > end
        //PrintTreeInOrder(Strada->Root);
        percorso = PercorsoBackward(startNode, endNode);
    }

    bool isCorrect = CheckPercorso(percorso);
    if(!isCorrect) printf("Percorso trovato, ma non attuabile! ");

    return percorso;
}
