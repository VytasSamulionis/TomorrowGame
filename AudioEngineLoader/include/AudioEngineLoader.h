/** @file AudioEngineLoader.h */
#pragma once

#include "../include/AudioEngine.h"
#include "../include/Log.h"

/** Loads the audio engine and returns its interface. */
class AudioEngineLoader {
public:
    /** Constructor.
    @param[in] _instance WinMain instance */
    AudioEngineLoader (HINSTANCE _instance);

    /** Destructor. */
    ~AudioEngineLoader ();

    /** Enables log.
    Pass NULL as a parameter to disable the log.
    @param[in] _log pointer to the LogManager */
    void EnableLog (LogManager* _log);

    /** Creates AudioEngine interface.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL the call to the windows API function failed */
    void CreateAudioEngine ();

    /** Returns AudioEngine interface.
    @return AudioEngine interface */
    IAudioEngine* GetAudioEngine () const;

    /** Returns DLL handle.
    @return DLL handle */
    HINSTANCE GetModule () const;
    
    /** Releases resources.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL the call to the windows API function failed */
    void Release ();

private:
    IAudioEngine* m_AudioEngine;     /**< Interface class. */
    HINSTANCE m_Instance;           /**< WinMain instance. */
    HMODULE m_DLL;                  /**< DLL instance. */

    LogManager* m_Log;              /**< Log manager. */
};