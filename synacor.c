#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t *memory;
uint16_t *stack;
uint16_t registers[8] = {0};
uint16_t iptr = 0;
uint16_t sptr = 0;

int halt() {
	return 0;
}

int set() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
	b = (b >= 32768 ? registers[b - 32768] : b);
	registers[a] = b;
	iptr += 3;
	return 1;
}

int push() {
	uint16_t a = memory[iptr+1];
	a = (a >= 32768 ? registers[a - 32768] : a);
	stack[sptr] = a;
	iptr += 2;
	sptr++;
	return 1;
}

int pop() {
	uint16_t a = memory[iptr+1]-32768;
	sptr--;
	if (sptr < 0)
		return 0;
	registers[a] = stack[sptr];
	iptr += 2;
	return 1;
}


int eq() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = (b == c);
	iptr += 4;
	return 1;

}

int gt() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = (b > c);
	iptr += 4;
	return 1;
}

int jmp() {
	uint16_t a = memory[iptr+1];
	iptr = (a >= 32768 ? registers[a - 32768] : a);	
	return 1;
}

int jt() {
	uint16_t a = memory[iptr+1];
       	uint16_t b = memory[iptr+2];
	a = (a >= 32768 ? registers[a - 32768] : a);
	b = (b >= 32768 ? registers[b - 32768] : b);
	if (a) {
		iptr = b;
	} else {
		iptr += 3;
	}
	return 1;
}

int jf() {
	uint16_t a = memory[iptr+1];
       	uint16_t b = memory[iptr+2];
	a = (a >= 32768 ? registers[a - 32768] : a);
	b = (b >= 32768 ? registers[b - 32768] : b);
	if (a) {
		iptr += 3;
	} else {
		iptr = b;
	}
	return 1;
}

int add() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = (b + c) % 32768;
	iptr += 4;
	return 1;
}

int mult() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = (b * c) % 32768;
	iptr += 4;
	return 1;
}

int mod() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = b % c;
	iptr += 4;
	return 1;
}

int and() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = (b & c);
	iptr += 4;
	return 1;
}

int or() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
       	uint16_t c = memory[iptr+3];
	b = (b >= 32768 ? registers[b - 32768] : b);
	c = (c >= 32768 ? registers[c - 32768] : c);
	registers[a] = (b | c);
	iptr += 4;
	return 1;
}

int not() {
	uint16_t a = memory[iptr+1] - 32768;
       	uint16_t b = memory[iptr+2];
	b = (b >= 32768 ? registers[b - 32768] : b);
	registers[a] = ~b & 0x7FFF;
	iptr += 3;
	return 1;
}

int rmem() {
	uint16_t a = memory[iptr+1]-32768;
       	uint16_t b = memory[iptr+2];
	b = (b >= 32768 ? registers[b - 32768] : b);
	registers[a] = memory[b];
	iptr += 3;
	return 1;
}

int wmem() {
	uint16_t a = memory[iptr+1];
       	uint16_t b = memory[iptr+2];
	a = (a >= 32768 ? registers[a - 32768] : a);
	b = (b >= 32768 ? registers[b - 32768] : b);
	memory[a] = b;
	iptr += 3;
	return 1;
}

int call() {
	uint16_t a = memory[iptr+1];
	a = (a >= 32768 ? registers[a - 32768] : a);
	stack[sptr] = iptr + 2;
	sptr++;
	iptr = a;
	return 1;
}

int ret() {
	sptr--;
	if (sptr < 0)
		return 0;
	iptr = stack[sptr];
	return 1;
}

int out() {
	uint16_t a = memory[iptr+1];
	a = (a >= 32768 ? registers[a-32768] : a);
	putchar((char)a);
	fflush(stdout);
	iptr += 2;
	return 1;
}

int in() {
	uint16_t a = memory[iptr+1] - 32768;
	registers[a] = (uint16_t)getchar();
	iptr += 2;
	return 1;
}

int noop() {
	iptr++;
	return 1;
}

int (*op[22])() = {
	halt,
	set,
	push,
	pop,
	eq,
	gt,
	jmp,
	jt,
	jf,
	add,
	mult,
	mod,
	and,
	or,
	not,
	rmem,
	wmem,
	call,
	ret,
	out,
	in,
	noop
};

void coredump(uint16_t instr) {
	printf("Instruction: %d\n", instr);
	printf("Instruction pointer: %d\n", iptr);
	printf("Stack pointer: %d\n", sptr);
	printf("Registers:\n");
	for (int i = 0; i < 8; i++) {
		printf("%d: 0x%04X\n", i, registers[i]);
	}
}

int main(int argc, char** argv) {
	memory = malloc(sizeof(uint16_t) * 32768);
	stack = malloc(sizeof(uint16_t) * 32768);

	FILE *f = fopen(argv[1], "rb");
	fread(memory, sizeof(uint16_t), 32768, f);
	fclose(f);
	uint16_t instr = 0;
	int res = 0;
	while (1) {
		instr = memory[iptr];
		if (instr > 21) {
			printf("Error! Invalid instruction! \n");
			coredump(instr);
			break;
		}
		res = op[instr]();
		if (!res)
			break;
	}
	free(memory);
	free(stack);
	return 0;
}
