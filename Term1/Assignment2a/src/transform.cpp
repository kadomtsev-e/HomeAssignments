// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "transform.h"
#include <iostream>
#include <fstream>

namespace filerev {

void reverse_file(const char* input_path, const char* output_path) {
    std::ifstream in(input_path, std::ios::binary | std::ios::ate);
    if (!in) {
        std::cerr << "Error: Could not open input file " << input_path << std::endl;
        return;
    }

    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    if (!in.read(buffer, size)) {
        std::cerr << "Error: Could not read file content" << std::endl;
        delete[] buffer;
        return;
    }
    in.close();

    // Reverse array
    for (std::streamsize i = 0; i < size / 2; ++i) {
        char temp = buffer[i];
        buffer[i] = buffer[size - 1 - i];
        buffer[size - 1 - i] = temp;
    }

    std::ofstream out(output_path, std::ios::binary);
    if (!out) {
        std::cerr << "Error: Could not open output file " << output_path << std::endl;
        delete[] buffer;
        return;
    }

    if (!out.write(buffer, size)) {
        std::cerr << "Error: Could not write to file" << std::endl;
    }
    
    out.close();
    delete[] buffer;
}

} // namespace filerev
