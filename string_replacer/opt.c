#include "opt.h"

#include <string.h>
#include <stdlib.h>

#define OPT_SYMBOL_LEN 2
#define OPT_SYMBOL_START '-'

static int init_rplce_opt(struct replace_opt *rplce_opt);
static bool is_opt(const char *opt_str);


static int init_rplce_opt(struct replace_opt *rplce_opt)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:init_rplce_opt} errcode = -1\n");
		return -1;
	}

	memset(rplce_opt, 0, sizeof(struct replace_opt));
	rplce_opt->strings.rplce_max_cnt = 1;


	return 0;
}

static inline int set_opt_single_file(struct replace_opt *rplce_opt, const char *single_file)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_single_files} errcode = -1\n");
		return -1;
	}
	
	size_t size = strlen(single_file) + 1;
	unsigned long *cnt = &rplce_opt->files.single_file_cnt;
	void *tmp = NULL;

	if (*cnt == 0) {
		tmp = rplce_opt->files.single_files = malloc(sizeof(char*));
		*cnt += 1;
	} else if (rplce_opt->files.single_files[*cnt - 1] != NULL) {
		tmp = realloc(rplce_opt->files.single_files, sizeof(char*) * (*cnt + 1));
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_single_files} errcode = -2\n");
			return -2;
		}
		*cnt += 1;
		rplce_opt->files.single_files[*cnt - 1] = NULL;
	}

	tmp = rplce_opt->files.single_files[*cnt - 1] = malloc(size);
	if (tmp == NULL) {
		perror("[ERROR] {fn:set_opt_single_files} errcode = -3\n");
		return -3;
	}

	memset(rplce_opt->files.single_files[*cnt - 1], 0, size);
	
	tmp = strncpy(rplce_opt->files.single_files[*cnt - 1], single_file, size);
	if (tmp == NULL) {
		free(rplce_opt->files.single_files[*cnt - 1]);
		rplce_opt->files.single_files[*cnt - 1] = NULL;
		perror("[ERROR] {fn:set_opt_single_files} errcode = -4\n");
		return -4;
	}
	
	return 0;
	
}

static inline int set_opt_list_file(struct replace_opt *rplce_opt, const char *list_file)
{
	if (rplce_opt == NULL || list_file == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_list_file} errcode = -1\n");
		return -1;
	}

	size_t size = strlen(list_file) + 1;
	unsigned long *cnt = &(rplce_opt->files.list_file_cnt);
	void *tmp = NULL;

	if (*cnt == 0) {
		tmp = rplce_opt->files.list_files = malloc(sizeof(char*));
		*cnt += 1;
	} else if (rplce_opt->files.list_files[*cnt - 1] != NULL) {
		tmp = realloc(rplce_opt->files.list_files, sizeof(char*) * (*cnt + 1) );
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_list_file} errcode = -2\n");
			return -2;
		}
		*cnt += 1;
		rplce_opt->files.list_files[*cnt - 1] = NULL;
	}

	tmp = rplce_opt->files.list_files[*cnt - 1] = malloc(size);
	if (tmp == NULL) {
		perror("[ERROR] {fn:set_opt_list_file} errcode = -3\n");
		return -3;
	}

	memset(rplce_opt->files.list_files[*cnt - 1], 0 , size);
	
	tmp = strncpy(rplce_opt->files.list_files[*cnt - 1], list_file, size);
	if (tmp == NULL) {
		free(rplce_opt->files.list_files[*cnt - 1]);
		rplce_opt->files.list_files[*cnt - 1] = NULL;
		perror("[ERROR] {fn:set_opt_list_file} errcode = -4\n");
		return -4;
	}
	
	return 0;
}

static inline int set_opt_trgt_str(struct replace_opt *rplce_opt, const char *trgt_str)
{
	if (rplce_opt == NULL || trgt_str == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_trgt_str} errcode = -1\n");
		return -1;
	}
	
	size_t size = strlen(trgt_str) + 1;
	unsigned long *cnt  = &(rplce_opt->strings.trgt_str_cnt);
	void * tmp = NULL;

	if (*cnt == 0) {
		tmp = rplce_opt->strings.trgt_strs = malloc(sizeof(char*));
		*cnt += 1;
	} else if (rplce_opt->strings.trgt_strs[*cnt - 1] != NULL) {
		tmp = realloc(rplce_opt->strings.trgt_strs, sizeof(char*) * (*cnt + 1));
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_trgt_str} errcode = -2\n");
			return -2;
		}
		*cnt += 1;
		rplce_opt->strings.trgt_strs[*cnt - 1] = NULL; //useless 아래 malloc이 실패 시 NULL이 나옴. 하지만 혹시 모르니 
	}

	tmp = rplce_opt->strings.trgt_strs[*cnt - 1] = malloc(size);
	if (tmp == NULL) {
		perror("[ERROR] {fn:set_opt_trgt_str} errcode = -3\n");
		return -3;
	}

	memset(rplce_opt->strings.trgt_strs[*cnt - 1], 0, size);
	tmp = strncpy(rplce_opt->strings.trgt_strs[*cnt - 1], trgt_str, size);
	if (tmp == NULL) {
		free(rplce_opt->strings.trgt_strs[*cnt - 1]);
		rplce_opt->strings.trgt_strs[*cnt - 1] = NULL;
		perror("[ERROR] {fn:set_opt_trgt_str} errcode = -4\n");
		return -4; 
	}
	return 0;
}	

