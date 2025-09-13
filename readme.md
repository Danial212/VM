# Virtual Machine Assembly Language

A custom assembly-like language for a virtual machine implementation, designed for educational purposes and OS development.

## 📋 Table of Contents
- [Syntax Overview](#syntax-overview)
- [Instruction Set Architecture (ISA)](#instruction-set-architecture-isa)
- [Memory Operations](#memory-operations)
- [Control Flow](#control-flow)
- [Arithmetic Operations](#arithmetic-operations)
- [Input/Output Operations](#inputoutput-operations)
- [Stack Operations](#stack-operations)
- [Function Operations](#function-operations)
- [File Operations](#file-operations)

## 🔤 Syntax Overview

| Element | Syntax | Example |
|---------|--------|---------|
| **Command Symbol** | `$` | Used for commands |
| **Register Access** | `R[address]` | `R2`, `R15` |
| **RAM Access** | `#[address]` | `#850`, `#100` |
| **Pointer Dereference** | `[address]` | `[#32]` - value at RAM address stored in #32 |

---

## 📚 Instruction Set Architecture (ISA)

### 💾 Memory Operations

#### `LOAD` - Load Value
Loads the second parameter's value into the first address.
```assembly
LOAD [destination] [source]
```
**Examples:**
```assembly
LOAD R2 1        # Load immediate value 1 into register R2
LOAD R4 [#32]    # Load value from RAM address stored in #32 into R4
LOAD #100 R2     # Load value from R2 into RAM address 100
```

#### `INP` - Input
Gets user string input of specified length and saves it into RAM.
```assembly
INP [length] [RAM_address/Ram_pointer]
```
**Example:**
```assembly
INP 14 #100      # Get 14-character input and store at RAM address 100
```

### 🔄 Control Flow

#### `LABEL` - Define Label
Defines a jump target for control flow operations.
```assembly
LABEL [label_name]
```
**Example:**
```assembly
LABEL loop       # Define a label named "loop"
```

#### `GOTO` - Unconditional Jump
Jumps to the specified label.
```assembly
GOTO [label_name]
```
**Example:**
```assembly
GOTO loop        # Jump to "loop" label
```

#### `IF` - Conditional Jump/Call
Checks a condition and executes GOTO or CALL if true.
```assembly
IF [operand1] [condition] [operand2] GOTO/CALL [target]
```
**Examples:**
```assembly
IF 1 <= R1 GOTO loop     # If 1 <= R1, jump to loop
IF R2 == 0 CALL ErrorFunc # If R2 equals 0, call ErrorFunc
```

**Supported Conditions:** `==`, `!=`, `<`, `<=`, `>`, `>=`

### ➕ Arithmetic Operations

Performs mathematical operations and stores the result in the first register.
```assembly
ADD/SUB/MUL/DIV/MOD [register1] [register2]
```
**Examples:**
```assembly
ADD R1 R2        # R1 = R1 + R2
SUB R3 R4        # R3 = R3 - R4
MUL R1 5         # R1 = R1 * 5
DIV R2 R3        # R2 = R2 / R3
MOD R1 R2        # R1 = R1 % R2
```

### 📤 Input/Output Operations

#### `OUT` - Output
Prints the value from a register or RAM address.
```assembly
OUT [Register/RAM]
OUT -s [RAM_address]     # Print string until '\0'
```
**Examples:**
```assembly
OUT R1           # Print numeric value in R1
OUT -s #100      # Print string starting at RAM address 100
```

### 📚 Stack Operations

#### `PUSH` - Push to Stack
Pushes a value onto the stack.
```assembly
PUSH [Register/RAM/value]
```
**Example:**
```assembly
PUSH R1          # Push value in R1 to stack
PUSH 42          # Push immediate value 42 to stack
```

#### `POP` - Pop from Stack
Pops a value from the stack and stores it in the target location.
```assembly
POP [Register/RAM]
```
**Example:**
```assembly
POP R2           # Pop stack value into register R2
POP #325         # Pop stack value into RAM address 325
```

### 🔧 Function Operations

#### `FUNC` - Function Declaration
Declares the start of a function block.
```assembly
FUNC [function_name]
```
**Example:**
```assembly
FUNC compare     # Declare function named "compare"
```

#### `ENDF` - End Function
Marks the end of a function block.
```assembly
ENDF
```

#### `CALL` - Function Call
Calls a function and jumps to its code block.
```assembly
CALL [function_name]
```
**Example:**
```assembly
CALL compare     # Call the "compare" function
```

#### `RET` - Return
Returns from the current function.
```assembly
RET
```

### 📁 File Operations

#### `FOPEN` - Open File
Opens a file for reading and writing.
```assembly
FOPEN [file_name]
```
**Example:**
```assembly
FOPEN test2.txt  # Open file "test2.txt"
```

#### `FCLOSE` - Close File
Closes an opened file.
```assembly
FCLOSE [file_name]
```
**Example:**
```assembly
FCLOSE test2.txt # Close file "test2.txt"
```

#### `F_FLU` - Flush File
Flushes the buffer of an opened file.
```assembly
F_FLU [file_name]
```
**Example:**
```assembly
F_FLU test2.txt  # Flush file "test2.txt"
```

#### `F_REA` - Read from File
Reads a value from the file at the current cursor position and writes it to the given address.
```assembly
F_REA [file_name] [RAM_address/Ram_pointer]
```
**Example:**
```assembly
F_REA test2.txt R6    # Read from file into register R6
```

#### `F_WRI` - Write to File
Writes a given value (in number format) to the opened file.
```assembly
F_WRI [file_name] [value]
```
**Example:**
```assembly
F_WRI test2.txt R2    # Write value from R2 to file
```

#### `F_GOTO` - File Seek
Changes the file cursor position.
```assembly
F_GOTO [file_name] [cursor_operation]
```
**Examples:**
```assembly
F_GOTO test2.txt 15   # Set cursor to position 15
F_GOTO test2.txt +5   # Move cursor forward 5 positions
F_GOTO test2.txt -2   # Move cursor backward 2 positions
```

---

## ⚠️ Important Notes

- **Function Blocks:** Always close function declarations with `ENDF`
- **File Operations:** Files must be opened with `FOPEN` before use
- **Memory Addressing:** Use `#` for direct RAM access, `[]` for pointer dereferencing
- **String Termination:** Strings should be null-terminated (`\0`) for proper printing

## 📝 Example Program

```assembly
FUNC ReadAndEcho
    FOPEN input.txt
    LOAD R4 100          # Memory pointer
    LOAD R2 1            # Increment value
    
    LABEL ReadLoop
        F_REA input.txt R6
        LOAD [R4] R6
        ADD R4 R2
        IF R6 != -1 GOTO ReadLoop
    
    LOAD [R4] 0          # Null terminate
    OUT -s #100          # Print string
    FCLOSE input.txt
RET
ENDF

CALL ReadAndEcho
```
