/** @file RenderCache.h */

#pragma once

#include <windows.h>
#include <vector>
#include "../include/Engine.h"
#include "../include/VertexCacheManager.h"
//#include "../include/VertexCache.h"

class VertexCacheManager;
class VertexCache;

/** Structures of the render cache. */
namespace RenderCacheNS {

    /** Vertex cache and its primitive type. */
    struct PrimitiveCache {
        /** Constructor. */
        PrimitiveCache () {
            Cache = NULL;
        }

        /** Destructor. */
        ~PrimitiveCache ();

        VertexCache* Cache; /**< Vertex cache. */
        PRIMITIVETYPE Type; /**< Primitive type. */
    };

    /** RenderCache value. */
    struct RenderCacheValue {
        UINT SkinId;    /**< Skin ID. */
        std::vector<RenderCacheNS::PrimitiveCache*> PrimitiveCaches;    /**< @see RenderCacheNS::PrimitiveCache */
    };

    /** Tree node. */
    struct Node {
    public:
        /** Constructor. */
        Node () {
            Left = NULL;
            Right = NULL;
        }
        /** Destructor. */
        ~Node () {
            for (UINT i = 0; i < Value.PrimitiveCaches.size(); i++) {
                delete Value.PrimitiveCaches[i];
            }
            delete Left;
            delete Right;
        }
        RenderCacheNS::RenderCacheValue Value;  /**< Node's value. */
        Node* Left;     /**< Left node. */
        Node* Right;    /**< Right node. */
        UINT Height;    /**< Height of the node. */
    };
}

/** AVL tree of the vertex caches. */
class RenderCache {
public:
    /** Constructor.
    @param[in] _vertexCacheManager  pointer to the VertexCacheManager */
    RenderCache (VertexCacheManager* _vertexCacheManager);

    /** Destructor. */
    ~RenderCache ();

    /** Inserts new node.
    @param[in] _skinId  skin ID
    @param[in] _type  primitive type
    @param[in] _fvf  vertex format 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void Insert (UINT _skinId, PRIMITIVETYPE _type, VERTEXFORMATTYPE _fvf);

    /** Destroys entire tree. */
    void Destroy ();

    /** Checks if value with specified skin ID exists.
    @param[in] _skinId  skin ID
    @return @c true value with specifed skin ID exists. @c false otherwise */
    bool IsValue (UINT _skinId);

    /** Getter: vertex cache.
    @param[in] _skinId  skin ID
    @param[in] _type  primitive type
    @return vertex cache. NULL if vertex cache was not found. */
    VertexCache* GetVertexCache (UINT _skinId, PRIMITIVETYPE _type);
    
    /** Getter: tree root node.
    @return tree root node */
    inline RenderCacheNS::Node* GetRoot () {
        return m_Root;
    }
private:
    /** Updates the height of the node.
    @param[in] _node  node */
    inline void UpdateHeight (RenderCacheNS::Node* _node) {
        _node->Height = GetHeight (_node->Left) + 1;
        if (_node->Height < GetHeight(_node->Right) + 1) {
            _node->Height = GetHeight(_node->Right) + 1;
        }
    }
    
    /** Getter: height of the node.
    @param[in] _node  node
    @return height of the node. */
    inline UINT GetHeight (RenderCacheNS::Node* _node) {
        if (_node) {
            return _node->Height;
        }
        return 0;
    }

    /** Inserts the new node.
    @param[in] _node  tree node
    @param[in] _skinId  skin ID
    @param[in] _type  primitive type
    @param[in] _fvf  vertex format 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void Insert (RenderCacheNS::Node*& _node, UINT _skinId, PRIMITIVETYPE _type, VERTEXFORMATTYPE _fvf);

    /** Rotates node to the left.
    @param[in] _node  tree node */
    void RotateLeft (RenderCacheNS::Node*& _node);

    /** Rotates node to the right.
    @param[in] _node  tree node */
    void RotateRight (RenderCacheNS::Node*& _node);

    /** Search for the skin ID.
    @param[in] _node  tree node
    @param[in] _skinId  skin ID
    @return @c true skin ID is found. @c false otherwise */
    bool Find (RenderCacheNS::Node* _node, UINT _skinId);

    /** Creates vertex cache for the node.
    @param[in] _node  tree node
    @param[in] _type  primitive type
    @param[in] _fvf  vertex format 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void CreateVertexCache (RenderCacheNS::Node* _node, PRIMITIVETYPE _type, VERTEXFORMATTYPE _fvf);

    /** Search for vertex cache.
    @param[in] _node  tree node
    @param[in] _skinId  skin ID
    @param[in] _type  primitive type */
    VertexCache* FindVertexCache (RenderCacheNS::Node* _node, UINT _skinId, PRIMITIVETYPE _type);

    RenderCacheNS::Node* m_Root;    /**< Tree root. */
    VertexCacheManager* m_VertexCacheManager;   /**< Pointer to the vertex cache manager. */
};