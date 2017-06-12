#include "../include/AudioEngineLoader.h"

AudioEngineLoader::AudioEngineLoader (HINSTANCE _instance): m_Instance (_instance) {
    m_AudioEngine = NULL;
    m_DLL = NULL;
    m_Log = NULL;
}

AudioEngineLoader::~AudioEngineLoader () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("AudioEngineLoader is shutting down...\n");
    }
    #endif
    if (m_AudioEngine) {
        delete m_AudioEngine;
        m_AudioEngine = NULL;
    }    
    if (m_DLL) {
        if (!FreeLibrary (m_DLL)) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: attempt to free AudioEngine.dll failed.\n");
            }
            #endif
        }
    }
}

void AudioEngineLoader::EnableLog (LogManager* _log) {
    m_Log = _log;
}

// loads rendering dll and inits m_Device
void AudioEngineLoader::CreateAudioEngine () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Creating device...\n");
    }
    #endif
    m_DLL = LoadLibrary ("AudioEngine.dll");
    if (!m_DLL) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot open AudioEngine.dll.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "LoadLibrary() failed.");
    }
    // gets function pointer
    CREATEAUDIOENGINE CreateAudioEngine = 
        (CREATEAUDIOENGINE) GetProcAddress (m_DLL, "CreateAudioEngine");
    if (!CreateAudioEngine) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot find function CreateAudioEngine () in the DLL.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetProcAddress() failed.");
    }
    
    HRESULT hr = CreateAudioEngine (m_Instance, &m_AudioEngine);
    if (FAILED (hr)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot create audio engine.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_OUT_OF_MEM, "CreateAudioEngine() failed.");
    }
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Device created.\n");
    }
    #endif
}

// returns m_Device
IAudioEngine* AudioEngineLoader::GetAudioEngine () const {
    return m_AudioEngine;
}

// returns handle to a rendering dll
HINSTANCE AudioEngineLoader::GetModule () const {
    return m_DLL;
}


// releases m_Device
void AudioEngineLoader::Release () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Releasing audio engine...\n");
    }
    #endif
    // gets function pointer
    RELEASEAUDIOENGINE ReleaseAudioEngine = NULL;
    if (m_DLL) {
        ReleaseAudioEngine = 
            (RELEASEAUDIOENGINE) GetProcAddress (m_DLL, "ReleaseAudioEngine");
    }
    if (!ReleaseAudioEngine) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot find function ReleaseAudioEngine in the DLL.\n");
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "GetProcAddress() failed.");
    }
    if (m_AudioEngine) {
        ReleaseAudioEngine (&m_AudioEngine);
    }
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Device released.\n");
    }
    #endif
}