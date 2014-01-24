#pragma once

#include "../phgutils.h"

template <typename NodeType>
class Tree{
public:
	Tree():mRoot(nullptr){}
    virtual ~Tree(){if( mRoot ) delete mRoot;}

    const NodeType* root() const
    {
        return mRoot;
    }

private:
    // non-copyable
    Tree(const Tree&);
    Tree& operator=(const Tree&);

protected:
	// node type is supposed to have a destructor
    NodeType* mRoot;
};
