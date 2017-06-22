// Store and print the paths of a binary tree

#include <stdio.h>
#include <stdlib.h>

#define PATHSZ 1000

typedef struct btnode* BTNode;
struct btnode {
    int data;
    struct btnode* left;
    struct btnode* right;
};

BTNode BTNodeCreate(int data) {
    BTNode new = malloc(sizeof (struct btnode));
    if (new == NULL) {
        fprintf(stderr, "stack overflow");
        exit(EXIT_FAILURE);
    }
    new->data = data;
    new->left = NULL;
    new->right = NULL;
    
    return new;
}

//
// Linked list of path arrays
//

typedef struct node* Node;
struct node {
    int* path;
    int pathLen;
    struct node* next;
};

Node NodeCreate(int* path, int pathLen) {
    Node new = malloc(sizeof (struct node));
    if (new == NULL) {
        fprintf(stderr, "stack overflow");
        exit(EXIT_FAILURE);
    }
    new->path = calloc(PATHSZ, sizeof (int));
    for (int i = 0; i < pathLen; i++) {
        new->path[i] = path[i];
        // calloc didn't set all elements to 0??
        //printf("%d", new->path[i]);
    }
    new->pathLen = pathLen;
    new->next = NULL;
    return new;
}

Node NodeAppend(Node head, int* path, int pathLen) {
    if (head == NULL) return NodeCreate(path, pathLen);
    Node cur = head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = NodeCreate(path, pathLen);
    return head;
}

//
// Printing all paths
//

void ArrayPrint(int* A, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d%s", A[i], (i < n - 1) ? ", " : "\n");
    }
}

void NodePrint(Node head) {
    while (head != NULL) {
        ArrayPrint(head->path, head->pathLen);
        head = head->next;
    }
}

void printPathsR(BTNode node, int* path, int pathLen, Node* paths) {
    if (node == NULL) return;
    path[pathLen] = node->data;
    pathLen++;
    
    if (node->left == NULL && node->right == NULL) {
        *paths = NodeAppend(*paths, path, pathLen);
    } else {
        printPathsR(node->left, path, pathLen, paths);
        printPathsR(node->right, path, pathLen, paths);
    }
}

int main() {
    //   10
    //  8  2
    // 3 5  2
    BTNode root = BTNodeCreate(10);
    root->left = BTNodeCreate(8);
    root->right = BTNodeCreate(2);
    root->left->left = BTNodeCreate(3);
    root->left->right = BTNodeCreate(5);
    root->right->left = BTNodeCreate(2);
    
    Node* paths = malloc(sizeof (Node));
    int path[PATHSZ];
    printPathsR(root, path, 0, paths);
    
    NodePrint(*paths);
    
    return 0;
}
