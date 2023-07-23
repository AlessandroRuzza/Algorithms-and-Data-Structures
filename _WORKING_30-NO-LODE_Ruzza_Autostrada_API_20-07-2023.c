#define RELEASE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define RED true
#define BLACK false

struct RB_Node{
    int dist;
    struct RB_Node* left;
    struct RB_Node* right;
    struct RB_Node* p;
    bool color;

    int max_car;
    struct RB_Tree{     
        struct RB_Node* Root;
    } carTree;
};

typedef struct RB_Node Node;
typedef struct RB_Tree Tree;

Node* MakeNode(int dist){
    Node* n = (Node*)malloc(sizeof(Node));
    n->dist = dist;
    n->left = NULL;
    n->right = NULL;
    n->p = NULL;
    n->color = BLACK;
    n->max_car = -1;
    n->carTree.Root = NULL;
    return n;
}
#define IS_CAR_VALUE -100
Node* MakeCarNode(int autonomy){
    Node* car = (Node*)malloc(sizeof(Node));
    car->dist = autonomy;
    car->left = NULL;
    car->right = NULL;
    car->p = NULL;
    car->color = BLACK;
    car->max_car = IS_CAR_VALUE; // using this as "flag" to identify Car nodes
    return car;
}
void ClearMemoryTree(Node* Root){
    if(Root == NULL)
        return;
    ClearMemoryTree(Root->left);
    ClearMemoryTree(Root->right);

    if(Root->max_car != IS_CAR_VALUE) 
        ClearMemoryTree(Root->carTree.Root);

    free(Root);
}

Node* TreeMax(Node* x){
    if(x == NULL) return NULL;
    while(x->right != NULL)
        x = x->right;
    return x;
}
Node* TreeMin(Node* x){
    while(x->left != NULL)
        x = x->left;
    return x;
}
Node* TreeSuccessor(Node* x){
    if(x->right != NULL)
        return TreeMin(x->right);
    Node *y = x->p;
    while(y != NULL && x == y->right){
        x = y;
        y = y->p;
    }
    return y;
}
Node* TreePredecessor(Node* x){
    if(x->left != NULL)
        return TreeMax(x->left);
    Node *y = x->p;
    while(y != NULL && x == y->left){
        x = y;
        y = y->p;
    }
    return y;
}
Node* TreeSearch(Node* x, int dist){
    if(x == NULL || x->dist == dist)
        return x;
    if(dist < x->dist)
        return TreeSearch(x->left, dist);
    else    
        return TreeSearch(x->right, dist);
}

void LeftRotate(Tree* T, Node* x){
    // rotate and update references
    Node* y = x->right;
    x->right = y->left;
    if(y->left != NULL)
        y->left->p = x;
    y->p = x->p;
    // update root and parent left/right node according to rotation
    if(x->p == NULL)
        T->Root = y;
    else if(x->p->left == x) 
        x->p->left = y;
    else    
        x->p->right = y;
    y->left = x;
    x->p = y;
}
void RightRotate(Tree* T, Node* x){
    // rotate and update references
    Node* y = x->left;
    x->left = y->right;
    if(y->right != NULL)
        y->right->p = x;
    y->p = x->p;
    // update root and parent left/right node according to rotation
    if(x->p == NULL)
        T->Root = y;
    else if(x->p->left == x) 
        x->p->left = y;
    else    
        x->p->right = y;
    y->right = x;
    x->p = y;
}

void TreeInsertFixUp(Tree* T, Node* z);
void TreeInsert(Tree* T, Node* z){
    Node* y = NULL;
    Node* x = T->Root;
    while(x != NULL){
        y = x;
        if(z->dist < x->dist)
            x = x->left;
        else x = x->right;
    }
    z->p = y;
    if(y == NULL)
        T->Root = z;
    else if(z->dist < y->dist)
        y->left = z;
    else y->right = z;
    z->left = NULL;
    z->right = NULL;
    z->color = RED;
    TreeInsertFixUp(T, z);
}

