// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <HalfEdge.h>
#include <Mesh.h>
#include <Circle.h>

#include <limits>

class MeshTool
{
private:

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  MeshTool()
  {
  }

public:

  // =======================================================================
  // function : LegalizeTriangle
  // purpose  :
  // =======================================================================
  static void LegalizeTriangle (Mesh&     theMesh,
                                HalfEdge* theSharedEdge)
  {
    HalfEdge* aOpposedEdge = theSharedEdge->HalfPtr;

    const Node* aNode1 = theSharedEdge->NodePtr;
    const Node* aNode2 =  aOpposedEdge->NodePtr;

    const Node* aNodeFwd = theSharedEdge->PrevPtr->NodePtr;
    const Node* aNodeBwd =  aOpposedEdge->PrevPtr->NodePtr;

    if (isToFlip (aNodeBwd, aNode1, aNode2, aNodeFwd) ||
        isToFlip (aNodeFwd, aNode2, aNode1, aNodeBwd))
    {
      theMesh.Flip (theSharedEdge);
    }
  }

  // =======================================================================
  // function : Angle
  // purpose  :
  // =======================================================================
  static bool Angle(
    const Node* theNode1,
    const Node* theNode2,
    const Node* theNode3,
    double&     theSin,
    double&     theCos)
  {
    const Point aVec1 (
      theNode3->Pos->Coord.X - theNode2->Pos->Coord.X,
      theNode3->Pos->Coord.Y - theNode2->Pos->Coord.Y);

    const Point aVec2 (
      theNode1->Pos->Coord.X - theNode2->Pos->Coord.X,
      theNode1->Pos->Coord.Y - theNode2->Pos->Coord.Y);

    const double aLen1 = sqrt (aVec1.Dot (aVec1));
    if (aLen1 < std::numeric_limits<double>::min ())
      return false;

    const double aLen2 = sqrt (aVec2.Dot (aVec2));
    if (aLen2 < std::numeric_limits<double>::min ())
      return false;

    const double aNorm = (aLen1 * aLen2);
    theSin = aVec1.Cross (aVec2) / aNorm;
    theCos = aVec1.Dot   (aVec2) / aNorm;

    return true;
  }

  // =======================================================================
  // function : Angle
  // purpose  :
  // =======================================================================
  static bool Angle(
    const HalfEdge* theHalfEdge1,
    const HalfEdge* theHalfEdge2,
    double&         theSin,
    double&         theCos)
  {
    const Node* aNode1 = theHalfEdge1->NodePtr;
    const Node* aNode2 = theHalfEdge2->NodePtr;
    const Node* aNode3 = theHalfEdge2->HalfPtr->NodePtr;

    return Angle (aNode1, aNode2, aNode3, theSin, theCos);
  }

private:

  // =======================================================================
  // function : isToFlip
  // purpose  :
  // =======================================================================
  static bool isToFlip (
    const Node* theCheckNode,
    const Node* theNode1,
    const Node* theNode2,
    const Node* theNode3)
  {
    Circle aCircle = Circle::Create (*theNode1->Pos, *theNode2->Pos, *theNode3->Pos);
    if (aCircle.SqRadius < 0.)
      return false;

    const Point aDiff {
      theCheckNode->Pos->Coord.X - aCircle.Pos.Coord.X,
      theCheckNode->Pos->Coord.Y - aCircle.Pos.Coord.Y
    };

    return (aDiff.Dot (aDiff) + std::numeric_limits<double>::epsilon () < aCircle.SqRadius);
  }
};
