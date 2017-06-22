// Print the shortest path/s for a knight to move from one square to another

#include <stdio.h>
#include <stdlib.h>

#define PATHSZ 10
#define BOARDSZ 8

//
// Store paths in a linked list of arrays
//

typedef struct node* Node;
struct node {
    int* path; // path[PATHSZ]
    int pathSz;
    struct node* next;
};

Node NodeCreate(int* path, int pathSz) {
    Node new = malloc(sizeof (struct node));
    if (new == NULL) {
        fprintf(stderr, "stack overflow");
        exit(EXIT_FAILURE);
    }
    new->path = calloc(PATHSZ, sizeof (int));
    for (int i = 0; i < pathSz; i++) {
        new->path[i] = path[i];
        // calloc didn't set all elements to 0??
        //printf("%d", new->path[i]);
    }
    new->pathSz = pathSz;
    new->next = NULL;
    return new;
}

Node NodeAppend(Node head, int* path, int pathSz) {
    if (head == NULL) return NodeCreate(path, pathSz);
    Node cur = head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = NodeCreate(path, pathSz);
    return head;
}

Node NodeDelete(Node head, Node node) {
    Node cur = head;
    Node prev = NULL;
    while (cur != node && cur != NULL) {
        prev = cur;
        cur = cur->next;
    }
    Node next = cur->next;
    
    if (cur == NULL) return NULL;
    if (prev == NULL) {
        free(cur);
        return next;
    }
    prev->next = next;
    free(cur);
    return head;
}

//
// Int, Square conversion
//

int getX(char* sq) { return sq[0] - 'a' + 1; }
int getY(char* sq) { return sq[1] - '0'; }

char* IntToSq(int num) {
    int y = num % 10;
    int x = (num - y) / 10;
    // doesn't work if char sq[3];
    char* sq = malloc(3 * sizeof (char));
    sprintf(sq, "%c%d", x + 'a' - 1, y);
    sq[2] = '\0';
    return sq;
}

//
// Print paths
//

void ArrayPrint(int* A, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s%s", IntToSq(A[i]), (i < n - 1) ? ", " : "\n");
    }
}

void NodePrint(Node head) {
    while (head != NULL) {
        ArrayPrint(head->path, head->pathSz);
        head = head->next;
    }
}

//
// Recursive traversal
//

void BoardTraverse(int srcX, int srcY, int destX, int destY, int* path, int pathSz, int* minPathSz, Node* paths) {
    if (   srcX < 1 || srcX > BOARDSZ
        || srcY < 1 || srcY > BOARDSZ
        || pathSz >= *minPathSz) {
        return;
    }
    
    path[pathSz++] = srcX * 10 + srcY;
    if (srcX == destX && srcY == destY) {
        *paths = NodeAppend(*paths, path, pathSz);
        if (pathSz < *minPathSz) *minPathSz = pathSz;
    } else {
        int dx[8] = {-2, -1, 1, 2,  2,  1, -1, -2};
        int dy[8] = { 1,  2, 2, 1, -1, -2, -2, -1};
        for (int i = 0; i < 8; i++) { // knight has max 8 moves
            BoardTraverse(srcX + dx[i], srcY + dy[i], destX, destY, path, pathSz, minPathSz, paths);
        }
    }
}

Node BoardTraverseWrapper(char* src, char* dest) {
    int path[PATHSZ];
    int minPathSz = PATHSZ;
    Node* paths = malloc(sizeof (Node));
    BoardTraverse(getX(src), getY(src), getX(dest), getY(dest), path, 0, &minPathSz, paths);
    
    Node cur = *paths;
    while (cur != NULL) {
        if (cur->pathSz != minPathSz) {
            Node copy = cur;
            cur = cur->next;
            *paths = NodeDelete(*paths, copy);
        } else {
            cur = cur->next;
        }
    }
    
    return *paths;
}

// The previous implementation of traversal had a few flaws.
//     1. Paths were being remembered. In the new implementation, it is clear that the recursive functions are called immediately after any change to the path array.
//     2. Conversion between Int and Square caused messiness.
//     3. The if/else checks were in the opposite order. This caused two issues, the source cell was not added to path, the initial path size was 1.
#if 0
void BoardTraverse(char* src, char* dest, int* path, int pathSz, int* minPathSz, Node* paths) {
    int dx[8] = {-2, -1, 1, 2,  2,  1, -1, -2};
    int dy[8] = { 1,  2, 2, 1, -1, -2, -2, -1};
    
    for (int i = 0; i < 8; i++) { // knight has max 8 moves
        int x = src[0] - 'a' + 1 + dx[i];
        int y = src[1] - '0' + dy[i];
        
        if (   x < 1 || x > BOARDSZ
            || y < 1 || y > BOARDSZ) continue;
        if (pathSz >= *minPathSz) continue;
        path[pathSz] = x * 10 + y;
        pathSz++;
        
        if (   x == dest[0] - 'a' + 1
            && y == dest[1] - '0') {
            *paths = NodeAppend(*paths, path, pathSz);
            if (pathSz < *minPathSz) *minPathSz = pathSz;
        } else {
            BoardTraverse(IntToSquare(x * 10 + y), dest, path, pathSz, minPathSz, paths);
        }
    }
}
#endif

//
// Main
//

int main() {
    char* src = "a3";
    char* dest = "h8";
    Node paths = BoardTraverseWrapper(src, dest);
    NodePrint(paths);
    
    return 0;
}
