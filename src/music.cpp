/*
filename  music.cpp

MUSIC类的定义
*/

#include "ege_head.h"
#include "mmsystem.h"
//#include <Digitalv.h>
typedef struct
{
	DWORD_PTR   dwCallback;
	DWORD   dwItem;
	DWORD   dwValue;
	DWORD   dwOver;
	LPSTR   lpstrAlgorithm;
	LPSTR   lpstrQuality;
} MCI_DGV_SETAUDIO_PARMSA;

typedef struct
{
	DWORD_PTR   dwCallback;
	DWORD   dwItem;
	DWORD   dwValue;
	DWORD   dwOver;
	LPWSTR  lpstrAlgorithm;
	LPWSTR  lpstrQuality;
} MCI_DGV_SETAUDIO_PARMSW;

#ifdef UNICODE
typedef MCI_DGV_SETAUDIO_PARMSW MCI_DGV_SETAUDIO_PARMS;
#else
typedef MCI_DGV_SETAUDIO_PARMSA MCI_DGV_SETAUDIO_PARMS;
#endif // UNICODE
#ifndef MCI_DGV_SETAUDIO_VOLUME
#define MCI_DGV_SETAUDIO_ITEM               0x00800000L
#define MCI_DGV_SETAUDIO_VALUE              0x01000000L

#define MCI_DGV_SETAUDIO_VOLUME             0x00004002L
#define MCI_SETAUDIO                        0x0873
#endif
// end of Digitalv.h

