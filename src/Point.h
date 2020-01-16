// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

struct Point
{
  union
  {
    struct
    {
      double X;
      double Y;
    } Coord;

    double Data[2];
  };

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  Point(const double theX = 0.0,
        const double theY = 0.0)
    : Coord {theX, theY}
  {
  }

  // =======================================================================
  // function : Dot
  // purpose  :
  // =======================================================================
  double Dot (const Point& theOther) const
  {
    return Coord.X * theOther.Coord.X + Coord.Y * theOther.Coord.Y;
  }

  // =======================================================================
  // function : Cross
  // purpose  :
  // =======================================================================
  double Cross (const Point& theOther) const
  {
    return Coord.X * theOther.Coord.Y - Coord.Y * theOther.Coord.X;
  }
};
