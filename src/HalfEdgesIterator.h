// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <HalfEdge.h>

class HalfEdgesIterator
{
public:

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  HalfEdgesIterator (
    HalfEdge*  theStartHalfEdge,
    const bool isForward)
    : myCurrent   (theStartHalfEdge)
    , myIsForward (isForward)
  {
  }

  // =======================================================================
  // function : Destructor
  // purpose  :
  // =======================================================================
  ~HalfEdgesIterator()
  {
    HalfEdge* aHalfEdge = myCurrent;
    do
    {
      aHalfEdge->IsVisited = false;
      aHalfEdge = aHalfEdge->NextPtr;
    }
    while (aHalfEdge != myCurrent);
  }

  // =======================================================================
  // function : !
  // purpose  :
  // =======================================================================
  bool operator! ()
  {
    return !myCurrent->IsVisited;
  }

  // =======================================================================
  // function : ++
  // purpose  :
  // =======================================================================
  void operator++()
  {
    myCurrent->IsVisited = true;

    myCurrent = myIsForward ?
      myCurrent->NextPtr :
      myCurrent->PrevPtr;
  }

  // =======================================================================
  // function : ++
  // purpose  :
  // =======================================================================
  void operator++(int)
  {
    this->operator ++ ();
  }

  // =======================================================================
  // function : ->
  // purpose  :
  // =======================================================================
  const HalfEdge* operator->() const
  {
    return myCurrent;
  }

  // =======================================================================
  // function : ->
  // purpose  :
  // =======================================================================
  HalfEdge* operator->()
  {
    return myCurrent;
  }

  // =======================================================================
  // function : *
  // purpose  :
  // =======================================================================
  const HalfEdge* operator*() const
  {
    return myCurrent;
  }

  // =======================================================================
  // function : *
  // purpose  :
  // =======================================================================
  HalfEdge* operator*()
  {
    return myCurrent;
  }

private:
  HalfEdge* myCurrent;
  bool      myIsForward;
};
