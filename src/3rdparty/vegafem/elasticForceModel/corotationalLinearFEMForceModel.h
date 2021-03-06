/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 1.0                               *
 *                                                                       *
 * "forceModel" library , Copyright (C) 2007 CMU, 2009 MIT, 2012 USC     *
 * All rights reserved.                                                  *
 *                                                                       *
 * Code author: Jernej Barbic                                            *
 * http://www.jernejbarbic.com/code                                      *
 *                                                                       *
 * Research: Jernej Barbic, Fun Shing Sin, Daniel Schroeder,             *
 *           Doug L. James, Jovan Popovic                                *
 *                                                                       *
 * Funding: National Science Foundation, Link Foundation,                *
 *          Singapore-MIT GAMBIT Game Lab,                               *
 *          Zumberge Research and Innovation Fund at USC                 *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of the BSD-style license that is            *
 * included with this library in the file LICENSE.txt                    *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the file     *
 * LICENSE.TXT for more details.                                         *
 *                                                                       *
 *************************************************************************/

/*
  Force model for the Corotational linear FEM material.
*/

#ifndef _COROTATIONALLINEARFEMFORCEMODEL_H_
#define _COROTATIONALLINEARFEMFORCEMODEL_H_

#include "corotationalLinearFEM.h"
#include "forceModel.h"

class CorotationalLinearFEMForceModel : virtual public ForceModel
{
public:
  CorotationalLinearFEMForceModel(CorotationalLinearFEM * corotationalLinearFEM, int warp=1);
  virtual ~CorotationalLinearFEMForceModel(); 

  virtual void GetInternalForce(double * u, double * internalForces);
  virtual void GetTangentStiffnessMatrixTopology(SparseMatrix ** tangentStiffnessMatrix);
  virtual void GetTangentStiffnessMatrix(double * u, SparseMatrix * tangentStiffnessMatrix); 

  virtual void GetForceAndMatrix(double * u, double * internalForces, SparseMatrix * tangentStiffnessMatrix);

  inline void SetWarp(int warp) { this->warp = warp; }

protected:
  CorotationalLinearFEM * corotationalLinearFEM;
  int warp;
};

#endif

