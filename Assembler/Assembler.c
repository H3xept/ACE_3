#include <stdlib.h>
#include <stdio.h>
#include "../utilities/colors.h"
#include "../utilities/utilities.h"
#include <assert.h>

typedef uint16_t uword_t;
typedef int16_t word_t;

#include <string.h>
#include <ctype.h>

const int n = 1;
#define machine_is_bigendian() ( (*(char*)&n) == 0 )
// Private class methods for Program
uword_t __To_Big_Endian(uword_t word)
{	
	return ((word << 8) | (word >> 8));
}

void to_lower(char* str)
{
	if (!str){
		return;
	}
	for(int i = 0; str[i]; i++)
  		str[i] = tolower(str[i]);
}

char* sstrtok(char* str, char* divider)
{
	char* new = NULL;
	if (!str)
		goto RET;

	new = malloc(sizeof(char)*strlen(str)+1);
	strcpy(new, str);
	if (*(new+strlen(new)-1) == '\n')
		*(new+strlen(new)-1) = '\0';
RET:
	return strtok(new, divider);
}

int is_label (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return !(*p == '\0');
}

unsigned int str_to_int(char* str)
{
	if (!str){
		_err("No string provided!", NULL);
	}

	unsigned int rt = 0;
	for (int i = 0; i < strlen(str); i++)
		rt += *(str+i)*2-'0';
	return rt;
}

uint8_t reg_to_int(char* reg)
{
	static char* registers[] = {"$pc","$ir","$ra","$sp","$fp","$t1","$t2","$t3","$t4","$s1","$s2","$s3","$s4","$s5","$pr","$fr"};
	for (int i = 0; i < sizeof(registers)/sizeof(char*); i++)
		if (!strcmp(registers[i], reg)) return i;
	_err("Register %s not found.",reg);
}

unsigned int is_register(char* str)
{
	return *str == '$';
}

void require_register(char* str)
{
	if (!str || !is_register(str))
	{
		_err("A register is required!",NULL);
	}
}

void require_immediate(char* str)
{
	if (is_register(str) || is_label(str))
	{
		_err("An immediate is required!", NULL);
	}
}

void require_label(char* str)
{
	if (!is_label(str))
	{
		_err("A label is required!", NULL);
	}
}

void terminate_at_occurrence(char* str, char c)
{
	for (int i = 0; i < strlen(str); ++i)
		if (*(str+i) == c) {*(str+i) = '\0'; return;}
}

static uword_t handle_HALT()
{
	return 0x0000;
}

int index_of(char* str, char** arr, size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		if (!strcmp(str, *(arr+i))) return i;
	} return -1;
} 

static uword_t handle_JUMP(char** labels, uword_t* addresses, size_t size)
{
	char* fst = sstrtok(NULL," ");
	uword_t addr = 0;
	if (is_label(fst)) {
		int index = index_of(fst, labels, size);
		if (index == -1)
		{
			_err("Label not found!", NULL);
		}
		addr = addresses[index];
	} else {
		addr = (uword_t)strtol(fst, NULL, 0);
		if (addr > 4095)
		{
			_err("Immediate out of bounds for jump: %d (MAX: 4095)", addr);
		}
	}
	return 0x1000 | addr;
}

static uword_t handle_SKC()
{
	char* fst = sstrtok(NULL," ");
	require_register(fst);
	return 0x2000 | reg_to_int(fst);
}

static uword_t handle_LOAD()
{
	char* fst = sstrtok(NULL, " ");
	require_register(fst);
	char* snd = strtok(NULL, " ");
	if (!snd) {_err("Second argument required for LOAD", NULL);} 
	int8_t snd_int;
	if (is_register(snd))
	{
		snd_int = reg_to_int(snd);
	}else if (!is_label(snd)){
		word_t integ = strtol(snd, NULL, 0);
		if (integ < -64 || integ > 63) {
			_err("Immediate out of bounds!", NULL);
		}
		snd_int = integ;
	}
	else {
		_err("Labels not supported for LOAD", NULL);
	}
	return 0x3000 | reg_to_int(fst) << 8 | !is_register(snd)<<7 | (snd_int&0x7F);
}

static uword_t handle_STORE()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0x4000 | reg_to_int(r1)<<4 | reg_to_int(r2);
}

static uword_t handle_IN()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	return 0x5000 | reg_to_int(r1);
}

static uword_t handle_OUT()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* arg2 = sstrtok(NULL, " ");
	require_immediate(arg2);
	return 0x6000 | (strtol(arg2,NULL,0) > 0)<< 4 | reg_to_int(r1);
}

static uword_t handle_MOVE()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0x7000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_ADD()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0x8000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_MUL()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0x9000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_DIV()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0xA000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_AND()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0xB000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_OR()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0xC000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_NOT()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	return 0xD000 | reg_to_int(r1);
}

