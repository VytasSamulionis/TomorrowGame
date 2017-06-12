/** @file AudioEngine.h
The audio engine's interface file */

#include <Windows.h>
#include "../include/ErrorMessage.h"
#include "../include/Engine.h"

class IAudioEngine {
public:
    /** Initializes audio engine.
    @param[in] _globalSettings global settings file (*.xgs) */
    virtual void Initialize (const char* _globalSettings) = 0;

    /** Loads the sound and wave bank files.
    Loaded sounds are in memory.
    @param[in] _soundBankFile sound bank file (*.xsb)
    @param[in] _waveBankFile wave bank file (*.xwb) 
    @return audio ID */
    virtual UINT LoadInMemory (const char* _soundBankFile, const char* _waveBankFile) = 0;

    /** Loads the sound and wave bank files.
    Loaded sounds are streamed.
    @param[in] _soundBankFile sound bank file (*.xsb)
    @param[in] _waveBankFile wave bank file (*.xwb) 
    @return audio ID */
    virtual UINT LoadStream (const char* _soundBankFile, const char* _waveBankFile) = 0;

    /** Prepares the sound.
    @param[in] _id audio ID
    @param[in] _soundName sound name
    @return sound ID*/
    virtual UINT Prepare (UINT _id, const char* _soundName) = 0;

    /** Plays the sound.
    @param[in] _id audio's ID
    @param[in] _soundName sound's name 
    @return sound's ID */
    virtual UINT Play (UINT _id, const char* _soundName) = 0;

    /** Plays 3D sound.
    @param[in] _id audio's ID
    @param[in] _soundName sound's name
    @param[in] _position sound's position
    @param[in] _front sound's front orientation
    @param[in] _top sound's top orientation
    @return sound's ID */
    virtual UINT Play3D (
        UINT _id, 
        const char* _soundName, 
        const VECTOR3& _position,
        const VECTOR3& _front,
        const VECTOR3& _top) = 0;

    /** Updates 3D sound's position.
    @param[in] _id sound's ID
    @param[in] _position sound's position
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    virtual void Update3DSoundPosition (UINT _id, const VECTOR3& _position) = 0;

    /** Updates 3D sound's front orientation.
    @param[in] _id sound's ID
    @param[in] _front sound's front orientation
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    virtual void Update3DSoundFront (UINT _id, const VECTOR3& _front) = 0;

    /** Updates 3D sound's top orientation.
    @param[in] _id sound's ID
    @param[in] _top sound's top orientation
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE */
    virtual void Update3DSoundTop (UINT _id, const VECTOR3& _top) = 0;

    /** Stops the sound.
    @param[in] _id sound ID */
    virtual void Stop (UINT _id) = 0;

    /** Pauses the sound.
    @param[in] _id sound ID */
    virtual void Pause (UINT _id) = 0;

    /** Unpauses the sound.
    @param[in] _id sound ID */
    virtual void Unpause (UINT _id) = 0;

    /** Setter: listener's position.
    @param[in] _position listener's position */
    virtual void SetListenerPosition (const VECTOR3& _position) = 0;

    /** Setter: listener's front orientation.
    @param[in] _position listener's front orientation */
    virtual void SetListenerFront (const VECTOR3& _front) = 0;

    /** Setter: listener's top orientation.
    @param[in] _position listener's top orientation */
    virtual void SetListenerTop (const VECTOR3& _top) = 0;

    /** Updates the engine.
    It must be called every frame. */
    virtual void Update () = 0;
};

extern "C" {
    typedef HRESULT (*CREATEAUDIOENGINE) (HINSTANCE, IAudioEngine**);
    typedef void (*RELEASEAUDIOENGINE) (IAudioEngine**);
}