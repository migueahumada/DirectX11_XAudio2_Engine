#include "AudioAPI.h"

AudioAPI::AudioAPI(void* handleWindow) 
{
	HWND hwnd = static_cast<HWND>(handleWindow);

	UINT flags = 0;

#if defined (_DEBUG)
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif // defined (_DEBUG)


	HRESULT result = XAudio2Create(&m_pXAudio2, flags);
	if (FAILED(result))
	{
		MessageBox(hwnd,L"XAudio couldn't be created", L"Error",MB_OK);
		return;
	}
	result = m_pXAudio2->StartEngine();
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Failed to initialize XAudio2 engine", L"Error", MB_OK);
		return;
	}

	result = m_pXAudio2->CreateMasteringVoice(	&m_pMainMasteringVoice,
												XAUDIO2_DEFAULT_CHANNELS,
												XAUDIO2_DEFAULT_SAMPLERATE);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create Main MasteringVoice", L"Error", MB_OK);
		return;
	}
}

AudioAPI::~AudioAPI()
{
	SAFE_RELEASE(m_pXAudio2);
}

SPtr<Audio> AudioAPI::CreateSoundEffect(const String& name, const String& filepath)
{
	SPtr<Audio> pSoundEffect = make_shared<Audio>(name, filepath);
	if (!pSoundEffect)
	{
		MessageBox(nullptr, L"Failed to create Sound Effect", L"Error", MB_OK);
		return nullptr;
	}

	HRESULT hr = m_pXAudio2->CreateSourceVoice(&pSoundEffect->m_pSourceVoice,(WAVEFORMATEX*)&pSoundEffect->m_waveFile);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create Source Voice", L"Error", MB_OK);
		return nullptr;
	}

	pSoundEffect->m_pSourceVoice->SubmitSourceBuffer(&pSoundEffect->m_buffer);

	return pSoundEffect;
}

SPtr<Channel> AudioAPI::CreateChannel(SPtr<Audio> audio)
{
	SPtr<Channel> pChannel = make_shared<Channel>();

	HRESULT hr = m_pXAudio2->CreateSubmixVoice(	&pChannel->m_pXAudio2SubmixVoice, 
									pChannel->m_pAudioSource->m_waveFile.Format.nChannels,
									pChannel->m_pAudioSource->m_waveFile.Format.nSamplesPerSec);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create Channel", L"Error", MB_OK);
		return nullptr;
	}

	return pChannel;
}

void AudioAPI::Play(SPtr<Audio> audio, SPtr<Channel> channel)
{
	if (!audio || !channel)
	{
		return;
	}
	

	channel->m_pAudioSource->m_pSourceVoice->Start(0);

}

void AudioAPI::Play(SPtr<Audio> audio)
{
	if (!audio)
	{
		return;
	}

	audio->m_pSourceVoice->Start(0);

}


