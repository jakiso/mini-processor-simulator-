// Charles Anderson and Jade Soto
// CDA 3103 - Angell
// 4/25/2021

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
 int aluControlNumber = ALUControl;

 // Z = A + B
 if (aluControlNumber == 0)
 {
   *ALUresult = A + B;
 }

 // Z = A – B
 else if (aluControlNumber == 1)
 {
   *ALUresult = A - B;
 }

 // if A < B, Z = 1; otherwise, Z = 0
 else if (aluControlNumber == 2)
 {
   if ((signed)A < (signed)B)
   {
     *ALUresult = 1;
   }
   else
   {
     *ALUresult = 0;
   }
 }

 // if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
 else if (aluControlNumber == 3)
 {
   if (A < B)
   {
     *ALUresult = 1;
   }
   else
   {
     *ALUresult = 0;
   }
 }

 // Z = A AND B
 else if (aluControlNumber == 4)
 {
   *ALUresult = (A & B);
 }

 // Z = A OR B
 else if (aluControlNumber == 5)
 {
   *ALUresult = (A | B);
 }

 // Shift left B by 16 bits
 else if (aluControlNumber == 6)
 {
   *ALUresult = B << 16;
 }

 // Z = NOT A
 else if (aluControlNumber == 7)
 {
   *ALUresult = ~A;
 }

 // Assigns Zero value based on ALUresult
 if (ALUresult == 0)
 {
   *Zero = 1;
 }
 else
 {
   *Zero = 0;
 }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  // Halt returns true if PC is not properly word aligned
if ((PC % 4) != 0)
{
  return 1;
}

// Bitshifts PC twice to get the location in Mem array
// Sets instruction to Mem array value
else
{
  *instruction = Mem[PC >> 2];
}

return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
  unsigned opbitmask = 0b00000000000000000000000000111111;
  unsigned offsetbitmask = 0b00000000000000001111111111111111;
  unsigned jsecbitmask = 0b00000011111111111111111111111111;
  unsigned rtypebitmask = 0b11111;

 // Bitmask and shift (if needed) instruction to the appropiate placing
 *op = (instruction >> 26) & opbitmask; // Instruction 31 - 26
 *r1 = (instruction >> 21) & rtypebitmask; // Instruction 25 - 21
 *r2 = (instruction >> 16) & rtypebitmask; // Instruction 20 - 16
 *r3 = (instruction >> 11) & rtypebitmask; // Instruction 15 - 11
 *funct = instruction & opbitmask;// Instruction 5 - 0
 *offset = instruction & offsetbitmask;// Instruction 15 - 0
 *jsec = instruction & jsecbitmask;// Instruction 25 - 0
}





/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
 // Sets the instructions for r types
 if (op == 0x0)
 {
   controls->RegDst = 1;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 0;
   controls->MemtoReg = 0;
   controls->ALUOp = 7;
   controls->MemWrite = 0;
   controls->ALUSrc = 0;
   controls->RegWrite = 1;
 }

 // Sets the instructions for addi
 else if (op == 0x8)
 {
   controls->RegDst = 0;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 0;
   controls->MemtoReg = 0;
   controls->ALUOp = 0;
   controls->MemWrite = 0;
   controls->ALUSrc = 1;
   controls->RegWrite = 1;
 }

 // Sets the instructions for load word
 else if (op == 0x23)
 {
   controls->RegDst = 0;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 1;
   controls->MemtoReg = 1;
   controls->ALUOp = 0;
   controls->MemWrite = 0;
   controls->ALUSrc = 1;
   controls->RegWrite = 1;
 }

 // Sets the instructions for store word
 else if (op == 0x2b)
 {
   controls->RegDst = 0;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 0;
   controls->MemtoReg = 0;
   controls->ALUOp = 0;
   controls->MemWrite = 1;
   controls->ALUSrc = 1;
   controls->RegWrite = 0;
 }

