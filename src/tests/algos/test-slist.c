#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <ycc/algos/slist.h>

struct node {
	struct slist_head slist;
	int data;
};

int int_cmp(void *priv, struct slist_head *a, struct slist_head *b)
{
	struct node *n1, *n2;

	n1 = slist_entry(a, typeof(*n1), slist);
	n2 = slist_entry(b, typeof(*n1), slist);

	return n1->data - n2->data;
}

void do_test(unsigned int c)
{
	unsigned int i;
	struct node *p, *n;
	SLIST_HEAD(head);

	for (i = 0; i < c; ++i) {
		p = (struct node*)malloc(sizeof(*p));

		if (!p) {
			perror("malloc failed");
			exit(1);
		}

		p->data = c - i;
		slist_add(&p->slist, &head);
	}

	slist_for_each_entry(p, &head, slist) printf("%d ", p->data);
	printf("\n");

	slist_reverse(&head);

	slist_for_each_entry(p, &head, slist) printf("%d ", p->data);
	printf("\n");

	srand(time(NULL));
	slist_for_each_entry(p, &head, slist) p->data = (size_t)rand()%100;
	printf("before sort: ");
	slist_for_each_entry(p, &head, slist) printf("%d ", p->data);
	printf("\n");
	slist_sort(NULL, &head, int_cmp);
	printf("after sort: ");
	slist_for_each_entry(p, &head, slist) printf("%d ", p->data);
	printf("\n");
	slist_for_each_entry_safe(p, n, &head, slist) free(p);
}

int main()
{
	unsigned int i;
	for (i = 1; i < 10; ++i) {
		printf("--------------------------\n");
		do_test(i);
	}

	return 0;
}
