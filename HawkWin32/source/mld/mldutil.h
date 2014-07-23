#pragma once

#ifdef _DEBUG

#include <cassert>
#include <windows.h> 
#define _CRTBLD
#include "dbgint.h"
#undef _CRTBLD
#ifdef _WIN32
#define ADDRESSFORMAT      "0x%.8X" 
#else
#define ADDRESSFORMAT      "0x%.16X"
#endif

#define BLOCKMAPCHUNKSIZE  64        
#define CALLSTACKCHUNKSIZE 32        
#define VLDINTERNALBLOCK   0xbf42    

#define VLD_STATUS_INSTALLED     0x1
#define VLD_STATUS_NEVER_ENABLED 0x2

inline static void* operator new (unsigned int size, char *file, int line)
{
    void *pdata = _malloc_dbg(size, _CRT_BLOCK | (VLDINTERNALBLOCK << 16), file, line);

    return pdata;
}

inline static void* operator new[] (unsigned int size, char *file, int line)
{
    void *pdata = _malloc_dbg(size, _CRT_BLOCK | (VLDINTERNALBLOCK << 16), file, line);

    return pdata;
}

#define new new(__FILE__, __LINE__)


inline static void operator delete (void *p, char *file, int line)
{
    _CrtMemBlockHeader *pheader = pHdr(p);

    _free_dbg(p, pheader->nBlockUse);
}

inline static void operator delete[] (void *p, char *file, int line)
{
    _CrtMemBlockHeader *pheader = pHdr(p);

    _free_dbg(p, pheader->nBlockUse);
}

inline static void operator delete (void *p)
{
    _CrtMemBlockHeader *pheader = pHdr(p);

    _free_dbg(p, pheader->nBlockUse);
}

inline static void operator delete[] (void *p)
{
    _CrtMemBlockHeader *pheader = pHdr(p);

    _free_dbg(p, pheader->nBlockUse);
}

void strapp (char **dest, char *source);

class CallStack
{
public:
    CallStack ();
    CallStack (const CallStack &source);
    ~CallStack ();

    bool  operator == (const CallStack &target);
    DWORD operator [] (unsigned long index);

    void clear ();
    void push_back (const DWORD programcounter);
    unsigned long size ();

private:
    class Chunk 
	{
    private:
		Chunk () : next(0) { memset(frames,0,sizeof(DWORD) * CALLSTACKCHUNKSIZE); }
        Chunk (const Chunk &source) { assert(false); } 

        Chunk     *next;
        DWORD     frames [CALLSTACKCHUNKSIZE];

        friend class CallStack;
    };

    unsigned long     m_capacity; 
    unsigned long     m_size;    
    CallStack::Chunk  m_store;    
    CallStack::Chunk *m_topchunk; 
    unsigned long     m_topindex; 
};

class BlockMap
{
    enum color_e { black, red };

public:
    BlockMap ();
    BlockMap (const BlockMap &source);
    ~BlockMap ();

    void erase (unsigned long request);
    CallStack* find (unsigned long request);
    CallStack* insert (unsigned long request);

private:
    class Chunk;

    class StackLink 
	{
	private:
		StackLink() : next(0) {}

    private:
        StackLink *next;
        CallStack  stack;

        friend class BlockMap;
    };

    class Node 
	{
    private:
        Node () {}
        Node (const Node &node) { assert(false); } 

        color_e        color;
        StackLink     *data;
        unsigned long  key;
        Node          *left;
        Node          *parent;
        Node          *right;

        friend class BlockMap;
        friend class Chunk;
    };

    class Chunk 
	{
    private:
        Chunk () {}
        Chunk (const Chunk &source) { assert(false); } 

        Chunk     *next;
        Node       nodes [BLOCKMAPCHUNKSIZE];
        StackLink  stacks [BLOCKMAPCHUNKSIZE];

        friend class BlockMap;
    };

    inline void _rotateleft (BlockMap::Node *node);
    inline void _rotateright (BlockMap::Node *node);

    BlockMap::Node      *m_freenodes;  
    BlockMap::StackLink *m_freestacks; 
    BlockMap::Node      *m_max;        
    BlockMap::Node       m_nil;       
    BlockMap::Node      *m_root;       
    BlockMap::Chunk      m_store;      
    BlockMap::Chunk     *m_storetail;  
};

