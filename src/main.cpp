/*
 * The MIT License
 *
 * Copyright 2016 Danila Sergeyev <danila.sergeyev@gmail.com>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <cstring>
#include "wav_file.h"

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "Output file is not specified" << std::endl;
        return 1;
    }
    else if (argc > 2)
    {
        std::cout << "Too many input arguments" << std::endl;
        return 1;
    }
    if (strcmp(argv[1], "help") == 0)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "$ ./mariachi <output-file>" << std::endl;
        return 0;
    }
    
    mariachi::WavFile<mariachi::kStereo> sample(argv[1], 44100, 16);
    mariachi::StereoGenerator g(261.626, 32760, 2.5, sample.sample_rate());
    sample << g;

    return 0;
}
