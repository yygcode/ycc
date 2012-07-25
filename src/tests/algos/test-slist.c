#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ycc/algos/slist.h>

struct node {
	struct slist_head slist;
	int data;
};

int main()
{
	int i;
	struct node *p;
	SLIST_HEAD(head);

	for (i = 0; i < 10; ++i) {
		p = (struct node*)malloc(sizeof(*p));

		if (!p) {
			perror("malloc failed");
			exit(1);
		}

		p->data = 10 - i;
		slist_add(&p->slist, &head);
	}

	slist_for_each_entry(p, &head, slist) printf("%d ", p->data);
	printf("\n");

	slist_reverse(&head);

	slist_for_each_entry(p, &head, slist) printf("%d ", p->data);
	printf("\n");

	return 0;
}