/////////////////////////////////////////////////////////////////////////////////////

void strapp (char **dest, char *source)
{
    int  length = 0;
    char *temp  = 0;

    temp   = *dest;
    length = (int)strlen(*dest) + (int)strlen(source);
    *dest  = new char [length + 1];
    strncpy(*dest, temp, length);
    strncat(*dest, source, length);
    delete [] temp;
}

BlockMap::BlockMap ()
{
    unsigned long index;

    m_max        = &m_nil;
    m_nil.color  = black;
    m_nil.data   = NULL;
    m_nil.key    = 0;
    m_nil.left   = &m_nil;
    m_nil.parent = NULL;
    m_nil.right  = &m_nil;
    m_root       = &m_nil;
    m_store.next = NULL;
    m_storetail  = &m_store;

    for (index = 0; index < BLOCKMAPCHUNKSIZE - 1; index++) {
        m_store.nodes[index].parent = &m_store.nodes[index + 1];
        m_store.stacks[index].next = &m_store.stacks[index + 1];
    }
    m_store.nodes[index].parent = NULL;
    m_store.stacks[index].next = NULL;
    m_freenodes = m_store.nodes;
    m_freestacks = m_store.stacks;
}

BlockMap::BlockMap (const BlockMap &source)
{
    assert(false);

	unsigned long index;

	m_max        = &m_nil;
	m_nil.color  = black;
	m_nil.data   = NULL;
	m_nil.key    = 0;
	m_nil.left   = &m_nil;
	m_nil.parent = NULL;
	m_nil.right  = &m_nil;
	m_root       = &m_nil;
	m_store.next = NULL;
	m_storetail  = &m_store;

	for (index = 0; index < BLOCKMAPCHUNKSIZE - 1; index++) {
		m_store.nodes[index].parent = &m_store.nodes[index + 1];
		m_store.stacks[index].next = &m_store.stacks[index + 1];
	}
	m_store.nodes[index].parent = NULL;
	m_store.stacks[index].next = NULL;
	m_freenodes = m_store.nodes;
	m_freestacks = m_store.stacks;
}

BlockMap::~BlockMap ()
{
    BlockMap::Chunk *chunk = m_store.next;
    BlockMap::Chunk *temp;

    while (chunk) {
        temp = chunk;
        chunk = chunk->next;
        delete temp;
    }
}

void BlockMap::_rotateleft (BlockMap::Node *node)
{
    BlockMap::Node *child = node->right;

    node->right = child->left;
    if (child->left != &m_nil) {
        child->left->parent = node;
    }

    child->parent = node->parent;
    if (node->parent == &m_nil) {
        m_root = child;
    }
    else {
        if (node == node->parent->left) {
            node->parent->left = child;
        }
        else {
            node->parent->right = child;
        }
    }
    child->left = node;
    node->parent = child;
}

void BlockMap::_rotateright (BlockMap::Node *node)
{
    BlockMap::Node *child = node->left;

    node->left = child->right;
    if (child->right != &m_nil) {
        child->right->parent = node;
    }

    child->parent = node->parent;
    if (node->parent == &m_nil) {
        m_root = child;
    }
    else {
        if (node == node->parent->left) {
            node->parent->left = child;
        }
        else {
            node->parent->right = child;
        }
    }
    child->right = node;
    node->parent = child;
}

