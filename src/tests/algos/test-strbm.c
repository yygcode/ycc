#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <config-os.h>

#include <ycc/algos/string.h>

int main()
{
	char needle[32];
	const char *haystack = "I'm yanyg, i'm from china, yyg is a good boy~!";
	size_t table_sgs[64], table_ebc[UCHAR_MAX+1];

	int i;
	for (i = 0; i < strlen(haystack); ++i) {
		strncpy(needle, haystack+i, 16);
		needle[16] = '\0';
		strbm_init(needle, strlen(needle), table_sgs, table_ebc);
		char *result = strbm_find(haystack, needle,
					  strlen(haystack), strlen(needle),
					  table_sgs, table_ebc);
		if (result)
			printf("find success: %s\n", result);
		else
			printf("find failed !\n");

	}

	return 0;
}
