/*
 * PS_OclPolygonizer.h
 *
 *  Created on: 2011-12-04
 *      Author: pourya
 */
#ifndef PS_OCLPOLYGONIZER_H
#define PS_OCLPOLYGONIZER_H

#include "PS_PolyMemManager.h"
#include "PS_ComputeDevice.h"
#include "PS_VectorMath.h"

using namespace PS::SIMDPOLY;
using namespace PS::HPC;
using namespace PS::FUNCTIONALMATH;

//DATASIZES
#define DATASIZE_HEADER			12
#define DATASIZE_OPERATOR		8
#define DATASIZE_PRIMITIVE		20

//Operator
#define OFFSET_OP_LINK_FLAGS 	1
#define OFFSET_OP_CHILDREN 		2
#define OFFSET_OP_PARENT_LINK	3

//Primitive
#define OFFSET_PRIM_LINK_FLAGS 	1
#define OFFSET_PRIM_IDX_MATRIX 	1
#define OFFSET_PRIM_PARENT_LINK 3

//HEADER
#define OFFSET_COUNT_PRIMS 		8
#define OFFSET_COUNT_OPS 		9
#define OFFSET_COUNT_MTXNODES	10
#define OFFSET_CELLSIZE 		11

namespace PS{
namespace HPC{

/*!
 * GPU polygonizer for iso-surface extraction of the animated BlobTree on the GPU
 */
class GPUPoly{
public:
	GPUPoly();
	explicit GPUPoly(const char* lpFilePath);
	virtual ~GPUPoly();
	void cleanup();

	//Produce vertices count from the table
	static void ProduceNumVerticesTable(const char* chrOutput);

	/*!
	 * Read a model from disk and converts it into a proper format for GPU
	 */
	bool readModel(const char* lpFilePath);


	//Draws the mesh using accelerated memory buffer objects
	void drawMesh(bool bWireFrameMode = false);
	void drawBBox();

	static void DrawBox(const svec3f& lo, const svec3f& hi, const svec3f& color, float lineWidth);

	/*!
	*@param cellsize the cubic cellsize for the polygonizer
	*@return true when done
	*/
	int run(float cellsize = DEFAULT_CELL_SIZE);

	/*!
	 * Run the algorithm in tandem
	 */
	int runTandem(float cellsize = DEFAULT_CELL_SIZE);

private:
	void init();

private:
	typedef struct CellParam{
		U8 corner1[12];
		U8 corner2[12];
		U8 edgeaxis[12];
		U32 ctNeededCells[3];
		U32 ctTotalCells;
	};

	CellParam m_param;

	//Output:
	//Mesh Buffer Object for Drawing
	MESH_BUFFER_OBJECTS m_outMesh;

	//Compute Kernels
	PS::HPC::ComputeDevice* m_lpGPU;
	PS::HPC::ComputeKernel* m_lpKernelCellConfig;
	PS::HPC::ComputeKernel* m_lpKernelComputeConfig;
	PS::HPC::ComputeKernel* m_lpKernelComputeMesh;

	//Inputs:
	//BlobNode Matrix
	SOABlobNodeMatrices m_mtxNode;
	SOABlobBoxMatrices m_mtxBox;

	//Data Header
	float m_arrHeader[DATASIZE_HEADER];

	//Primitives Data
	float PS_SIMD_ALIGN(m_arrPrims[PS_SIMD_PADSIZE(MAX_TREE_NODES*DATASIZE_PRIMITIVE)]);

	//Operators Data
	float PS_SIMD_ALIGN(m_arrOps[PS_SIMD_PADSIZE(MAX_TREE_NODES*DATASIZE_OPERATOR)]);

	svec3f m_bboxLo;
	svec3f m_bboxHi;

	//Count of primitives and ops
	U32 m_ctPrims;
	U32 m_ctOps;
	U32 m_ctCells;
};

/*!
* Compute Mesh Centroids
*/
int Run_MeshCentroidKernel(float* arrVertices, U32* arrFaces, 
						   U32 ctVertices, U32 ctFaces,
						   float*& lpCentriods);


int Run_SphereDistKernel();
int Run_ArrayTestKernel();

}
}




#endif // PS_OCLPOLYGONIZER_H
