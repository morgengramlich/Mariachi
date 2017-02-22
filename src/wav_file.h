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

#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <fstream>
#include <string>
#include "generator.h"

namespace mariachi
{
    enum FileType
    {
        kMono = 1,
        kStereo,
    };

    template<FileType Type>
    class WavFile
    {
    public:
        WavFile(const std::string& name
               ,unsigned long sample_rate
               ,unsigned short bps);
        virtual ~WavFile() {};

        WavFile<Type>& operator<<(Generator generator);
        WavFile<kStereo>& operator<<(StereoGenerator generator);

        // getters
        inline const std::string& name() const { return name_; };
        inline unsigned long sample_rate() const { return sample_rate_; };
        inline unsigned short bps() const { return bps_; };
        inline unsigned short num_channels() const { return Type; };

    private:
        // private variables
        const std::string name_;
        unsigned long sample_rate_;
        unsigned long byte_rate_;
        unsigned short block_align_;
        unsigned short bps_;
        unsigned long file_size_;

        std::ofstream out_;

        // private methods
        template <typename T>
        void writeLittleEndian(T value, unsigned int size = sizeof(T));

        // constants
        static const unsigned short kHeaderSize = 44;
    };

    template<FileType Type>
    WavFile<Type>::WavFile(const std::string& name
                          ,unsigned long sample_rate
                          ,unsigned short bps):
        name_(name)
       ,sample_rate_(sample_rate)
       ,bps_(bps)
       ,byte_rate_(sample_rate * Type * bps / 8)
       ,block_align_(Type * bps / 8)
       ,out_(name, std::ofstream::binary)
       ,file_size_(kHeaderSize)
    {
        out_ << "RIFF";
        writeLittleEndian(kHeaderSize - 8, 4);
        out_ << "WAVEfmt ";
        writeLittleEndian(16, 4);
        writeLittleEndian(1, 2);
        writeLittleEndian((Type == kMono) ? 1 : 2, 2);
        writeLittleEndian(sample_rate_, 4);
        writeLittleEndian(byte_rate_, 4);
        writeLittleEndian(block_align_, 2);
        writeLittleEndian(bps_, 2);

        out_ << "data";
        writeLittleEndian(0, 4);
    }

    template<FileType Type>
    WavFile<Type>& WavFile<Type>::operator<<(Generator generator)
    {
        out_.seekp(file_size_);
        unsigned long size = sample_rate_ * generator.time();
        for (unsigned long i = 0; i < size; i++)
        {
            writeLittleEndian((unsigned int)generator.next(), 2);
        }

        file_size_ = out_.tellp();
        out_.seekp(kHeaderSize - 4);
        writeLittleEndian(file_size_ - kHeaderSize, 4);
        out_.seekp(4);
        writeLittleEndian(file_size_ - 8, 4);

        return *this;
    }

    template<>
    WavFile<kStereo>& WavFile<kStereo>::operator<<(StereoGenerator generator)
    {
        out_.seekp(file_size_);
        unsigned long size = sample_rate_ * generator.time();
        for (unsigned long i = 0; i < size; i++)
        {
            writeLittleEndian((unsigned int)generator.nextLeft(), 2);
            writeLittleEndian((unsigned int)generator.nextRight(), 2);
        }

        file_size_ = out_.tellp();
        out_.seekp(kHeaderSize - 4);
        writeLittleEndian(file_size_ - kHeaderSize, 4);
        out_.seekp(4);
        writeLittleEndian(file_size_ - 8, 4);

        return *this;
    }

    template<FileType Type>
    template <typename T>
    void WavFile<Type>::writeLittleEndian(T value, unsigned int size)
    {
        while (size > 0)
        {
            out_.put(static_cast<char>(value & 0xFF));
            value >>= 8;
            size--;
        }
    }
}

#endif /* WAV_FILE_H */
