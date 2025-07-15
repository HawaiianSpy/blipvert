//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021-2025 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "blipvert.h"
#include "Utilities.h"
#include "YUVtoRGB.h"
#include "ToFillColor.h"

using namespace std;
using namespace blipvert;

uint32_t width = 1920;
uint32_t height = 1080;
int32_t numframes = 60;

ofstream logFile;

void Log(const string& message)
{
    cout << message;
    if (logFile.is_open())
        logFile << message;
}

void LogLine(const string& message)
{
    Log(message + "\n");
}

vector<const MediaFormatID*> RGBFormats = {
    &MVFMT_RGBA, &MVFMT_RGB32, &MVFMT_RGB24,
    &MVFMT_RGB565, &MVFMT_RGB555, &MVFMT_ARGB1555
};

vector<const MediaFormatID*> YUVFormats = {
    &MVFMT_AYUV, &MVFMT_YUY2, &MVFMT_UYVY, &MVFMT_YVYU, &MVFMT_VYUY,
    &MVFMT_Y800, &MVFMT_Y16, &MVFMT_IYU1, &MVFMT_IYU2, &MVFMT_Y41P,
    &MVFMT_CLJR, &MVFMT_I420, &MVFMT_YV12, &MVFMT_YUV9, &MVFMT_YVU9,
    &MVFMT_IMC1, &MVFMT_IMC2, &MVFMT_IMC3, &MVFMT_IMC4,
    &MVFMT_NV12, &MVFMT_NV21, &MVFMT_Y41T, &MVFMT_Y42T, &MVFMT_YV16
};

void FramerateTest(const MediaFormatID& in_format, const MediaFormatID& out_format,
    uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    t_transformfunc encodeTransPtr = FindVideoTransform(in_format, out_format);
    if (!encodeTransPtr)
    {
        LogLine("Framerate test: " + string(in_format) + " to " + string(out_format) + " aborted: no transform of that type available.");
        return;
    }

    t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(in_format);
    if (!fillBufFunctPtr)
    {
        LogLine("Framerate test: " + string(in_format) + " to " + string(out_format) + " aborted: No transform for " + string(in_format) + " is available.");
        return;
    }

    //if (in_format == MVFMT_YV12/* || out_format == MVFMT_YV12*/)
    //{
    //    __debugbreak();
    //}

    int thread_count = thread::hardware_concurrency();
    int maxInputThreadCount = GetFormatMaxThreadCount(in_format, width, height, thread_count);
    int maxOutputThreadCount = GetFormatMaxThreadCount(out_format, width, height, thread_count);
    int maxthreadCount = min(maxInputThreadCount, maxOutputThreadCount);
    if (maxthreadCount == 1)
    {
        LogLine("Framerate test: " + string(in_format) + " to " + string(out_format) + " aborted: Maximum allowed thread count for " + string(in_format) + " is 1.");
        return;
    }
    else if (maxthreadCount < thread_count)
    {
        thread_count = maxthreadCount;
    }

    uint32_t inBufSize = CalculateBufferSize(in_format, width, height);
    uint32_t outBufSize = CalculateBufferSize(out_format, width, height);

    unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufSize]);
    unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufSize]);
    memset(inBuf.get(), 0, inBufSize);
    memset(outBuf.get(), 0, outBufSize);

    if (IsYUVColorspace(in_format))
    {
        uint8_t Y, U, V;
        FastRGBtoYUV(red, green, blue, &Y, &U, &V);
        fillBufFunctPtr(Y, U, V, alpha, width, height, inBuf.get(), 0);
    }
    else
    {
        fillBufFunctPtr(red, green, blue, alpha, width, height, inBuf.get(), 0);
    }

    struct WorkItem
    {
        Stage inStage;
        Stage outStage;
    };

    queue<WorkItem> jobQueue;
    mutex queueMutex;
    condition_variable cv;
    bool shutdown = false;
    int jobsPending = 0;

    vector<thread> workers;
    for (int i = 0; i < thread_count; ++i)
    {
        workers.emplace_back([&]() {
            while (true)
            {
                WorkItem work;
                {
                    unique_lock<mutex> lock(queueMutex);
                    cv.wait(lock, [&]() { return !jobQueue.empty() || shutdown; });

                    if (shutdown && jobQueue.empty())
                        return;

                    work = jobQueue.front();
                    jobQueue.pop();
                }

                encodeTransPtr(&work.inStage, &work.outStage);

                {
                    lock_guard<mutex> lock(queueMutex);
                    --jobsPending;
                    if (jobsPending == 0)
                        cv.notify_all();
                }
            }
            });
    }

    t_stagetransformfunc pstage_in = FindTransformStage(in_format);
    t_stagetransformfunc pstage_out = FindTransformStage(out_format);

    Log("Framerate test: " + string(in_format) + " to " + string(out_format));

    auto start = chrono::steady_clock::now();

    for (int frame = 0; frame < numframes; ++frame)
    {
        {
            lock_guard<mutex> lock(queueMutex);
            jobsPending = thread_count;

            for (int i = 0; i < thread_count; ++i)
            {
                WorkItem work;
                pstage_in(&work.inStage, i, thread_count, width, height, inBuf.get(), 0, false, nullptr);
                pstage_out(&work.outStage, i, thread_count, width, height, outBuf.get(), 0, false, nullptr);

                jobQueue.push(move(work));
            }
        }

        cv.notify_all();

        {
            unique_lock<mutex> lock(queueMutex);
            cv.wait(lock, [&]() { return jobsPending == 0; });
        }
    }

    auto end = chrono::steady_clock::now();
    double ms = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end - start).count());
    int framerate = static_cast<int>(floor(1000.0 / (ms / static_cast<double>(numframes))));
    LogLine(" @ " + to_string(framerate) + " fps with " + to_string(thread_count) + " persistent threads.");

    {
        lock_guard<mutex> lock(queueMutex);
        shutdown = true;
    }
    cv.notify_all();
    for (auto& t : workers)
        t.join();
}

