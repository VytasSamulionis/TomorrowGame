#include "../include/AudioEngine.h"

AudioEngine::AudioEngine (HINSTANCE _dll) {
    m_DLL = _dll;
    m_GlobalSettingsData = NULL;
    m_Listener = NULL;
    m_Emitter = NULL;
    if (FAILED (CoInitialize (NULL))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CoInitialize() failure.");
    }
    if (FAILED (XACT3CreateEngine (0, &m_AudioEngine))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "XACT3CreateEngine() failure.");
    }
}

AudioEngine::~AudioEngine () {
    /* Shutdown the audio engine. */
    if (m_AudioEngine) {
        m_AudioEngine->ShutDown ();
        m_AudioEngine->Release ();
        m_AudioEngine = NULL;
    }
    std::vector<AudioBank>::iterator i;
    for (i = m_AudioBanks.begin (); i != m_AudioBanks.end (); i++) {
        if (i->WaveData) {
            UnmapViewOfFile (i->WaveData);
        }
        if (i->SoundData) {
            delete[] i->SoundData;
        }
    }
    delete[] m_GlobalSettingsData;
    delete[] m_Listener;
    delete[] m_Emitter;
    m_AudioBanks.clear ();
    m_Sounds.clear ();
}

void AudioEngine::Initialize (const char* _globalSettings) {
    bool isSucceeded = false;
    DWORD dwBytesRead;
    HANDLE file = CreateFile(
        _globalSettings, 
        GENERIC_READ, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_EXISTING, 
        0, 
        NULL);
    DWORD globalSettingsFileSize = 0;
    if (file) {
        globalSettingsFileSize = GetFileSize(file, NULL);
        if (globalSettingsFileSize != INVALID_FILE_SIZE) {
            try {
                m_GlobalSettingsData = new BYTE[globalSettingsFileSize];
            } catch (std::bad_alloc) {
                THROW_ERROR (ERRC_OUT_OF_MEM);
            }
            if (ReadFile (file, m_GlobalSettingsData, globalSettingsFileSize, &dwBytesRead, NULL) != 0) {
                isSucceeded = true;
            }
        }
        CloseHandle (file);
    }
    if (!isSucceeded) {
        if(m_GlobalSettingsData) {
            delete[] m_GlobalSettingsData;
            m_GlobalSettingsData = NULL;
        }
        globalSettingsFileSize = 0;
    }
    XACT_RUNTIME_PARAMETERS params = {0};
    params.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    params.pGlobalSettingsBuffer = m_GlobalSettingsData;
    params.globalSettingsBufferSize = globalSettingsFileSize;
    if (FAILED (m_AudioEngine->Initialize (&params))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Audio engine's Initialize() failure.");
    }
    if (FAILED (XACT3DInitialize (m_AudioEngine, m_XACT3DInstance))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "3D audio engine's Initialize() failure.");
    }
    ZeroMemory (&m_DspSettings, sizeof (m_DspSettings));

    WAVEFORMATEXTENSIBLE format;
    m_AudioEngine->GetFinalMixFormat(&format);

    m_DspSettings.SrcChannelCount = 2;
    m_DspSettings.DstChannelCount = format.Format.nChannels;
    m_DspSettings.pMatrixCoefficients = new FLOAT32[m_DspSettings.SrcChannelCount * m_DspSettings.DstChannelCount];

    try {
        m_Listener = new X3DAUDIO_LISTENER;
        m_Emitter = new X3DAUDIO_EMITTER;
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    ZeroMemory (m_Emitter, sizeof (X3DAUDIO_EMITTER));
    ZeroMemory (m_Listener, sizeof (X3DAUDIO_LISTENER));

    X3DAUDIO_VECTOR front, top, zero;
    front.x = 0.0f; front.y = 0.0f; front.z = 1.0f;
    top.x = 0.0f; top.y = 1.0f; top.z = 0.0f;
    zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;
    m_Listener->OrientFront = front;
    m_Listener->OrientTop = top;
    m_Listener->Position = zero;
    m_Listener->Velocity = zero;

    m_Emitter->OrientFront = front;
    m_Emitter->OrientTop = top;
    m_Emitter->Position = zero;
    m_Emitter->Velocity = zero;
    m_Emitter->ChannelCount = 2;
    m_Emitter->CurveDistanceScaler = 1.0;
    m_Emitter->DopplerScaler = 1.0f;
}

