#include <stdio.h>
#include <stdlib.h>

#include "replace.h"
#include "opt.h"

int main(int argc, char ** argv)
{
	if (argc < 4) {
		perror("[ERROR] Too few arguments");
		exit(-1);
	}
	struct replace_opt *rplce_opt = create_rplce_opt();
	bool tmp = parse_rplce_opt(rplce_opt, argc - 1, argv+1);

	if (tmp == false) {
		perror("[ERROR] Parse options failed\n");
		exit(-1);
	} else {
		printf("success\n");
	}
	destroy_rplce_opt(rplce_opt);
	return 0;

}