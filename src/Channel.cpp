#include <Channel.h>

Channel::Channel()
{
}

Channel::~Channel()
{
	if (m_pXAudio2SubmixVoice)
	{
		m_pXAudio2SubmixVoice->DestroyVoice();
		m_pXAudio2SubmixVoice = nullptr;
	}
}


