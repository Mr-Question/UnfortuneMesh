// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <HalfEdge.h>
#include <Mesh.h>
#include <MeshTool.h>
#include <LocalContext.h>

#include <stdexcept>

class Triangulator
{
public:

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  Triangulator(LocalContext& theContext)
    : myContext (theContext)
  {
  }

  // =======================================================================
  // function : AddNode
  // purpose  :
  // =======================================================================
  void AddNode (const Node* theNode)
  {
    if (theNode == nullptr)
      return;

    if (myContext.Data.HalfEdges.empty())
    {
      addFirstNode (theNode);
    }
    else if (myContext.Data.HalfEdges.size() == 1)
    {
      addSecondNode (theNode);
    }
    else
    {
      addNode (theNode);
    }
  }

private:

  // =======================================================================
  // function : addFirstNode
  // purpose  :
  // =======================================================================
  void addFirstNode  (const Node* theNode)
  {
    myContext.Front = myContext.Data.AddHalfEdge (theNode);
  }

  // =======================================================================
  // function : addSecondNode
  // purpose  :
  // =======================================================================
  void addSecondNode (const Node* theNode)
  {
    HalfEdge* aPrevHalfEdge = &myContext.Data.HalfEdges.back();
    HalfEdge* aNextHalfEdge =  myContext.Data.AddHalfEdge (theNode);

    aNextHalfEdge->HalfPtr = aPrevHalfEdge;
    aNextHalfEdge->PrevPtr = aPrevHalfEdge;
    aNextHalfEdge->NextPtr = aPrevHalfEdge;

    aPrevHalfEdge->HalfPtr = aNextHalfEdge;
    aPrevHalfEdge->PrevPtr = aNextHalfEdge;
    aPrevHalfEdge->NextPtr = aNextHalfEdge;
  }

  // =======================================================================
  // function : addNode
  // purpose  :
  // =======================================================================
  void addNode       (const Node* theNode)
  {
    // Find half-edge hit by projection of node on front.
    HalfEdge* aStartFront = myContext.Front;
    const bool isForward = (myContext.Front->NodePtr->Pos->Coord.X <= theNode->Pos->Coord.X);
    do
    {
      if (isWithinSegment (theNode, myContext.Front))
        break;

      myContext.Front = isForward ? myContext.Front->NextPtr : myContext.Front->PrevPtr;
    }
    while (myContext.Front != aStartFront);

    if (!isWithinSegment (theNode, myContext.Front))
      throw std::runtime_error ("Point is out of front");

    myContext.Front = addTriangle  (theNode, myContext.Front);
    aStartFront = myContext.Front->NextPtr;

    myContext.Front = correctFront (myContext.Front, false); // Here we need left half-edge.
  }

  // =======================================================================
  // function : addTriangle
  // purpose  :
  // =======================================================================
  HalfEdge* addTriangle (const Node* theNode, HalfEdge* theFaceEdge)
  {
    HalfEdge* aFaceEdge1 = myContext.Data.AddTriangle (theNode, theFaceEdge);

    MeshTool::LegalizeTriangle (myContext.Data, theFaceEdge);

    return aFaceEdge1->HalfPtr;
  }

  // =======================================================================
  // function : isWithinSegment
  // purpose  :
  // =======================================================================
  bool isWithinSegment (
    const Node*     theNode,
    const HalfEdge* theSegment)
  {
    const double aMinX = theSegment         ->NodePtr->Pos->Coord.X;
    const double aMaxX = theSegment->HalfPtr->NodePtr->Pos->Coord.X;
    return (aMinX <= theNode->Pos->Coord.X &&
            aMaxX >= theNode->Pos->Coord.X);
  }

  // =======================================================================
  // function : correctFront
  // purpose  :
  // =======================================================================
  HalfEdge* correctFront (HalfEdge* theFront, const bool isForward)
  {
    const HalfEdge* aStartFront = theFront;
    do
    {
      HalfEdge* aHalfEdge1 = isForward ? theFront : theFront->PrevPtr;
      HalfEdge* aHalfEdge2 = isForward ? theFront->NextPtr : theFront;

      double aSin = 0., aCos = 0.;
      if (!MeshTool::Angle(aHalfEdge1, aHalfEdge2, aSin, aCos))
        throw std::runtime_error ("Too short segment");

      if (aSin > myContext.SinMinAngleThreshold && aCos > myContext.CosMaxAngleThreshold)
      {
        HalfEdge* aFaceEdge = myContext.Data.MakeTriangle (aHalfEdge1, aHalfEdge2);
        if (aFaceEdge == nullptr)
          throw std::runtime_error ("Unexpected..., but failure.");

        MeshTool::LegalizeTriangle (myContext.Data, aHalfEdge1);
        MeshTool::LegalizeTriangle (myContext.Data, aHalfEdge2);

        theFront = aFaceEdge->HalfPtr;
      }
      else
      {
        break;
      }
    }
    while (theFront != aStartFront);

    return theFront;
  }

private:

  LocalContext& myContext;
};
