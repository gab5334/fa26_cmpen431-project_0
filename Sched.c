#include <stdio.h>
#include <stdlib.h>

typedef enum { TYPE_R, TYPE_I, TYPE_L, TYPE_S } InstType;

typedef struct {
  char op;        // 'R', 'I', 'L', or 'S'
  InstType type;  // Reflects opcode char
  int destReg;    // Destination register, -1 if unused
  int srcReg1;    // First source register, -1 if unused
  int srcReg2;    // Second source register, -1 if unused
  int imm;        // Immediate value, -1 if none

  int fetchComplete;  // The cycle during which instruction finished the respective stage
  int decodeComplete;
  int executeComplete;
  int memComplete;
  int wbComplete;
} Instruction;

typedef struct {
  int valid;
  Instruction *instr;;
} Latch;

Instruction *program = NULL; // Global array containing each instruction
int icount;           // Total number of instructions
int nextFetch;        //index of instruct not yet fetched
int cycleCount;       // Current cycle occuring
int completedInsts;

Latch FD, DE, EM, MW; //latches able to be referenced globally
Latch nextFD, nextDE, nextEM, nextMW;

void initStructuresAndCounts(int argc, char **argv){

  FILE *file = fopen(argv[1], 'r'); //argv[1] = input filename
  if (!file){ //error
    printf("Couldn't open file");
    exit(1);
  }

  // Loop through maximum of 32 lines, count how many there actually are
  icount = 0;
  char line[128];
  while (fgets(line, sizeof(line), file)){
    if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;  //should only increment if the line isn't blank
    icount++;
  }

  // Allocate memory for global array
  program = malloc(icount * sizeof(Instruction));
  if (program == NULL){ //memory allocation failed
    printf("Couldn't allocate memory");
    fclose(file);
    exit(1);
  }

  // Reset file and parse into program[]
  rewind(file);

  int i = 0;
  while (fgets(line, sizeof(line), file)){
    if (line[0] == '\n' || line[0] == '\0') continue;

    char type_char;
    int regA, regB, regC;

    // Go through comma separated lines
    if (sscanf(line, '%c,%d,%d,%d', &type_char, &regA, &regB, &regC) == 4){
      program[i].op = type_char;

      if (type_char == 'R'){        //R,<REG>,<REG>,<REG>
        program[i].type = TYPE_R;
        program[i].destReg = regA;
        program[i].srcReg1 = regB;
        program[i].srcReg2 = regC;
        program[i].imm = -1;
      }
      else if (type_char == 'I'){   //I,<REG>,<REG>,<IMM>
        program[i].type = TYPE_I;
        program[i].destReg = regA;
        program[i].srcReg1 = regB;
        program[i].imm = regC;
        program[i].srcReg2 = -1;
      }
      else if (type_char == 'L'){   //L,<REG>,<IMM>,<REG>
        program[i].type = TYPE_L;
        program[i].destReg = regA;
        program[i].imm = regB;
        program[i].srcReg1 = regC;
        program[i].srcReg2 = -1;
      }
      else if (type_char == 'S'){   //S,<REG>,<IMM>,<REG>
        program[i].type = TYPE_S;
        program[i].destReg = -1;
        program[i].srcReg2 = regA;
        program[i].imm = regB;
        program[i].srcReg1 = regC;
      }

      i++;

    }
  }
  
  fclose(file);

  FD.valid = DE.valid = EM.valid = MW.valid = 0; //latches empty at start

  nextFetch = 0;
  cycleCount = 0;
  completedInsts = 0;
}

void WB(Latch *MW_in){ // Would update register contents, but we are not emulating instruction execution, just scheduling.
  //Results from L types would be available here for forwarding; instructions leave pipeline on completing writeback
  if (MW_in->valid){
    MW_in->instr->wbComplete = cycleCount;
    completedInsts++;
  }
}

void Mem(Latch *EM_in, Latch *MW_out){ // Would perform any memory lookup, but we are not emulating instruction execution, just scheduling.
  if (EM_in->valid){
  //Results from R and I types would be available here for forwarding
    EM_in->instr->memComplete = cycleCount;
    MW_out->valid = 1;
    MW_out->instr = EM_in->instr;
  } else {
    MW_out->valid = 0;
  }
}

void Execute(Latch *DE_in, Latch *EM_out){ // Would perform the operation, but we are not emulating instruction execution, just scheduling
  if (!DE_in->valid){
    DE_in->instr->executeComplete = cycleCount;
    EM_out->valid = 1;
    EM_out->instr = DE_in->instr;
  } else {
    EM_out->valid = 0;
  }
}

int Decode(Latch *FD_in, Latch *DE_current, Latch *EM_current, Latch *DE_out){  // Checks for hazards. If there are no hazards, then the instruction can advance
  if (!FD_in->valid){
    DE_out->valid = 0;
    return 0; //no stall
  }
  /*To do hazard detection, we look at the
resources in use by older instructions in the later pipeline (E,M,W)
stages, and check whether or not an operand for the instruction in D is
defined by a still-in-process instruction.

One logical way to organize this then, as in the HW design, is to have
state associated with each pipeline stage that is analogous to the pipeline
latch that holds the information about an in-process instruction in HW.
This would include the instruction type (so that when an output is
available is known, since L types produce in a different stage and S types
don't write registers) and the destination register, if any. This state
would then propagate down the pipeline stages each cycle
Depending on your design choices, you could be moving your entire
instruction record along, or a pointer or index value allowing access to a
per-instruction data type, or actually decode signals into their own
pipeline latch specific structure of decoded subfields to emulate HW
you need
to, in Decode, check whether a register you are reading is/isn't going to
be ready in the cycle you need to use it and wait and try again next cycle
if the answer is no (introducing a no-op into E instead of the instruction
in D)*/
  //is the current instruction type L
    //must check that current destination reg
    //look at the resources in use by older instructions in the later pipeline (E,M,W) stages, and check whether or not
    //an operand for the instruction in D is defined by a still-in-process instruction
  //else
    //proceed as normal
}

int Fetch(Latch *FD_out, int stall){  // Moves the fetched instruction to decode if decode is not stalled
  if (stall){
    return 0; // If there is a stall, fetch does not proceed this cycle
  }

  if (nextFetch < icount){ //incoming instruction comes from next unfetched location
    FD_out->instr = program[nextFetch];
    FD_out->valid = 1;
    FD_out->cycle_entered = cycleCount; //tracks timing for emitOutput
  } else {
    FD_out->valid = 0; // No more instructions left to enter the pipeline
  }
  
}

emitOutput(){ // Writes which cycle each instr finished each stage to output.txt
  FILE *fileout fopen('output.txt','w');

  int i = 0;
  while (i < icount){
    int fe = program[i].fetchComplete;
    int de = program[i].decodeComplete;
    int ex = program[i].executeComplete;
    int mem = program[i].memComplete;
    int wb = program[i].wbComplete;
    fprintf(fileout,'%d,%d,%d,%d,%d\n', fe, de, ex, mem, wb);
  }
  
  fclose(fileout);

}

int main(int argc, char** argv){
  initStructuresAndCounts(argc,argv); 

  ...

  while(completedInsts<icount){
    WB(&MW);
    Mem(&EM, &MW);
    Execute(&DE, &EM);
    int stall = Decode(&FD, &DE, &EM, &MW); 
    Fetch(&FD, stall);
    cyclecount++; 
  }
  emitOutput(); 
  return 0;
}