// Sets the instructions for load upper immediate
 else if (op == 0xf)
 {
   controls->RegDst = 0;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 0;
   controls->MemtoReg = 0;
   controls->ALUOp = 6;
   controls->MemWrite = 0;
   controls->ALUSrc = 1;
   controls->RegWrite = 1;
 }

 // Sets the instructions for branch on equal
 else if (op == 0x4)
 {
   controls->RegDst = 2;
   controls->Jump = 0;
   controls->Branch = 1;
   controls->MemRead = 0;
   controls->MemtoReg = 2;
   controls->ALUOp = 1;
   controls->MemWrite = 0;
   controls->ALUSrc = 2;
   controls->RegWrite = 0;
 }

 // Sets the instructions for set less than immediate
 else if (op == 0xa)
 {
   controls->RegDst = 1;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 0;
   controls->MemtoReg = 0;
   controls->ALUOp = 2;
   controls->MemWrite = 0;
   controls->ALUSrc = 0;
   controls->RegWrite = 1;
 }

 // Sets the instructions for set less than unsigned
 else if (op == 0xb)
 {
   controls->RegDst = 1;
   controls->Jump = 0;
   controls->Branch = 0;
   controls->MemRead = 0;
   controls->MemtoReg = 0;
   controls->ALUOp = 3;
   controls->MemWrite = 0;
   controls->ALUSrc = 0;
   controls->RegWrite = 1;
 }

 // Sets the instructions for jump
 else if (op == 0x2)
 {
   controls->RegDst = 2;
   controls->Jump = 1;
   controls->Branch = 2;
   controls->MemRead = 0;
   controls->MemtoReg = 2;
   controls->ALUOp = 0;
   controls->MemWrite = 0;
   controls->ALUSrc = 2;
   controls->RegWrite = 0;
 }

 // Returns one if not valid op code
 else
 {
   return 1;
 }

 return 0;

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
  // Reads the registers addressed by r1 and r2 from Reg, and writes the read values to data1 and data2
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
 unsigned negativeMask = 0xFFFF0000;
 unsigned zeroMask = 0x0000FFFF;
 unsigned negativeSignBit = offset >> 15;

 // Puts ones for first 16 bits
 if (negativeSignBit == 1)
 {
   *extended_value = offset | negativeMask;
 }

 // Puts zeros for first 16 bits
 else
 {
   *extended_value = offset & zeroMask;
 }

}

int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // R Type function
    if(ALUOp == 7)
    {
      switch(funct)
      {
      // ALU will do addition
      case 32:
        ALUOp = 0;
        break;
      // ALU will do subtraction
      case 34:
        ALUOp = 1;
        break;
      // ALU will do set less than
      case 42:
        ALUOp = 2;
        break;
      // ALU will do set less than unsigned
      case 43:
        ALUOp = 3;
        break;
      // ALU will do AND operation
      case 36:
        ALUOp = 4;
        break;
      // ALU will do OR operation
      case 37:
        ALUOp = 5;
        break;
      // ALU will shift left
      case 4:
        ALUOp = 6;
        break;
    // Default for halt or don't care
    default:
      return 1;
    }
    if(ALUSrc == 1)
    {
      data2 = extended_value;
    }
    ALU(data1, data2, ALUOp, ALUresult, Zero);
  }
    if(ALUOp != 7)
    {
      if(ALUSrc == 1)
      {
        data2 = extended_value;
      }
      ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    return 0;
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  // If alignment is invalid, returns halt as 1
  if(((MemWrite == 1) || (MemRead == 1)) && (ALUresult % 4 != 0))
  {
    return 1;
  }
  // Sets memory data to memory of ALUresult shifted by 2 bits
  if(MemRead == 1)
  {
    *memdata = Mem[ALUresult >> 2];
  }
  // Sets memory of ALU result to data2 shifted 2 bits
  if(MemWrite == 1)
  {
    Mem[ALUresult >> 2] = data2;
  }
  return 0;
    
    
}


void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  // If writing is on
  if(RegWrite == 1)
  {
    if(MemtoReg == 1)
    {
      // Memory goes to r3
      if(RegDst == 1)
      {
          Reg[r3] = memdata;
      }
      // Memory goes to register
      if(RegDst == 0)
      {
        Reg[r2] = memdata;
      }
    }
    if(MemtoReg == 0)
    {
      // ALUresult goes to r3
      if(RegDst == 1)
      {
        Reg[r3] = ALUresult;
      }
      // ALUresult goes to r2
      if(RegDst == 0)
      {
        Reg[r2] = ALUresult;
      }
    }

  }
}


/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
  *PC = *PC + 4;
  
  // Takes care of branch and jump
  if ((Zero == 1) && (Branch == 1) && (Jump == 0))
  {
    *PC = *PC + (extended_value << 2);
  }

  // Left shifts bits of jsec by 2
  // Uses upper 4 bits of PC
  if ((Jump == 1) && (Branch == 0))
  {
    unsigned shiftedJSec = jsec << 2;
    unsigned fourPCBits = (*PC & 0xf0000000);
    *PC = shiftedJSec | fourPCBits;
  }
}
