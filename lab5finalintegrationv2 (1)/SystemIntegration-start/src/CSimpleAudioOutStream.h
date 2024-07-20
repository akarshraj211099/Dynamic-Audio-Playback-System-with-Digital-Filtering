/**
 * \file CSimpleAudioOutStream.h
 * \brief interface of CSimpleAudioOutStream
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




#ifndef CSIMPLEAUDIOOUTSTREAM_H_
#define CSIMPLEAUDIOOUTSTREAM_H_

#include "portaudio.h"
#include <stdlib.h>
#include <iostream>
#include "skslib.h"
using namespace std;

class CSimpleAudioOutStream {

	enum STATE
	{
		NOT_READY, READY, PLAYING
	};

private:
PaStream *m_stream;
STATE m_state;

public:
	CSimpleAudioOutStream();
	virtual ~CSimpleAudioOutStream();

	void start();
	void stop();
	void resume();
	void pause();
	void open(int channels,double sampleRate,unsigned long framesPerBuffer);
	void play(float *dataBuffer,unsigned long numFrames);

	void close();

};

#endif /* CSIMPLEAUDIOOUTSTREAM_H_ */
