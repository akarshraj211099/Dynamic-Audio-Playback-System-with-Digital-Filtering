/**
 * \file CIOWarriorExt.h
 *
 * \brief Interface for CIoWarriorExt
 * \date 05.10.21
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */
#ifndef CIOWARRIOREXT_H_
#define CIOWARRIOREXT_H_

#include <SKSLib.h>
#include "CIOWarrior.h"

class CIOWarriorExt: public CIOWarrior {
public:
	/**
	 * bitmasks of the buttons SW1 ... SW6 on the extension board
	 *
	 * to be used by readButtons()
	 *
	 * these values are not corresponding with the pin numbers of the buttons
	 * because they are not linear connected (see scheme of extension board)
	 */
	enum BUTTONS {
		BTN_1 = 0x1,
		BTN_2 = 0x2,
		BTN_3 = 0x4,
		BTN_4 = 0x8,
		BTN_5 = 0x10,
		BTN_6 = 0x20,
		/**
		 * masking all buttons
		 */
		BTN_ALL = 0x3f
	};
	/**
	 * \brief initializes IoWExtension
	 */
	CIOWarriorExt();
	/**
	 * \brief deinitializes IoWExtension
	 */
	~CIOWarriorExt();
	/**
	 * \brief opens IoW with extension
	 *
	 * calls detect() => throwing error if no extension is detected
	 *
	 * \exception
	 * - device not ready
	 */
	void open(void);
	/**
	 * \brief prints device info
	 *
	 * prints hint that extension is available and all infos of
	 * CIOWarrior::printDeviceInfo()
	 */
	void printDeviceInfo(void);
	/**
	 * \brief reads status of 6 custom buttons on extension board at port 0
	 *
	 * the state of the buttons is returned in Bit 0 (SW1) to Bit 5 (Sw6)
	 *
	 * a new report is only generated, if a change occurs at the input pins
	 * \see readButton()
	 *
	 * \return state of custom buttons SW1 ... SW6
	 * 1 if a transition on the pin from released (high) to pressed (low)
	 *
	 * \exception
	 * - device not ready
	 */
	uint8_t readButtons(uint8_t mask = CIOWarriorExt::BTN_ALL);
	/**
	 * \brief writes binary pattern on the LEDs D1 ... D16
	 *
	 * the pattern is high active
	 *
	 * \param value binary pattern
	 * \exception
	 * - device not ready
	 */
	void writeLEDs(uint16_t value);
private:
	/**
	 * \brief detects extension
	 * \exception
	 * - device not ready
	 * - extension not available
	 */
	void detect(void);
};
#endif /* CIOWARRIOREXT_H_ */