UINT AudioEngine::LoadInMemory (const char* _soundBankFile, const char* _waveBankFile) {
    try {
        m_AudioBanks.push_back (AudioBank ());
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    AudioBank& audioBank = m_AudioBanks.back (); /* Get the reference to the last element. */
    audioBank.IsReady = false;

    /* Setup wave bank. */
    HANDLE waveFile = CreateFile (
        _waveBankFile, 
        GENERIC_READ, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_EXISTING, 
        0, 
        NULL);
    HRESULT hr = E_FAIL;
    if (waveFile != INVALID_HANDLE_VALUE) {
        DWORD fileSize = GetFileSize (waveFile, NULL);
        if (fileSize != -1) {
            HANDLE fileMap = CreateFileMapping (waveFile, NULL, PAGE_READONLY, 0, fileSize, 0);
            if (fileMap) {
                audioBank.WaveData = MapViewOfFile (fileMap, FILE_MAP_READ, 0, 0, 0);
                if (audioBank.WaveData) {
                    hr = m_AudioEngine->CreateInMemoryWaveBank (audioBank.WaveData, fileSize, 0, 0, &audioBank.WaveBank);
                }
                CloseHandle (fileMap);
            } else {
                THROW_DETAILED_ERROR (ERRC_BAD_FILE, _waveBankFile);
            }
        }
        CloseHandle (waveFile);
    } else {
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _waveBankFile);
    }
    if (FAILED(hr)) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateInMemoryWaveBank() failure.");
    }

    /* Setup sound bank. */
    hr = E_FAIL;
    HANDLE soundFile = CreateFile (_soundBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (soundFile != INVALID_HANDLE_VALUE) {
        DWORD fileSize = GetFileSize (soundFile, NULL);
        if (fileSize != -1) {
            try {
                audioBank.SoundData = new BYTE[fileSize];
            } catch (std::bad_alloc) {
                THROW_ERROR (ERRC_OUT_OF_MEM);
            }
            DWORD bytesRead;
            if (ReadFile (soundFile, audioBank.SoundData, fileSize, &bytesRead, NULL)) {
                hr = m_AudioEngine->CreateSoundBank (audioBank.SoundData, fileSize, 0, 0, &audioBank.SoundBank);
            }
        }
        CloseHandle (soundFile);
    } else {
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _soundBankFile);
    }
    if (FAILED (hr)) {
        char msg[MAX_PATH];
        sprintf (msg, "Cannot setup sound bank %s", _soundBankFile);
        THROW_DETAILED_ERROR (ERRC_BAD_FILE, msg);
    }
    audioBank.IsReady = true;
    return m_AudioBanks.size() - 1;
}

UINT AudioEngine::LoadStream (const char* _soundBankFile, const char* _waveBankFile) {
    try {
        m_AudioBanks.push_back (AudioBank ());
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    AudioBank& audioBank = m_AudioBanks.back (); /* Get the reference to the last element. */
    audioBank.IsReady = false;

    /* Setup wave bank. */
    audioBank.WaveData = CreateFile (
        _waveBankFile, 
        GENERIC_READ, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, 
        NULL);
    HRESULT hr = E_FAIL;
    if (audioBank.WaveData != INVALID_HANDLE_VALUE) {
        XACT_WAVEBANK_STREAMING_PARAMETERS params;
        params.file = audioBank.WaveData;
        params.offset = 0;
        params.packetSize = 64;
        params.flags = 0;
        hr = m_AudioEngine->CreateStreamingWaveBank (&params, &audioBank.WaveBank);
    } else {
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _waveBankFile);
    }
    if (FAILED(hr)) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateStreamingWaveBank() failure.");
    }

    /* Setup sound bank. */
    hr = E_FAIL;
    HANDLE soundFile = CreateFile (_soundBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (soundFile != INVALID_HANDLE_VALUE) {
        DWORD fileSize = GetFileSize (soundFile, NULL);
        if (fileSize != -1) {
            try {
                audioBank.SoundData = new BYTE[fileSize];
            } catch (std::bad_alloc) {
                THROW_ERROR (ERRC_OUT_OF_MEM);
            }
            DWORD bytesRead;
            if (ReadFile (soundFile, audioBank.SoundData, fileSize, &bytesRead, NULL)) {
                hr = m_AudioEngine->CreateSoundBank (audioBank.SoundData, fileSize, 0, 0, &audioBank.SoundBank);
            }
        }
        CloseHandle (soundFile);
    } else {
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _soundBankFile);
    }
    if (FAILED (hr)) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "CreateSoundBank() failure.");
    }
    audioBank.IsReady = true;
    return m_AudioBanks.size() - 1;
}

