/** @file ObjManager.h */

#pragma once

#include "..\include\ObjModel.h"
#include "..\include\ErrorMessage.h"
#include <vector>

class ObjModel;

//#ifdef _DEBUG
//    #define _CRTDBG_MAP_ALLOC
//    #include <stdlib.h>
//    #include <crtdbg.h>
//    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//    #define new DEBUG_NEW
//#endif

/** Rendering buffer information. */
struct Buffer {
    UINT BufferId;  /**< Static buffer ID. */
    UINT Num;       /**< Number of the vertices. */
};

/** A manager which controls the *.obj models loading. */
class ObjManager {
public:
    /** Constructor.
    @param[in] _device A pointer to RenderDevice
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    ObjManager (RenderDevice* _device);

    /** Destructor. */
    ~ObjManager ();

    /** Loads the *.obj model. 
    If *.mtl file exists, it is used to get the information about materials.
    @param[in] _filename a name of the model file
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_FILE_NOT_FOUND the specified file does not exist
        - @c ERRC_BAD_FILE the file is either corrupted or not *.obj format
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_OUT_OF_RANGE invalid ID 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return ID of the model */
    UINT Load (const char* _filename);

    /** Loads the *.obj model. 
    It does not use the information of the *.mtl file.
    It uses specified skin instead.
    @param[in] _filename a name of the model file
    @param[in] _skinId a skin ID
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_FILE_NOT_FOUND the specified file does not exist
        - @c ERRC_BAD_FILE the file is either corrupted or not *.obj format
        - @c ERRC_OUT_OF_MEM not enough memory to load the model
        - @c ERRC_OUT_OF_RANGE invalid ID 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return ID of the model */
    UINT Load (const char* _filename, UINT _skinId);

    /** Adds the model to the manager.
    @param[in] _model A pointer to the ObjModel
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM no more models can be added

    @return ID of the model */
    UINT AddModel (ObjModel* _model);

    /** Adds the rendering information of the model to the buffer. 
    @param[in] _modelData the vertices of the model
    @param[in] _numVertices number of the vertices
    @param[in] _fvf vertex format
    @param[in] _startVertex at which vertex the rendering should be started
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid 
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format

    @return buffer ID */
    UINT AddModelToRenderingBuffer (void* _modelData, UINT _numVertices, VERTEXFORMATTYPE _vft, UINT& _startVertex);

    /** Getter: number of the vertices of the buffer.
    @param[in] _fvf vertex format
    @param[in] _bufferId buffer ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_UNKNOWN_FVF wrong vertex format

    @return number of the vertices */
    UINT GetBufferNumVertices (VERTEXFORMATTYPE _vft, UINT _bufferId);

    /** Adds index information to the buffer.
    @param[in] _indexData the indices
    @param[in] _numIndices the number of the indices 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid 
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL

    @return buffer ID */
    UINT AddToIndexBuffer (WORD* _indexData, UINT _numIndices);

    /** Getter: the number of the indices of the buffer.
    @return the number of the indices */
    UINT GetIndexBufferNumIndices ();

    /** Getter: A pointer to the ObjModel.
    @param[in] _id model ID
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE the model ID is invalid

    @return A pointer to the ObjModel */
    ObjModel* GetModel (UINT _id);

    /** Unloads all the models. */
    void UnloadAll ();

private:
    RenderDevice* m_Device; /**< A pointer to RenderDevice. */

    /** The vector of the rendering buffers information. @see Buffer */
    std::vector<std::vector<Buffer>> m_RenderingBuffers;    
    Buffer m_IndexBuffer;               /**< Index buffer ID. */
    std::vector<ObjModel*> m_Models;    /**< The vector of the pointers to the ObjModel objects. */
};