#pragma once

#ifdef _XBOX //Big-Endian
	#define fourccRIFF 'RIFF'
	#define fourccDATA 'data'
	#define fourccFMT 'fmt '
	#define fourccWAVE 'WAVE'
	#define fourccXWMA 'XWMA'
	#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
	#define fourccRIFF 'FFIR'
	#define fourccDATA 'atad'
	#define fourccFMT ' tmf'
	#define fourccWAVE 'EVAW'
	#define fourccXWMA 'AMWX'
	#define fourccDPDS 'sdpd'
#endif

#include <xaudio2.h>
#include <fstream>

using std::fstream;

class Audio
{
public:
	Audio() = default;
	~Audio() {
		if (m_pDataBuffer)
		{
			delete[] m_pDataBuffer;
			m_pDataBuffer = nullptr;
		}
		
	}

	//Load a wave file to a XAUDIO2BUFFER struct
	void load(const char* filename);
	
	void encode(const char* filename);
	
	WAVEFORMATEXTENSIBLE m_waveFile{ 0 };
	XAUDIO2_BUFFER m_buffer{ 0 };

protected:

	unsigned char* m_pDataBuffer = nullptr;
};

