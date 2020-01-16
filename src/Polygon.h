// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <HalfEdge.h>
#include <Mesh.h>
#include <Context.h>
#include <HalfEdgesIterator.h>

#include <limits>
#include <stdexcept>

class Polygon
{
public:

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  Polygon(Context& theContext)
    : myContext (theContext)
  {
  }

  // =======================================================================
  // function : MakeConvex
  // purpose  :
  // =======================================================================
  void MakeConvex ()
  {
    while (true)
    {
      bool isRunning = false;
      double aMaxCos = std::numeric_limits<double>::lowest ();
      HalfEdge* aHalfEdge = myContext.Front;
      for (HalfEdgesIterator aIt (aHalfEdge, true); !aIt; ++aIt)
      {
        double aSin = 0., aCos = 0.;
        if (!MeshTool::Angle (*aIt, aIt->NextPtr, aSin, aCos))
          throw std::runtime_error ("Too short segment");

        if (aSin > 0.)
        {
          if (aCos > aMaxCos)
          {
            isRunning = true;
            aMaxCos   = aCos;
            aHalfEdge = *aIt;
          }
        }
      }

      if (isRunning)
      {
        HalfEdge* aFaceEdge = myContext.Data.MakeTriangle (
          aHalfEdge, aHalfEdge->NextPtr);

        HalfEdge* aHalfEdge1 = aFaceEdge->PrevPtr;
        HalfEdge* aHalfEdge2 = aFaceEdge->NextPtr;

        MeshTool::LegalizeTriangle (myContext.Data, aHalfEdge1);
        MeshTool::LegalizeTriangle (myContext.Data, aHalfEdge2);

        myContext.Front = aFaceEdge->HalfPtr;
      }
      else
      {
        break;
      }
    }
  }

private:

  Context& myContext;
};
