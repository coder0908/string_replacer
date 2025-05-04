#ifndef __REPLACE_H__
#define __REPLACE_H__

#include "opt.h"
#include <errno.h>
#include <stdbool.h>

struct replacer {
	struct replace_opt rplce_opt;

	unsigned long file_cnt;
	FILE **files;
};

struct replacer* create_rplcer(const struct replace_opt *rplce_opt);
void destroy_rplcer(struct replacer *rplcer);
bool rplce_files();


#endif /*__REPLACE_H__*/