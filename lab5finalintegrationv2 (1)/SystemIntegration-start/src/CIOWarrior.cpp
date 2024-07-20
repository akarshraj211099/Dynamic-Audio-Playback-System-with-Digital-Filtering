/**
 * \file CIOWarrior.cpp
 *
 * \brief implementation of CIOWarrior, CIOWarriorExt - <b><u>DISFUNCTIONAL VERSION !!!!!!</u></b>
 * \date 05.10.21
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
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
/**
 * \file CIOWarrior.cpp
 *
 * \brief implementation of the IOWarrior40 control class
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */
using namespace std;
#include <iostream>
#include <string>
#include <typeinfo>
#include <SKSLib.h>
#include "CIOWarrior.h"
#include "iowkit.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * CIOWarrior class implementation
 * You find a detailed description for each method in the following comments.
 * Implement each method after its comment (at the position of the respective comment
 * starting with todo)
 */
/**
 * \brief initializes attributes
 */
// todo implement the constructor() here
CIOWarrior::CIOWarrior()
{
	m_handle = NULL;
	m_reportIn.ReportID = 0;
	m_reportIn.Value=0;
	m_reportOut.ReportID = 0;
	m_reportOut.Value=0;

	m_lastError = E_OK;
	m_state = S_NOTREADY;
}

/**
 * \brief closes the device
 * \transition
 * S_READY -> S_NOTREADY
 */
// todo implement destructor() here
CIOWarrior::~CIOWarrior()
{

	IowKitCloseDevice(m_handle);
	m_state = S_NOTREADY;
}

/**
 * \brief Opens IOWarrior device.
 *
 * The default value sets all outputs to high. In this state all port pins
 * can also be read as input
 *
 * \exception
 * - no device found
 * - wrong device id (IoW found, but no IoW40)
 *
 * \transition
 * S_NOTREADY -> S_READY
 *
 * \test
 * to test the validation of the IoW device type, connect an IoW24Kit
 * instead of IoW40Kit
 *
 * functional description
 * ======================
 * - skips all steps if already in READY state
 * - opens IoWDevice, sets last error and throws exception if no device found
 * - reads product id, closes device, sets last error and throws exception if not an IoWarrior40 detected
 * - sets all output port pins to logical 1
 * - sets state to READY
 * - sets last error to OK
 */
// todo implement open() here
void CIOWarrior::open(){

	if(m_state != S_READY)
	{
		m_handle = IowKitOpenDevice();
		if(m_handle == NULL)
		{
			m_lastError=E_NODEVICE;
			throw CException(CException::SRC_IOWarrior, E_NODEVICE, "No Device found.");
			close();

		}

		if (IowKitGetProductId(m_handle)!= IOWKIT_PRODUCT_ID_IOW40)
		{
			m_lastError=E_INVALIDDEVICE;
			throw CException(CException::SRC_IOWarrior, E_INVALIDDEVICE, "Not an IoWarrior40 ");
			close();

		}

		m_reportOut.Value = 0XFFFFFFFF;
		int res = IowKitWrite(m_handle, IOW_PIPE_IO_PINS,(char*)&m_reportOut, sizeof(m_reportOut));
				if(res != sizeof(m_reportOut)){
					m_lastError=E_WRITEERROR;
					throw CException(CException::SRC_IOWarrior, E_WRITEERROR,"Failed to write");
					}
		m_state = S_READY;
		m_lastError=E_OK;
	}



}


/**
 * \brief Closes IOWarrior device.
 *
 * the value of finalOutVal should be selected so, that the power
 * consumption of the device is minimized (e.g. all LEDs off). this depends
 * on the attached circuits. 0xffxxxxxx switches built-in LEDs on port 3 off
 * (they are low active)
 *
 * \transition
 * S_READY -> S_NOTREADY
 *
 * functional description
 * ======================
 * - skips if state is NOTREADY
 * - sets all output pins to logical 1
 * - closes IoW device
 * - sets state to NOTREADY
 */
// todo implement close() here
void CIOWarrior::close(){
	if(m_state!= S_READY)
		return;

	m_reportOut.Value = 0XFFFFFFFF;
	int res = IowKitWrite(m_handle, IOW_PIPE_IO_PINS,(char*)&m_reportOut, sizeof(m_reportOut));
		if(res != sizeof(m_reportOut)){
			m_lastError=E_WRITEERROR;
			throw CException(CException::SRC_IOWarrior, E_WRITEERROR,"Failed to write");
					}
	 IowKitCloseDevice(m_handle);
	 m_handle = NULL;
	 m_state = S_NOTREADY;
	 m_lastError=E_OK;
}

/**
 * \brief Writes 8-bit pattern to IOWarriors LED port 3
 * \param data 8-bit pattern shown on LEDs (1-bit switches the LED on, 0-bit switches the LED off)
 *
 * \exception
 * - device not ready
 * - write error
 *
 * functional description
 * ======================
 * - throws an exception if NOTREADY
 * - switches all LEDs on whose bit is set in data
 */
