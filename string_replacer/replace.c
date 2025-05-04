#include "replace.h"
#include <stdio.h>
#include <stdlib.h>

static bool single_files_opener(struct replacer *rplcer)
{
	if (rplcer == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:file_opener}\n");
		return false;
	}
	
	const struct replace_opt rplce_opt = rplcer->rplce_opt;
	const char **names = rplce_opt.files.single_files;
	const unsigned long name_cnt = rplce_opt.files.single_file_cnt;

	void *tmp = NULL;
	if (rplcer->files == NULL) {
		if (rplcer->file_cnt != 0) {
			perror("[ERROR] {fn:single_files_opener}\n");
			return false;
		}
		tmp = rplcer->files = malloc(name_cnt * sizeof(FILE*)); //!!free size
	} else {
		tmp = realloc(rplcer->files, (name_cnt + rplcer->file_cnt) * sizeof(FILE*));
	}

	if (tmp == NULL) {
		perror("[ERROR] {fn:single_files_opener}\n");
		return false;
	}

	for (unsigned long i = 0; i < name_cnt; i++) {
		if (names[i] == NULL) {
			// if (i != name_cnt - 1) {
			errno = EINVAL; //적절한 에러로 바꾸기// opt.c 바꾸기. 쓰레기 
			perror("[ERROR] {fn:single_files_opener}\n");
			// }
			goto fail;
		}

		tmp = rplcer->files[rplcer->file_cnt] = fopen(names[i], "r+");
		if (tmp == NULL) {
			perror("[ERROR] {fn:single_files_opener}\n");
			goto fail;
		}
		rplcer->file_cnt++;
	}

	return true;

	fail:
	relloc(rplcer->files, rplcer->file_cnt * sizeof(FILE*));
	return false;
}

static bool list_file_opener(struct replacer *rplcers)
{
	if (rplcer == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:file_opener}");
		return false;
	}
	struct replace_opt *rplce_opt = rplcer->rplce_opt;

}

static bool file_opener(struct replacer *rplcer)
{
	if (rplcer == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:file_opener}");
		return false;
	}

	struct replace_opt *rplce_opt = rplcer->rplce_opt;
	list_file_opener(rplcer);
	single_file_opener(rplcer);
	
}


struct replacer* create_rplcer()
{

}

void destroy_rplcer(struct replacer *rplcer)
{

}

bool rplce_files(const struct replace_opt *rplce_opt)
{

}