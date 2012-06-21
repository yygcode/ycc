#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ycc/algos/sptree.h>

#ifndef SIZE
#define SIZE 1024*100
#endif

struct node {
	int val;
	struct spt_node spt_node;
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

static inline int compare(const struct spt_node *spt_node, const void *arg)
{
	struct node *p = spt_entry(spt_node, struct node, spt_node);

	return p->val - *(int*)arg;
}

static int compare_link(const struct spt_node *spt_node1,
			const struct spt_node *spt_node2,
			const void *arg)
{
	struct node *p1 = spt_entry(spt_node1, struct node, spt_node);
	struct node *p2 = spt_entry(spt_node2, struct node, spt_node);
	return p1->val - p2->val;
}

static void destroy(struct spt_node *node, const void *arg)
{
	node_free(spt_entry(node, struct node, spt_node));
}

int main()
{
	int i, val;
	struct node *p;
	struct spt_node *spt_node;

	SPT_DECLARE(spt);

	srand( (unsigned int)time(NULL) );

	for (i = 0; i < SIZE; ++i) {
		p = node_alloc(rand()%100 + 100);
		spt_insert(&p->spt_node, &spt, compare_link, NULL);
	}

#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", spt_height_min(&spt), spt_height_max(&spt));
#endif

	spt_node = spt_first(&spt);
	if (spt_node) {
		val = spt_entry(spt_node, struct node, spt_node)->val;
		spt_node = spt_next(spt_node);

		while (spt_node) {
			register int val_new = spt_entry(spt_node, struct node, spt_node)->val;
			if (val > val_new) {
				printf("error: %d, %d\n", val, val_new);
			}
			spt_node = spt_next(spt_node);
		}
	}
#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", spt_height_min(&spt), spt_height_max(&spt));
#endif
	spt_node = spt_first(&spt);
	printf("1 node_cnt: %d\n", node_cnt);
	i = 105;
	spt_erase_equal(&spt, compare, destroy, &i, NULL);
#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", spt_height_min(&spt), spt_height_max(&spt));
#endif
	printf("2 node_cnt: %d\n", node_cnt);
	spt_clear(&spt, destroy, NULL);
	printf("3 node_cnt: %d\n", node_cnt);
#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", spt_height_min(&spt), spt_height_max(&spt));
#endif
	spt_node = spt_first(&spt);
	if (spt_node) {
		printf("error: spt_node should be null\n");
	}


	return 0;
}
