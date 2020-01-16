// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Mesh.h>

#include <cmath>

struct Context
{
  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  Context(
    Mesh&        theMesh,
    const double theMinAngleThresholdDeg = 1.,
    const double theMaxAngleThresholdDeg = 179.)
    : Data (theMesh)
    , SinMinAngleThreshold (sin (theMinAngleThresholdDeg * M_PI / 180.))
    , CosMaxAngleThreshold (cos (theMaxAngleThresholdDeg * M_PI / 180.))
    , Front (nullptr)
  {
  }

  Mesh&                Data;
  double               SinMinAngleThreshold;
  double               CosMaxAngleThreshold;
  HalfEdge*            Front;
};
