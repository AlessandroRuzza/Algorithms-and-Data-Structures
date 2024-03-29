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
void InitNode(Node* n, int dist){
    n->dist = dist;
    n->left = NULL;
    n->right = NULL;
    n->p = NULL;
    n->color = BLACK;
    n->max_car = -1;
    n->carTree.Root = NULL;
}
Node* MakeNode(int dist){
    Node* n = (Node*)malloc(sizeof(Node));
    InitNode(n, dist);
    return n;
}
#define IS_CAR_VALUE -100
void InitCar(Node* car, int autonomy){
    InitNode(car, autonomy);
    car->max_car = IS_CAR_VALUE; // using this as "flag" to identify Car nodes
}
Node* MakeCarNode(int autonomy){
    Node* car = (Node*)malloc(sizeof(Node));
    InitCar(car, autonomy);
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