void TreeDeleteFixUp(Tree* T, Node* z);
void TreeDelete(Tree* T, Node* z){
    Node *x, *y;
    if(z->left == NULL || z->right == NULL)
        y = z;
    else y = TreeSuccessor(z);
    if(y->left != NULL)
        x = y->left;
    else x = y->right;
    if(x != NULL)
        x->p = y->p;
    if(y->p == NULL)
        T->Root = x;
    else if(y == y->p->left)
        y->p->left = x;
    else y->p->right = x;
    if(y != z){
        z->dist = y->dist;
        z->max_car = y->max_car;
        Node* temp = z->carTree.Root;
        z->carTree = y->carTree;
        y->carTree.Root = temp;
    }
    if(y->color == BLACK && x != NULL)
        TreeDeleteFixUp(T, x);

    y->left = NULL;
    y->right = NULL;
    ClearMemoryTree(y);
}

void TreeDeleteFixUp(Tree* T, Node* x){
    if(x->color == RED || x->p == NULL){
        x->color = BLACK;
    }
    else if(x == x->p->left){
        Node *w = x->p->right;
        if(w != NULL && w->color == RED){
            w->color = BLACK;
            x->p->color = RED;
            LeftRotate(T, x->p);
            w = x->p->right;
        }
        if(w != NULL && (w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)){
            w->color = RED;
            TreeDeleteFixUp(T, x->p);
        }
        else{
            if(w->right == NULL || w->right->color == BLACK){
                if(w->left != NULL)
                    w->left->color = BLACK;
                w->color = RED;
                RightRotate(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            if(w->right != NULL)
                w->right->color = BLACK;
            LeftRotate(T, x->p);
        }
    }
    else{
        Node *w = x->p->left;
        if(w != NULL && w->color == RED){
            w->color = BLACK;
            x->p->color = RED;
            LeftRotate(T, x->p);
            w = x->p->left;
        }
        if(w != NULL && (w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)){
            w->color = RED;
            TreeDeleteFixUp(T, x->p);
        }
        else{
            if(w->left == NULL || w->left->color == BLACK){
                if(w->right != NULL)
                    w->right->color = BLACK;
                w->color = RED;
                LeftRotate(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            if(w->left != NULL)
                w->left->color = BLACK;
            RightRotate(T, x->p);
        }
    }
}

void TreeInsertFixUp(Tree* T, Node* z){
    Node *x, *y;
    if(z == T->Root)
        T->Root->color = BLACK;
    else{
        x = z->p;
        if(x->color == RED){
            if(x == x->p->left){
                y = x->p->right;
                if(y != NULL && y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    TreeInsertFixUp(T, x->p);
                }
                else{
                    if(z == x->right){
                        z = x;
                        LeftRotate(T, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    RightRotate(T, x->p);
                }
            }
            else{
                y = x->p->left;
                if(y != NULL && y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    TreeInsertFixUp(T, x->p);
                }
                else{
                    if(z == x->left){
                        z = x;
                        RightRotate(T, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    LeftRotate(T, x->p);
                }
            }
        }
    }
}

void PrintTreeInOrder(Node* root){
    if(root == NULL) return;
    PrintTreeInOrder(root->left);
    fprintf(stderr, "%d - %d\n", root->dist, root->max_car);
    PrintTreeInOrder(root->right);
} 
 
#ifndef RELEASE
    #ifndef RED
        #include "Tree.c"
    #endif
#endif
typedef struct DoubleLinkedListNode{
    Node *s;
    struct DoubleLinkedListNode* next;
    struct DoubleLinkedListNode* prev;
} ListNode;

typedef struct DoubleLinkedList{
    ListNode* HEAD;
    ListNode* TAIL;
    int length;
} List;

List* MakeEmptyList(){
    List* percorso = (List*)malloc(sizeof(List));
    percorso->HEAD = NULL;
    percorso->TAIL = NULL;
    percorso->length = 0;
    return percorso;
}
ListNode* MakeListNode(Node* x){
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->s = x;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void PrintList(List* list){
    ListNode* x = list->HEAD;
    while(x != NULL){
        printf("%d", x->s->dist);
        x = x->next;
        if(x != NULL) printf(" ");
    }
    printf("\n");
}
void PrintErrorList(List* list){
    ListNode* x = list->HEAD;
    while(x != NULL){
        fprintf(stderr, "%d", x->s->dist);
        x = x->next;
        if(x != NULL) fprintf(stderr, " ");
    }
    fprintf(stderr, "\n");
}
void ClearMemoryList(List* list){
    ListNode* x = list->HEAD;
    ListNode* y;
    while(x != NULL){
        y = x;
        x = x->next;
        free(y);
    }
    free(list);
}

void Append(List* to, List* from){
    if(from->HEAD == NULL) return; //inutile fare append di lista vuota
    to->TAIL->next = from->HEAD;
    to->TAIL = from->TAIL;
    to->length += from->length;
}

void InsertTail(List* list, Node* x){
    ListNode* node = MakeListNode(x);
    list->length++;

    if(list->TAIL == NULL){
        list->HEAD = node;
        list->TAIL = node;
    }
    else{
        node->prev = list->TAIL;
        list->TAIL->next = node;
        list->TAIL = node;
    }
}
void InsertHead(List* list, Node* x){
    ListNode* node = MakeListNode(x);
    list->length++;
    
    if(list->HEAD == NULL){
        list->HEAD = node;
        list->TAIL = node;
    }
    else{
        node->next = list->HEAD;
        list->HEAD->prev = node;
        list->HEAD = node;
    }
}

void InsertAfter(List* list, ListNode* element, Node* x){
    ListNode* node = MakeListNode(x);
    list->length++;

    if(element->next == NULL) // element is TAIL
        list->TAIL = node;
    else
        element->next->prev = node;

    node->next = element->next;
    element->next = node;
    node->prev = element;
}
void InsertBefore(List* list, ListNode* element, Node* x){
    ListNode* node = MakeListNode(x);
    list->length++;

    if(element->prev == NULL) // element is HEAD
        list->HEAD = node;
    else
        element->prev->next = node;

    node->prev = element->prev;
    element->prev = node;
    node->next = element;

}

void Reverse(List* list){
    if(list->HEAD == NULL) return;
    ListNode* x = list->HEAD;
    ListNode* y;

    list->HEAD = list->TAIL;
    list->TAIL = x;
    while (x != NULL){
        y = x->next;
        x->next = x->prev;
        x->prev = y;
        x = y;
    }
    
}
 
 
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
        if(autonomia > nuovaStazione->max_car){
            nuovaStazione->max_car = autonomia;
        }
    }
    //if(numAuto > 0)
        //nuovaStazione->max_car = TreeMax(nuovaStazione->carTree.Root)->dist;
    
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
    if(autonomia > stazione->max_car) 
        stazione->max_car = autonomia;

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
        ListNode* stopAt = percorso->HEAD; // x è terzultima tappa
        ListNode* prev2_Tail = percorso->TAIL->prev->prev; // x è terzultima tappa
        //ListNode* prev3_Tail = percorso->TAIL->prev->prev->prev; // x è terzultima tappa
        for(int i=0; i <= percorso->length/2+1; i++){ // dubious length/2
            ListNode* xNode = prev2_Tail; // x è terzultima tappa
            //PrintErrorList(percorso);
            while (xNode != stopAt){
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
            if(stopAt != prev2_Tail)
                stopAt = stopAt->next;
            if(i == percorso->length/2)
                stopAt = percorso->HEAD;
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
    //fprintf(stderr, "%s\n", comment);
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
            //BreakPoint(Strada);
            break;
        default: //fprintf(stderr, "Comando non identificato!\n");
        case END:
            break;

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