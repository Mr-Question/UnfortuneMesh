// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

struct Node;

struct HalfEdge
{
  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  HalfEdge(const Node* theNode = nullptr)
   : NodePtr (theNode)
   , HalfPtr (nullptr)
   , PrevPtr (nullptr)
   , NextPtr (nullptr)
   , IsFront (true)
   , IsVisited (false)
  {
  }

  const Node* NodePtr;
  HalfEdge*   HalfPtr;
  HalfEdge*   PrevPtr;
  HalfEdge*   NextPtr;
  bool        IsFront;
  bool        IsVisited;
};
