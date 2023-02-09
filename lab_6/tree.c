#include "tree.h"
#include "io.h"
#include <stdlib.h>
#include <string.h>

node_t *node_create(char *word)
{
    node_t *node;
    node = malloc(sizeof(node_t));
    
    if (!node)
        return NULL;

    char *tmp = strdup(word);
    if (tmp == NULL)
        return NULL;
    
    node->word = tmp;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void node_print(node_t *node, void *param)
{
    const char *frm = param;
    printf(frm, node->word);
}

void node_free(node_t *node, void *param)
{
    (void) param;
    free(node->word);
    free(node);
}

void node_to_dot(node_t *tree, void *param)
{
    FILE *f = param;

    if (tree->left)
        fprintf(f, "\"%s\" -> \"%s\";\n", tree->word, tree->left->word);

    if (tree->right)
        fprintf(f, "\"%s\" -> \"%s\";\n", tree->word, tree->right->word);
}

node_t *btree_insert(node_t *tree, node_t *node)
{
    int cmp;

    if (tree == NULL)
        return node;

    cmp = strcmp(node->word, tree->word);
    if (cmp < 0)
        tree->left = btree_insert(tree->left, node);
    else if (cmp > 0)
        tree->right = btree_insert(tree->right, node);
    else
    {
        printf("Элемент уже существует\n");
        return tree;
    }

    return tree;
}

node_t *btree_find(node_t *tree, char *name)
{
    int cmp;

    while (tree != NULL)
    {
        cmp = strcmp(name, tree->word);
        if (cmp == 0)
            return tree;
        else if (cmp < 0)
            tree = tree->left;
        else
            tree = tree->right;
    }

    return NULL;
}

node_t *btree_find_prev(node_t *tree, node_t *node)
{
    int cmp;
    node_t *prev = NULL;

    while (tree != NULL)
    {
        cmp = strcmp(node->word, tree->word);
        if (cmp == 0)
            return prev;
        else if (cmp < 0)
        {
            prev = tree;
            tree = tree->left;
        }
        else
        {
            prev = tree;
            tree = tree->right;
        }
    }

    return NULL;
}

node_t *btree_del_elem(node_t *tree, node_t *target)
{
    node_t *prev = btree_find_prev(tree, btree_find(tree, target->word));

    if (target->left == NULL && target->right == NULL)
    {
        if (prev->right == target)
            prev->right = NULL;
        else
            prev->left = NULL;
    }
    else if (target->right == NULL || target->left == NULL)
    {
        if (prev->right == target)
        {
            if (target->right != NULL)
                prev->right = target->right;
            else
                prev->right = target->left;
        }
        else
        {
            if (target->right != NULL)
                prev->left = target->right;
            else
                prev->left = target->left;
        }
    }
    else
    {
        node_t *left_max = find_max(target->left);
        char *tmp = strdup(left_max->word);
        tree = btree_del_elem(tree, left_max);
        free(target->word);
        target->word = tmp;
        return tree;
    }

    node_free(target, NULL);
    return tree;
}

node_t *find_max(node_t *tree)
{
    node_t *cur = tree;

    while (cur->right != NULL)
        cur = cur->right;

    return cur;
}

void btree_apply_pre(node_t *tree, void (*f)(node_t *, void *), void *arg)
{
    if (tree == NULL)
        return;

    f(tree, arg);
    btree_apply_pre(tree->left, f, arg);
    btree_apply_pre(tree->right, f, arg);
}

void btree_apply_post(node_t *tree, void (*f)(node_t *, void *), void *arg)
{
    if (tree == NULL)
        return;

    btree_apply_post(tree->left, f, arg);
    btree_apply_post(tree->right, f, arg);
    f(tree, arg);
}

void btree_export_to_dot(node_t *tree, FILE *add)
{
    FILE *f = fopen("tree.gv", "w");

    fprintf(f, "digraph %s {\n", "tree");
    fprintf(f, "node [style=filled]\n");

    btree_apply_pre(tree, node_to_dot, f);
    if (add != NULL)
        file_copy(add, f);

    fprintf(f, "}\n");

    fclose(f);
}

void add_color(node_t *node)
{
    FILE *f = fopen("color", "w");
    fprintf(f, "\"%s\" [fillcolor=greenyellow]\n", node->word);
    fclose(f);
}

void find_words(node_t *tree, char symb, int *count)
{
    if (tree == NULL)
        return;

    if (tree->word[0] == symb)
    {
        (*count)++;
        FILE *f = fopen("color", "a");
        fprintf(f, "\"%s\" [fillcolor=coral]\n", tree->word);
        fclose(f);
    }

    find_words(tree->left, symb, count);
    find_words(tree->right, symb, count);
}
