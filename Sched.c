// How/Where should I read the input file?

typedef struct {
  int valid;
  Instruction instr;
  int regA, regB, imm;
  int cycle_entered;
} Latch;

Latch FD, DE, EM, MW; //able to be referenced globally

initStructuresAndCounts(argc,argv){
  // argc = cycle count
  // argv = current instruction
  // These are actually incorrect, this func is just being passed the command-line args from main from the OS
  // Not rly sure what the purpose of this func is when we already definied a Latch structure
}

int Fetch(Latch *FD_out){
  //Fetch - can move the fetched instruction to decode if decode is not stalled
  //incoming instruction comes from next unfetched location
}

int Decode(Latch *FD_in, Latch *FD_out){
  //Decode - checks for hazards. If there are no hazards, then the instruction can advance
}

int Execute(Latch *DE_in, Latch *DE_out){
  //Execute - would perform the operation, but we are not emulating instruction execution, just scheduling
}

int Mem(Latch *EM_in, *EM_out){
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

  int completedInsts = 0; //placeholder
  int icount = 0;         //placeholder

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