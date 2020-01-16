// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>
#include <Node.h>
#include <HalfEdge.h>

#include <deque>
#include <stddef.h>

struct Mesh
{
  std::deque<Node>     Nodes;
  std::deque<HalfEdge> HalfEdges;

  // =======================================================================
  // function : Init
  // purpose  : Initializes nodes of the mesh by the given points.
  // =======================================================================
  inline void Init(const std::deque<Point>& thePoints)
  {
    for (size_t i = 0; i < thePoints.size(); ++i)
    {
      Nodes.push_back (Node {&thePoints[i], i});
    }
  }

  // =======================================================================
  // function : AddHalfEdge
  // purpose  : Adds new half-edge using the given node as its start.
  // =======================================================================
  HalfEdge* AddHalfEdge(const Node* theNode)
  {
    HalfEdges.push_back (HalfEdge {theNode});
    return &HalfEdges.back();
  }

  // =======================================================================
  // function : AddTriangle
  // purpose  : Creates triangle using the given node and half-edge.
  //            Returns new half-edge corresponding to the left side
  //            of new triangle.
  // =======================================================================
  HalfEdge* AddTriangle (
    const Node* theNode,
    HalfEdge*   theFaceEdge)
  {
    const Node* theNode1 = theFaceEdge         ->NodePtr;
    const Node* theNode2 = theFaceEdge->NextPtr->NodePtr;

    HalfEdge* aFaceEdge1  = AddHalfEdge (theNode);
    HalfEdge* aFaceEdge2  = AddHalfEdge (theNode2);

    HalfEdge* aFrontEdge1 = AddHalfEdge (theNode1);
    HalfEdge* aFrontEdge2 = AddHalfEdge (theNode);

    // Setup front half-edges
    aFrontEdge1->NextPtr = aFrontEdge2;
    aFrontEdge1->PrevPtr = theFaceEdge->PrevPtr;
    aFrontEdge1->HalfPtr = aFaceEdge1;

    theFaceEdge->PrevPtr->NextPtr = aFrontEdge1;

    aFrontEdge2->NextPtr = theFaceEdge->NextPtr;
    aFrontEdge2->PrevPtr = aFrontEdge1;
    aFrontEdge2->HalfPtr = aFaceEdge2;

    theFaceEdge->NextPtr->PrevPtr = aFrontEdge2;

    // Setup face
    aFaceEdge1->NextPtr = theFaceEdge;
    aFaceEdge1->PrevPtr = aFaceEdge2;
    aFaceEdge1->HalfPtr = aFrontEdge1;

    aFaceEdge2->NextPtr = aFaceEdge1;
    aFaceEdge2->PrevPtr = theFaceEdge;
    aFaceEdge2->HalfPtr = aFrontEdge2;

    theFaceEdge->NextPtr = aFaceEdge2;
    theFaceEdge->PrevPtr = aFaceEdge1;

    theFaceEdge->IsFront = false;
    aFaceEdge1 ->IsFront = false;
    aFaceEdge2 ->IsFront = false;

    return aFaceEdge1;
  }

  // =======================================================================
  // function : MakeTriangle
  // purpose  : Makes new triangle between end nodes of the given half-edges.
  // =======================================================================
  inline HalfEdge* MakeTriangle(HalfEdge* theHalfEdge1,
                                HalfEdge* theHalfEdge2)
  {
    if (theHalfEdge1->NextPtr != theHalfEdge2)
      return nullptr;

    HalfEdge* aFaceEdge = theHalfEdge1->PrevPtr;
    if (aFaceEdge != theHalfEdge2->NextPtr)
    {
      aFaceEdge = MakePolygon(theHalfEdge1, theHalfEdge2);
    }

    aFaceEdge   ->IsFront = false;
    theHalfEdge1->IsFront = false;
    theHalfEdge2->IsFront = false;

    return aFaceEdge;
  }

  // =======================================================================
  // function : MakePolygon
  // purpose  : Makes new triangle between end nodes of the given half-edges.
  // =======================================================================
  inline HalfEdge* MakePolygon(HalfEdge* theHalfEdge1,
                               HalfEdge* theHalfEdge2)
  {
    const Node* aNode1 = theHalfEdge1->NodePtr;
    const Node* aNode3 = theHalfEdge2->HalfPtr->NodePtr;

    HalfEdge* aFrontEdge = AddHalfEdge (aNode1);
    HalfEdge* aFaceEdge  = AddHalfEdge (aNode3);

    aFrontEdge->NextPtr = theHalfEdge2->NextPtr;
    aFrontEdge->PrevPtr = theHalfEdge1->PrevPtr;
    aFrontEdge->HalfPtr = aFaceEdge;

    aFrontEdge->NextPtr->PrevPtr = aFrontEdge;
    aFrontEdge->PrevPtr->NextPtr = aFrontEdge;

    aFaceEdge->NextPtr = theHalfEdge1;
    aFaceEdge->PrevPtr = theHalfEdge2;
    aFaceEdge->HalfPtr = aFrontEdge;

    theHalfEdge1->PrevPtr = aFaceEdge;
    theHalfEdge2->NextPtr = aFaceEdge;

    return aFaceEdge;
  }

  // =======================================================================
  // function : Flip
  // purpose  : Flip edge of neighboring triangles.
  // =======================================================================
  inline void Flip (HalfEdge* theHalfEdge)
  {
    HalfEdge* aOpposedEdge = theHalfEdge->HalfPtr;
    if (theHalfEdge->IsFront || aOpposedEdge->IsFront)
      return;

    const Node* aNodeFwd = theHalfEdge ->PrevPtr->NodePtr;
    const Node* aNodeBwd = aOpposedEdge->PrevPtr->NodePtr;

    // Connect border edges of pair
    theHalfEdge ->PrevPtr->NextPtr = aOpposedEdge->NextPtr;
    theHalfEdge ->NextPtr->PrevPtr = aOpposedEdge->PrevPtr;

    aOpposedEdge->PrevPtr->NextPtr = theHalfEdge ->NextPtr;
    aOpposedEdge->NextPtr->PrevPtr = theHalfEdge ->PrevPtr;

    // Flip inner edge
    theHalfEdge ->NextPtr = theHalfEdge ->NextPtr->NextPtr;
    theHalfEdge ->PrevPtr = theHalfEdge ->PrevPtr->NextPtr;

    aOpposedEdge->NextPtr = aOpposedEdge->NextPtr->NextPtr;
    aOpposedEdge->PrevPtr = aOpposedEdge->PrevPtr->NextPtr;

    // Reconnect border edges to inner
    theHalfEdge ->NextPtr->PrevPtr = theHalfEdge;
    theHalfEdge ->PrevPtr->NextPtr = theHalfEdge;

    aOpposedEdge->NextPtr->PrevPtr = aOpposedEdge;
    aOpposedEdge->PrevPtr->NextPtr = aOpposedEdge;

    // Update nodes
    theHalfEdge ->NodePtr = aNodeBwd;
    aOpposedEdge->NodePtr = aNodeFwd;
  }
};