void RunTest(const MediaFormatID& in_format, const MediaFormatID& out_format)
{
    FramerateTest(in_format, out_format, 128, 128, 128, 255);
}

void RunAllTransforms(int thread_count)
{
    LogLine("Multi-thread transform frame rate test run for resolution: " + to_string(width) + " x " + to_string(height) + "...\n");

    auto start = chrono::steady_clock::now();

    for (const MediaFormatID* in_format : RGBFormats)
        for (const MediaFormatID* out_format : YUVFormats)
            RunTest(*in_format, *out_format);

    for (const MediaFormatID* in_format : YUVFormats)
        for (const MediaFormatID* out_format : RGBFormats)
            RunTest(*in_format, *out_format);

    for (const MediaFormatID* in_format : RGBFormats)
        for (const MediaFormatID* out_format : RGBFormats)
            if (*in_format != *out_format)
                RunTest(*in_format, *out_format);

    for (const MediaFormatID* in_format : YUVFormats)
        for (const MediaFormatID* out_format : YUVFormats)
            if (*in_format != *out_format)
                RunTest(*in_format, *out_format);

    auto end = chrono::steady_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    long long minutes = ms / 60000;
    long long seconds = (ms % 60000) / 1000;
    LogLine("\n\nElapsed Time For " + to_string(width) + " x " + to_string(height) + " Test = " + to_string(minutes) + ":" + to_string(seconds) + "\n\n");
}

int main()
{
    int thread_count = thread::hardware_concurrency();
    if (thread_count == 0) thread_count = 4; // Fallback.

    logFile.open("Multi-thread_framerate_results.txt");
    if (!logFile.is_open())
    {
        cerr << "Error: Could not open output log file." << endl;
        return 1;
    }

    InitializeLibrary();

    bool save = get_UseFasterLooping();
    set_UseFasterLooping(false); // Make sure faster looping is off.

    auto start = chrono::steady_clock::now();

    width = 1920;
    height = 1080;
    RunAllTransforms(thread_count);

    width = 3840;
    height = 2160;
    RunAllTransforms(thread_count);

    auto end = chrono::steady_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    long long minutes = ms / 60000;
    long long seconds = (ms % 60000) / 1000;
    LogLine("Finished. Total Elapsed Time = " + to_string(minutes) + ":" + to_string(seconds));

    set_UseFasterLooping(save);

    logFile.close();
    return 0;
}