void BlockMap::erase (unsigned long request)
{
    BlockMap::Node      *child;
    BlockMap::Node      *cur;
    BlockMap::Node      *erasure;
    BlockMap::Node      *node = m_root;
    BlockMap::Node      *sibling;
    BlockMap::StackLink *stacklink;

    while (node != &m_nil) {
        if (node->key == request) {
            break;
        }
        else if (node->key > request) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
    if (node == &m_nil) {
        return;
    }

    if ((node->left == &m_nil) || (node->right == &m_nil)) {
        erasure = node;
        if (node == m_max) {
            if (m_max->left == &m_nil) {
                m_max = node->parent;
            }
            else {
                m_max = m_max->left;
                while (m_max->right != &m_nil) {
                    m_max = m_max->right;
                }
            }
        }
    }
    else {
        erasure = node->right;
        while (erasure->left != &m_nil) {
            erasure = erasure->left;
        }
    }

    if (erasure->left != &m_nil) {
        child = erasure->left;
    }
    else {
        child = erasure->right;
    }

    child->parent = erasure->parent;
    if (child->parent == &m_nil) {
        m_root = child;
    }
    else {
        if (erasure == erasure->parent->left) {
            erasure->parent->left = child;
        }
        else {
            erasure->parent->right = child;
        }
    }

    if (erasure != node) {
        stacklink = node->data;

        node->data = erasure->data;
        node->key  = erasure->key;
        if (erasure == m_max) {
            m_max = node;
        }
    }
    else {
        stacklink = erasure->data;
    }

    if (erasure->color == black) {
        cur = child;
        while ((cur != m_root) && (cur->color == black)) {
            if (cur == cur->parent->left) {
                sibling = cur->parent->right;
                if (sibling->color == red) {
                    sibling->color = black;
                    cur->parent->color = red;
                    _rotateleft(cur->parent);
                    sibling = cur->parent->right;
                }
                if ((sibling->left->color == black) && (sibling->right->color == black)) {
                    sibling->color = red;
                    cur = cur->parent;
                }
                else {
                    if (sibling->right->color == black) {
                        sibling->left->color = black;
                        sibling->color = red;
                        _rotateright(sibling);
                        sibling = cur->parent->right;
                    }
                    sibling->color = cur->parent->color;
                    cur->parent->color = black;
                    sibling->right->color = black;
                    _rotateleft(cur->parent);
                    cur = m_root;
                }
            }
            else {
                sibling = cur->parent->left;
                if (sibling->color == red) {
                    sibling->color = black;
                    cur->parent->color = red;
                    _rotateright(cur->parent);
                    sibling = cur->parent->left;
                }
                if ((sibling->left->color == black) && (sibling->right->color == black)) {
                    sibling->color = red;
                    cur = cur->parent;
                }
                else {
                    if (sibling->left->color == black) {
                        sibling->right->color = black;
                        sibling->color = red;
                        _rotateleft(sibling);
                        sibling = cur->parent->left;
                    }
                    sibling->color = cur->parent->color;
                    cur->parent->color = black;
                    sibling->left->color = black;
                    _rotateright(cur->parent);
                    cur = m_root;
                }
            }
        }
        cur->color = black;
    }

    erasure->parent = m_freenodes;
    m_freenodes = erasure;

    stacklink->stack.clear();
    stacklink->next = m_freestacks;
    m_freestacks = stacklink;
}

CallStack* BlockMap::find (unsigned long request)
{
    BlockMap::Node *cur = m_root;

    while (cur != &m_nil) {
        if (cur->key == request) {
            return &cur->data->stack;
        }
        else if (cur->key > request) {
            cur = cur->left;
        }
        else {
            cur = cur->right;
        }
    }

    return NULL;
}

CallStack* BlockMap::insert (unsigned long request)
{
    BlockMap::Chunk     *chunk;
    BlockMap::Node      *cur;
    unsigned long        index;
    BlockMap::Node      *node;
    BlockMap::Node      *parent;
    BlockMap::StackLink *stacklink;
    BlockMap::Node      *uncle;

    if (request > m_max->key) {
        parent = m_max;
    }
    else {
        parent = &m_nil;
        cur = m_root;
        while (cur != &m_nil) {
            parent = cur;
            if (cur->key == request) {
                cur->data->stack.clear();
                return &cur->data->stack;
            }
            else if (cur->key > request) {
                cur = cur->left;
            }
            else {
                cur = cur->right;
            }
        }
    }

    if ((m_freenodes == NULL) || (m_freestacks == NULL)) {
        chunk = new BlockMap::Chunk;
        chunk->next = NULL;
        for (index = 0; index < BLOCKMAPCHUNKSIZE - 1; index++) {
            chunk->nodes[index].parent = &chunk->nodes[index + 1];
            chunk->stacks[index].next = &chunk->stacks[index + 1];
        }
        chunk->nodes[index].parent = NULL;
        chunk->stacks[index].next = NULL;
        m_freenodes = chunk->nodes;
        m_freestacks = chunk->stacks;
        m_storetail->next = chunk;
        m_storetail = chunk;
    }
    node = m_freenodes;
    stacklink = m_freestacks;
    m_freenodes = m_freenodes->parent;
    m_freestacks = m_freestacks->next;

    node->color  = red;
    node->data   = stacklink;
    node->key    = request;
    node->left   = &m_nil;
    node->parent = parent;
    node->right  = &m_nil;
    if (parent == &m_nil) {
        m_root = node;
        m_max = node;
    }
    else {
        if (parent->key > request) {
            parent->left = node;
        }
        else {
            parent->right = node;
            if (parent == m_max) {
                m_max = node;
            }
        }
    }

    cur = node;
    while (cur->parent->color == red) {
        if (cur->parent == cur->parent->parent->left) {
            uncle = cur->parent->parent->right;
            if (uncle->color == red) {
                cur->parent->parent->color = red;
                cur->parent->color = black;
                uncle->color = black;
                cur = cur->parent->parent;
            }
            else {
                if (cur == cur->parent->right) {
                    cur = cur->parent;
                    _rotateleft(cur);
                }
                cur->parent->color = black;
                cur->parent->parent->color = red;
                _rotateright(cur->parent->parent);
            }
        }
        else {
            uncle = cur->parent->parent->left;
            if (uncle->color == red) {
                cur->parent->parent->color = red;
                cur->parent->color = black;
                uncle->color = black;
                cur = cur->parent->parent;
            }
            else {
                if (cur == cur->parent->left) {
                    cur = cur->parent;
                    _rotateright(cur);
                }
                cur->parent->color = black;
                cur->parent->parent->color = red;
                _rotateleft(cur->parent->parent);
            }
        }
    }

    m_root->color = black;

    return &node->data->stack;
}

CallStack::CallStack ()
{
    m_capacity   = CALLSTACKCHUNKSIZE;
    m_size       = 0;
    m_store.next = NULL;
    m_topchunk   = &m_store;
    m_topindex   = 0;
}

CallStack::CallStack (const CallStack &source)
{
	m_capacity   = CALLSTACKCHUNKSIZE;
	m_size       = 0;
	m_store.next = NULL;
	m_topchunk   = &m_store;
	m_topindex   = 0;

    assert(false);
}

CallStack::~CallStack ()
{
    CallStack::Chunk *chunk = m_store.next;
    CallStack::Chunk *temp;

    while (chunk) {
        temp = chunk;
        chunk = temp->next;
        delete temp;
    }
}

bool CallStack::operator == (const CallStack &target)
{
    CallStack::Chunk       *chunk = &m_store;
    unsigned long           index;
    CallStack::Chunk       *prevchunk = NULL;
    const CallStack::Chunk *targetchunk = &target.m_store;

    if (m_size != target.m_size) {
        return false;
    }

    while (prevchunk != m_topchunk) {
        for (index = 0; index < ((chunk == m_topchunk) ? m_topindex : CALLSTACKCHUNKSIZE); index++) {
            if (chunk->frames[index] != targetchunk->frames[index]) {
                return false;
            }
        }
        prevchunk = chunk;
        chunk = chunk->next;
        targetchunk = targetchunk->next;
    }

    return true;
}

DWORD CallStack::operator [] (unsigned long index)
{
    unsigned long     count;
    CallStack::Chunk *chunk = &m_store;
    unsigned long     chunknumber = index / CALLSTACKCHUNKSIZE;

    for (count = 0; count < chunknumber; count++) {
        chunk = chunk->next;
    }

    return chunk->frames[index % CALLSTACKCHUNKSIZE];
}

void CallStack::clear ()
{
    m_size     = 0;
    m_topchunk = &m_store;
    m_topindex = 0;
}

void CallStack::push_back (const DWORD programcounter)
{
    CallStack::Chunk *chunk;

    if (m_size == m_capacity) {
        chunk = new CallStack::Chunk;
        chunk->next = NULL;
        m_topchunk->next = chunk;
        m_topchunk = chunk;
        m_topindex = 0;
        m_capacity += CALLSTACKCHUNKSIZE;
    }
    else if (m_topindex == CALLSTACKCHUNKSIZE) {
        m_topchunk = m_topchunk->next;
        m_topindex = 0;
    }

    m_topchunk->frames[m_topindex++] = programcounter;
    m_size++;
}

unsigned long CallStack::size ()
{
    return m_size;
}

#endif
