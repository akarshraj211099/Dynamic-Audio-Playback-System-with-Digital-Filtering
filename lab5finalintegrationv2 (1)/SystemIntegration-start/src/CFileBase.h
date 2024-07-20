/**
 * \file CFileBase.h
 *
 * \brief interface class CFileBase
 * \date 17.11.2023
 *
 * \see File
 */
#ifndef FILEBASE_H_
#define FILEBASE_H_

#include <string>
using namespace std;

/**
 * base class for files contains common properties and common behavior for
 * all files
 */

class CFileBase {
public:
	enum FILEMODES {
		FILE_MODEUNKNOWN = 0x00,
		FILE_READ = 0x01,
		FILE_WRITE = 0x02,
		FILE_WRITEAPPEND = 0x04
	};
	enum FILEERRORS {
		FILE_E_UNKNOWNOPENMODE,
		FILE_E_NOFILE,
		FILE_E_FILENOTOPEN,
		FILE_E_NOBUFFER,
		FILE_E_READ,
		FILE_E_CANTREAD,
		FILE_E_CANTWRITE,
		FILE_E_WRITE,
		FILE_E_SPECIAL
	};

private:
	unsigned int m_mode;	// file mode

protected:
	string m_path;			// complete file path

public:
	CFileBase(string path, unsigned int mode = FILE_MODEUNKNOWN);
	virtual ~CFileBase();

	/**
	 * prints content of the file on console
	 */
	virtual void print(void);

protected:
	/**
	 * Methods for the retrieval of file mode
	 */
	bool isFileW();
	bool isFileR();
	bool isFileWA();
	/**
	 * Methods for conversion of enum values into text
	 */
	string getErrorTxt(FILEERRORS err);
	string getModeTxt();
};
#endif /* FILEBASE_H_ */
