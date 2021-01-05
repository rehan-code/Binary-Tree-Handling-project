 #include "tree.h"
 /**
  *  File Name: tree.c
  *  Author: Rehan Nagoor Mohideen
  *  Student ID: 1100592
  *  Purpose: Contains functions for CIS2520 assignment 4
  */

struct Performance *newPerformance() {
    struct Performance *new_performance = malloc(sizeof(struct Performance));//allocate memory to the structure
    if (new_performance == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space for mallocof newPerformance\n");
        exit(0);
    }
    //initialize values to 0
    new_performance->reads = 0;
    new_performance->writes = 0;
    new_performance->mallocs = 0;
    new_performance->frees = 0;

    return new_performance;
}

void attachNode( struct Performance*performance, struct Node**node_ptr, void *src, unsigned int width ) {
    struct Node *newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space to malloc a new Node\n");
        exit(0);
    }

    newNode->data = malloc(width);
    if (newNode->data == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space to malloc data in newNode\n");
        exit(0);
    }
    memcpy(newNode->data, src, width);//copy the data into the node
    //initialize the nodes to null and link it to the tree
    newNode->lt = NULL;
    newNode->gte = NULL;
    *node_ptr = newNode;

    performance->mallocs++;
    performance->writes++;
}


int comparNode( struct Performance *performance, struct Node**node_ptr, int (*compar)(const void *, const void *), void *target) {
    performance->reads++;
    return compar(target, ((*node_ptr)->data));
}

struct Node**next( struct Performance *performance, struct Node**node_ptr, int direction) {
    if (node_ptr == NULL || *node_ptr == NULL) {//If list is empty
        fprintf(stderr, "ERROR: list is empty.\n");
        exit(0);
    }

    performance->reads++;

    if (direction < 0) {//if the direction is less than zero, return the **pointer to the less than node
        return &((*node_ptr)->lt);
    } else {//otherwise return the **pointer to the greater than equal node
        return &((*node_ptr)->gte);
    }
}

void readNode( struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width ) {
    if (node_ptr == NULL || *node_ptr == NULL) {//If list is empty
        fprintf(stderr, "ERROR: list is empty.\n");
        exit(0);
    }

    memcpy(dest, (*node_ptr)->data, width);//copy the data to the dest
    performance->reads++;
}

void detachNode(struct Performance *performance, struct Node**node_ptr) {
    if (node_ptr == NULL || *node_ptr == NULL) {//If list is empty
        fprintf(stderr, "ERROR: list is empty.\n");
        exit(0);
    }
    //free the data and the node and set the pointer to NULL
    free((*node_ptr)->data);
    free(*node_ptr);
    *node_ptr = NULL;
    performance->frees++;
}

int isEmpty( struct Performance *performance, struct Node**node_ptr ) {
    if (*node_ptr == NULL) {//If the pointer pointed to by node_ptr is NULL
        return 1;
    }
    return 0;
}

void addItem( struct Performance *performance, struct Node**node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width ) {
    struct Node **temp_ptr = node_ptr;

    while (!isEmpty(performance, temp_ptr)) {//loop to find the position in the tree (where it is NULL) to add the new item
        temp_ptr = next(performance, temp_ptr, comparNode(performance, temp_ptr, compar, src));
    }
    attachNode(performance, temp_ptr, src, width);
}

void freeTree( struct Performance *performance, struct Node**node_ptr) {
    struct Node **temp_ptr = node_ptr;

    if (!isEmpty(performance, temp_ptr)) {//if the node is not null recursively free the rest of the tree
        freeTree(performance, next(performance, temp_ptr, -1));//recursively free the less than side nodes
        freeTree(performance, next(performance, temp_ptr, 0));//recursively free the gte side nodes
        detachNode(performance, temp_ptr);
    }
}

int searchItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width ) {
    struct Node **temp_ptr = node_ptr;
    int direction = comparNode(performance, temp_ptr, compar, target);

    while (!isEmpty(performance, temp_ptr) && direction != 0) {//loop if this is not the node were looking for and it is not empty
        temp_ptr = next(performance, temp_ptr, direction);//got to the next node in the tree based on direction
        if (!isEmpty(performance, temp_ptr)) {//if not empty (if it doesnt not exist)
            direction = comparNode(performance, temp_ptr, compar, target);//check if found or calculate direction
        }
    }
    
    if (direction == 0) {// if found copy the data and return 1
        readNode(performance, temp_ptr, target, width);
        return 1;        
    }
    return 0;
}