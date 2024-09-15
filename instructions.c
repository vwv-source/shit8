#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "draw.h"

//This file basically houses everything critical to the emulator

extern uint8_t registers[16];
extern unsigned char* memory;
extern uint16_t pc;
extern uint16_t index;
extern uint16_t stack[16];
extern uint8_t stackp;
extern uint8_t dtimer;
extern uint8_t stimer;
extern int display[64*32];
extern uint16_t opcode;
uint8_t keypad[16] = {0};

int delay = 2000;

void OP_00E0(){
    for(int i = 0; i<64*32; i++){
        display[i] = 0;
    }
}

void OP_00EE(){
    stackp--;
    pc = stack[stackp];
}

void OP_1nnn(){
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

void OP_2nnn(){
    uint16_t address = opcode & 0x0FFFu;

    stack[stackp] = pc;
    stackp++;
    pc = address;
}

void OP_3xkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t VK = opcode & 0x00FFu;

    if(registers[Vx] == VK){
        pc += 2;
    }
}

void OP_4xkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t VK = opcode & 0x00FFu;

    if(registers[Vx] != VK){
        pc += 2;
    }
}

void OP_5xy0(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] == registers[Vy]){
        pc += 2;
    }
}

void OP_6xkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = (opcode & 0x00FFu);

    registers[Vx] = kk;
}

void OP_7xkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = (opcode & 0x00FFu);

    registers[Vx] = registers[Vx]+kk;
}

void OP_8xy0(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; 

    registers[Vx] = registers[Vy];
}

void OP_8xy1(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; 

    registers[Vx] |= registers[Vy];
}

void OP_8xy2(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; 

    registers[Vx] &= registers[Vy];
}

void OP_8xy3(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u; 

    registers[Vx] ^= registers[Vy];
}

void OP_8xy4(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t total = registers[Vx] + registers[Vy];

    if(total > 255u){
        registers[0xF] = 1;
    }else{
        registers[0xF] = 0;
    }

    registers[Vx] = total & 0xFFu;
}

void OP_8xy5(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t total = registers[Vx] + registers[Vy];

    if(registers[Vx] > registers[Vy]){
        registers[0xF] = 1;
    }else{
        registers[0xF] = 0;
    }

    registers[Vx] -= registers[Vy];
}

void OP_8xy6(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = registers[Vx] & 0x1u;

    registers[Vx] >>= 1;
}

void OP_8xy7(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t total = registers[Vx] + registers[Vy];

    if(registers[Vy] > registers[Vx]){
        registers[0xF] = 1;
    }else{
        registers[0xF] = 0;
    }

    registers[Vx] = registers[Vy] - registers[Vx];
}

void OP_8xyE(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

    registers[Vx] <<= 1;
}

void OP_9xy0(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] != registers[Vy]){
        pc += 2;
    }
}


void OP_Annn(){
    uint16_t address = (opcode & 0x0FFFu);

    index = address;
}

void OP_Bnnn(){
    uint16_t address = (opcode & 0x0FFFu);

    pc = registers[0] + address;
}

void OP_Cxkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = (opcode & 0x00FFu);
    
    uint8_t randi = rand();

    registers[Vx] = randi & kk;
}

void OP_Dxyn(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = (opcode & 0x000Fu);

    uint8_t xpos = registers[Vx] % 64;
    uint8_t ypos = registers[Vy] % 32;

    registers[0xF] = 0;

    for(unsigned int y = 0; y<height; y++){
        uint8_t spriteByte = memory[index + y];
        for(unsigned int x = 0; x<8; x++){
            uint8_t spritePixel = spriteByte & (0x80u >> x);

			if (spritePixel)
			{
				if (display[(ypos + y) * 64 + (xpos + x)])
				{
                    display[(ypos + y) * 64 + (xpos + x)] = 0;
					registers[0xF] = 1;
				}else{
                    display[(ypos + y) * 64 + (xpos + x)] = 1;
                }
			}
        }
    }
}

void OP_Ex9E(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if(keypad[registers[Vx]]){
        pc += 2;
    }
}

void OP_ExA1(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if(!keypad[registers[Vx]]){
        pc += 2;
    }
}


void OP_Fx07(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[Vx] = dtimer;
}

void OP_Fx0A(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    for(int i = 0; i<16; i++){
        if(keypad[i] != 0){
            registers[Vx] = i;
        }
    }
}

void OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	dtimer = registers[Vx];
}

void OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	stimer = registers[Vx];
}

void OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += registers[Vx];
}

void OP_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = registers[Vx];

	index = 0x50u + (5 * digit);
}

void OP_Fx33()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = registers[Vx];

	memory[index + 2] = value % 10;
	value /= 10;

	memory[index + 1] = value % 10;
	value /= 10;

	memory[index] = value % 10;
}

void OP_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[index + i] = registers[i];
	}
}

void OP_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		registers[i] = memory[index + i];
	}
}

void OP_Loop(){
    opcode = (memory[pc] << 8u) | memory[pc + 1];

	pc += 2;

    //This could've been improved by using a table (notice the similarities between the opcodes)
    //But it wouldnt have made a big difference here IMO
    switch((opcode & 0xF000u) >> 12u){
        case 0x0:
            switch(opcode & 0x00FFu){
                case 0xE0:
                    OP_00E0();
                    break;
                case 0xEE:
                    OP_00EE();
            }
            break;
        case 0x1:
            OP_1nnn();
            break;
        case 0x2:
            OP_2nnn();
            break;
        case 0x3:
            OP_3xkk();
            break;
        case 0x4:
            OP_4xkk();
            break;
        case 0x5:
            OP_5xy0();
            break;
        case 0x6:
            OP_6xkk();
            break;
        case 0x7:
            OP_7xkk();
            break;
        case 0x8:
            switch(opcode & 0x000Fu){
                case 0x0:
                    OP_8xy0();
                    break;
                case 0x1:
                    OP_8xy1();
                    break;
                case 0x2:
                    OP_8xy2();
                    break;
                case 0x3:
                    OP_8xy3();
                    break;
                case 0x4:
                    OP_8xy4();
                    break;
                case 0x5:
                    OP_8xy5();
                    break;
                case 0x6:
                    OP_8xy6();
                    break;
                case 0x7:
                    OP_8xy7();
                    break;
                case 0xE:
                    OP_8xyE();
                    break;
            }
            break;
        case 0x9:
            OP_9xy0();
            break;
        case 0xA:
            OP_Annn();
            break;
        case 0xB:
            OP_Bnnn();
            break;
        case 0xC:
            OP_Cxkk();
            break;
        case 0xD:
            OP_Dxyn();
            break;
        case 0xE:
            switch(opcode & 0x00FFu){
                case 0x9E:
                    OP_Ex9E();
                    break;
                case 0xA1:
                    OP_ExA1();
                    break;
            }
            break;
        case 0xF:
            switch(opcode & 0x00FFu){
                case 0x07:
                    OP_Fx07();
                    break;
                case 0x0A:
                    OP_Fx0A();
                    break;
                case 0x15:
                    OP_Fx15();
                    break;
                case 0x18:
                    OP_Fx18();
                    break;
                case 0x1E:
                    OP_Fx1E();
                    break;
                case 0x29:
                    OP_Fx29();
                    break;
                case 0x33:
                    OP_Fx33();
                    break;
                case 0x55:
                    OP_Fx55();
                    break;
                case 0x65:
                    OP_Fx65();
                    break;
            }
            break;
    };

    if (dtimer > 0)
	{
		--dtimer;
	}

	if (stimer > 0)
	{
		--stimer;
	}

    //bad, linux-specific function AFAIK
    usleep(delay);
}