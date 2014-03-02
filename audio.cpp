#include "audio.h"
#include <iostream>
#include "portaudio.h"

using namespace std;

SoundManager::SoundManager()
{
    recording = false;
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        cout << "SoundManager Init error..\n";
    }
    cur_input_device = Pa_GetDefaultInputDevice();
    if(cur_input_device == paNoDevice)
        cout << "ERROR: No input device detected\n";
    cur_output_device = Pa_GetDefaultOutputDevice();
    if(cur_output_device == paNoDevice)
        cout << "ERROR: NO output device detected\n";
    totalFrames = SAMPLE_RATE * MAX_BROADCAST_TIME;
    data = paTestData();
}


SoundManager::~SoundManager()
{
    delete [] data.recordedSamples;
}

PaError SoundManager::record()
{
    //if(!recording)
    //{

        //recording = true;
        Pa_OpenDefaultStream(&audio_stream,1,0, PA_SAMPLE_TYPE , SAMPLE_RATE ,paFramesPerBufferUnspecified, recordCallback, &data);
        PaError err = Pa_StartStream(audio_stream);
        if(err != paNoError)
            cout << "Recording\n" << endl;
        while( ( err = Pa_IsStreamActive( audio_stream ) ) == 1 )
        {
            Pa_Sleep(1000);
            cout << "index = " << data.frameIndex  << "\n";
            cout.flush();
        }
   // }
        return err;
}

PaError SoundManager::play()
{
    //if(!recording)
    //{

        //recording = true;
        Pa_OpenDefaultStream(&audio_stream,0,1, PA_SAMPLE_TYPE , SAMPLE_RATE ,paFramesPerBufferUnspecified, recordCallback, &data);
        PaError err = Pa_StartStream(audio_stream);
        if(err != paNoError)
            cout << "Recording\n" << endl;
        while( ( err = Pa_IsStreamActive( audio_stream) ) == 1 ) Pa_Sleep(100);
        return err;
}



PaError SoundManager::stop()
{
        PaError err = Pa_CloseStream(audio_stream);
        if(err != paNoError)
            cout << "Stopped\n" << endl;
        return err;
}

static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;
    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE; /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE; /* right */
        }
    }
    else
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = *rptr++; /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++; /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
 */
static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        /* final buffer... */
        for( i=0; i<framesLeft; i++ )
        {
            *wptr++ = *rptr++; /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++; /* right */
        }
        for( ; i<framesPerBuffer; i++ )
        {
            *wptr++ = 0; /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = 0; /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *wptr++ = *rptr++; /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++; /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

paTestData::paTestData()
{
    maxFrameIndex = SAMPLE_RATE * MAX_BROADCAST_TIME; /* Record for a few seconds. */
    frameIndex = 0;
    recordedSamples =  new SAMPLE[maxFrameIndex * NUM_CHANNELS];
}

paTestData::~paTestData()
{
    delete [] recordedSamples;

}