static uword_t handle_SHL()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0xE000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t handle_SHR()
{
	char* r1 = sstrtok(NULL, " ");
	require_register(r1);
	char* r2 = sstrtok(NULL, " ");
	require_register(r2);
	return 0xF000 | reg_to_int(r1) << 4 | reg_to_int(r2);
}

static uword_t* handle_LOADL(char** labels, uword_t* addresses, size_t size)
{	

	char* r1 = sstrtok(NULL, " ");
	char* label = sstrtok(NULL, " ");
	require_label(label);
	int index = index_of(label, labels, size);
	index = addresses[index];

	if (index == -1)
	{
		_err("Label not found!", NULL);
	}
	char* assembly = calloc(100, sizeof(char));
	uword_t* ret = calloc(5, sizeof(uword_t));

	if(index > 0 && index < 4096){
		if(index < 64){
			sprintf(assembly, "load %s %d",r1,index);
			strtok(assembly, " ");
			*(ret) = handle_LOAD();
		} else if(index < 128) {
			sprintf(assembly, "load %s %d",r1,index-128);
			strtok(assembly, " ");
			*(ret) = handle_LOAD();
		} else {
			sprintf(assembly, "load %s %d",r1,index>>6);
			strtok(assembly, " ");
			*(ret) = handle_LOAD();

			sprintf(assembly, "load $pr 6");
			strtok(assembly, " ");
			*(ret+1) = handle_LOAD();

			sprintf(assembly, "shl %s $pr",r1);
			strtok(assembly, " ");
			*(ret+2) = handle_SHL();

			sprintf(assembly, "load $pr %d",index&0x3f);
			strtok(assembly, " ");
			*(ret+3) = handle_LOAD();

			sprintf(assembly, "or %s $pr",r1);
			strtok(assembly, " ");
			*(ret+4) = handle_OR();
		} 

		if (index < 128)
		{
			sprintf(assembly, "load $pr 2");
			strtok(assembly, " ");
			*(ret+1) = handle_LOAD();

			sprintf(assembly, "add $pc $pr");
			strtok(assembly, " ");
			*(ret+2) = handle_ADD();

			sprintf(assembly, "or $pr $pr");
			strtok(assembly, " ");
			*(ret+3) = handle_OR();

			sprintf(assembly, "or $pr $pr");
			strtok(assembly, " ");
			*(ret+4) = handle_OR();
		}
		return ret;
	}
	return 0;
}

static size_t file_lines(char* file_name)
{
	assert(file_name);
	FILE* file = fopen(file_name, "r");
	size_t lines_n = 0;
	if (!file){
		_err("Couldn't open file %s",file_name);
	}
	char* buffer = calloc(1, sizeof(char)*100);
	while(fgets(buffer, sizeof(char)*100, file))
		lines_n++;
	fclose(file);
	return lines_n;
}

static char** get_lines(char* file_name)
{
	uword_t lines_n = file_lines(file_name);
	char** rt = malloc(sizeof(char*)*lines_n);
	for (int i = 0; i < lines_n; ++i)
		*(rt+i) = calloc(100, sizeof(char));

	FILE* file = fopen(file_name, "r");
	if (!file){
		_err("Couldn't open file %s",file_name);
	}
	char* buffer = calloc(1, sizeof(char)*100);
	int i = 0;
	while(fgets(buffer, sizeof(char)*100, file)){
		strcpy(*(rt+i),buffer);
		i++;
	}
	fclose(file);
	return rt;
}

unsigned int is_instruction(char* str)
{
	char* s = sstrtok(str, " ");
	printf("string->%s->[",str);
	switch(str_to_int(s))
	{
		case 658: //halt 
		case 696: //jump 
		case 498: //skc
		case 640: //load
		case 874: //store
		case 334: //in
		case 544: //out
		case 686: //move
		case 450: //add
		case 524: //mul
		case 502: //div
		case 470: //and
		case 354: //or
		case 530: //not
		case 510: //shl
		case 522: //shr
		case 808: // loadl
			printf("1]\n");
			return 1;
		default:
			printf("0]\n");
			return 0;
	}
}

char* trim(char* str)
{
	while (*str == ' ' || *str == '\t' || *str == '\n') str++;
	while (*(str+strlen(str)-1) == ' ' 
		|| *(str+strlen(str)-1) == '\t' 
		|| *(str+strlen(str)-1) == '\n') 
		*(str+strlen(str)-1) = '\0';
	return str;
}

unsigned int is_line_label(char* str)
{
	if (*(str+strlen(str)-1) == ':')
	{
		for (int i = 0; i < strlen(str); ++i)
			if ((*(str+i) == ' ' || *(str+i) == '\t')) return 0;
		return is_label(str);
	}return 0;
}

