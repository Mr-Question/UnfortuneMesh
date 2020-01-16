// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>
#include <Node.h>
#include <Triangulator.h>
#include <Mesh.h>
#include <Frame.h>
#include <Polygon.h>
#include <Context.h>

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
    Context&           theContext,
    const double       theScale     = 1.,
    const Frame::Type  theFrameType = Frame::Triangle)
    : myContext (theContext)
    , myTool    (theContext)
  {
    if (thePoints.size() < 3)
    {
      return;
    }

    std::pair<Point, Point> aBox = box (thePoints);
    Frame::Add (aBox, thePoints, theFrameType);
    scale    (thePoints, theScale, aBox);

    myContext.Data.Init (thePoints);

    sort ();
    triangulate ();
  }

private:

  // =======================================================================
  // function : box
  // purpose  :
  // =======================================================================
  std::pair<Point, Point> box(const std::deque<Point>& thePoints)
  {
    const double aDoubleMin = std::numeric_limits<double>::lowest();
    const double aDoubleMax = std::numeric_limits<double>::max();

    std::pair<Point, Point> aBox = std::make_pair (
      Point {aDoubleMax, aDoubleMax},
      Point {aDoubleMin, aDoubleMin}
    );

    for (size_t i = 0; i < thePoints.size(); ++i)
    {
      const Point& aPoint = thePoints[i];
      for (int i = 0; i < 2; ++i)
      {
        aBox.first .Data[i] = std::min (aBox.first .Data[i], aPoint.Data[i]);
        aBox.second.Data[i] = std::max (aBox.second.Data[i], aPoint.Data[i]);
      }
    }

    return aBox;
  }

  // =======================================================================
  // function : scale
  // purpose  :
  // =======================================================================
  void scale(std::deque<Point>&             thePoints,
             const double                   theScale,
             const std::pair<Point, Point>& theBox)
  {
    const Point aScale {
      theScale / (theBox.second.Coord.X - theBox.first.Coord.X),
      theScale / (theBox.second.Coord.Y - theBox.first.Coord.Y)
    };

    std::for_each (thePoints.begin(), thePoints.end(), [&theBox, &aScale](
      Point& thePoint)
    {
      for (int i = 0; i < 2; ++i)
      {
        thePoint.Data[i] = (thePoint.Data[i] - theBox.first.Data[i]) * aScale.Data[i];
      }
    });
  }

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
    std::for_each (myContext.Data.Nodes.begin(),
                   myContext.Data.Nodes.end(), [this](
      const Node& theNode)
    {
      myTool.AddNode (&theNode);
    });

    Polygon aPolygon (myContext);
    aPolygon.MakeConvex ();
  }

private:

  Context&     myContext;
  Triangulator myTool;
};
