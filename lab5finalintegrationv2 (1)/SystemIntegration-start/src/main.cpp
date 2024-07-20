/**
 * \file main.cpp
 * \brief main, test and auxiliary functions for ASDD Lab05
 *
 * \date 17.01.2022
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 */

#include "CAudioPlayerController.h"
#include "CFileFilter.h"
#include "CFilter.h"

#include "CFileSound.h"
#include "CFileFilter.h"
// Include other necessary headers for your classes
#include <iostream>

/**
 * horizontal divider for test list output
 */
string hDivider(80, '-');

/*
 * *****************************************************
 * declarations of test functions (to be called in main)
 * *****************************************************
 */
// laboratory tasks
void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w,
		string &fltfile);

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	cout << "Systemintegration started" << endl;

	/*
	 * todo: Check if Test_Lab01_SoundFilterPlayTest() works
	 * comment out after check
	 */

	string sndname = "jazzyfrenchy";
	string sndf = ".\\files\\sounds\\" + sndname + ".wav";
	string sndfw = ".\\files\\sounds\\" + sndname + "_filtered.wav";
	string fltf = ".\\files\\filters\\2000Hz_lowpass_Order6.txt";

	//Test_Lab01_SoundFilterPlayTest(sndf,sndfw,fltf);

	CAudioPlayerController myController; 	// create the controller
	try {
		myController.run();					// run the controller
	} catch (CException &e) {
		cout << e << endl;
	}
	cout << "Systemintegration finished" << endl;
}

// todo insert Test_Lab01_SoundFilterPlayTest() here
void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w,string &fltfile) {

	cout << endl << hDivider << endl << __FUNCTION__ << " started." << endl
				<< endl;

	// Read from a Sound File
	CFileSound sndfil_rd(soundfile,CFileBase::FILE_READ);
	sndfil_rd.open();

	// Write to a Sound File
	CFileSound sndfil_wr(sndfile_w,CFileBase::FILE_WRITE);

	sndfil_wr.setSampleRate(sndfil_rd.getSampleRate());
	sndfil_wr.setNumChannels(sndfil_rd.getNumChannels());
	sndfil_wr.setFormat(sndfil_rd.getFormat());

	sndfil_wr.open();

	// Filter text file
	//CFileFilter fltrFile(fltfile,CFileBase::FILE_READ,sndfil_rd.getSampleRate());

	CFileFilter fltrFile(fltfile,CFileBase::FILE_READ);
	fltrFile.open();
	fltrFile.read(sndfil_rd.getSampleRate());

	int N = fltrFile.getOrder();
	float *b = fltrFile.getBCoeffs();
	float *a = fltrFile.getACoeffs();

    // fILTER

	//CFilter(const string filePath, float *ca, float *cb, int order, int channels = 2);

	CFilter fltr(fltfile, a, b, N, sndfil_rd.getNumChannels());


	float duration = 1;
	long framesPerBuff = duration*sndfil_rd.getSampleRate();
	int buffSize = framesPerBuff*sndfil_rd.getNumChannels();

	float *readBuff = new float[buffSize];  //important make sure that buffSize is an integer

	float *writeBuff = new float[buffSize];

	CSimpleAudioOutStream audiostream;

	//void open(int channels,double sampleRate,unsigned long framesPerBuffer);

	double sampleRate = sndfil_rd.getSampleRate();
	audiostream.open(sndfil_rd.getNumChannels(),sampleRate,framesPerBuff);
	audiostream.start();

	int rs = 0;
	//int read(float *buf, int bufsize);
	do{

		rs = sndfil_rd.read(readBuff, buffSize);
		fltr.filter(readBuff, writeBuff,framesPerBuff); // straight forward difference equation
		audiostream.play(writeBuff,framesPerBuff);

	}while(rs!=0);

	audiostream.stop();
	audiostream.close();

	cout << endl << __FUNCTION__ << " finished." << endl << hDivider << endl;



}


