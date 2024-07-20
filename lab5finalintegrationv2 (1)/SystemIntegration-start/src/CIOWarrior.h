/**
 * \file CIOWarrior.h
 *
 * \brief Interface for CIOWarrior, CIoWarriorExt
 * \date 05.10.21
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */


/**
 * \file CIOWarrior.h
 *
 * \brief Interface for CIOWarrior
 * \date 05.10.21
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */
#ifndef CIOWARRIOR_H_
#define CIOWARRIOR_H_

#include <SKSLib.h>
#include <wtypes.h>
#include <iowkit.h>
#include "CPlayerCVDevice.h"

/**
 * \brief Controller for a bare IoW40Kit device
 */
class CIOWarrior:public CPlayerCVDevice {
public:
	// todo define the enums ERRORS and STATES here (see UML class diagram)

	enum ERRORS
	{
		E_OK, E_NODEVICE, E_INVALIDDEVICE,E_WRITEERROR,E_DEVICENOTREADY, E_NOEXTENSION

	};
	enum STATES
	{
		S_NOTREADY ,S_READY
	};

protected:
	// todo define the attributes according to the UML class diagram here
	IOWKIT_HANDLE m_handle;
	IOWKIT40_IO_REPORT m_reportIn;
	IOWKIT40_IO_REPORT m_reportOut;
	ERRORS m_lastError;
	STATES m_state ;
public:
	// todo define the methods according to the UML class diagram here

	CIOWarrior();
	virtual ~CIOWarrior();
	void open();
	void close();
	void writeLEDs(uint8_t data);
	bool keyPressed();
	void printState();
	STATES getState();
	string getStateStr();
	ERRORS getLastError();
	string getLastErrorStr();
	void printDeviceInfo();


protected:
	// todo define the protected method according to the UML class diagram here

	void writeReportOut();

};

/**
 * \brief class for IoWarrior Extension board of FBEIT
 */

#endif /* CIOWARRIOR_H_ */



/*#ifndef CIOWARRIOR_H_
#define CIOWARRIOR_H_

#include <SKSLib.h>
#include <wtypes.h>
#include <iowkit.h>

*
 * \brief Controller for a bare IoW40Kit device

class CIOWarrior {
public:
	// todo define the enums ERRORS and STATES here (see UML class diagram)
	// < ---------- SAMPLE-START ----------->
	*
	 * \brief Error Codes

	enum ERRORS {
		*
		 *  \brief no error occurred
		 *
		 *  one of these codes is handed to if an instance of CIOWarrior throws an exception of type CException

		E_OK,
		*
		 * \brief no IoWarrior device connected

		E_NODEVICE,
		*
		 * \brief no IoWarrior40 Kit device detected

		E_INVALIDDEVICE,
		*
		 * \brief write error on IoW device

		E_WRITEERROR,
		*
		 * \brief device not in the ready state

		E_DEVICENOTREADY,
		*
		 * \brief no extension board available

		E_NOEXTENSION
	};
	*
	 * Device States
	 *
	 * implementing a simple state machine which behaves as follows:
	 * \image html CIoWarrior_StateMachine.png

	enum STATES {
		*
		 * device object is created but connection to device not opened

		S_NOTREADY,
		*
		 * device object is created and connection to device opened

		S_READY
	};
	// < ----------- SAMPLE-END ------------>

protected:
	// todo define the attributes according to the UML class diagram here
	// < ---------- SAMPLE-START ----------->
	*
	 * handle of the device (NULL if no device is connected at program start)

	IOWKIT_HANDLE m_handle;
	*
	 * report structure of IoWarrior40 for output values

	IOWKIT40_IO_REPORT m_reportOut;
	*
	 * report structure of IoWarrior40 for input values

	IOWKIT40_IO_REPORT m_reportIn;
	*
	 * last error occurred (CIOWarrior::E_OK if no error occurred)

	ERRORS m_lastError;
	*
	 * current state of the instance

	STATES m_state;
	// < ----------- SAMPLE-END ------------>
public:
	// todo define the public methods according to the UML class diagram here
	// < ---------- SAMPLE-START ----------->
	*
	 * \brief initializes attributes

	CIOWarrior();

	*
	 * \brief closes the device
	 * \transition
	 * S_READY -> S_NOTREADY

	virtual ~CIOWarrior();

	*
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
	 * - opens IoWDevice, throws exception if no device found
	 * - reads product id, closes device and throws error if not an IoWarrior40 detected
	 * - sets all output port pins to logical 1
	 * - sets state to READY
	 * - sets last Error to OK

	void open(void);

	*
	 * \brief Closes IOWarrior device.
	 *
	 * the value of finalOutVal should be selected so, that the power
	 * consumption of the device is minimized(e.g. all LEDs off). this depends
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

	void close(void);
	*
	 * \brief Writes 8-bit pattern to IOWarriors LED port 3
	 * \param data 8-bit pattern shown on LEDs (1-bit switches the LED on, 0-bit switches the LED off)
	 * \exception
	 * - device not ready
	 * - write error
	 *
	 *
	 * functional description
	 * ======================
	 * - throws error if NOTREADY
	 * - sets all LED on whose bit is set in data

	void writeLEDs(uint8_t data);

	*
	 * \brief detects if the key was pressed
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
	 * - throws error if NOTREADY
	 * - check if new report is available (any input pin has changed)
	 * - returns true if pressed SW1 is detected, otherwise returns false

	bool keyPressed(void);

	*
	 * \brief Prints the current state of IOWarrior instance.
	 *
	 * functional description
	 * ======================
	 * prints m_lastError and m_state both as strings and numerical values on
	 * the console

	void printState(void);

	*
	 * \brief delivers current state of IOWarrior object
	 * \return state as numerical value

	STATES getState(void);

	*
	 * \brief delivers current state of IOWarrior object as a string
	 * \return state as string

	string getStateStr(void);

	*
	 * \brief delivers last error of IOWarrior instance
	 * \return error number

	ERRORS getLastError(void);

	*
	 * \brief delivers last error of IOWarrior instance
	 * \return last error text

	string getLastErrorStr(void);

	*
	 * \brief prints device info on console
	 *
	 * prints the infos of CIOWarrior:printDeviceInfo() plus a message that the
	 * extension is available
	 *
	 * \exception
	 * - device not ready
	 *
	 *
	 * functional description
	 * ======================
	 * - throws error if NOTREADY
	 * - reads serial number, Product revision and API version
	 * - prints read data on console

	void printDeviceInfo(void);

	//  ************************** Extension board ****************
	// < ----------- SAMPLE-END ------------>
protected:
	// todo define the protected methods according to the UML class diagram here
	// < ---------- SAMPLE-START ----------->
	*
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

	void writeReportOut(void);
	// < ----------- SAMPLE-END ------------>
};
#endif  CIOWARRIOR_H_ */
