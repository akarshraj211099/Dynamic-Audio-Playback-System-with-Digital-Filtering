/**
 * \file CAudioPlayerController.cpp
 * \brief implementation of CAudioPlayerController
 *
 * \date 10.01.2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 */
#define USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>			// functions to scan files in folders (used in Lab04prep_DBAdminInsert)
using namespace std;



#include "CException.h"
#include "CUserInterface.h"
#include "CAudioPlayerController.h"
#include "CFileFilter.h"
#include "CFilter.h"
#include "CFileSound.h"

CAudioPlayerController::CAudioPlayerController() {
	m_pSFile = NULL;		// association with 1 or 0 CSoundFile-objects
	m_pFilter = NULL;		// association with 1 or 0 CFilter-objects
}

CAudioPlayerController::~CAudioPlayerController() {
	if (m_pSFile)
		delete m_pSFile;
	if (m_pFilter)
		delete m_pFilter;
}

void CAudioPlayerController::run() {
	// if an exception has been thrown by init(), the user is not able to use the player
	// therefore it is handled by main (unrecoverable error)
	init();

	/***************************************************************
	 * main menue of the player
	 *
	 * note: the last item of the menu must be empty (see CUserInterfaceCmdIOW code)
	 *
	 ***************************************************************/
	string mainMenue[] = { "select sound", "select filter", "play",
			"choose amplitude scale", "terminate player", "" };
	while (1) {
		// if an exception will be thrown by one of the methods, the main menu will be shown
		// after an error message has been displayed. The user may decide, what to do (recoverable error)
		// For instance, if the user selects a filter without having selected a sound file before, an error
		// message tells the user to select a sound file. He/She may decide to do this and then select a filter
		// once more. In this case, the error condition is eliminated and the program may continue regularly.
		try {
			// display the menu and get the user's choice
			int selitem = m_ui.getListSelection(mainMenue, "choose an action");
			// process the user's choice by calling the appropriate CAudioPlayerControllerMethod
			switch (selitem) {
			case 0:
				chooseSound();
				break;
			case 1:
				chooseFilter();
				break;
			case 2:
				play();
				break;
			case 3:
				chooseAmplitudeScale();
				break;
			case 4:
				return;
			default:
				m_ui.printMessage("invalid selection. \n");
			}
		} catch (CException &e) // Exception from the ASDD components
		{
			string eMsg = "Error from: ";
			m_ui.printMessage(
					eMsg + e.getSrcAsString() + "" + e.getErrorText() + "\n");
		} catch (const std::invalid_argument &e) // exception from stod occurs if the last input is not a number
		{
			m_ui.printMessage("Enter a number to choose an action. \n");
		}
	}
}

void CAudioPlayerController::init() {
	// choose player control/visualization device
	string cvMenu[] = { "IOWarrior", "console", "" };
	int cvSel = m_ui.getListSelection(cvMenu,
			"select a device for player control / visualization:");
	// initialize the user interface
	switch (cvSel) {
	case 0:
		m_ui.init(CUserInterface::IOWARRIOR_EXT);
		break;
	case 1:
	default:
		m_ui.init(CUserInterface::CONSOLE);
		break;
	}
}

