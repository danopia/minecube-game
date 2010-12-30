/* Octree implementation header */
#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <cstddef>

template <class T>
class Octree
{
    public:
        Octree(Octree<T> *initchildren[8])
        {
            value = NULL;
            children = initchildren;
        }
        Octree(T initvalue)
        {
            value = initvalue;
            children = (T)NULL;
        };
        Octree()
        {
            value = NULL;
            children = NULL;
        };

                //Only value or children will ever be non-null
        T value; 
        Octree<T> *children[8];


        bool Insert(T *val)
        {
            if(children == NULL)
                return false;
            //Go to the next free cell
            int freeIndex = 0;
            while(children[freeIndex] != NULL && freeIndex <= 8)
                freeIndex++;
            if(freeIndex == 8) return false;
            children[freeIndex] = val;
            return true;
        }   
        bool Remove(int index)
        {
            if(children == NULL || children[index] == NULL)
                return false;
            delete children[index];
            children[index] = NULL;
            return true;
        }

};
#endif
