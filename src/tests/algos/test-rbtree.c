#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ycc/algos/rbtree.h>

struct node {
	int val;
	struct rb_node rb_node;
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

static inline int compare(const struct rb_node *rb_node, const void *arg)
{
	struct node *p = rb_entry(rb_node, struct node, rb_node);

	return p->val - *(int*)arg;
}

static int compare_link(const struct rb_node *rb_node1,
			const struct rb_node *rb_node2,
			const void *arg)
{
	struct node *p1 = rb_entry(rb_node1, struct node, rb_node);
	struct node *p2 = rb_entry(rb_node2, struct node, rb_node);
	return p1->val - p2->val;
}

static void destroy(struct rb_node *node, const void *arg)
{
	node_free(rb_entry(node, struct node, rb_node));
}

int main()
{
	int i, val;
	struct node *p;
	struct rb_node *rb_node;

	RB_DECLARE(rb);

	srand( (unsigned int)time(NULL) );

	for (i = 0; i < 1024*1024; ++i) {
		p = node_alloc(rand()%100 + 100);
		rb_insert(&p->rb_node, &rb, compare_link, NULL);
	}

#ifndef NDEBUG
	printf("height: {%zu, %zu}\n", rb_height_min(&rb), rb_height_max(&rb));
#endif

	rb_node = rb_first(&rb);
	if (rb_node) {
		val = rb_entry(rb_node, struct node, rb_node)->val;
		rb_node = rb_next(rb_node);

		while (rb_node) {
			register int val_new = rb_entry(rb_node, struct node, rb_node)->val;
			if (val > val_new) {
				printf("error: %d, %d\n", val, val_new);
			}
			rb_node = rb_next(rb_node);
		}
	}
	rb_node = rb_first(&rb);
	printf("1 node_cnt: %d\n", node_cnt);
	i = 105;
	rb_erase_equal(&rb, compare, destroy, &i, NULL);
	printf("2 node_cnt: %d\n", node_cnt);
	rb_clear(&rb, destroy, NULL);
	printf("3 node_cnt: %d\n", node_cnt);
	rb_node = rb_first(&rb);
	if (rb_node) {
		printf("error: rb_node should be null\n");
	}


	return 0;
}
