// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <HalfEdge.h>

#include <deque>
#include <stack>
#include <algorithm>

class TrianglesIterator
{
public:

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  TrianglesIterator (std::deque<HalfEdge>& theMesh)
    : myMesh (theMesh)
  {
    for (int i = 0; i < myMesh.size () && myStack.empty (); ++i)
    {
      if (!myMesh[i].IsFront)
        myStack.push (&myMesh[i]);
    }
  }

  // =======================================================================
  // function : Destructor
  // purpose  :
  // =======================================================================
  ~TrianglesIterator()
  {
    std::for_each (myMesh.begin (), myMesh.end (), [](
      HalfEdge& theHalfEdge)
    {
      theHalfEdge.IsVisited = false;
    });
  }

  // =======================================================================
  // function : !
  // purpose  :
  // =======================================================================
  bool operator! ()
  {
    return !myStack.empty ();
  }

  // =======================================================================
  // function : ++
  // purpose  :
  // =======================================================================
  void operator++()
  {
    if (myStack.empty ())
      return;

    HalfEdge* aHalfEdge = myStack.top ();
    myStack.pop ();
    for (int i = 0; i < 3; ++i)
    {
      aHalfEdge->IsVisited = true;

      HalfEdge* aOtherHalfEdge = aHalfEdge->HalfPtr;
      if (!aOtherHalfEdge->IsFront && !aOtherHalfEdge->IsVisited)
      {
        myStack.push (aOtherHalfEdge);
      }

      aHalfEdge = aHalfEdge->NextPtr;
    }

    while (!myStack.empty () && myStack.top ()->IsVisited)
    {
      myStack.pop ();
    }
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
    return myStack.top ();
  }

  // =======================================================================
  // function : ->
  // purpose  :
  // =======================================================================
  HalfEdge* operator->()
  {
    return myStack.top ();
  }

  // =======================================================================
  // function : *
  // purpose  :
  // =======================================================================
  const HalfEdge* operator*() const
  {
    return myStack.top ();
  }

  // =======================================================================
  // function : *
  // purpose  :
  // =======================================================================
  HalfEdge* operator*()
  {
    return myStack.top ();
  }

private:
  std::deque<HalfEdge>& myMesh;
  std::stack<HalfEdge*> myStack;
};
