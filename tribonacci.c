// Find the nth Tribonacci number
// f(1) = f(2) = f(3) = 1
// Integers that can't be stored in any C data type are represented in reverse order by linked lists

#include <stdio.h>
#include <stdlib.h>

typedef struct node* Node;
struct node {
    int data;
    struct node* next;
};

Node NodeCreate(int data) {
    Node new = malloc(sizeof (struct node));
    if (new == NULL) {
        fprintf(stderr, "stack overflow");
        exit(EXIT_FAILURE);
    }
    new->data = data;
    new->next = NULL;
    return new;
}

Node NodeAppend(Node head, int data) {
    if (head == NULL) return NodeCreate(data);
    
    Node cur = head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = NodeCreate(data);
    return head;
}

Node NodeReverse(Node head) {
    if (head == NULL || head->next == NULL) return head;
    
    Node cur = head;
    Node prev = NULL;
    Node next = NULL;
    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

Node NodeSum(Node a, Node b) {
    Node head = NULL;
    int carry = 0;
    int sum = 0;
    
    while (a != NULL && b != NULL) {
        sum = a->data + b->data + carry;
        head = NodeAppend(head, sum % 10);
        if (sum > 9) carry = 1;
        else carry = 0;
        
        a = a->next;
        b = b->next;
    }
    if (a != NULL && b == NULL) {
        while (a != NULL) {
            sum = a->data + carry;
            head = NodeAppend(head, sum % 10);
            if (sum > 9) carry = 1;
            else carry = 0;
            
            a = a->next;
        }
    } else if (a == NULL && b != NULL) {
        while (b != NULL) {
            sum = b->data + carry;
            head = NodeAppend(head, sum % 10);
            if (sum > 9) carry = 1;
            else carry = 0;
            
            b = b->next;
        }
    }
    if (carry) head = NodeAppend(head, 1);
    
    return head;
}

void NodePrint(Node head) {
    while (head != NULL) {
        printf("%d%s", head->data, (head->next == NULL) ? "\n" : ", ");
        head = head->next;
    }
}

void NodePrint2(Node head) {
    head = NodeReverse(head);
    while (head != NULL) {
        printf("%d", head->data);
        head = head->next;
    }
    printf("\n");
}

void NodeDelete(Node head) {
    Node cur = NULL;
    while (head != NULL) {
        cur = head;
        head = head->next;
        free(cur);
    }
}

Node NodeTribonacci(int n) {
    if (n == 1 || n == 2 || n == 3) return NodeCreate(1);
    
    Node f1 = NodeCreate(1);
    Node f2 = NodeCreate(1);
    Node f3 = NodeCreate(1);
    
    Node* fminus2 = &f1;
    Node* fminus1 = &f2;
    Node* f = &f3;
    
    int i = 3;
    while (i < n) {
        Node itmSum = NodeSum(*fminus1, *fminus2); // intermediate sum
        Node finSum = NodeSum(*f, itmSum); // final sum
        
        NodeDelete(itmSum);
        NodeDelete(*fminus2);
        
        *fminus2 = *fminus1;
        *fminus1 = *f;
        *f = finSum;
        
        i++;
    }
    
    return *f;
}

int main(int argc, const char * argv[]) {
    
    // testing the interface with lists a and b
    Node a = NULL;
    Node b = NULL;
    a = NodeAppend(a, 9);
    a = NodeAppend(a, 9);
    a = NodeAppend(a, 9);
    
    b = NodeAppend(b, 5);
    b = NodeAppend(b, 5);
    b = NodeAppend(b, 1);
    b = NodeAppend(b, 1);
    b = NodeAppend(b, 1);
    
    NodePrint(a);
    NodePrint(b);
    Node sum = NodeSum(a, b);
    NodePrint(sum);
    NodePrint2(sum);
    
    // the actual code for the tribonacci sequence, n = 4000
    Node sumX = NodeTribonacci(4000);
    NodePrint2(sumX);
    return 0;
}
