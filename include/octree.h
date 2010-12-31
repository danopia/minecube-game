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
        //Only value or children will ever be non-null
        T value; 
        vector<Octree<T> > children;
        bool hasChildren;

        Octree();
        Octree(vector<Octree<T> > initchildren);
        Octree(T initvalue);
        bool Insert(T val);
        bool Remove(int index);

};

template <class T>
Octree<T>::Octree() : value(0), hasChildren(false) {}

template <class T>
Octree<T>::Octree(vector<Octree<T> > initchildren) : value(0), hasChildren(true), children(initchildren) {}

template <class T>
Octree<T>::Octree(T initvalue) : value(initvalue), hasChildren(false), children(vector<Octree<T> >(0)) {}

template <class T>
bool Octree<T>::Insert(T val)
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

template <class T>
bool Octree<T>::Remove(int index)
{
    if(!hasChildren || children[index] == NULL)
        return false;
    children[index] = NULL;
    return true;
}
#endif
