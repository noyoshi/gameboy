#include "memory.h"

uint8_t read_byte(Proc * p, uint16_t address) {
    return p->memory[address];
}

void write_byte(Proc * p, uint16_t address, uint8_t value) {
    p->memory[address] = value;
    
    /* http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics */
    
    /* if we are writing a byte that is in the vram, we should update the internal representation 
     * of the tiles 
     */

    if ((address & 0xF000) == 0x8000 || (address & 0xF000) == 0x9000) {
        // Then this should trigger an update to the tile map
        write_tile(p, address, value);
    }
}

void write_tile(Proc * p, uint16_t address, uint8_t value) {
    uint16_t base_address = address & 0x1FFE;
    int tile = (base_address >> 4) & 511;
    int y = (base_address >> 1) & 7;
    int bit_index;
    
    for (int i = 0; i < 8; i ++) {
        bit_index = 1 << (7 - i);

        /* 
         * if memory looks like ..... 1 .....
         *                      ..... 0 .....
         * result should be 1, 
         * if memory looks like ..... 1 .....
         *                      ..... 1 .....
         * result should be 3, etc
         */

        p->tileset[tile][i][y] = p->memory[address] & bit_index ? 1 : 0;
        p->tileset[tile][i][y] += p->memory[address+1] & bit_index ? 2 : 0;
    }

    // TODO I think here we can update the video as well?
}


// TODO not sure if we are ever going to use this?
// uint16_t read_word(Proc * p, uint16_t address)

// void write_word(Proc * p, uint16_t address, uint16_t word);


// TODO get this to use the write_byte function
void proc_initialize_memory(Proc * p) { 
    /* initializes the memory, always set these on reset */
    p->memory[0xFF05] = 0;
    p->memory[0xFF06] = 0;
    p->memory[0xFF07] = 0;

    p->memory[0xFF10] = 0x80;
    p->memory[0xFF11] = 0xBF;
    p->memory[0xFF12] = 0xF3;
    p->memory[0xFF14] = 0xBF;
    p->memory[0xFF16] = 0x3F;
    p->memory[0xFF17] = 0x00;
    p->memory[0xFF19] = 0xBF;

    p->memory[0xFF1A] = 0x7F;
    p->memory[0xFF1B] = 0xFF;
    p->memory[0xFF1C] = 0x9F;
    p->memory[0xFF1E] = 0xBF;

    p->memory[0xFF20] = 0xFF;
    p->memory[0xFF21] = 0x00;
    p->memory[0xFF22] = 0x00;
    p->memory[0xFF23] = 0xBF;
    p->memory[0xFF24] = 0x77;
    p->memory[0xFF25] = 0xF3; 
    p->memory[0xFF26] = 0xF1; // 0xF0 for SGB

    p->memory[0xFF40] = 0x91; 
    p->memory[0xFF42] = 0x00; 
    p->memory[0xFF43] = 0x00; 
    p->memory[0xFF45] = 0x00; 
    p->memory[0xFF47] = 0xFC; 
    p->memory[0xFF48] = 0xFF; 
    p->memory[0xFF49] = 0xFF; 

    p->memory[0xFF4A] = 0x00;
    p->memory[0xFF4B] = 0x00;
    p->memory[0xFFFF] = 0x00;
}
