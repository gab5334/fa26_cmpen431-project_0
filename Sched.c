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