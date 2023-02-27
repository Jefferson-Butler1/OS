/**
 * Read a list of numbers, sort it, and print it out.
 *
 * @Jeff Butler
 */

#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0 
/** The name of this program. */
char *prog;

/**
 * A node in a doubly-linked list.
 *
 * The following diagram illustrates a single node.
 *
 *                    +------+
 *                    | -0.3 |
 *                    | next | ---> pointer to a
 * pointer to a  <--- | prev |      next node
 * previous node      +------+
 */
struct node {
    double value;       ///< A value
    struct node *next;  ///< Pointer to the next node
    struct node *prev;  ///< Pointer to the previous node
};

/**
 * A doubly-linked list.
 *
 * This doubly-linked list has dummy head and tail to simplify
 * insertion, deletion, and swapping.
 *
 * The following diagram illustrates a singleton list.
 *
 *           +------+      +------+      +------+
 *   list -> | dummy|      | -0.3 |      | dummy|
 *           | next | ---> | next | ---> | next | ---> NULL
 * NULL <--- | prev | <--- | prev | <--- | prev |
 *           +------+      +------+      +------+
 */
struct list {
    struct node *head;  ///< Dummy head
    struct node *tail;  ///< Dummy tail
};

struct list *list_new();
void list_add(struct list *, double);
void list_swap(struct node *);
void read(struct list *);
void sort(struct list *);
void print(struct list *);

int main(int argc, char *argv[]) {
    prog = argv[0];
    struct list *l = list_new();
    read(l);
    sort(l);
    print(l);
}

/**
 * Create a new empty doubly-linked list.
 * @return a new empty list
 */
struct list *list_new() {
    struct list * new = (struct list *) malloc(sizeof(struct list));
    struct node *head = (struct node * ) malloc(sizeof(struct node));
    struct node *tail = (struct node * ) malloc(sizeof(struct node));
    head -> next = tail;
    tail -> prev = head;
    head -> value = -1;
    tail -> value = -1;
    new -> head = head;
    new -> tail = tail;
    if(DEBUG) printf("successfully created a new list\n");
    return new;
}

/**
 * Insert a value at the end of the doubly-linked list.
 * @param l  Pointer to a valid doubly-linked list
 * @param d  Value to add
 */
void list_add(struct list *l, double d) {
   struct node * new = (struct node *) malloc(sizeof(struct node));
   struct node * old = l->tail -> prev;
   struct node * tail= l->tail;

   new -> value= d;
   new -> prev = old;
   new -> next = tail;
   tail-> prev = new;
   old -> next = new;
   if(DEBUG) printf("successfully added an element\n");
}

/**
 * Swap this node with the next.
 * @param n  the node to swap with its next neighbor
 */
void list_swap(struct node *n) {
    if(DEBUG) printf("successfully entered swap\n");
    struct node *n0 = n->prev;
    struct node *n1 = n->next;
    struct node *n2 = n;
    struct node *n3 = n->next->next;

    n0->next = n1;
    
    n1->prev = n0;
    n1->next = n2;
    
    n2->prev = n1;
    n2->next = n3;

    n3->prev = n2;
    if(DEBUG) printf("successfully switched\n");
}

/**
 * Read values into a list.
 * @param l  Pointer to a valid doubly-linked list
 * Read values from standard input and insert them at the end of the
 * list.  Stop when EOF is encountered.
 */
void read(struct list *l) {
    double  d = 0;
    while(scanf("%lf", &d) != EOF){
        list_add(l, d); 
    }
    if(DEBUG) printf("successfully read in list\n");
}

/**
 * Sort the given list in ascending order.
 * @param l  Pointer to a valid doubly-linked list to sort
 */
void sort(struct list *l) {
    struct node *last = l->tail;
    while (1) {
        int inorder = 1;
        struct node *n = l->head -> next;
        while (n != last) {
            struct node *next = n->next;
            if (next == last) break;
            if (n->value > next->value) {
                if(DEBUG) printf("Two values to compare: %lf %lf\n", n->value, next->value);
		list_swap(n);
                inorder = 0;
            }
            n = n->next;
        }
        if (inorder) break;
    }
    if(DEBUG) printf("successfully sorted list\n");
}

/**
 * Print each value of the list, one per line to standard output
 * @param l  Pointer to valid doubly-linked list to print
 */
void print(struct list *l) {
    struct node *last = l -> tail;
    struct node *n    = l -> head -> next;
    while(n != last){
	printf("%lf\n", n -> value);
	n = n -> next;
    }
}
