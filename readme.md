Command sysmbol     --->    $
Accessing Register  --->    R[register_address] --Example--> R2
Accessing RAM       --->    #[location_address] --Example--> #850

keywords:
Using 'LOAD', you load secound parameter's value into first addres
    LOAD [Register/RAM address/Ram pointer] [Register/RAM address/value/Ram pointer]
    example: LOAD R2 1

Using 'INP', you get user string input of the given length, and save into RAM
    LOAD [length] [RAM address/Ram pointer]
    example: INP 14 #100

Using '[]', We point to a address of RAM, whitch is saved in another cell, like a pointer
    example: LOAD R4 [#32], --> Looks for the number in RAM #32, and then for the cell with the founded number and then loads it into R4 register

Using 'LABEL', we define a label
    LABEL [label_name]
    example: LABEL loop

Using 'GOTO', we jump into the target label
    LABEL [label_name]
    example: GOTO loop

Using 'IF', we check a condition, and if that was treu we jump into a labl
    IF [operator_1] [condition] [operator_2] GOTO [label_name]
    example: IF 1 <= R1 GOTO loop

Using math operation, we check do the proper operation, and save the value into the first given register
    ADD/SUB/Div/MUL/DIV/MOD [register_1] [register_2]
    example: ADD R1 R2

Using 'OUT', we print out the given value from ram or register
    OUT [Register/RAM]
    example: OUT R1
    + with '-s' symbol, we print the string from start point at the memory, until we hit '\0'
    example: OUT -s #100

Using 'PUSH', we push a value into the stack
    Push [Register/RAM/value]
    example: PUSH R1

Using 'POP', we pop the from stack, and save into target storage
    POP [Register/RAM]
    example: POP #325

Using 'FUNC', we decelare a function, 
WARNING: always close a function block with 'ENDF'
    FUNC [funcrion_name]
    example: FUNC compare

Using 'ENDF', we finish a function block,
    ENDF

Using 'CALL', we call a funtion and jump into it's code block
    CALL [funcrion_name]
    example: CALL compare

Using 'RET', we return back of a function, we're inside if that
    RET
