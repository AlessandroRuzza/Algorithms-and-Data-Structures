#ifndef RED
    #include "Tree.c"
#endif

typedef struct DoubleLinkedListNode{
    Node *s;
    struct DoubleLinkedListNode* next;
    struct DoubleLinkedListNode* prev;
} ListNode;

typedef struct DoubleLinkedList{
    ListNode* HEAD;
    ListNode* TAIL;
} List;

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
    to->TAIL->next = from->HEAD;
    to->TAIL = from->TAIL;
}

void InsertTail(List* list, Node* x){
    ListNode* node = MakeListNode(x);

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
    if(element->next == NULL) // element is TAIL
        list->TAIL = node;

    element->next->prev = node;
    node->next = element->next;
    element->next = node;
    node->prev = element;
}
void InsertBefore(List* list, ListNode* element, Node* x){
    ListNode* node = MakeListNode(x);
    if(element->prev == NULL) // element is HEAD
        list->HEAD = node;

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
    while (x != NULL)
    {
        y = x->next;
        x->next = x->prev;
        x->prev = y;
        x = y;
    }
    
}
