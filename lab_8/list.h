#ifndef __LIST_H__
#define __LIST_H__

typedef struct node
{
    int v;
    int weight;
    struct node *next;
} node_t;

typedef struct graph
{
    node_t **arr;
    int len;
} graph_t;

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}
#define MIN(a, b) ((a < b) ? a : b)

#define OK 0
#define ALLOC_ERROR 1

void print_graph_list(graph_t *graph, int *colors, int s);
int check_graph(graph_t *graph);
int input_graph_list(graph_t *graph);
node_t *list_add_end(node_t *head, node_t *node);
node_t *find_node(graph_t *graph, int i, int j);
void find_min_v_list(graph_t *graph, int *res, int *size);
void free_graph_list(graph_t *graph);

#endif