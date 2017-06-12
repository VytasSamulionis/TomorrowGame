/** @file AudioEngine.h
IAudioEngine interface implementation */

#pragma once

#include "../../AudioEngineLoader/include/AudioEngine.h"
#include <vector>
#include <xact3.h>
#include <xact3d3.h>

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif
#pragma comment (lib, "x3daudio.lib")

/** Structure to hold the audio information. */
struct AudioBank {
    bool IsReady;
    IXACT3WaveBank* WaveBank;       /**< Wave bank. */
    void* WaveData;                 /**< Wave bank's data. */
    IXACT3SoundBank* SoundBank;     /**< Sound bank. */
    void* SoundData;                /**< Sound bank's data. */
};

class AudioEngine: public IAudioEngine {
public:
    /** Constructor.
    @param[in] _dll DLL instance of the audio engine
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL */
    AudioEngine (HINSTANCE _dll);
    
    /** Destructor. */
    ~AudioEngine ();

    /** Initializes audio engine.
    @param[in] _globalSettings global settings file (*.xgs)
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    void Initialize (const char* _globalSettings);

    /** Loads the sound and wave bank files.
    Loaded sounds are streamed.
    @param[in] _soundBankFile sound bank file (*.xsb)
    @param[in] _waveBankFile wave bank file (*.xwb) 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_FILE_NOT_FOUND either sound or wave bank file does not exist
        - @c ERRC_BAD_FILE mapping the file had failed
        - @c ERRC_API_CALL 

    @return audio ID */
    UINT LoadInMemory (const char* _soundBankFile, const char* _waveBankFile);

    /** Loads the sound and wave bank files.
    Loaded sounds are streamed.
    @param[in] _soundBankFile sound bank file (*.xsb)
    @param[in] _waveBankFile wave bank file (*.xwb) 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_FILE_NOT_FOUND either sound or wave bank file does not exist
        - @c ERRC_BAD_FILE mapping the file had failed
        - @c ERRC_API_CALL 

    @return audio ID */
    UINT LoadStream (const char* _soundBankFile, const char* _waveBankFile);
    
    /** Prepares the sound.
    @param[in] _id audio ID
    @param[in] _soundName sound name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL

    @return sound ID */
    UINT Prepare (UINT _id, const char* _soundName);

    /** Plays the sound.
    @param[in] _id audio ID
    @param[in] _soundName sound name 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL

    @return sound ID */
    UINT Play (UINT _id, const char* _soundName);

    /** Plays 3D sound.
    @param[in] _id audio's ID
    @param[in] _soundName sound's name
    @param[in] _position sound's position
    @param[in] _front sound's front orientation
    @param[in] _top sound's top orientation
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL

    @return sound's ID */
    UINT Play3D (
        UINT _id, 
        const char* _soundName, 
        const VECTOR3& _position,
        const VECTOR3& _front,
        const VECTOR3& _top);

    /** Updates 3D sound's position.
    @param[in] _id sound's ID
    @param[in] _position sound's position
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    void Update3DSoundPosition (UINT _id, const VECTOR3& _position);

    /** Updates 3D sound's front orientation.
    @param[in] _id sound's ID
    @param[in] _front sound's front orientation
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    void Update3DSoundFront (UINT _id, const VECTOR3& _front);

    /** Updates 3D sound's top orientation.
    @param[in] _id sound's ID
    @param[in] _top sound's top orientation
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    void Update3DSoundTop (UINT _id, const VECTOR3& _top);

    /** Stops the sound.
    @param[in] _id sound ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    void Stop (UINT _id);

    /** Pauses the sound.
    @param[in] _id sound ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    void Pause (UINT _id);

    /** Unpauses the sound.
    @param[in] _id sound ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    void Unpause (UINT _id);

    /** Setter: listener's position.
    @param[in] _position listener's position */
    void SetListenerPosition (const VECTOR3& _position);

    /** Setter: listener's front orientation.
    @param[in] _position listener's front orientation */
    void SetListenerFront (const VECTOR3& _front);

    /** Setter: listener's top orientation.
    @param[in] _position listener's top orientation */
    void SetListenerTop (const VECTOR3& _top);

    /** Updates the engine.
    It must be called every frame. */
    void Update ();
private:
    HINSTANCE m_DLL;                        /**< DLL instance. */
    IXACT3Engine* m_AudioEngine;            /**< XACT audio engine. */
    void* m_GlobalSettingsData;             /**< Global settings data. */
    X3DAUDIO_LISTENER* m_Listener;          /**< Listener. */
    X3DAUDIO_EMITTER* m_Emitter;            /**< Emitter. */
    X3DAUDIO_HANDLE m_XACT3DInstance;       /**< 3D Audio instance. */
    X3DAUDIO_DSP_SETTINGS m_DspSettings;    /**< DSP Settings. */
    std::vector<AudioBank> m_AudioBanks;    /**< Audio banks. @see AudioBank */
    std::vector<IXACT3Cue*> m_Sounds;       /**< Sound cues. */
};

// exported functions
extern "C" __declspec (dllexport) HRESULT CreateAudioEngine (HINSTANCE _instance, IAudioEngine** _interface);
extern "C" __declspec (dllexport) void ReleaseAudioEngine (IAudioEngine** _interface);