void CAudioPlayerController::chooseFilter() {
	if (!m_pSFile) // a sound file must have been created by the chooseSoundFile method before
	{
		m_ui.printMessage(
				"Error from selectFilter: No sound file. Select sound file before filter!\n");
		return;
	}

	string chosenFile;
	int fid = CUI_UNKNOWN;
	int delay_ms = 0;
	float gFB = 0., gFF = 0.;

	// show the menu and get the user's choice
	string fltMenue[] = { "delay filter", "other filter", "remove filter", "" };
	int idChoice = m_ui.getListSelection(fltMenue, "choose a filter type");

	if (idChoice == 0) // delay filter
			{
		// ask the user for the values of delay (ms), feed forward gain and feedback gain
		if (false == _configDelayFilter(delay_ms, gFF, gFB)) {
			m_ui.printMessage(
					"Error from selectFilter: Wrong configuration for delay filter. Did not change filter. \n");
			return;
		} else {
			// create a new delay filter (delete the old filter, if there already was one)
			_createDelayFilter(delay_ms, gFF, gFB);
		}
	} else if (idChoice == 1)	// "normal" filter
			{
		string filePath = ".\\files\\filters\\", fileExt = ".txt";
		// show a list of the filters available in the path and get the users choice (fid) and
		// the appropriate filter file (chosenFile)
		fid = _chooseFilterFile(chosenFile, filePath, fileExt);
		if (fid == CUI_UNKNOWN)		// the user has chosen a filter
		{
			// wrong ID
			m_ui.printMessage(
					"Error from selectFilter: No filter data available! Did not change filter. \n");
		} else {
			// create the filter for the current sound file (delete the old filter, if there already was one)
			_createFilter(chosenFile);
		}
	} else // remove the current filter
	{
		if (m_pFilter) // if there was a filter object from a preceding choice of the user
		{
			delete m_pFilter;	// ... delete this
			m_pFilter = NULL;		// currently we have no filter
		}
		m_ui.printMessage("Message from selectFilter: Filter removed. \n");
		return;
	}
}

void CAudioPlayerController::play() {
	// todo delete next line and implement play() here

	float duration = 0.125;
	long framesPerBuff = duration*m_pSFile->getSampleRate();
	int buffSize = framesPerBuff*m_pSFile->getNumChannels();

	float *readBuff = new float[buffSize];  //important make sure that buffSize is an integer

	float *writeBuff = new float[buffSize];

	float *com_buff  = new float[buffSize];


	int channels = m_pSFile->getNumChannels();
	double sampleRate = m_pSFile->getSampleRate();
	m_audioStream.open(m_pSFile->getNumChannels(),sampleRate,framesPerBuff);
	m_audioStream.start();
	enum PlaybackState                     // States to pause and play the audio
	{
		PLAY, PAUSE
	};
	PlaybackState currentState = PLAY;


	int rs = 0;
	do{
		if (m_ui.keyPressed(false)) {

			switch (currentState) {
			case PLAY:
				m_audioStream.pause(); // If the current state is PLAY then the audiosteam is stopped
				cout << "Playback paused. Press ENTER to resume." << endl;
				currentState = PAUSE;
				break;

			case PAUSE: // If the current state is PAUSE then the audiosteam is resumed
				m_audioStream.resume();
				cout << "Playback resumed. Press ENTER to pause." << endl;
				currentState = PLAY;
				break;

			}
		}
		if (currentState == PLAY) {

			rs = m_pSFile->read(readBuff, buffSize);
			if(m_pFilter!=NULL)
			{
				m_pFilter->filter(readBuff, writeBuff,framesPerBuff);
				com_buff = writeBuff;
			}
			else
			{
				com_buff = readBuff;
			}


			//m_audioStream.play(com_buff,framesPerBuff);
			m_audioStream.play(com_buff,rs/channels);
			m_ui.visualizeAmplitude(com_buff, buffSize);

			//m_ui.visualizeAmplitude(writeBuff, buffSize);
		}
	} while(rs!=0);

	m_pSFile->rewind();

	m_audioStream.stop();
	m_audioStream.close();

	//m_ui.printMessage("play() not yet implemented!\n");
}