namespace ege
{

// Class MUSIC Construction
MUSIC::MUSIC()
{
	m_DID = MUSIC_ERROR;
	m_dwCallBack = 0;
}

// Class MUSIC Destruction
MUSIC::~MUSIC()
{
	if ( m_DID != MUSIC_ERROR) {
		Close();
	}
}

// open a music file. szStr: Path of the file
DWORD MUSIC::OpenFile(LPCSTR _szStr)
{
	MCIERROR        mciERR  = ERROR_SUCCESS;
	MCI_OPEN_PARMSA mci_p   = {0};

	mci_p.lpstrElementName   = _szStr;
	mci_p.lpstrDeviceType    = NULL;
	mci_p.dwCallback         = (DWORD_PTR)m_dwCallBack;

	if (m_DID != MUSIC_ERROR)
		Close();

	mciERR = mciSendCommandA(
		0,
		MCI_OPEN,
		MCI_OPEN_SHAREABLE|MCI_NOTIFY|MCI_OPEN_ELEMENT,
		(DWORD_PTR)&mci_p);

	if (mciERR != ERROR_SUCCESS) {
		mciERR = mciSendCommandA(
			0,
			MCI_OPEN,
			MCI_NOTIFY|MCI_OPEN_ELEMENT,
			(DWORD_PTR)&mci_p);
	}

	if (mciERR == ERROR_SUCCESS) {
		m_DID = mci_p.wDeviceID;

		//
		// Set time format with milliseconds
		//
		{
			MCI_SET_PARMS mci_p = {0};
			mci_p.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			//DWORD dw =
			mciSendCommandW(
				m_DID,
				MCI_SET,
				MCI_NOTIFY|MCI_SET_TIME_FORMAT,
				(DWORD_PTR)&mci_p);
		}
	}

	return mciERR;
}

// open a music file. szStr: Path of the file
DWORD MUSIC::OpenFile(LPCWSTR _szStr)
{
	MCIERROR        mciERR  = ERROR_SUCCESS;
	MCI_OPEN_PARMSW mci_p   = {0};

	mci_p.lpstrElementName   = _szStr;
	mci_p.lpstrDeviceType    = NULL;
	mci_p.dwCallback         = (DWORD_PTR)m_dwCallBack;

	if (m_DID != MUSIC_ERROR)
		Close();

	mciERR = mciSendCommandW(
		0,
		MCI_OPEN,
		MCI_OPEN_SHAREABLE|MCI_NOTIFY|MCI_OPEN_ELEMENT,
		(DWORD_PTR)&mci_p);

	if (mciERR != ERROR_SUCCESS) {
		mciERR = mciSendCommandW(
			0,
			MCI_OPEN,
			MCI_NOTIFY|MCI_OPEN_ELEMENT,
			(DWORD_PTR)&mci_p);
	}

	if (mciERR == ERROR_SUCCESS) {
		m_DID = mci_p.wDeviceID;

		//
		// Set time format with milliseconds
		//
		{
			MCI_SET_PARMS mci_p = {0};
			mci_p.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			//DWORD dw =
			mciSendCommandW(
				m_DID,
				MCI_SET,
				MCI_NOTIFY|MCI_SET_TIME_FORMAT,
				(DWORD_PTR)&mci_p);
		}
	}

	return mciERR;
}

// mciPlay(DWORD dwFrom, DWORD dwTo, DWORD dwCallBack)
// play the music stream.
DWORD MUSIC::Play(DWORD dwFrom, DWORD dwTo)
{
	ASSERT_TRUE(m_DID);
	MCIERROR        mciERR  = ERROR_SUCCESS;
	MCI_PLAY_PARMS  mci_p   = {0};
	DWORD           dwFlag  = MCI_NOTIFY;

	mci_p.dwFrom        = dwFrom;
	mci_p.dwTo          = dwTo;
	mci_p.dwCallback    = (DWORD_PTR)m_dwCallBack;

	if (dwFrom != MUSIC_ERROR)
		dwFlag |= MCI_FROM;

	if (dwTo   != MUSIC_ERROR)
		dwFlag |= MCI_TO;

	mciERR = mciSendCommandW(
		m_DID,
		MCI_PLAY,
		dwFlag,
		(DWORD_PTR)&mci_p);

	Sleep(1);

	return mciERR;
}

// pause the music stream.
DWORD MUSIC::Pause()
{
	ASSERT_TRUE(m_DID);
	MCIERROR            mciERR  = ERROR_SUCCESS;
	MCI_GENERIC_PARMS   mci_p   = {0};

	mci_p.dwCallback = (DWORD_PTR)m_dwCallBack;

	mciERR = mciSendCommandW(
		m_DID,
		MCI_PAUSE,
		MCI_NOTIFY,
		(DWORD_PTR)&mci_p);

	return mciERR;
}

// stop the music stream.
DWORD MUSIC::Stop()
{
	ASSERT_TRUE(m_DID);
	MCIERROR            mciERR  = ERROR_SUCCESS;
	MCI_GENERIC_PARMS   mci_p   = {0};

	mci_p.dwCallback = (DWORD_PTR)m_dwCallBack;

	mciERR = mciSendCommandW(
		m_DID,
		MCI_STOP,
		MCI_NOTIFY,
		(DWORD_PTR)&mci_p);

	return mciERR;
}

DWORD MUSIC::SetVolume(float value)
{
	ASSERT_TRUE(m_DID);
	MCIERROR                mciERR  = ERROR_SUCCESS;
	MCI_DGV_SETAUDIO_PARMSW mci_p   = {0};
	mci_p.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mci_p.dwValue = (DWORD)(value * 1000); //此处就是音量大小 (0--1000)

	mciERR = mciSendCommandW(
		m_DID,
		MCI_SETAUDIO,
		MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM,
		(DWORD_PTR)&mci_p);

	return mciERR;
}

// seek the music stream playposition to `dwTo`
DWORD MUSIC::Seek(DWORD dwTo)
{
	ASSERT_TRUE(m_DID);
	MCIERROR        mciERR  = ERROR_SUCCESS;
	MCI_SEEK_PARMS  mci_p   = {0};

	mci_p.dwCallback    = (DWORD_PTR)m_dwCallBack;
	mci_p.dwTo          = dwTo;

	mciERR = mciSendCommandW(
		m_DID,
		MCI_SEEK,
		MCI_NOTIFY,
		(DWORD_PTR)&mci_p);

	return mciERR;
}

// close the music stream.
DWORD MUSIC::Close()
{
	if(m_DID != MUSIC_ERROR) {
		MCIERROR mciERR = ERROR_SUCCESS;
		MCI_GENERIC_PARMS mci_p = {0};

		mci_p.dwCallback = (DWORD_PTR)m_dwCallBack;

		mciERR = mciSendCommandW(
			m_DID,
			MCI_CLOSE,
			MCI_NOTIFY,
			(DWORD_PTR)&mci_p);

		m_DID = MUSIC_ERROR;
		return mciERR;
	} else {
		return ERROR_SUCCESS;
	}
}

// get the playing position. return by milliseconds
DWORD MUSIC::GetPosition()
{
	ASSERT_TRUE(m_DID);
	MCI_STATUS_PARMS mci_p = {0};

	mci_p.dwCallback    = (DWORD_PTR)m_dwCallBack;
	mci_p.dwItem        = MCI_STATUS_POSITION;

	mciSendCommandW(
		m_DID,
		MCI_STATUS,
		MCI_NOTIFY|MCI_STATUS_ITEM,
		(DWORD_PTR)&mci_p);

	return (DWORD)mci_p.dwReturn;
}

// get the length of the music stream. return by milliseconds
DWORD MUSIC::GetLength()
{
	ASSERT_TRUE(m_DID);
	MCI_STATUS_PARMS mci_p = {0};

	mci_p.dwCallback    = (DWORD_PTR)m_dwCallBack;
	mci_p.dwItem        = MCI_STATUS_LENGTH;

	mciSendCommandW(
		m_DID,
		MCI_STATUS,
		MCI_NOTIFY|MCI_STATUS_ITEM,
		(DWORD_PTR)&mci_p);

	return (DWORD)mci_p.dwReturn;
}

DWORD MUSIC::GetPlayStatus()
{
	ASSERT_TRUE(m_DID);
	MCI_STATUS_PARMS mci_p = {0};

	mci_p.dwCallback    = (DWORD_PTR)m_dwCallBack;
	mci_p.dwItem        = MCI_STATUS_MODE;

	mciSendCommandW(
		m_DID,
		MCI_STATUS,
		MCI_NOTIFY|MCI_STATUS_ITEM,
		(DWORD_PTR)&mci_p);

	return (DWORD)mci_p.dwReturn;
}

} // namespace ege
