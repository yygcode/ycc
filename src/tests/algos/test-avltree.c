#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ycc/algos/avltree.h>

struct node {
	int val;
	struct avl_node avl_node;
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

static inline int compare(const struct avl_node *avl_node, const void *arg)
{
	struct node *p = avl_entry(avl_node, struct node, avl_node);

	return p->val - *(int*)arg;
}

static int compare_link(const struct avl_node *avl_node1,
			const struct avl_node *avl_node2,
			const void *arg)
{
	struct node *p1 = avl_entry(avl_node1, struct node, avl_node);
	struct node *p2 = avl_entry(avl_node2, struct node, avl_node);
	return p1->val - p2->val;
}

static void destroy(struct avl_node *node, const void *arg)
{
	node_free(avl_entry(node, struct node, avl_node));
}

int main()
{
	int i, val;
	struct node *p;
	struct avl_node *avl_node;

	AVL_DECLARE(avl);

	srand( (unsigned int)time(NULL) );

	for (i = 0; i < 1024*1024; ++i) {
		p = node_alloc(rand()%100 + 100);
		avl_insert(&p->avl_node, &avl, compare_link, NULL);
	}

#ifndef NDEBUG
	printf("height: {%zu, %zu}\n",
	       avl_height_min(&avl), avl_height_max(&avl));
#endif
	if (!avl_isvalid(&avl)) {
		printf ("avl_isvalid failed !\n");
		return 1;
	}

	avl_node = avl_first(&avl);
	if (avl_node) {
		val = avl_entry(avl_node, struct node, avl_node)->val;
		avl_node = avl_next(avl_node);

		while (avl_node) {
			register int val_new = avl_entry(avl_node, struct node,
							 avl_node)->val;
			if (val > val_new) {
				printf("error: %d, %d\n", val, val_new);
			}
			avl_node = avl_next(avl_node);
		}
	}
	avl_node = avl_first(&avl);
	printf("1 node_cnt: %d\n", node_cnt);
	i = 105;
	avl_erase_equal(&avl, compare, destroy, &i, NULL);
	printf("2 node_cnt: %d\n", node_cnt);
	if (!avl_isvalid(&avl)) {
		printf ("avl_isvalid failed 1 !\n");
		return 1;
	}
	avl_clear(&avl, destroy, NULL);
	printf("3 node_cnt: %d\n", node_cnt);
	avl_node = avl_first(&avl);
	if (avl_node) {
		printf("error: avl_node should be null\n");
	}


	return 0;
}