void CAudioPlayerController::chooseSound() {
	// todo delete next line and implement chooseSound() here
	// show the sound menu and get the user's choice

	string sndFolderPath = ".\\files\\sounds\\";
	string sndFileExt = ".wav";
	string soundList [25];

	/**
			 * \brief reads all filenames with the given extension from the given directory and writes them
			 * into a string array
			 * \param path[in] - directory where the files are stored
			 * \param ext[in] - extension of the files
			 * \param filelist[in/out] - array for the file names to be stored
			 * \param maxNumFiles[in/out] - size of the array
			 * \return number of files stored in the list (maxNumFiles at maximum)
			 */


	uint16_t numSounds = _getFiles(sndFolderPath, sndFileExt, soundList, 25);

	// Dynamically create a string array for the sound selection with one element more that the length of the soundlist(soundmenu)
	string *soundMenu = new string[numSounds+1];
	int soundListPos = 0;


	while(soundListPos < numSounds)
	{
		CFileSound sndFile(sndFolderPath + soundList[soundListPos],CFileBase::FILE_READ);
		sndFile.open();

		// write the soundfile info at the soundlist position into the soundmenu
		soundMenu[soundListPos] = soundList[soundListPos] + "[" + to_string(sndFile.getSampleRate()) + "Hz, " + to_string(sndFile.getNumChannels()) + "Channels ]";
		sndFile.close();
		soundListPos++;

	}

	/**
	 * \brief displays a menu and returns the user's choice
	 *
	 * \param items [in]: pointer to array containing items of menue list; last item MUST be an empty string
	 * \param prompt [in]: text printed for user information what to do
	 */
	int sid = m_ui.getListSelection(soundMenu, "Choose a sound");

	if(sid != CUI_UNKNOWN)
	{
		// Dynamically create a new sound file obj with the path of the chosen sound file


		if(m_pSFile)
		{
			delete m_pSFile;
		}

		//store the address of the dynamically created sound file obj in the controllers soundfile attribute
		m_pSFile = new CFileSound(sndFolderPath + soundList[sid],CFileBase::FILE_READ);
		m_pSFile->open();

		_adaptFilter();

		//Delete the sound menu
		delete[] soundMenu;


	}


}
/**
	 * \brief Displays a menu with the options "linear scale" and "logarithmic scale" and
	 * \brief lets the user choose an option and calls m_ui.setAmplitudeScaling() according to
	 * \brief the users choice.
	 *
	 * if the users choice is invalid, an error message is shown and the scaling will not be changed.
	 *
	 */

void CAudioPlayerController::chooseAmplitudeScale() {
	// todo delete next line and implement chooseAmplitudeScale() here
	string display[] = {"linear scale","logarithmic scale",""};
	int display_id = m_ui.getListSelection(display, "Choose a display option");
	switch(display_id)
	{
	case(0):m_ui.setAmplitudeScaling(CAmpMeter::SCALING_MODE_LIN);
	break;
	case(1):m_ui.setAmplitudeScaling(CAmpMeter::SCALING_MODE_LOG);
	break;
	default:m_ui.printMessage("Invalid choice. Please choose a valid option.");
	break;
	}


}

/**
 * private helper methods
 */
bool CAudioPlayerController::_configDelayFilter(int &delay_ms, float &gFF,
		float &gFB) {
	m_ui.printMessage("Delay filter configuration\n------------\n");
	delay_ms = abs(m_ui.getUserInputInt("delay in msec: "));
	gFF = m_ui.getUserInputFloat("feed forward gain: ");
	gFB = m_ui.getUserInputFloat("feed back gain  (0 <= gFB < 1): ");
	if ((gFB < 0.) || (gFB >= 1.))
		return false;
	return true;
}

void CAudioPlayerController::_createDelayFilter(int delay_ms, float gFF,
		float gFB) {
	// todo: comment out, if the delay filter class exists
	m_ui.printMessage(
			"Delay filters are not yet implemented. Filter won't be changed. \n");
	// todo: comment in, if the delay filter class exists (adjust the class name if necessary)
//	if (m_pFilter)
//		delete m_pFilter;
//	m_pFilter = new CDelayFilter(gFF, gFB, delay_ms, m_pSFile->getSampleRate(),
//			m_pSFile->getNumChannels());
}

