#include <stdio.h>
#include <stdlib.h>
#include "net/rime.h"
#include "contiki.h"
/**
  * This sample is about how to implement a queue in c
  *
  * Type of item is int
  * Add item to tail
  * Get item from head
  * Can get the size
  * Can display all content
  */

typedef struct Message {
  uint8_t type;
  uint8_t tag;   
  char * message;
  uint8_t weight;
  float wait_time;
  rimeaddr_t target;

} Message;

/**
 * The Node struct,
 * contains item and the pointer that point to next node.
 */
typedef struct Node {
  struct Message item;
  struct Node* next;
} Node;

/**
 * The Queue struct, contains the pointers that
 * point to first node and last node, the size of the Queue,
 * and the function pointers.
 */
typedef struct Queue {
    Node* head;
    Node* tail;

    void (*push) (struct Queue*, struct Message); // add item to tail
    // get item from head and remove it from queue
    struct Message (*pop) (struct Queue*);
    // get item from head but keep it in queue
    struct Message (*peek) (struct Queue*);
    // display all element in queue
    void (*display) (struct Queue*);
    // size of this queue
    int size;
} Queue;

/*
int main () {
    Queue queue = createQueue();
    queue.display(&queue);

    printf("push item 2\n");
    queue.push(&queue, 2);    
    printf("push item 3\n");
    queue.push(&queue, 3);
    printf("push item 6\n");
    queue.push(&queue, 6);

    queue.display(&queue);

    printf("peek item %d\n", queue.peek(&queue));
    queue.display(&queue);

    printf("pop item %d\n", queue.pop(&queue));
    printf("pop item %d\n", queue.pop(&queue));
    queue.display(&queue);

    printf("pop item %d\n", queue.pop(&queue));
    queue.display(&queue);
    printf("push item 6\n");
    queue.push(&queue, 6);

    queue.display(&queue);
    system("PAUSE");
}
*/
/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void push (Queue* queue, struct Message item) {
    // Create a new node
    Node* n = (Node*) malloc (sizeof(Node));
    n->item = item;
    n->next = NULL;

    if (queue->head == NULL) { // no head
        queue->head = n;
    } else{
        queue->tail->next = n;
    }
    queue->tail = n;
    queue->size++;
}
/**
 * Return and remove the first item.
 */
struct Message pop (Queue* queue) {
    // get the first item
    Node* head = queue->head;
    struct Message item = head->item;
    // move head pointer to next node, decrease size
    queue->head = head->next;
    queue->size--;
    // free the memory of original head
    free(head);
    return item;
}
/**
 * Return but not remove the first item.
 */
struct Message peek (Queue* queue) {
    Node* head = queue->head;
    return head->item;
}
/**
 * Show all items in queue.
 */
void display (Queue* queue) {
    printf("\nDisplay: ");
    // no item
    if (queue->size == 0)
        printf("No item in queue.\n");
    else { // has item(s)
        Node* head = queue->head;
        int i, size = queue->size;
        printf("%d item(s):\n", queue->size);
        for (i = 0; i < size; i++) {
            if (i > 0)
                printf(", ");
            printf("%s", head->item.message);
            head = head->next;
        }
    }
    printf("\n\n");
}
/**
 * Create and initiate a Queue
 */
Queue createQueue () {
    Queue queue;
    queue.size = 0;
    queue.head = NULL;
    queue.tail = NULL;
    queue.push = &push;
    queue.pop = &pop;
    queue.peek = &peek;
    queue.display = &display;
    return queue;
}

Message createMessage () {
    Message m;
    m.type = 0;
    m.message = "";
    m.tag = 0;
    m.weight = 0;
    m.wait_time = 0;
    return m;
}