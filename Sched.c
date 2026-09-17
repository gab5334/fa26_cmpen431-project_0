typedef struct {
  char opcode[0];
  int destReg;
  int srcReg1;
  int srcReg2;
} Instruction //Do I need to differentiate between R, I, L, and S Instructs?

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
  FILE *file = fopen(argv[1], 'r');
  icount = 0;
  char line[256];
  while (fgets(line, sizeof(line), file)){
    icount++;
  }

  // Allocate memory for global array
  program = malloc(icount * sizeof(Instruction));
  if (program == NULL){
    //memory allocation failed
    fclose(file);
    exit(1);
  }

  rewind(file);

  int index = 0;
  while (fgets(line, sizeof(line), file)){
    //parsing lines according to ISA syntax
    index++;
  }

  // argv[1] = input filename
  // read line by line, parse into 'program', count into icount
  // set FD.valid = DE.valid = EM.valid = MW.valid = 0 (empty at start)
  
  fclose(file);
  nextFetch = 0;
  cycleCount = 0;
  completedInsts = 0;
}

int Fetch(Latch *FD_out, int stall){
  //Fetch - can move the fetched instruction to decode if decode is not stalled
  //incoming instruction comes from next unfetched location
}

int Decode(Latch *FD_in, Latch *DE_current, Latch *EM_current, Latch *DE_out){
  //Decode - checks for hazards. If there are no hazards, then the instruction can advance
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
    WB(...);
    Mem(...);
    Execute(...);
    Decode(...); 
    Fetch(...);
    cyclecount++; 
  }
  emitOutput(); 
  return 0;
}