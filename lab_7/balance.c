#include "balance.h"
#include <string.h>

int height(node_t* p)
{
	return p ? p->height : 0;
}

int bfactor(node_t* p)
{
	return height(p->right) - height(p->left);
}

void fix_height(node_t* p)
{
	int hl = height(p->left);
	int hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

void fix_tree_height(node_t *tree)
{
    if (!tree)
        return;
    
    fix_tree_height(tree->left);
    fix_tree_height(tree->right);
    fix_height(tree);
}

node_t* rotate_right(node_t* p)
{
	node_t* q = p->left;
	p->left = q->right;
	q->right = p;

	fix_height(p);
	fix_height(q);

	return q;
}

node_t* rotate_left(node_t* q)
{
	node_t* p = q->right;
	q->right = p->left;
	p->left = q;

	fix_height(q);
	fix_height(p);

	return p;
}

node_t* balance_node(node_t* p)
{
	fix_height(p);

	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotate_right(p->right);

		return rotate_left(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotate_left(p->left);

		return rotate_right(p);
	}
	return p;
}

void balance_tree(node_t *tree, node_t **avl_tree)
{
    if (!tree)
        return;

    balance_tree(tree->left, avl_tree);
    balance_tree(tree->right, avl_tree);

    *avl_tree = avl_insert(*avl_tree, tree->word);
}

node_t* avl_insert(node_t *tree, char *word)
{
	if (!tree)
        return node_create(word);

	if (strcmp(word, tree->word) < 0)
		tree->left = avl_insert(tree->left, word);
	else if (strcmp(word, tree->word) > 0)
		tree->right = avl_insert(tree->right, word);
        
	return balance_node(tree);
}

node_t* find_min(node_t* tree)
{
	return (tree->left) ? find_min(tree->left) : tree;
}

node_t* del_min(node_t* tree)
{
	if (!tree->left)
		return tree->right;
	
	tree->left = del_min(tree->left);

	return balance_node(tree);
}

node_t* avl_del_elem(node_t* tree, char *word)
{
	if (!tree)
        return NULL;

	if (strcmp(word, tree->word) < 0)
		tree->left = avl_del_elem(tree->left, word);
	else if (strcmp(word, tree->word) > 0)
		tree->right = avl_del_elem(tree->right, word);	
	else
	{
		node_t* l = tree->left;
		node_t* r = tree->right;
		node_free(tree, NULL);

		if (!r)
			return l;

		node_t* min = find_min(r);
		min->right = del_min(r);
		min->left = l;

		return balance_node(min);
	}

	return balance_node(tree);
}