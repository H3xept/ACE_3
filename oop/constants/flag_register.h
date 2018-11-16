typedef struct {
	unsigned int halt:1;
	unsigned int overflow:1;
	unsigned int input:1;
	unsigned int exit_code:2;
} FlagRegister;
