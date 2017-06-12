#include "../include/RenderCache.h"

using namespace std;
using namespace RenderCacheNS;

RenderCacheNS::PrimitiveCache::~PrimitiveCache() {
    delete Cache;
}

RenderCache::RenderCache (VertexCacheManager* _VertexCacheManager) {
    m_Root = NULL;
    m_VertexCacheManager = _VertexCacheManager;
}

RenderCache::~RenderCache () {
    Destroy ();
}

void RenderCache::Destroy () {
    delete m_Root;
}

void RenderCache::RotateLeft (Node*& _node) {
    Node* node;
    node = _node->Right;
    _node->Right = node->Left;
    node->Left = _node;
    UpdateHeight (_node);
    UpdateHeight (node);

    _node = node;
}

void RenderCache::RotateRight (Node*& _node) {
    Node* node;
    node = _node->Left;
    _node->Left = node->Right;
    node->Right = _node;
    UpdateHeight (_node);
    UpdateHeight (node);

    _node = node;
}

void RenderCache::Insert (Node*& _node, UINT _skinId, PRIMITIVETYPE _type, VERTEXFORMATTYPE _fvf) {
    try {
        if (_node) {
            if (_node->Value.SkinId > _skinId) {
                if (!_node->Left) {
                    _node->Left = new Node;
                    _node->Left->Value.SkinId = _skinId;
                    CreateVertexCache (_node->Left, _type, _fvf);
                    _node->Left->Height = 1;
                    _node->Height++;
                } else {
                    Insert (_node->Left, _skinId, _type, _fvf);
                    if (GetHeight (_node->Left) - GetHeight (_node->Right) == 2) {
                        if (_node->Left->Value.SkinId > _skinId) {
                            RotateRight (_node);
                        } else {
                            RotateLeft (_node->Left);
                            RotateRight (_node);
                        }
                    }
                }
            } else if (_node->Value.SkinId < _skinId) {
                if (!_node->Right) {
                    _node->Right = new Node;
                    _node->Right->Value.SkinId = _skinId;
                    CreateVertexCache (_node->Right, _type, _fvf);
                    _node->Right->Height = 1;
                    _node->Height++;
                } else {
                    Insert (_node->Right, _skinId, _type, _fvf);
                    if (GetHeight (_node->Right) - GetHeight (_node->Left) == 2) {
                        if (_node->Right->Value.SkinId < _skinId) {
                            RotateLeft (_node);
                        } else {
                            RotateRight (_node->Right);
                            RotateLeft (_node);
                        }
                    }
                }
            } else {    // _node->Value.SkinId == _skinId
                bool isType = false;
                for (UINT i = 0; i < _node->Value.PrimitiveCaches.size(); i++) {
                    if (_node->Value.PrimitiveCaches[i]->Type == _type) {
                        isType = true;
                        break;
                    }
                }
                if (!isType) {
                    CreateVertexCache (_node, _type, _fvf);
                }
            }
            UpdateHeight (_node);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void RenderCache::Insert (UINT _skinId, PRIMITIVETYPE _type, VERTEXFORMATTYPE _fvf) {
    try {
        if (m_Root == NULL) {
            m_Root = new Node;
            m_Root->Value.SkinId = _skinId;
            CreateVertexCache(m_Root, _type, _fvf);
            m_Root->Height = 0;
        } else {
            Insert (m_Root, _skinId, _type, _fvf);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

bool RenderCache::IsValue (UINT _skinId) {
    return Find (m_Root, _skinId);
}

bool RenderCache::Find (Node* _node, UINT _skinId) {
    if (_node) {
        if (_node->Value.SkinId > _skinId) {
            return Find (_node->Left, _skinId);
        }
        if (_node->Value.SkinId < _skinId) {
            return Find (_node->Right, _skinId);
        }
        if (_node->Value.SkinId == _skinId) {
            return true;
        }
    }
    return false;   // _node == NULL
}

VertexCache* RenderCache::FindVertexCache (Node* _node, UINT _skinId, PRIMITIVETYPE _type) {
    if (_node) {
        if (_node->Value.SkinId > _skinId) {
            return FindVertexCache (_node->Left, _skinId, _type);
        }
        if (_node->Value.SkinId < _skinId) {
            return FindVertexCache (_node->Right, _skinId, _type);
        }
        if (_node->Value.SkinId == _skinId) {
            for (UINT i = 0; i < _node->Value.PrimitiveCaches.size(); i++) {
                if (_node->Value.PrimitiveCaches[i]->Type == _type) {
                    return _node->Value.PrimitiveCaches[i]->Cache;
                }
            }
            return NULL;    // type is not found
        }
    }
    return NULL;    // _node == NULL
}

VertexCache* RenderCache::GetVertexCache (UINT _skinId, PRIMITIVETYPE _type) {
    return FindVertexCache (m_Root, _skinId, _type);
}

void RenderCache::CreateVertexCache (Node* _node, PRIMITIVETYPE _type, VERTEXFORMATTYPE _fvf) {
    try {
        if (_node) {
            PrimitiveCache* primCache = new PrimitiveCache;
            primCache->Type = _type;
            primCache->Cache = new VertexCache (
                _fvf,
                _type,
                m_VertexCacheManager,
                _node->Value.SkinId,
                MAX_VERTEX_NUM,
                MAX_INDEX_NUM);
            _node->Value.PrimitiveCaches.push_back (primCache);
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}