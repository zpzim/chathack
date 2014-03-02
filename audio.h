#ifndef AUDIO_H
#define AUDIO_H

#include "portaudio.h"
//#include "../../../portaudio/include/portaudio.h"

//#define PA_SAMPLE_TYPE paFloat32
const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 512;
const int NUM_CHANNELS = 1;
const int MAX_BROADCAST_TIME = 10;
#if 1
#define PA_SAMPLE_TYPE paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 0
#define PA_SAMPLE_TYPE paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE (128)
#define PRINTF_S_FORMAT "%d"
#endif

struct paTestData
{
    paTestData();
    ~paTestData();
    int frameIndex; /* Index into sample array. */
    int maxFrameIndex;
    SAMPLE *recordedSamples;
};

class SoundManager
{
public:
   SoundManager();
   ~SoundManager();
   PaError record();
   PaError stop();
   PaError play();

private:
    PaDeviceIndex cur_input_device;
    PaDeviceIndex cur_output_device;
    PaStream * audio_stream;
    bool recording;
    paTestData data;
    int totalFrames;
    int recordCallback( const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData );

    int playCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData );


};

#endif // AUDIO_H
