#include <stdio.h>;

typedef enum { TYPE_R, TYPE_I, TYPE_L, TYPE_S } InstType;

typedef struct {
  char op;        // 'R', 'I', 'L', or 'S'
  InstType type;  // Reflects opcode char
  int destReg;    // Destination register, -1 if unused
  int srcReg1;    // First source register, -1 if unused
  int srcReg2;    // Second source register, -1 if unused
  int imm;        // Immediate value, -1 if none
} Instruction;

Instruction *program = NULL; // global array! (one entry per line of the file)
int icount;           // total num of instructs
int nextFetch;        // index of instruct not yet fetched
int cycleCount;
int completedInsts;

typedef struct {
  int valid;
  Instruction instr;
  int regDest;
  int srcReg1, srcReg2, imm;
  int cycle_entered;
} Latch;

Latch FD, DE, EM, MW; //able to be referenced globally

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
    //but should only increment if the line isn't blank
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

      if (type_char == 'R'){        #R,<REG>,<REG>,<REG>
        program[i].type = TYPE_R;
        program[i].destReg = regA;
        program[i].srcReg1 = regB;
        program[i].srcReg2 = regC;
        program[i].imm = -1;
      }
      else if (type_char == 'I'){   #I,<REG>,<REG>,<IMM>
        program[i].type = TYPE_I;
        program[i].destReg = regA;
        program[i].srcReg1 = regB;
        program[i].imm = regC;
        program[i].srcReg2 = -1;
      }
      else if (type_char == 'L'){   #L,<REG>,<IMM>,<REG>
        program[i].type = TYPE_L;
        program[i].destReg = regA;
        program[i].imm = regB;
        program[i].srcReg1 = regC;
        program[i].srcReg2 = -1;
      }
      else if (type_char == 'S'){   #S,<REG>,<IMM>,<REG>
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

int Decode(Latch *FD_in, Latch *DE_current, Latch *EM_current, Latch *DE_out){
  //Decode - checks for hazards. If there are no hazards, then the instruction can advance

  
}

int Fetch(Latch *FD_out, int stall){
  //Fetch - can move the fetched instruction to decode if decode is not stalled
  //incoming instruction comes from next unfetched location
  if (stall){
    return 0; // If there is a stall, fetch does not proceed this cycle
  }

  if (nextFetch < icount){
    FD_out->instr = program[nextFetch];
    FD_out->valid = 1;
    FD_out->cycle_entered = cycleCount; //tracks timing for emitOutput
  } else {
    FD_out->valid = 0; // No more instructions left to enter the pipeline
  }
  
}

int Execute(Latch *DE_in, Latch *EM_out){
  //Execute - would perform the operation, but we are not emulating instruction execution, just scheduling
}

int Mem(Latch *EM_in, Latch *MW_out){
  //Memory - would perform any memory lookup, but we are not emulating instruction execution, just scheduling.
  //Results from R and I types would be available here for forwarding
}

int WB(Latch *MW_in){
  //Writeback - would update register contents, but we are not emulating instruction execution, just scheduling.
  //Results from L types would be available here for forwarding; instructions leave pipeline on completing writeback
}

emitOutput(){
  //Writes which cycle each instr finished each stage to output.txt
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