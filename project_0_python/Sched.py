instructions = []    # Contains dictionaries with the instructions info
icount = 0           # Total instruction count
current_cycle = 0    # The current number cycle
NUM_STAGES = 5              # This is a five stage pipeline

def parse_instructions(filename: str) -> None:
    """
    Takes in whole instruction text file
    Updates global array with components
    """
    #use read inputs??
    global icount

    with open(filename, 'r') as infile:
        for line in infile:
            line = line.strip()
            if (line == ''): # if the line is empty, proceed to the next one
                continue

            icount += 1
            assert icount <= 32, "Error: You may pass in a maximum of 32 instructions"

            line = line.split(",")
            if line[0] == "R":  #R,<REG>,<REG>,<REG>
                instructions.append({"op":"R", "destReg":line[1],"srcReg1":line[2],"srcReg2":line[3],"imm":-1})
            elif line[0] == "I":  #I,<REG>,<REG>,<IMM>
                instructions.append({"op":"I", "destReg":line[1],"srcReg1":line[2],"srcReg2":-1,"imm":line[3]})
            elif line[0] == "L":  #L,<REG>,<IMM>,<REG>
                instructions.append("op":"L", "destReg":line[1],"srcReg1":line[3],"srcReg2":-1,"imm":line[2]})
            elif line[0] == "S":  #S,<REG>,<IMM>,<REG>
                instructions.append("op":"S", "destReg":-1,"srcReg1":line[3],"srcReg2":line[1],"imm":line[2]})

    assert icount != 0, "Error: No instructions recieved"

def load_hazard(curr_instr : dict, index : int) -> None:
    """
    Checks for register-carried load-use dependencies
    Increments current_cycle if curr_instr depends on a value still being loaded by the preceding instruction
    """
    global current_cycle

    if index == 0:  # Need at least 2 instructions before there can be a load use hazard
        return

    prev_instr = instructions[index-1]
    
    #stall = 0   # 1 if yes stall, else 0
    if prev_instr["op"] != "L":
        return

    if curr_instr["op"] == "R":    # Check that you aren't writing to a register that 
        if (((curr_instr["srcReg1"] != -1) and (curr_instr["srcReg1"] == prev_instr["destReg"])) or 
            ((curr_instr["srcReg2"] != -1) and (curr_instr["srcReg2"] == prev_instr["destReg"]))):
            current_cycle += 1


def main():

    parse_instructions("test.in")

    with open("out.txt",'w') as outfile:

        for i in range(icount):
            for stage in range(NUM_STAGES):

                load_hazard(instructions[i], i)

                if (i == (NUM_STAGES-1)):
                    outfile.write(f"{current_cycle}\n")
                else:
                    outfile.write(f"{current_cycle},")

if __name__ == "__main__":
    main()