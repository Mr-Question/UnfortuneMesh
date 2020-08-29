// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>
#include <Node.h>
#include <BndBox.h>
#include <Triangulator.h>
#include <PointSetTool.h>
#include <Mesh.h>
#include <Frame.h>
#include <Polygon.h>
#include <LocalContext.h>

#include <algorithm>
#include <limits>

class Launcher
{
public:

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  Launcher(
    std::deque<Point>& thePoints,
    LocalContext&      theContext)
    : myContext (theContext)
  {
    if (thePoints.size() < 3)
    {
      return;
    }

    const BndBox aBox = PointSetTool::Box (thePoints);
    Frame::Add    (aBox, thePoints, myContext.FrameType);
    PointSetTool::Scale (thePoints, myContext.Scale, aBox);

    myContext.Data.Init (thePoints);

    sort ();
    triangulate ();
  }

private:

  // =======================================================================
  // function : sort
  // purpose  :
  // =======================================================================
  void sort()
  {
    std::sort (myContext.Data.Nodes.begin(), myContext.Data.Nodes.end(), [](
      const Node& theNode1,
      const Node& theNode2)
    {
      if (theNode1.Pos->Coord.Y == theNode2.Pos->Coord.Y)
      {
        return theNode1.Pos->Coord.X < theNode2.Pos->Coord.X;
      }
      return theNode1.Pos->Coord.Y < theNode2.Pos->Coord.Y;
    });
  }

  // =======================================================================
  // function : triangulate
  // purpose  :
  // =======================================================================
  void triangulate ()
  {
    Triangulator aTool (myContext);
    std::for_each (myContext.Data.Nodes.begin(),
                   myContext.Data.Nodes.end(), [&aTool](
      const Node& theNode)
    {
      aTool.AddNode (&theNode);
    });

    Polygon aPolygon (myContext);
    aPolygon.MakeConvex ();
  }

private:

  LocalContext& myContext;
};
