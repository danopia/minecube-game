/* Octree implementation header */
#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <cstddef>
#include <vector>
#include <iostream>
using namespace std;


template <class T>
class Octree
{
    public:
        Octree()
        {
            value = 0;
            hasChildren = false; //Default to a leaf
        }
        Octree(vector<Octree<T> > initchildren)
        {
            value = 0;
            hasChildren = true;
            children = initchildren;
        }
        Octree(T initvalue)
        {
            value = initvalue;
            hasChildren = false;
            children = vector<Octree<T> >(0);
        }

        //Only value or children will ever be non-null
        T value; 
        vector<Octree<T> > children;
        bool hasChildren;

        bool Insert(T val)
        {
            if(!hasChildren)
                return false;
            //Go to the next free cell
            int freeIndex = 0;
            while(children[freeIndex] != NULL && freeIndex <= 7)
                freeIndex++;
            if(children[freeIndex] != NULL) return false;
            children[freeIndex] = val;
            return true;
        }   
        bool Remove(int index)
        {
            if(!hasChildren || children[index] == NULL)
                return false;
            children[index] = NULL;
            return true;
        }

};
#endif
