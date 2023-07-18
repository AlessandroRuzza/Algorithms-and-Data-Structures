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
bool IsBetterBackward(List* a, List* b){
    if(a->length != b->length) return a->length < b->length;
    ListNode *x, *y;
    x = a->TAIL;
    y = b->TAIL;
    while(x->s->dist == y->s->dist && x != NULL && y != NULL){
        x = x->prev;
        y = y->prev;
    }
    // here x != y, meaning x.dist != y.dist
    return x->s->dist < y->s->dist; // if true, a is better than b
}
List* PercorsoMinDistBackward(Stazione start, Stazione end, int length){
    List* percorso = NULL;
    List* tempPercorso = NULL;

    if(CanReach(start, end)){  // here length == 2 should be true
        percorso = MakeEmptyList();
        InsertHead(percorso, end);
        InsertHead(percorso, start);
    }
    else if(length > 2){ 
        Stazione x = TreePredecessor(start);
        while(CanReach(start, x)){ // CanReach(start, end) is false, so x will never reach end 
            x = TreePredecessor(x);
        }
        x = TreeSuccessor(x); // x is set to furthest station (lowest dist) that can be reached from start    
        while(x != start && (length <= 6 || percorso == NULL)){  // dubious length <= 6
            //if(length >= 13) fprintf(stderr, "Start: %d  -  x: %d - len: %d  -  end: %d\n", start->dist, x->dist, length, end->dist);
            tempPercorso = PercorsoMinDistBackward(x, end, length-1);

            if(tempPercorso != NULL){
                if(percorso == NULL){
                    percorso = tempPercorso;
                    //break;
                }
                else if(IsBetterBackward(tempPercorso, percorso)){
                    ClearMemoryList(percorso);
                    percorso = tempPercorso;
                    fprintf(stderr, " Foundcha!\n");
                }
                else ClearMemoryList(tempPercorso);
            }

            x = TreeSuccessor(x);
        }
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
    percorso = NULL;                                                     // ORG
    percorso = PercorsoMinDistBackward(start, end, minLength);            // ORG
                                                       
    /*
    percorso = MakeEmptyList();  
    List* tempPercorso = NULL;
    Stazione x = TreeSuccessor(end);
    while (CanReach(x, end) && x != start)
    {
        tempPercorso = PercorsoNaiveBackward(start, x);
        InsertTail(tempPercorso, end);
        Append(tempPercorso, percorso);
        if(tempPercorso->length == minLength){
            InsertHead(percorso, end);
            end = x;
            //break;
        }
        ClearMemoryList(tempPercorso);
        x = TreeSuccessor(x);
    }
    // */
    fprintf(stderr, "Using PercorsoBackward!\n");
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
        /*
        if(percorso != NULL){
            List* naive = PercorsoNaiveBackward(startNode, endNode);
            fprintf(stderr, "Percorso >>> Naive? %d\n", IsBetterBackward(percorso, naive));
            fprintf(stderr, "Naive   : "); PrintErrorList(naive);
            fprintf(stderr, "Percorso: "); PrintErrorList(percorso);
            ClearMemoryList(naive);
        }
        */
    }

    bool isCorrect = CheckPercorso(percorso);
    if(!isCorrect) fprintf(stderr, "Percorso trovato, ma non attuabile! ");

    return percorso;
}