static inline int set_opt_new_str(struct replace_opt *rplce_opt, const char *new_str)
{
	if (rplce_opt == NULL || new_str == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_new_str} errcode = -1\n");
		return -1;
	}

	size_t size = strlen(new_str) + 1;
	void * tmp = NULL;

	if (rplce_opt->strings.new_str == NULL) {
		tmp = rplce_opt->strings.new_str = malloc(size);
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_new_str} errcode = -2\n");
			return -2;
		}
		memset(rplce_opt->strings.new_str, 0, size);
		
	} else {
		tmp = realloc(rplce_opt->strings.new_str, size);
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_new_str} errcode = -3\n");
			return -3;
		}
	}
		
	tmp = strncpy(rplce_opt->strings.new_str, new_str, strlen(new_str) + 1);
	if (tmp == NULL) {
		perror("[ERROR] {fn:set_opt_new_str} errcode = -4\n");
		return -4;
	}
	
	return 0;
}

static inline int set_opt_rplce_max_cnt(struct replace_opt *rplce_opt, unsigned long cnt)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_rplce_max_cnt} errcode = -1\n");
		return -1;
	}
	
	rplce_opt->strings.rplce_max_cnt = cnt;
	return 0;
}

static inline int set_opt_rplce_jmp_cnt(struct replace_opt *rplce_opt, unsigned long cnt)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_rplce_jmp_cnt} errcode = -1\n");
		return -1;
	}
	rplce_opt->strings.rplce_jmp_cnt = cnt;
	return 0;
}

static inline int set_opt_start_str(struct replace_opt *rplce_opt, const char *start_str)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_start_str} errcode = -1\n");
		return -1;
	}
	size_t size = strlen(start_str) + 1;
	void * tmp = NULL;

	if (rplce_opt->strings.start_str != NULL) {
		if (start_str == NULL) {
			free(rplce_opt->strings.start_str);
			rplce_opt->strings.start_str = NULL;
			return 0;
		}
		tmp = realloc(rplce_opt->strings.start_str, size);
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_start_str} errcode = -2\n");
			return -2;
		}
	} else {
		tmp = rplce_opt->strings.start_str = malloc(size);
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_start_str} errcode = -3\n");
			return -3;
		}
		memset(rplce_opt->strings.start_str, 0, size);
	}
		
	tmp = strncpy(rplce_opt->strings.start_str, start_str, strlen(start_str) + 1);
	if (tmp == NULL) {
		perror("[ERROR] {fn:set_opt_start_str} errcode = -4\n");
		return -4;
	}
	
	return 0;
}

static inline int set_opt_end_str(struct replace_opt *rplce_opt, const char *end_str)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_end_str} errcode = -1\n");
		return -1;
	}
	size_t size = strlen(end_str) + 1;
	void * tmp = NULL;

	if (rplce_opt->strings.end_str != NULL) {
		if (end_str == NULL) {
			free(rplce_opt->strings.end_str);
			rplce_opt->strings.end_str = NULL;
			return 0;
		}
		tmp = realloc(rplce_opt->strings.end_str, size);
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_end_str} errcode = -2\n");
			return -2;
		}
	} else {
		tmp = rplce_opt->strings.end_str = malloc(size);
		if (tmp == NULL) {
			perror("[ERROR] {fn:set_opt_end_str} errcode = -3\n");
			return -3;
		}
		memset(rplce_opt->strings.end_str, 0, size);
	}
		
	tmp = strncpy(rplce_opt->strings.end_str, end_str, strlen(end_str) + 1);
	if (tmp == NULL) {
		perror("[ERROR] {fn:set_opt_end_str} errcode = -4\n");
		return -4;
	}
	
	return 0;
}

static inline int set_opt_from_start(struct replace_opt *rplce_opt, bool is)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_from_start} errcode = -1\n");
		return -1;
	}
	rplce_opt->from_start = is;
	return 0;
}

static inline int set_opt_to_end(struct replace_opt *rplce_opt, bool is)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:set_opt_to_end} errcode = -1\n");
		return -1;
	}
	rplce_opt->to_end = is;
	return 0;

}

