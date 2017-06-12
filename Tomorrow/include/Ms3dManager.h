/** @file Ms3dLoader.h */
#pragma once

#include "../include/Ms3dModel.h"
#include <vector>

/** Loads *.ms3d model files. */
class Ms3dLoader {
public:
    /** Constructor. */
    Ms3dLoader ();

    /** Constructor.
    @param[in] _logFile  the name of a log file */
    Ms3dLoader (const char* _logFile);

    /** Constructor.
    @param[in] _log  the pointer to LogManager object */
    Ms3dLoader (LogManager* _log);

    /** Destructor. */
    ~Ms3dLoader ();

    /** Loads ms3d model.
    @param[in] _modelFile  filename of the model
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_FILE_NOT_FOUND the specified model filename does not exist
        - @c ERRC_BAD_FILE the file is either corrupted or not *.ms3d format
        - @c ERRC_OUT_OF_MEM not enough memory to load the model

    @return ID of the loaded model. */
    UINT LoadModel (const char* _modelFile);
    
    /** Returns the loaded model filename.
    @param[in] _id  ID of the loaded model.
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE the ID of the model is not valid

    @return filename of the loaded model.
    If the model is not loaded, NULL is returned. */
    const char* GetModelFile (UINT _id) const;
    
    /** Returns the pointer to Ms3dModel object.
    @param[in] _id  ID of the loaded model
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE the ID of the model is not valid

    @return the pointer to the Ms3dModel object */
    Ms3dModel* GetModel (UINT _id) const;
    
    /** Unloads all the models. */
    void UnloadModels ();

private:
    std::vector<Ms3dModel*> m_Models;   /**< A vector of the pointers to the loaded models */

    LogManager* m_Log;                  /**< A log manager */
};