# **Assembler Project**

## **Overview**
This project is a two-pass assembler designed to convert assembly language source code into machine code. The assembler processes `.as` files and generates corresponding `.ob` (object), `.ext` (external), and `.ent` (entry) files. It validates the assembly code for syntax and semantics while handling labels, directives, and instructions.

## **Features**
- **Two-Pass Processing:** 
  - **First Pass:** Parses the assembly code to define symbols, detect errors, and build intermediate structures.
  - **Second Pass:** Resolves addresses, processes directives, and generates machine code.
- **File Generation:** Outputs:
  - `.ob` files containing machine code.
  - `.ext` files listing external symbols referenced.
  - `.ent` files listing symbols declared as entry points.
- **Error Handling:** Provides detailed error messages for syntax and semantic issues in the assembly code.
- **Dynamic Memory Management:** Uses efficient memory allocation for managing symbols and instructions.

## **Installation**
1. Clone the repository:
   ```bash
   git clone github.com/danielkhayat16/assembly_project
   ```
2. Navigate to the project directory:
   ```bash
   cd assembler-project
   ```
3. Build the project using the provided Makefile:
   ```bash
   make
   ```

## **Usage**
1. Place your `.as` assembly files in the project directory.
2. Run the assembler:
   ```bash
   ./assembler file1.as file2.as ...
   ```
3. The output files (`.ob`, `.ext`, `.ent`) will be generated in the same directory as the input files.

### **Command-Line Example**
```bash
./assembler example.as
```

This command processes `example.as` and generates the following:
- `example.ob`: Machine code output.
- `example.ext`: List of external symbols.
- `example.ent`: List of entry symbols.

## **File Format**
### **Input (.as)**
The assembler supports:
- Instructions like `mov`, `cmp`, `add`, etc.
- Directives such as `.data`, `.string`, `.entry`, and `.extern`.
- Label definitions (e.g., `LABEL: instruction`).

### **Output**
- **`.ob` File:** Contains the binary representation of the assembly code.
- **`.ext` File:** Lists external symbols with their usage addresses.
- **`.ent` File:** Lists entry symbols with their resolved addresses.

## **Code Structure**
- **`assembler.c`:** Main entry point and control logic.
- **`pre_assembler.c`:** Processes the assembly file to identify symbols and directives.
- **`memory.c`:** Handles memory manipulation inside the project.
- **`helper_functions.c`:** Contains utility functions for string and memory operations.

## **Error Handling**
The assembler performs extensive validation to ensure correctness. Errors are categorized into:
1. **Syntax Errors:** Invalid instructions, missing operands, etc.
2. **Semantic Errors:** Undefined symbols, invalid addressing modes, etc.

Errors are printed with context to help locate and fix issues.

## **Future Enhancements**
- Add support for macros and advanced directives.
- Improve optimization of generated machine code.
- Implement a user-friendly GUI for assembling files.

## **Contributors**
- Daniel Khayat: [GitHub Profile](https://github.com/danielkhayat16)