UINT AudioEngine::Prepare (UINT _id, const char* _soundName) {
    if (_id >= m_AudioBanks.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    XACTINDEX index = m_AudioBanks[_id].SoundBank->GetCueIndex (_soundName);
    IXACT3Cue* cue;
    if (FAILED (m_AudioBanks[_id].SoundBank->Prepare (index, 0, 0, &cue))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Prepare() failure.");
    }
    m_Sounds.push_back (cue);
    return m_Sounds.size() - 1;
}

UINT AudioEngine::Play (UINT _id, const char* _soundName) {
    if (_id >= m_AudioBanks.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    XACTINDEX index = m_AudioBanks[_id].SoundBank->GetCueIndex (_soundName);
    IXACT3Cue* cue;
    if (FAILED (m_AudioBanks[_id].SoundBank->Play (index, 0, 0, &cue))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Play() failure.");
    }
    m_Sounds.push_back (cue);
    return m_Sounds.size() - 1;
}

UINT AudioEngine::Play3D (UINT _id, const char* _soundName, const VECTOR3& _position, const VECTOR3& _front, const VECTOR3& _top) {
    if (_id >= m_AudioBanks.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    UINT soundId = Prepare (_id, _soundName);
    m_Emitter->Position = (*(X3DAUDIO_VECTOR*)_position.data());
    m_Emitter->OrientFront = (*(X3DAUDIO_VECTOR*)_front.data());
    m_Emitter->OrientTop = (*(X3DAUDIO_VECTOR*)_top.data());
    XACT3DCalculate (m_XACT3DInstance, m_Listener, m_Emitter, &m_DspSettings);
    XACT3DApply (&m_DspSettings, m_Sounds[soundId]);
    if (FAILED (m_Sounds[soundId]->Play ())) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "Play() failure.");
    }
    return soundId;
}

void AudioEngine::Update3DSoundPosition (UINT _id, const VECTOR3& _position) {
    if (_id >= m_Sounds.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Emitter->Position = (*(X3DAUDIO_VECTOR*)_position.data());
    XACT3DCalculate (m_XACT3DInstance, m_Listener, m_Emitter, &m_DspSettings);
    XACT3DApply (&m_DspSettings, m_Sounds[_id]);
}

void AudioEngine::Update3DSoundFront (UINT _id, const VECTOR3& _front) {
    if (_id >= m_Sounds.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Emitter->OrientFront = (*(X3DAUDIO_VECTOR*)_front.data());
    XACT3DCalculate (m_XACT3DInstance, m_Listener, m_Emitter, &m_DspSettings);
    XACT3DApply (&m_DspSettings, m_Sounds[_id]);
}

void AudioEngine::Update3DSoundTop (UINT _id, const VECTOR3& _top) {
    if (_id >= m_Sounds.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Emitter->OrientTop = (*(X3DAUDIO_VECTOR*)_top.data());
    XACT3DCalculate (m_XACT3DInstance, m_Listener, m_Emitter, &m_DspSettings);
    XACT3DApply (&m_DspSettings, m_Sounds[_id]);
}

void AudioEngine::Stop (UINT _id) {
    if (_id >= m_Sounds.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Sounds[_id]->Stop (0);
}

void AudioEngine::Pause (UINT _id) {
    if (_id >= m_Sounds.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Sounds[_id]->Pause (TRUE);
}

void AudioEngine::Unpause (UINT _id) {
    if (_id >= m_Sounds.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    m_Sounds[_id]->Pause (FALSE);
}

void AudioEngine::Update () {
    if (m_AudioEngine) {
        m_AudioEngine->DoWork ();
    }
}

void AudioEngine::SetListenerPosition (const VECTOR3& _position) {
    m_Listener->Position = (*(X3DAUDIO_VECTOR*)_position.data());
}

void AudioEngine::SetListenerFront (const VECTOR3& _front) {
    m_Listener->OrientFront = (*(X3DAUDIO_VECTOR*)_front.data());
    
}

void AudioEngine::SetListenerTop (const VECTOR3& _top) {
    m_Listener->OrientTop = (*(X3DAUDIO_VECTOR*)_top.data());
}

// creates audio engine interface
HRESULT CreateAudioEngine (HINSTANCE _instance, IAudioEngine** _interface) {
    if (*_interface) {
        delete *_interface;
        *_interface = NULL;
    }
    try {
        *_interface = new AudioEngine (_instance);
    } catch (std::bad_alloc) {
        return E_FAIL;
    }
    return S_OK;
}

// releases render device
void ReleaseAudioEngine (IAudioEngine** _interface) {
    if (*_interface) {
        delete *_interface;
        *_interface = NULL;
    }
}
