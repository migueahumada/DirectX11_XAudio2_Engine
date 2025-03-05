#include "Audio.h"

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


