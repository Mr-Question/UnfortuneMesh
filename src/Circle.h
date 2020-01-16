// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>

#include <limits>
#include <cmath>

struct Circle
{
  Point  Pos;
  double SqRadius;

  // =======================================================================
  // function : Create
  // purpose  : Creates circumcircle for triangle defined by its three points.
  // =======================================================================
  static Circle Create(
    const Point& thePoint1,
    const Point& thePoint2,
    const Point& thePoint3)
  {
    Circle aCircle {Point{}, -1.};
    const Point aLink1(
      thePoint3.Coord.X - thePoint2.Coord.X,
      thePoint2.Coord.Y - thePoint3.Coord.Y
    );
    if (aLink1.Dot (aLink1) < std::numeric_limits<double>::epsilon ())
      return aCircle;

    const Point aLink2(
      thePoint1.Coord.X - thePoint3.Coord.X,
      thePoint3.Coord.Y - thePoint1.Coord.Y
    );
    if (aLink2.Dot (aLink2) < std::numeric_limits<double>::epsilon ())
      return aCircle;

    const Point aLink3(
      thePoint2.Coord.X - thePoint1.Coord.X,
      thePoint1.Coord.Y - thePoint2.Coord.Y
    );
    if (aLink3.Dot (aLink3) < std::numeric_limits<double>::epsilon ())
      return aCircle;

    const double aD = 2. * (thePoint1.Coord.X * aLink1.Coord.Y +
                            thePoint2.Coord.X * aLink2.Coord.Y +
                            thePoint3.Coord.X * aLink3.Coord.Y);

    if (std::abs(aD) < std::numeric_limits<double>::min ())
      return aCircle;

    const double aInvD = 1. / aD;
    const double aSqMod1 = thePoint1.Dot (thePoint1);
    const double aSqMod2 = thePoint2.Dot (thePoint2);
    const double aSqMod3 = thePoint3.Dot (thePoint3);
    aCircle.Pos.Coord.X = (aSqMod1 * aLink1.Coord.Y +
                           aSqMod2 * aLink2.Coord.Y +
                           aSqMod3 * aLink3.Coord.Y) * aInvD;

    aCircle.Pos.Coord.Y = (aSqMod1 * aLink1.Coord.X +
                           aSqMod2 * aLink2.Coord.X +
                           aSqMod3 * aLink3.Coord.X) * aInvD;

    const Point aDist1(
      thePoint1.Coord.X - aCircle.Pos.Coord.X,
      thePoint1.Coord.Y - aCircle.Pos.Coord.Y
    );

    const Point aDist2(
      thePoint2.Coord.X - aCircle.Pos.Coord.X,
      thePoint2.Coord.Y - aCircle.Pos.Coord.Y
    );

    const Point aDist3(
      thePoint3.Coord.X - aCircle.Pos.Coord.X,
      thePoint3.Coord.Y - aCircle.Pos.Coord.Y
    );

    aCircle.SqRadius = std::max(std::max(
      aDist1.Dot (aDist1), aDist2.Dot (aDist2)), aDist3.Dot (aDist3));

    return aCircle;
  }
};
