#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ycc/algos/treap.h>

struct node {
	int val;
	struct treap_node treap_node;
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

static inline int compare(const struct treap_node *treap_node, const void *arg)
{
	struct node *p = treap_entry(treap_node, struct node, treap_node);

	return p->val - *(int*)arg;
}

static int compare_link(const struct treap_node *treap_node1,
			const struct treap_node *treap_node2,
			const void *arg)
{
	struct node *p1 = treap_entry(treap_node1, struct node, treap_node);
	struct node *p2 = treap_entry(treap_node2, struct node, treap_node);
	return p1->val - p2->val;
}

static void destroy(struct treap_node *node, const void *arg)
{
	node_free(treap_entry(node, struct node, treap_node));
}

int main()
{
	int i, val;
	struct node *p;
	struct treap_node *treap_node;

	TREAP_DECLARE(treap);

	srand( (unsigned int)time(NULL) );

	for (i = 0; i < 1024*1024; ++i) {
		p = node_alloc(rand()%100 + 100);
		treap_insert(&p->treap_node, &treap, compare_link, NULL);
	}

#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", treap_height_min(&treap), treap_height_max(&treap));
#endif
	if (!treap_isvalid(&treap)) {
		printf("treap_isvalid failed !\n");
		return 1;
	}

	treap_node = treap_first(&treap);
	if (treap_node) {
		val = treap_entry(treap_node, struct node, treap_node)->val;
		treap_node = treap_next(treap_node);

		while (treap_node) {
			register int val_new = treap_entry(treap_node, struct node, treap_node)->val;
			if (val > val_new) {
				printf("error: %d, %d\n", val, val_new);
			}
			treap_node = treap_next(treap_node);
		}
	}
	treap_node = treap_first(&treap);
	printf("1 node_cnt: %d\n", node_cnt);
	i = 105;
	treap_erase_equal(&treap, compare, destroy, &i, NULL);
	if (!treap_isvalid(&treap)) {
		printf("treap_isvalid failed 1!\n");
		return 1;
	}
	printf("2 node_cnt: %d\n", node_cnt);
	treap_clear(&treap, destroy, NULL);
	printf("3 node_cnt: %d\n", node_cnt);
	treap_node = treap_first(&treap);
	if (treap_node) {
		printf("error: treap_node should be null\n");
	}


	return 0;
}
