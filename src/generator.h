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

#ifndef GENERATOR_H
#define GENERATOR_H

namespace mariachi
{
    class Generator
    {
    public:
        Generator(double frequency, double amplitude, double time, unsigned long sample_rate):
            frequency_(frequency)
           ,amplitude_(amplitude)
           ,time_(time)
           ,sample_rate_(sample_rate)
           ,counter_(0) {};
        virtual ~Generator() {};

        inline double time() const { return time_; };
        inline double frequency() const { return frequency_; };
        inline double amplitude() const { return amplitude_; };
        double next();

    protected:
        double time_; // time in seconds
        double frequency_;
        double amplitude_;
        unsigned long sample_rate_;
        int counter_;
    };

    class StereoGenerator : public Generator
    {
    public:
        StereoGenerator (double frequency, double amplitude, double time, unsigned long sample_rate):
            Generator(frequency, amplitude, time, sample_rate)
           ,channel_flag_(0) {};

        double nextLeft();
        double nextRight();
    private:
        unsigned short channel_flag_;
        void incrementCounterIfNeeded();
    };
}

#endif /* GENERATOR_H */
