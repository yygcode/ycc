#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ycc/algos/bstree.h>

#ifndef SIZE
#define SIZE 1024*100
#endif

struct node {
	int val;
	struct bst_node bst_node;
};

static int node_cnt = 0;

struct node* node_alloc(int val)
{
	struct node *p = malloc(sizeof(*p));

	if (p) {
		p->val = val;
		++node_cnt;
	}

	return p;
}

void node_free(struct node *p)
{
	free(p);
	if(p)--node_cnt;
}

static inline int compare(const struct bst_node *bst_node, const void *arg)
{
	struct node *p = bst_entry(bst_node, struct node, bst_node);

	return p->val - *(int*)arg;
}

static int compare_link(const struct bst_node *bst_node1,
			const struct bst_node *bst_node2,
			const void *arg)
{
	struct node *p1 = bst_entry(bst_node1, struct node, bst_node);
	struct node *p2 = bst_entry(bst_node2, struct node, bst_node);
	return p1->val - p2->val;
}

static void destroy(struct bst_node *node, const void *arg)
{
	node_free(bst_entry(node, struct node, bst_node));
}

int main()
{
	int i, val;
	struct node *p;
	struct bst_node *bst_node;

	BST_DECLARE(bst);

	srand( (unsigned int)time(NULL) );

	for (i = 0; i < SIZE; ++i) {
		p = node_alloc(rand()%100 + 100);
		bst_insert(&p->bst_node, &bst, compare_link, NULL);
	}

#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", bst_height_min(&bst), bst_height_max(&bst));
#endif

	bst_node = bst_first(&bst);
	if (bst_node) {
		val = bst_entry(bst_node, struct node, bst_node)->val;
		bst_node = bst_next(bst_node);

		while (bst_node) {
			register int val_new = bst_entry(bst_node, struct node, bst_node)->val;
			if (val > val_new) {
				printf("error: %d, %d\n", val, val_new);
			}
			bst_node = bst_next(bst_node);
		}
	}
	bst_node = bst_first(&bst);
	printf("1 node_cnt: %d\n", node_cnt);
	i = 105;
	bst_erase_equal(&bst, compare, destroy, &i, NULL);
	printf("2 node_cnt: %d\n", node_cnt);
	bst_clear(&bst, destroy, NULL);
	printf("3 node_cnt: %d\n", node_cnt);
	bst_node = bst_first(&bst);
	if (bst_node) {
		printf("error: bst_node should be null\n");
	}


	return 0;
}