unsigned int is_in_list_s(char* str, char** list, size_t list_size) // Is in list - string
{
	for (int i = 0; i < list_size; ++i)
	{
		if (!strcmp(str,*(list+i))) return 1;
	} return 0;
}

unsigned int is_comment_or_whitespace(char* str)
{
	trim(str);
	if (*str == '\0' || *str == '#')
		return 1;
	return 0;
}

int main(int argc, char const *argv[])
{
	//char* operators[] = {"halt","jump","skc","load","store","in","out","move","add","mul","div","and","or","not","shl","shr"};
	//char* registers[] = {"PC","IR","RA","SP","FP","T1","T2","T3","T4","S1","S2","S3","S4","S5","PR","FR"};
	if (argc < 3)
		_err("Too few args. Use %s <file_name> <output_file_name>",argv[0]);
	
	const char* file_name = argv[1];
	const char* output_file_name = argv[2];

	char** lines = get_lines((char*)file_name);

	char** labels = malloc(sizeof(char*)*200);
	uword_t* addresses = malloc(sizeof(uword_t)*200);

	size_t inserted = 0;
	uword_t address_n = 1;

	int lines_number = file_lines((char*)file_name);
	for (int i = 0; i < lines_number; i++)
	{	
		char* line = calloc(100,sizeof(char));
		strcpy(line, *(lines+i));
		line = trim(line);
		terminate_at_occurrence(line,'#');
		if (*line == '\0')
			continue;

		char* operator = NULL;
		to_lower(line);

		if (is_instruction(line))
		{
			address_n++;
			operator = sstrtok(line, " ");
			if (!strcmp(operator,"loadl")){
				address_n+=4;
			}

		} else if (is_line_label(line)){
			*(line+strlen(line)-1) = '\0';
			if (is_in_list_s(line,labels,inserted))
			{
				_err("Multiple instances of label %s found.",line);
			}
			*(labels+inserted) = line;
			*(addresses+inserted) = address_n;
			inserted++;
		} else if (!is_label(line)) {
			address_n++;
		}
	}

	address_n = 0;
	uword_t* words = calloc(lines_number,sizeof(uword_t));

	for (int i = 0; i < lines_number; i++)
	{	
		char* line = calloc(100,sizeof(char));
		strcpy(line, *(lines+i));
		line = trim(line);
		terminate_at_occurrence(line,'#');
		if (*line == '\0')
			continue;

		char* operator = NULL;
		to_lower(line);
		if(!is_instruction(line)){
			if (!is_label(line))
			{	
				long int num = strtol(line, NULL, 0);
				if (num > 32768 || num < -32769)
				{_err("Immediate number out of bounds: %d (MAX 32768 | MIN -32769)",num);}
				words[address_n++] = num;
			} else if (!is_comment_or_whitespace(line)){
				if (*(line+strlen(line)-1) == ':')
					continue;
				_err("%s is not an operator of this arch.--", line);
			}
		} else{
			operator = sstrtok(line, " ");
			uword_t* loadlwords;
			switch(str_to_int(operator))
			{
				case 658: //halt 
					words[address_n++] = handle_HALT();
					break;
				case 696: //jump 
					words[address_n++] = handle_JUMP(labels, addresses, inserted);
					break;
				case 498: //skc
					words[address_n++] = handle_SKC();
					break;
				case 640: //load
					words[address_n++] = handle_LOAD();
					break;
				case 874: //store
					words[address_n++] = handle_STORE();
					break;
				case 334: //in
					words[address_n++] = handle_IN();
					break;
				case 544: //out
					words[address_n++] = handle_OUT();
					break;
				case 686: //move
					words[address_n++] = handle_MOVE();
					break;
				case 450: //add
					words[address_n++] = handle_ADD();
					break;
				case 524: //mul
					words[address_n++] = handle_MUL();
					break;
				case 502: //div
					words[address_n++] = handle_DIV();
					break;
				case 470: //and
					words[address_n++] = handle_AND();
					break;
				case 354: //or
					words[address_n++] = handle_OR();
					break;
				case 530: //not
					words[address_n++] = handle_NOT();
					break;
				case 510: //shl
					words[address_n++] = handle_SHL();
					break;
				case 522: //shr
					words[address_n++] = handle_SHR();
					break;
				case 808: // loadl
					loadlwords = handle_LOADL(labels, addresses, inserted);
					for (int i = 0; i < 5; i++)
						words[address_n++] = *(loadlwords+i);
					break;
				default:
					_err("%s is not an operator of this arch.", line);}
		}
	}

	FILE* file = fopen(output_file_name,"wb");
	for (int i = 0; i < address_n; ++i)
	{
		if(!machine_is_bigendian())
			words[i] = __To_Big_Endian(words[i]);
	}
	fwrite(words, sizeof(uword_t),address_n,file);
	fclose(file);
	return 0;
}