static bool is_opt(const char *opt_str)
{
	if (opt_str == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:is_opt} \n");
		return false;
	}

	size_t str_len = strlen(opt_str);

	if (str_len != OPT_SYMBOL_LEN || opt_str[0] != OPT_SYMBOL_START) //not error
		return false;

	bool ret;
	switch(opt_str[1]) {
	case OPT_SYM_TRGTSTR:
		ret = true;
		break;
	case OPT_SYM_NEWSTR:
		ret = true;
		break;
	case OPT_SYM_STARTSTR:
		ret = true;
		break;
	case OPT_SYM_ENDSTR:
		ret = true;
		break;
	case OPT_SYM_FROMSTART:
		ret = true;
		break;
	case OPT_SYM_TOEND:
		ret = true;
		break;
	case OPT_SYM_LISTFILE:
		ret = true;
		break;
	case OPT_SYM_SINGLEFILE:
		ret = true;
		break;
	case OPT_SYM_RPLCEMAXCNT:
		ret = true;
		break;
	case OPT_SYM_RPLCEJMPCNT:
		ret = true;
		break;
	default:
		ret = false;
	}

	return ret;


}


struct replace_opt* create_rplce_opt()
{
	struct replace_opt *rplce_opt = malloc(sizeof(struct replace_opt));
	if (rplce_opt == NULL) {
		perror("[ERROR] {fn:create_rplce_opt}");
		return NULL;
	}
	if (init_rplce_opt(rplce_opt) != 0) {
		destroy_rplce_opt(rplce_opt);
		return NULL;
	}
	return rplce_opt;
}

void destroy_rplce_opt(struct replace_opt *rplce_opt)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:destroy_rplce_opt} errcode = -1\n");
		return;
	}
	
	unsigned long i;

	if (rplce_opt->files.single_files == NULL)
		goto list_file;
	for (i = 0; i < rplce_opt->files.single_file_cnt; i++) {
		if (rplce_opt->files.single_files[i] == NULL)
			continue;
		free(rplce_opt->files.single_files[i]);
	}
	free(rplce_opt->files.single_files);
	rplce_opt->files.single_file_cnt = 0;

	list_file:
	if (rplce_opt->files.list_files == NULL)
		goto trgt_strs;
	for (i = 0; i < rplce_opt->files.list_file_cnt; i++) {
		if (rplce_opt->files.list_files[i] == NULL)
			continue;
			free(rplce_opt->files.list_files[i]);
	}
	free(rplce_opt->files.list_files);
	rplce_opt->files.list_file_cnt = 0;

	trgt_strs:
	if (rplce_opt->strings.trgt_strs == NULL)
		goto new_str;
	for (i = 0; i < rplce_opt->strings.trgt_str_cnt; i++) {
		if (rplce_opt->strings.trgt_strs[i] == NULL)
			continue;
		free(rplce_opt->strings.trgt_strs[i]);
	}
	free(rplce_opt->strings.trgt_strs);
	rplce_opt->strings.trgt_str_cnt = 0;

	new_str:
	if (rplce_opt->strings.new_str != NULL)
		free(rplce_opt->strings.new_str);
	
	if (rplce_opt->strings.start_str != NULL)
		free(rplce_opt->strings.start_str);
	if (rplce_opt->strings.end_str != NULL)
		free(rplce_opt->strings.end_str);
	
	free(rplce_opt);
	return;
}

bool parse_rplce_opt(struct replace_opt *rplce_opt, int argc, char **argv)
{
	if (rplce_opt == NULL) {
		errno = EINVAL;
		perror("[ERROR] {fn:parse_rplce_opt}\n");
		return false;
	}
	
	long i;
	char * tmp_str;
	bool suc = false;

	for (i = 0; i < argc; i++) {
		tmp_str = argv[i];
		if (is_opt(tmp_str) != true ) {
			errno = EINVAL;
			return false;
		}
		
		switch(tmp_str[1]) {
		case OPT_SYM_TRGTSTR:
			suc = set_opt_trgt_str(rplce_opt, argv[++i]) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_NEWSTR:
			suc = set_opt_new_str(rplce_opt, argv[++i]) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_STARTSTR:
			suc = set_opt_start_str(rplce_opt, argv[++i]) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_ENDSTR:
			suc = set_opt_end_str(rplce_opt, argv[++i]) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_FROMSTART:
			suc = set_opt_from_start(rplce_opt, true) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_TOEND:
			suc = set_opt_to_end(rplce_opt, true) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_LISTFILE:
			suc = set_opt_list_file(rplce_opt, argv[++i]) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_SINGLEFILE:
			suc = set_opt_single_file(rplce_opt, argv[++i]) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_RPLCEMAXCNT:
			suc = set_opt_rplce_max_cnt(rplce_opt, atoi(argv[++i])) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		case OPT_SYM_RPLCEJMPCNT:
			suc = set_opt_rplce_jmp_cnt(rplce_opt, atoi(argv[++i])) == 0?true:false;
			if (!suc)
				goto fail;
			break;
		default:
			goto fail;//useless
		}
	}


	return true;

	fail:
	perror("[ERROR] {fn:parse_rplce_opt}\n");
	return false;

}


