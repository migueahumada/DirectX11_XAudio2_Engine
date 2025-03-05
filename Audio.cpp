#include "Audio.h"

HRESULT Audio::decodeOLD(const WCHAR* filename) {
	
	//crea el handle del file para leerlo
	HANDLE hFile = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING,
		0,
		NULL);

	//Si el handle no existe pues vale queso
	if (hFile == INVALID_HANDLE_VALUE)
		return HRESULT_FROM_WIN32(GetLastError());

	//Le dice al puntero que se ponga al principio, si no es válido, entonces adios.
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	//Creamos dos variables de 4 bytes sin signo, una de tamaño y otra de posición.
	DWORD dwChunkSize;
	DWORD dwChunkPosition;

	findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);

	DWORD filetype;

	readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	findChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	readChunkData(hFile, &m_waveFile, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	findChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	unsigned char* pDataBuffer = new unsigned char[dwChunkSize];
	readChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	m_buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	m_buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	m_buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	return S_OK;
}

void Audio::load(const char* filename)
{
	fstream file(filename, std::ios::in | std::ios::beg | std::ios::binary);
	if (!file.is_open())
		return;

	unsigned long temp_buffer;

	file.read(reinterpret_cast<char*>(&temp_buffer), sizeof(unsigned long));
	if (temp_buffer != fourccRIFF)
		return;

	file.read(reinterpret_cast<char*>(&temp_buffer), sizeof(unsigned long));

	file.read(reinterpret_cast<char*>(&temp_buffer), sizeof(unsigned long));
	if (temp_buffer != fourccWAVE)
		return;

	while (file.peek() != EOF)
	{
		unsigned long chunkID;
		unsigned long chunkSize;
		
		//Read chunk ID
		file.read(reinterpret_cast<char*>(&chunkID), sizeof(unsigned long));

		file.read(reinterpret_cast<char*>(&chunkSize), sizeof(unsigned long));

		switch (chunkID)
		{
			case fourccFMT:

				file.read(reinterpret_cast<char*>(&m_waveFile), chunkSize);
				break;
			case fourccDATA:
				m_pDataBuffer = new unsigned char[chunkSize];
				file.read(reinterpret_cast<char*>(m_pDataBuffer), chunkSize);
				m_buffer.pAudioData = m_pDataBuffer;
				m_buffer.AudioBytes = chunkSize;
				break;

		default:

			file.seekg(chunkSize, std::ios::cur);
			break;
		}

	}

	m_buffer.Flags = XAUDIO2_END_OF_STREAM;

	file.close();
}


void Audio::encode(const char* filename)
{
	// TODO: Terminar este pedo
	fstream file(filename, std::ios::out | std::ios::beg | std::ios::binary);
	if (!file.is_open())
		return;

	int duration = 10;

	WAVEFORMATEXTENSIBLE wavefile;
	wavefile.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wavefile.Format.nChannels = 2;
	wavefile.Format.nSamplesPerSec = 48000;
	wavefile.Format.wBitsPerSample = 24;
	wavefile.Format.nBlockAlign = wavefile.Format.nChannels * wavefile.Format.wBitsPerSample / 8;
	wavefile.Format.nAvgBytesPerSec = wavefile.Format.nBlockAlign * wavefile.Format.nSamplesPerSec;
	wavefile.Format.cbSize = 22;

	wavefile.Samples.wValidBitsPerSample = 24;
	wavefile.Samples.wSamplesPerBlock = 343;
	wavefile.Samples.wReserved = 0;
	wavefile.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
	wavefile.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

	file.write(reinterpret_cast<char*>(&wavefile), sizeof(wavefile));


	file.close();
}

HRESULT Audio::findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		
		DWORD dwRead;
		if (!ReadFile(hFile,&dwChunkType,sizeof(DWORD),&dwRead,nullptr)) // 4 bytes
			hr = HRESULT_FROM_WIN32(GetLastError());
		
		if (!ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr)) // 4 bytes
			hr = HRESULT_FROM_WIN32(GetLastError());
		
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (!ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr)) // 4 bytes
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				return HRESULT_FROM_WIN32(GetLastError());
			break;
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}

HRESULT Audio::readChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (SetFilePointer(hFile, bufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (!ReadFile(hFile, buffer, buffersize, &dwRead, nullptr))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
