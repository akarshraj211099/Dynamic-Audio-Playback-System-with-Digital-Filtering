/**
 * \file CFileSound.h
 *
 * \brief interface class CFileSound
 * \date 17.11.2023
 *
 * \see File
 */
#ifndef FILESOUND_H_
#define FILESOUND_H_

#include "sndfile.h"
#include "CFileBase.h"
#include <string>
using namespace std;

/**
 * Soundfile handling class
 *
 */
class CFileSound: public CFileBase {
private:
	SNDFILE *m_pSFile;
	SF_INFO m_sfinfo;

public:
	/**
	 * \brief constructor of a sound file object
	 * \params path[in]  -  complete path to a sound file
	 * \params fMode[in] -  mode for opening the file (you can combine different modes by passing e.g. mode1 | mode2)
	 */
	CFileSound(string path, const FILEMODES mode);
	~CFileSound();

	/**
	 * opens a sound file
	 */
	void open();
	/**
	 * closes a sound file
	 */
	void close();
	/**
	 *\brief  reads the content of the sound file
	 *\brief  Functions for reading and writing the data chunk in terms of frames.
** The number of items actually read/written = frames * number of channels.
	 *\brief  returns the total number of frames read
	 *\params buf[in] - address of a buffer to store floating point audio data (size shall be numFrames * number of channels)
	 *\params bufsize[in] - size of the buffer in floating point elements
	 */
	int read(float *buf, int numFrames);
	/**
	 * writes the content of the sound file
	 */
	void write(float *buf, int bufsize);
	/**
	 * sets the file pointer of an open sound file back to the start
	 */
	void rewind();
	/**
	 * prints the properties of the sound file on the console
	 */
	void print(void);
	/**
	 * methods to retrieve information about the sound file
	 */
	int getNumFrames();
	int getSampleRate();
	int getNumChannels();
	int getFormat();

	/**
	 * methods to set information about the sound file (for writing)
	 */
	void setSampleRate(int fs);
	void setNumChannels(int numChan);
	void setFormat(int format);
};
#endif /* FILESOUND_H_ */
