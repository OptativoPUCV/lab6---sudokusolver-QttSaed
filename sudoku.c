#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int row_used[9][10] = {0};
    int col_used[9][10] = {0};
    int box_used[9][10] = {0};

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int box_idx = (i / 3) * 3 + j / 3;
            int val = n->sudo[i][j];

            if (val != 0) {
                if (row_used[i][val] || col_used[j][val] || box_used[box_idx][val]) {
                    return 0;
                }

                row_used[i][val] = 1;
                col_used[j][val] = 1;
                box_used[box_idx][val] = 1;
            }
        }
    }

    return 1;
}


List* get_adj_nodes(Node* n){
   List* list = createList();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                for (int val = 1; val <= 9; val++) {
                    Node* copy_node = copy(n);
                    copy_node->sudo[i][j] = val;
                    if (is_valid(copy_node)) {
                        pushBack(list, copy_node);
                    } else {
                        free(copy_node);
                    }
                }
                return list;
            }
        }
    }
    return list;
}



int is_final(Node* n){
   for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

Node* DFS(Node* initial, int* cont){
   Stack* s = createStack();
    push(s, initial);

    while (!is_empty(s)) {
        Node* curr = top(s);
        pop(s);
        (*cont)++;

        int is_final = 1;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (curr->sudo[i][j] == 0) {
                    is_final = 0;
                    break;
                }
            }
            if (!is_final) break;
        }

        if (is_final) {
            free(s);
            return curr;
        }

        List* adj_nodes = get_adj_nodes(curr);
        Node* temp = first(adj_nodes);
        while (temp != NULL){
            push(s, temp);
            temp = next(adj_nodes);
           
        }
        
        free(curr);
        free(adj_nodes);
    }

    free(s);
    return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/