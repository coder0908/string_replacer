#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

// #include <fcntl.h>
// #include <sys/uio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>

#define LISTFILE_INDEX 1

#define FLIST_SEPERATER '\n'
#define FNAME_MAX_LEN 40


int get_flist_fcnt(FILE *flist_f);
int get_flist_fnames(FILE *flist_f, char *dest, int max_entry_len, int max_cnt);

int replace_fstr(FILE *target_f, const char *start_str, int start_str_len, const char *end_str, int end_str_len, const char * replace_str, int replace_str_len);
long find_string(FILE *f, const char *str, int str_size);

int main(int argc, char **argv)
{
	if (argc < 5) {
		fprintf(stderr, "[ERROR] Too few arguments\n");
		fprintf(stderr, "[ERROR] -> ex) ./replacer target.txt start_str end_str replace_str");
		exit(-1);
	}

	FILE *flist_f = fopen("list.txt", "r");

	int target_cnt = get_flist_fcnt(flist_f);
	printf("[INFO] target count = %d\n", target_cnt);

	char *target_names = malloc(FNAME_MAX_LEN * target_cnt);
	memset(target_names, 0, FNAME_MAX_LEN * target_cnt);
	
	int read_target_cnt = get_flist_fnames(flist_f, target_names, FNAME_MAX_LEN, target_cnt);  
	fclose(flist_f);

	if (read_target_cnt == -1) {
		fprintf(stderr, "[ERROR] {fn:get_flist_names} target name is too long\n");
		goto error;
	}
	if (read_target_cnt == -2) {
		fprintf(stderr, "[ERROR] {fn:get_flist_names} arguments pointer is NULL");
		goto error;
	}
	if (read_target_cnt != target_cnt) {
		fprintf(stderr, "[ERROR] {fn:get_flist_names} reading \"file names list\" failed\n");
		fprintf(stderr, "->expect count = %d, read count = %d\n", target_cnt, read_target_cnt);
		goto error;
	}

	bool success = false;
	FILE *target_f;
	int tmp = !0;
	for (int i = 0; i < target_cnt; i++) {
		target_f = fopen(target_names + FNAME_MAX_LEN * i,  "r+");
		printf("\n[INFO] number = %d, file name = %s\n", i, target_names + FNAME_MAX_LEN * i);
		if (target_f == NULL)
			fprintf(stderr, "[ERROR] {fn:fopen} file open failed\n");

		tmp = replace_fstr(target_f, "#include ", 9, "stm32f4xx_hal.h\"", 16, "#includes \"stm32f4xx_hal.h\"", 26);
		fclose(target_f);
		success = (tmp == 0)?true:false;  
		if (!success) {
			fprintf(stderr, "[ERROR] {fn:replace_fstr} failed file's index = %d\n", i);
			goto error;
		}
	}

	

	ret:
	free(target_names);
	return 0;

	error:
	free(target_names);
	return -1;
}


int get_flist_fcnt(FILE *flist_f)
{
	char tmp_char = 0;
	int fcnt = 0;

	fseek(flist_f, 0, SEEK_SET);	int i;
	for (i = 0; ; i++) {
		tmp_char = fgetc(flist_f);
		if (tmp_char == '\n')
			fcnt++;
		if (tmp_char == EOF) {
			//printf("[INFO] {fn:get_flist_fcnt} End Of File\n");
			break;
		}
		//printf("[INFO] {fn:get_flist_fcnt} target file count = %d\n", fcnt);
		//printf("[INFO] {fn:get_flist_fcnt} list.txt position = %ld\n", fseek(flist_f, 0, SEEK_CUR));
		//printf("[INFO] {fn:get_flist_fcnt} read char = \"%c\"\n", tmp_char);
	}
	return fcnt;
}

//include line feed 
int strcopy_ln(const char * src, char *dest, int size)
{	
	if (dest == NULL)
		return -1;

	int i;
	for (i = 0; i < size; i++) {
		if (src[i] == '\n')
			break;
		dest[i] = src[i];
	}
	return i;
}

//return read names
int get_flist_fnames(FILE *flist_f, char *dest, int name_max_entry, int name_cnt)
{
	if (dest == NULL) {
		return -2;
	}
	if (name_max_entry < 1) {
		return 0;
	}
	// rewind(flist_f);
	fseek(flist_f, 0, SEEK_SET);
	bool error = false;
	char *name = malloc(name_max_entry);

	int read_cnt = 0;

	for (;;) {
		if (error) {
			free(name);
			return -3;
		}

		if (read_cnt == name_cnt)
			break;
		
		
		error = true;

		memset(name, 0, name_max_entry);

		for (int i = 0; i < name_max_entry; i++) {
			name[i] = fgetc(flist_f);
			if (name[i] == '\n') {
				strcopy_ln(name, dest + (read_cnt * name_max_entry), name_max_entry);
				read_cnt++;
				error = false;
				break;
			}
		}
	}
	free(name);
	return read_cnt;
}

//return offset from start of file
long find_string(FILE *f, const char *str, int str_size)
{
	
	if (str == NULL)
		return -1;
			
	// rewind(f);
	fseek(f, 0, SEEK_SET);
	int index_cnter = 0;
	long pos = -2;
	char tmp_char = 0;

	for (;;) {
		
		if (index_cnter == str_size)
			return pos;
		tmp_char = fgetc(f);
		if (tmp_char == EOF) 
			return -2;

		if (tmp_char == str[index_cnter]) {
			if (index_cnter == 0)
				pos = ftell(f) - 1; // 확인을 위해 읽었기 때문에, 시작 주소 +1이 반환된다. 따라서 -1해줘야 함.
			index_cnter++;
			//printf("[INFO] {fn:find_string} index counter rate %d/%d\n", index_cnter, str_size);

		} else {
			index_cnter = 0;
		}
	}


	
}


//0 means success other mean error
int replace_fstr(FILE *target_f, const char *start_str, int start_str_len, const char *end_str, int end_str_len, const char * replace_str, int replace_str_len)
{
	if (target_f == NULL || start_str == NULL || end_str == NULL || replace_str == NULL) 
		return -1;

	// bool flags = (start_str == NULL)?true:false;
	long start_pos, end_pos;
	
	start_pos = find_string(target_f, start_str, start_str_len);
	end_pos = find_string(target_f, end_str, end_str_len) + end_str_len;

	if (start_pos < 0 || end_pos < 0)
		fprintf(stderr, "[ERROR] {fn:replace_fstr} can't find same string\n");

	printf("[INFO] {fn:replace_fstr} start_pos = %ld, end_pos = %ld\n", start_pos, end_pos);
		
	// rewind(target_f);

	fseek(target_f, start_pos, SEEK_SET);
	for(int i = 0; i < replace_str_len; i++) {
		fputc(replace_str[i], target_f);
	}

	// fseek(target_f, replace_str_len, SEEK_CUR);
	// for (int i = 0; i < end_pos - start_pos - replace_str_len; i++) {
	// 	fputc(0, target_f);
	// }

	return 0;

}
