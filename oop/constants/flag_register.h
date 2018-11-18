#ifndef __FLAG_REGISTER__
#define __FLAG_REGISTER__

typedef struct {
	unsigned int halt:1;
	unsigned int overflow:1;
	unsigned int input:1;
	unsigned int exit_code:2;
} FlagRegister;

#endif