int CAudioPlayerController::_chooseFilterFile(string &chosenFile,
		string filePath, string fileExt) {
	string filterlist[25];

	// get all files that are stored in filePath and that extension matches fileExt
	uint16_t numflt = _getFiles(filePath, fileExt, filterlist, 25); // get the list of filter files

	// list the files and their information
	int fid = CUI_UNKNOWN;
	if (numflt)									// if there are filters that fit
	{
		// prepare a string array for the user interface, that will contain  a menu with the selection of filters
		// there is place for an additional entry for an unfiltered sound and an empty string
		string *pFlt = new string[numflt + 2];

		for (int i = 0; i < numflt; i++) {
			// create a filter file object
			CFileFilter fltfile(filePath + filterlist[i],
					CFileBase::FILE_READ);
			try {
				fltfile.open();
				// check if it has a filter with an appropriate sampling frequency
				fltfile.read(m_pSFile->getSampleRate());
				// insert the filter data into the string array
				pFlt[i] = fltfile.getFilterType() + ", order="
						+ to_string(fltfile.getOrder())
						+ fltfile.getFilterInfo();
				fltfile.close();
			} catch (CException &e) {
				// file does not have the appropriate sampling frequency
				if (e.getErrorCode() != CFileBase::FILE_E_FILENOTOPEN)
					fltfile.close();
				// file could not be opened (nothing to do)
			}
		}

		// add the last menu entry for the choice of an unfiltered sound
		pFlt[numflt] = "-1 [no filter]";

		// pass the arrays to the user interface and wait for the user's input
		// if the user provides a filterID which is not in pFIDs, the method returns
		// CUI_UNKNOWN
		fid = m_ui.getListSelection(pFlt, "choose a filter");
		if (fid != CUI_UNKNOWN)
			chosenFile = filePath + filterlist[fid];

		// destroy the array
		delete[] pFlt;
	}
	return fid;
}

void CAudioPlayerController::_createFilter(string filterFile) {
	// if there was a filter object from a preceding choice of the user that does not fit anymore, delete this
	if (m_pFilter)
		delete m_pFilter;

	CFileFilter fltfile(filterFile, CFileBase::FILE_READ);
	fltfile.open();
	fltfile.read(m_pSFile->getSampleRate());

	// create filter
	// Lab05 changed: get filter data
	int order = fltfile.getOrder();
	string type = fltfile.getFilterType();
	float *ac = fltfile.getACoeffs();
	float *bc = fltfile.getBCoeffs();
	m_pFilter = new CFilter(filterFile, ac, bc, order,
			m_pSFile->getNumChannels());
}

void CAudioPlayerController::_adaptFilter() {
	if (m_pFilter) {
		// check filter type
		if (typeid(*m_pFilter) == typeid(CFilter))
			_createFilter(((CFilter*) m_pFilter)->getFilePath());
		else {
			// todo: comment out, if CDelayFilter exists
			m_ui.printMessage(
					"Delay filters are not yet implemented. Filter will be deleted. \n");
			if (m_pFilter)
				delete m_pFilter;

			// todo: comment in, if the delay filter class exists (adjust the class name if necessary)
//			CDelayFilter *pdflt = (CDelayFilter*) m_pFilter;
//			_createDelayFilter(pdflt->getDelay(), pdflt->getGainFF(),
//					pdflt->getGainFB());
		}
	}
}

uint16_t CAudioPlayerController::_getFiles(string path, string ext,
		string *filelist, uint16_t maxNumFiles) {
	dirent *entry;
	DIR *dp;
	string file;

	dp = opendir(path.c_str());
	if (dp == NULL)
		throw CException(CException::SRC_File, -1,
				"Could not open folder." + path);

	uint16_t i = 0;
	while ((entry = readdir(dp))) {
		file = entry->d_name;
		if (file.rfind(ext) != string::npos) {
			if (i >= maxNumFiles) {
				m_ui.printMessage("Incomplete file list!\n");
				break;
			}
			filelist[i] = file;
			i++;
		}
	}
	closedir(dp);

	return i;
}
