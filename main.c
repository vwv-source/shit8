#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "instructions.h"
#include "draw.h"

uint8_t registers[16];
unsigned char* memory;
uint16_t pc;
uint16_t index;
uint16_t stack[16];
uint8_t stackp;
uint8_t dtimer;
uint8_t stimer;
int display[64*32];
uint16_t opcode;

extern int delay;
extern int scale;

uint8_t fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void readROM(char* filename){
    int size;

    FILE* fptr;
    fptr = fopen(filename, "rb");

    fseek(fptr, 0L, SEEK_END);
    size = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    unsigned char buffer[size];

    fread(buffer, sizeof(buffer), 1, fptr);

    fclose(fptr);

    for(int i = 0; i<size; i++){
        memory[0x200+i] = buffer[i];
    }
}

int main(int argc, char* argv[]){
    memory = malloc(4096*8);

    if(!argv[1]){
        printf("You need to specify a rom to load :P\n");
        printf("Syntax is: ./shit8 <ROM> <DELAY> (2) <SCALE> (10)\n");
        exit(0);
    }
    if(argv[2] && argc > 1){
        delay = atoi(argv[2])*1000;
    }
    if(argv[3] && argc > 2){
        scale = atoi(argv[3]);
    }

    readROM(argv[1]);
    pc = 0x200;
    
    //font loading
    for(int i = 0; i<80; i++){
        memory[0x50u + i] = fontset[i];
    }

    Display_Init();
    while(1){
        OP_Loop();
        Display();
    }

    return 0;
}