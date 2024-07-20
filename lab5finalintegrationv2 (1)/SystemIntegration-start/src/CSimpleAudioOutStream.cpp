// < ---------- SAMPLE-START ----------->
/**
 * \file CSimpleAudioOutStream.cpp
 * \brief implementation of CSimpleAudioOutStream
 *
 * \date Sept 24, 2019
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 */

/************************************************************************
 *
 *	 					ATTENTION !!!!!!!!!
 *
 * This file contains only rudimentary code in a nonfunctional version
 * to make the start project compilable and executable in its initial version.
 *
 * It should be replaced by an complete implementation.
 *
 ************************************************************************/

#include <stdlib.h>
#include <memory.h>
#include <string>
using namespace std;

#include "CSimpleAudioOutStream.h"

CSimpleAudioOutStream::CSimpleAudioOutStream() {

	m_stream = nullptr;
	m_state = NOT_READY;

}

CSimpleAudioOutStream::~CSimpleAudioOutStream() {

	close();
	// TODO Auto-generated destructor stub
}


void CSimpleAudioOutStream::open(int channels, double sampleRate,unsigned long framesPerBuffer){

	PaError err;            // PortAudio specific error values
	err = Pa_Initialize();  // init PortAudio API

	if(err!=paNoError)
	{
		throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));
	}
		/* get an audio device and configure the audio stream */
		//stream variables
		PaStreamParameters outputParameters;
		outputParameters.device = Pa_GetDefaultOutputDevice();

		if(outputParameters.device != paNoDevice)
		{
			outputParameters.channelCount = channels;
			outputParameters.sampleFormat = paFloat32;  // 32 bit floating point output
			outputParameters.hostApiSpecificStreamInfo = NULL;
			outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
			/*PaDeviceInfo Struct has PaTime defaultHighOutputLatency
			Pa_GetDeviceInfo(outputParameters.device) returns a ptr to the struct PaDeviceInfo*/

			/*
open the stream
			 */
			err = Pa_OpenStream	( &m_stream,//PaStream **
								  NULL, // const PaStreamParameters * 	inputParameters
								  &outputParameters,//const PaStreamParameters * 	outputParameters
								  sampleRate,//
								  framesPerBuffer,//
								  paClipOff,//PaStreamFlags 	streamFlags,,/* we won't output out of range samples so don't bother clipping them */
								  NULL,//	PaStreamCallback * 	streamCallback, /* no callback, use blocking API */
								  NULL //	void * 	userData /* no callback, so no callback userData */
			);

			m_state = READY;
		//	cout<<"m_state = READY(open)"<<endl;


			if(err==paNoError)
			{
				//Not necessary to evaluate the newly created stream
				const PaStreamInfo* pStreamInfo= Pa_GetStreamInfo(m_stream);
				cout << " Stream properties ";
				cout << " output latency= " << pStreamInfo->outputLatency;
				cout << "s sample rate= " << pStreamInfo->sampleRate << endl;
			}
		}
	}


void CSimpleAudioOutStream::start(){

	if(m_state == PLAYING)
	{
		PaError err;
		err = Pa_StartStream(m_stream);

		if(err != paNoError)
			throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));


		m_state = PLAYING;
		//cout<<"m_state = PLAYING(start)"<<endl;

	}

	if(m_state == NOT_READY)
	{
		throw CException(CException::SRC_SimpleAudioDevice,-1,"CAudionPlayback object must be in state READY! Did you forget to call open?");
	}
	if(m_state == READY)
	{
	PaError err;
	err = Pa_StartStream(m_stream);

	if(err != paNoError)
		throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));


	m_state = PLAYING;
	//cout<<"m_state = PLAYING(start)"<<endl;
	}

}


void CSimpleAudioOutStream::stop(){

	if(m_state == PLAYING)
	{

		PaError err;
		err = Pa_StopStream(m_stream);

		if(err != paNoError)
			throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));

		m_state = READY;
		//cout<<"m_state = READY(stop)"<<endl;
	}
	if(m_state == READY)
	{
		//cout<<"the state is not in playing to stop(READY)"<<endl;
	}
	if(m_state == NOT_READY)
	{
		//cout<<"the state is not in playing to stop(NOT_READY)"<<endl;
	}

}


void CSimpleAudioOutStream::resume()
{
	start();
}

void CSimpleAudioOutStream::pause(){
	stop();

}

void CSimpleAudioOutStream::close()
{
	if(m_state == PLAYING)
		throw CException(CException::SRC_SimpleAudioDevice, -1, "CAudionPlayback object must be in state READY you forgot to call stop/pause ?");
	if(m_state == READY)
	{    Pa_CloseStream(m_stream);
	m_stream = nullptr;
	}

	// Terminate PortAudio
	Pa_Terminate();
	m_state = NOT_READY;
	//cout<<"m_state = NOT_READY(close)"<<endl;
}




 void CSimpleAudioOutStream:: play(float *dataBuffer,unsigned long numFrames){
	PaError err;
	if(m_state == NOT_READY)
		throw CException(CException::SRC_SimpleAudioDevice, -1, "m_state != playing");

	if(m_state == READY)
	{
		err = Pa_StartStream(m_stream);

		if (err != paNoError)
			throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));

		m_state = PLAYING;
		//cout<<"m_state = PLAYING"<<endl;

	}

	// If m_state is already in PLAYING state
	if(m_state ==PLAYING)
	{
		err = Pa_WriteStream( m_stream,dataBuffer,numFrames);

		if(err != paNoError)
			throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));

	}

 }





// < ----------- SAMPLE-END ------------>
