/*
 * PS_Deformable.h
 *
 *  Created on: Sep 29, 2012
 *      Author: pourya
 */

#ifndef PS_DEFORMABLE_H_
#define PS_DEFORMABLE_H_

#include <vector>
#include "../PS_Base/PS_MathBase.h"
#include "../PS_Graphics/PS_MeshGLBuffer.h"

#include "corotationalLinearFEM.h"
#include "corotationalLinearFEMForceModel.h"
#include "generateMassMatrix.h"
#include "implicitBackwardEulerSparse.h"
#include "sceneObjectDeformable.h"
#include "graph.h"

using namespace std;

/*!
 *	Deformable model
 */
class Deformable{
public:
	Deformable();
	Deformable(const char* lpVegFilePath,
				const char* lpObjFilePath,
				std::vector<int>& vFixedVertices);

	virtual ~Deformable();


	void draw();
	void timestep();

	//Pick a vertex
	bool pickFreeVertex(double worldX, double worldY, double worldZ);

	//Haptic Interaction
	void hapticStart();
	void hapticEnd();
	bool hapticUpdateForce();
	bool hapticUpdateDisplace();
	void hapticSetCurrentForce(double extForce[3]);
	void hapticSetCurrentDisplacement(double dx, double dy, double dz);
	bool isHapticInProgress() const {return m_bHapticForceInProgress;}


	//Render
	bool renderVertices() const {return m_bRenderVertices;}
	void setRenderVertices(bool bRender) { m_bRenderVertices = bRender;}

	//Damping stiffness
	void setDampingStiffnessCoeff(double s);
	double getDampingStiffnessCoeff() const {return m_dampingStiffnessCoeff;}

	//Damping Mass
	void setDampingMassCoeff(double m);
	double getDampingMassCoeff() const {return m_dampingMassCoeff;}

private:
	/*!
	 * Setup procedure builds deformable model with the specified params
	 */
	void setup(const char* lpVegFilePath,
			    const char* lpObjFilePath,
			    std::vector<int>& vFixedVertices);

	void cleanup();
	void computeConstrainedDof(std::vector<int>& vArrOutputDof);
private:
	bool m_bHapticForceInProgress;
	double m_hapticCompliance;
	double m_hapticExtForce[3];
	double m_hapticExtDisplacements[3];
	int m_hapticForceNeighorhoodSize;

	double m_dampingStiffnessCoeff;
	double m_dampingMassCoeff;
	double m_timeStep;

	Graph* m_lpMeshGraph;
	//double* m_lpHapticForces;

	GLMeshBuffer m_meshBuffer;

	//Tetrahedra input mesh
	TetMesh* m_lpTetMesh;

	//Deformable Model
	CorotationalLinearFEM* m_lpDeformable;

	//ForceModel
	ForceModel* m_lpDeformableForceModel;

	//Integrator
	ImplicitBackwardEulerSparse* m_lpIntegrator;

	//
	SceneObjectDeformable * m_lpDeformableMesh;

	//Mass Matrix
	SparseMatrix* m_lpMassMatrix;

	//Time Step Counter
	U32 m_ctTimeStep;

	U32 m_dof;
	double* m_arrDisplacements;
	double* m_arrExtForces;

	//Fixed Vertices
	vector<int> m_vFixedVertices;
	int m_idxPulledVertex;

	bool m_bRenderFixedVertices;
	bool m_bRenderVertices;
};

#endif /* PS_DEFORMABLE_H_ */
