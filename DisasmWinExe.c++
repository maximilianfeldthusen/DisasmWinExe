#include <iostream>
#include <fstream>
#include <libdasm.h>

int main() {
    // Open the .exe file
    std::ifstream file("<path_to_exe_file>", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open the .exe file." << std::endl;
        return 1;
    }

    // Read the file contents into a buffer
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    // Disassemble the code
    dasm_State *dasm = dasm_init(DASM_ARCH_X86, DASM_OPTION_GENERATE);
    dasm_setupglobal(dasm, buffer.data(), buffer.size());
    dasm_setup(dasm, &my_actionlist);
    int status = dasm_link(dasm);
    if (status != DASM_S_OK) {
        std::cout << "Failed to link the disassembler." << std::endl;
        dasm_free(dasm);
        return 1;
    }

    // Print the disassembly result
    dasm_exec(dasm, 0);
    dasm_free(dasm);

    std::cout << "Disassembly finished." << std::endl;

    return 0;
}

// Action list for dasm_setup
