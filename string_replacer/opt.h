#ifndef __OPT_H__
#define __OPT_H__

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>


#define OPT_SYM_TRGTSTR 		'r'
#define OPT_SYM_NEWSTR 			'n'
#define OPT_SYM_STARTSTR 		's'
#define OPT_SYM_ENDSTR 			'e'
#define OPT_SYM_FROMSTART 		'S'
#define OPT_SYM_TOEND 			'E'
#define OPT_SYM_LISTFILE 		'l'
#define OPT_SYM_SINGLEFILE 		't'
#define OPT_SYM_RPLCEMAXCNT 		'c'
#define OPT_SYM_RPLCEJMPCNT 		'j'

struct replace_opt {
	struct {
		unsigned long single_file_cnt;
		char **single_files;

		unsigned long list_file_cnt;
		char **list_files;
	} files;

	struct {
		unsigned long trgt_str_cnt;
		char **trgt_strs;

		char *start_str;
		char *end_str;
		char *new_str;

		unsigned long rplce_max_cnt;
		unsigned long rplce_jmp_cnt;
	} strings;

	bool from_start;
	bool to_end;
};


struct replace_opt* create_rplce_opt();
void destroy_rplce_opt(struct replace_opt *rplce_opt);
bool parse_rplce_opt(struct replace_opt *rplce_opt, int argc, char **argv);

#endif /*__OPT_H__*/