// todo implement writeLEDs() here
void CIOWarrior::writeLEDs(uint8_t data)
{
	if(m_state==S_NOTREADY)
	{
		throw CException(CException::SRC_IOWarrior, E_DEVICENOTREADY, "Device is not ready");
	}
	m_reportOut.Bytes[3]=~data;
	int res = IowKitWrite(m_handle, IOW_PIPE_IO_PINS, (char*)&m_reportOut, sizeof(m_reportOut));
	if(res!=sizeof(m_reportOut))
	{
		throw(CException(CException::SRC_IOWarrior, E_WRITEERROR,"Failed to write."));
	}


}

/**
 * \brief detects if the key was pressed
 *
 * looks if button on the IOWarrior board (connected to port 0 pin 0) is pressed and returns immediately
 * it is not necessary to wait for the user to release the button,
 * because as long as the state of the ports doesn't change, no new report will
 * be generated by the device
 *
 * \return
 * - true: key pressed
 * - false: key released or no change since last scan of the IoW chip
 *
 * \exception
 * - device not ready
 *
 *
 * functional description
 * ======================
 * - throws an exception if NOTREADY
 * - check if new report is available (any input pin has changed)
 * - returns true if the button has been pressed, otherwise returns false
 */
// todo implement keyPressed() here
bool CIOWarrior::keyPressed()
{
	static uint8_t prev_state;
	uint8_t current_state;
	bool state = false;
	if(m_state!=S_READY)
	{
		throw CException(CException::SRC_IOWarrior, E_DEVICENOTREADY, "Device is not ready");
	}
	int res = IowKitReadNonBlocking(m_handle, IOW_PIPE_IO_PINS, (char*)&m_reportIn, sizeof(m_reportIn));

	if(res == sizeof(m_reportIn))
	{
		current_state = m_reportIn.Bytes[0]&0x00000001;
		// cout<<"Port:"<<hex<<m_reportIn.Value << endl;
		if((current_state == 1 )&&(prev_state==0))
		{
			state = true;
		}
		prev_state = current_state;
	}
		return state;

		/*if((m_reportIn.Bytes[0] & 0x01) == 0x00)
		{

			switch(prevpressed)
			case(true):

			if(!prevpressed)
			{
				prevpressed = true;
				return true;
			}

		}*/
		///return false;
	}


/**
 * \brief Prints the current state of IOWarrior instance.
 *
 * functional description
 * ======================
 * prints m_lastError and m_state both as strings and numerical values on
 * the console
 */
// todo implement printState() here
void CIOWarrior::printState()
{
	cout<<getStateStr()<<" "<<getState()<<endl;
	cout<<getLastErrorStr()<<" "<< getLastError() <<endl;
}

/**
 * \brief delivers current state of IOWarrior object
 * \return state as numerical value
 */
// todo implement getState() here

CIOWarrior::STATES CIOWarrior::getState(){
	return m_state;

}


/**
 * \brief delivers current state of IOWarrior object as a string
 * \return state as string
 */
// todo implement getStateStr() here
string CIOWarrior::getStateStr(){
	switch(m_state)
	{
	case S_READY: return "READY";

	case S_NOTREADY: return "NOT_READY";
	}
}


/**
 * \brief delivers last error of IOWarrior instance
 * \return error number
 */
// todo implement getLastError() here
CIOWarrior::ERRORS CIOWarrior::getLastError(){
	return m_lastError;

}


/**
 * \brief delivers last error of IOWarrior instance
 * \return last error text
 */
// todo implement getLastErrorStr() here
string CIOWarrior::getLastErrorStr()
{
	switch(m_lastError){
	case E_OK: return "OK";
	case E_NODEVICE: return"NO_DEVICE";
	case E_INVALIDDEVICE: return "INVALID_DEVICE";
	case E_WRITEERROR: return "WRITE_ERROR";
	case E_DEVICENOTREADY:return "DEVICE_NOT_READY";
	case E_NOEXTENSION: return "NO_EXTENSION";
	}
}

/**
 * \brief prints device info on console
 *
 * prints several information retrieved from a connected device
 *
 * \exception
 * - device not ready
 *
 *
 * functional description
 * ======================
 * - throws an exception if NOTREADY
 * - determines serial number, product revision and API version
 *   (see the documentation of the IOWarrior API to find the relevant API functions)
 * - prints these data on the console
 */
// todo implement printDeviceInfo() here
void CIOWarrior::printDeviceInfo()
{
if(m_state==S_NOTREADY)
	{
		throw CException(CException::SRC_IOWarrior, E_DEVICENOTREADY, "Device is not ready");
	}

  WCHAR buffer[9];
  if(IowKitGetSerialNumber(m_handle, buffer))
  printf("%ws\n", buffer);

  cout<< IowKitGetRevision(m_handle)<<endl;

}
/**
 * writes all 4 bytes of m_reportOut on the IoW40 ports
 *
 * \exception
 * - write error
 *
 *
 * functional description
 * ======================
 * - writes m_reportOut to all output pins
 * - checks if write was successful
 * - stores error no., closes IoW and throws exception if not
 */
// todo implement writeReportOut() here
void CIOWarrior::writeReportOut()
{

	int res = IowKitWrite(m_handle, IOW_PIPE_IO_PINS,(char*)&m_reportOut, sizeof(m_reportOut));
	if (res!= sizeof(m_reportOut))
	{
		m_lastError=E_WRITEERROR;
		throw CException(CException::SRC_IOWarrior, E_WRITEERROR,"Failed to write");
	}

}





