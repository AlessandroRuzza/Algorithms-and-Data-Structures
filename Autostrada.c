#ifndef RELEASE
    #include "Tree.c"
    #include "List.c"
#endif

typedef Node* Stazione;
typedef Node* Car;

bool AggiungiStazione(Tree* Strada, int dist, int numAuto){
    int autonomia=0, dump=0;
    if(TreeSearch(Strada->Root, dist) != NULL){
        for(int i=0; i<numAuto; i++){
            dump=scanf("%d", &autonomia);
        }
        return false;
    }
    Stazione nuovaStazione = MakeNode(dist);
    for(int i=0; i<numAuto; i++){
        dump=scanf("%d", &autonomia);
        Car car = MakeCarNode(autonomia);
        TreeInsert(&nuovaStazione->carTree, car);
    }
    if(numAuto > 0)
        nuovaStazione->max_car = TreeMax(nuovaStazione->carTree.Root)->dist;
    
    TreeInsert(Strada, nuovaStazione);
    dump=dump;
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
    if(stazione->carTree.Root == NULL)
        stazione->max_car = -1;
    else if(autonomia >= stazione->max_car){
        stazione->max_car = TreeMax(stazione->carTree.Root)->dist;
    }
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
    // qui x != y, ovvero x.dist != y.dist
    return x->s->dist < y->s->dist; // se vero, a è meglio di b
}

List* PercorsoBackward(Stazione start, Stazione end){
    List* percorso = PercorsoNaiveBackward(start, end);
    if(percorso == NULL) return NULL;
    //else, esiste almeno un percorso
      
    if(percorso->length >= 4){ // se c'è solo una tappa intermedia (len==3) è inutile
        for(int i=0; i <= percorso->length/2; i++){ // dubious length/2
            ListNode* xNode = percorso->TAIL->prev->prev; // x è terzultima tappa
            while (xNode != percorso->HEAD){
                ListNode* yNode = xNode->next;
                Stazione x = TreeSuccessor(yNode->s);
                Stazione y = yNode->s;
                Stazione xPrev = xNode->prev->s;
                Stazione yNext = yNode->next->s;

                while (x != xPrev){
                    y = yNode->s; 
                    while(CanReach(x, y) && y != yNext){ // y.dist < yNode.s.dist, ferma se raggiunge la tappa ancora dopo
                        if(CanReach(xPrev, x) && CanReach(y, yNext)){ // allora y è valida per essere sostituita
                            yNode->s = y;
                            xNode->s = x;
                        }
                        y = TreePredecessor(y);
                    }
                    x = TreeSuccessor(x); // x.dist increases
                }
                xNode = xNode->prev;
            }
        }
    }
    // fix problema della seconda tappa (non best dist) (utile anche se length == 3)
    if(percorso->length >= 3){
        ListNode* xNode = percorso->HEAD->next;
        Stazione xPrev = percorso->HEAD->s;
        Stazione x = TreePredecessor(xNode->s);
        Stazione xNext = xNode->next->s;
        while(x != xNext){
            if(CanReach(xPrev, x) && CanReach(x, xNext)){
                xNode->s = x;
            }
            x = TreePredecessor(x);
        }
    }
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
        percorso = PercorsoBackward(startNode, endNode);
    }

    bool isCorrect = CheckPercorso(percorso);
    if(!isCorrect) fprintf(stderr, "Percorso trovato, ma non attuabile! ");

    return percorso;
}