#ifndef B3_FIND_CONCAVE_SEPARATING_AXIS_H
#define B3_FIND_CONCAVE_SEPARATING_AXIS_H

#define B3_TRIANGLE_NUM_CONVEX_FACES 5

#include "Bullet3Common/shared/b3Int4.h"
#include "Bullet3Collision/NarrowPhaseCollision/shared/b3RigidBodyData.h"
#include "Bullet3Collision/NarrowPhaseCollision/shared/b3Collidable.h"
#include "Bullet3Collision/BroadPhaseCollision/shared/b3Aabb.h"
#include "Bullet3Collision/NarrowPhaseCollision/shared/b3BvhSubtreeInfoData.h"
#include "Bullet3Collision/NarrowPhaseCollision/shared/b3QuantizedBvhNodeData.h"
#include "Bullet3Collision/NarrowPhaseCollision/shared/b3ConvexPolyhedronData.h"

inline void b3Project(__global const b3ConvexPolyhedronData* hull, b3Float4ConstArg pos, b3QuatConstArg orn,
					  const b3Float4* dir, __global const b3Float4* vertices, float* min, float* max)
{
	min[0] = FLT_MAX;
	max[0] = -FLT_MAX;
	int numVerts = hull->m_numVertices;

	const b3Float4 localDir = b3QuatRotate(b3QuatInverse(orn), *dir);
	float offset = b3Dot(pos, *dir);
	for (int i = 0; i < numVerts; i++)
	{
		float dp = b3Dot(vertices[hull->m_vertexOffset + i], localDir);
		if (dp < min[0])
			min[0] = dp;
		if (dp > max[0])
			max[0] = dp;
	}
	if (min[0] > max[0])
	{
		float tmp = min[0];
		min[0] = max[0];
		max[0] = tmp;
	}
	min[0] += offset;
	max[0] += offset;
}

inline bool b3TestSepAxis(const b3ConvexPolyhedronData* hullA, __global const b3ConvexPolyhedronData* hullB,
						  b3Float4ConstArg posA, b3QuatConstArg ornA,
						  b3Float4ConstArg posB, b3QuatConstArg ornB,
						  b3Float4* sep_axis, const b3Float4* verticesA, __global const b3Float4* verticesB, float* depth)
{
	float Min0, Max0;
	float Min1, Max1;
	b3Project(hullA, posA, ornA, sep_axis, verticesA, &Min0, &Max0);
	b3Project(hullB, posB, ornB, sep_axis, verticesB, &Min1, &Max1);

	if (Max0 < Min1 || Max1 < Min0)
		return false;

	float d0 = Max0 - Min1;
	float d1 = Max1 - Min0;
	*depth = d0 < d1 ? d0 : d1;
	return true;
}

bool b3FindSeparatingAxis(const b3ConvexPolyhedronData* hullA, __global const b3ConvexPolyhedronData* hullB,
						  b3Float4ConstArg posA1,
						  b3QuatConstArg ornA,
						  b3Float4ConstArg posB1,
						  b3QuatConstArg ornB,
						  b3Float4ConstArg DeltaC2,

						  const b3Float4* verticesA,
						  const b3Float4* uniqueEdgesA,
						  const b3GpuFace* facesA,
						  const int* indicesA,

						  __global const b3Float4* verticesB,
						  __global const b3Float4* uniqueEdgesB,
						  __global const b3GpuFace* facesB,
						  __global const int* indicesB,
						  b3Float4* sep,
						  float* dmin)
{
	b3Float4 posA = posA1;
	posA.w = 0.f;
	b3Float4 posB = posB1;
	posB.w = 0.f;
	/*
	static int maxFaceVertex = 0;

	int curFaceVertexAB = hullA->m_numFaces*hullB->m_numVertices;
	curFaceVertexAB+= hullB->m_numFaces*hullA->m_numVertices;

	if (curFaceVertexAB>maxFaceVertex)
	{
		maxFaceVertex = curFaceVertexAB;
		printf("curFaceVertexAB = %d\n",curFaceVertexAB);
		printf("hullA->m_numFaces = %d\n",hullA->m_numFaces);
		printf("hullA->m_numVertices = %d\n",hullA->m_numVertices);
		printf("hullB->m_numVertices = %d\n",hullB->m_numVertices);
	}
*/

	int curPlaneTests = 0;
	{
		int numFacesA = hullA->m_numFaces;
		// Test normals from hullA
		for (int i = 0; i < numFacesA; i++)
		{
			const b3Float4 normal = facesA[hullA->m_faceOffset + i].m_plane;
			b3Float4 faceANormalWS = b3QuatRotate(ornA, normal);
			if (b3Dot(DeltaC2, faceANormalWS) < 0)
				faceANormalWS *= -1.f;
			curPlaneTests++;
			float d;
			if (!b3TestSepAxis(hullA, hullB, posA, ornA, posB, ornB, &faceANormalWS, verticesA, verticesB, &d))
				return false;
			if (d < *dmin)
			{
				*dmin = d;
				*sep = faceANormalWS;
			}
		}
	}
	if ((b3Dot(-DeltaC2, *sep)) > 0.0f)
	{
		*sep = -(*sep);
	}
	return true;
}

b3Vector3 unitSphere162[] =
	{
		b3MakeVector3(0.000000f, -1.000000f, 0.000000f),
		b3MakeVector3(0.203181f, -0.967950f, 0.147618f),
		b3MakeVector3(-0.077607f, -0.967950f, 0.238853f),
		b3MakeVector3(0.723607f, -0.447220f, 0.525725f),
		b3MakeVector3(0.609547f, -0.657519f, 0.442856f),
		b3MakeVector3(0.812729f, -0.502301f, 0.295238f),
		b3MakeVector3(-0.251147f, -0.967949f, 0.000000f),
		b3MakeVector3(-0.077607f, -0.967950f, -0.238853f),
		b3MakeVector3(0.203181f, -0.967950f, -0.147618f),
		b3MakeVector3(0.860698f, -0.251151f, 0.442858f),
		b3MakeVector3(-0.276388f, -0.447220f, 0.850649f),
		b3MakeVector3(-0.029639f, -0.502302f, 0.864184f),
		b3MakeVector3(-0.155215f, -0.251152f, 0.955422f),
		b3MakeVector3(-0.894426f, -0.447216f, 0.000000f),
		b3MakeVector3(-0.831051f, -0.502299f, 0.238853f),
		b3MakeVector3(-0.956626f, -0.251149f, 0.147618f),
		b3MakeVector3(-0.276388f, -0.447220f, -0.850649f),
		b3MakeVector3(-0.483971f, -0.502302f, -0.716565f),
		b3MakeVector3(-0.436007f, -0.251152f, -0.864188f),
		b3MakeVector3(0.723607f, -0.447220f, -0.525725f),
		b3MakeVector3(0.531941f, -0.502302f, -0.681712f),
		b3MakeVector3(0.687159f, -0.251152f, -0.681715f),
		b3MakeVector3(0.687159f, -0.251152f, 0.681715f),
		b3MakeVector3(-0.436007f, -0.251152f, 0.864188f),
		b3MakeVector3(-0.956626f, -0.251149f, -0.147618f),
		b3MakeVector3(-0.155215f, -0.251152f, -0.955422f),
		b3MakeVector3(0.860698f, -0.251151f, -0.442858f),
		b3MakeVector3(0.276388f, 0.447220f, 0.850649f),
		b3MakeVector3(0.483971f, 0.502302f, 0.716565f),
		b3MakeVector3(0.232822f, 0.657519f, 0.716563f),
		b3MakeVector3(-0.723607f, 0.447220f, 0.525725f),
		b3MakeVector3(-0.531941f, 0.502302f, 0.681712f),
		b3MakeVector3(-0.609547f, 0.657519f, 0.442856f),
		b3MakeVector3(-0.723607f, 0.447220f, -0.525725f),
		b3MakeVector3(-0.812729f, 0.502301f, -0.295238f),
		b3MakeVector3(-0.609547f, 0.657519f, -0.442856f),
		b3MakeVector3(0.276388f, 0.447220f, -0.850649f),
		b3MakeVector3(0.029639f, 0.502302f, -0.864184f),
		b3MakeVector3(0.232822f, 0.657519f, -0.716563f),
		b3MakeVector3(0.894426f, 0.447216f, 0.000000f),
		b3MakeVector3(0.831051f, 0.502299f, -0.238853f),
		b3MakeVector3(0.753442f, 0.657515f, 0.000000f),
		b3MakeVector3(-0.232822f, -0.657519f, 0.716563f),
		b3MakeVector3(-0.162456f, -0.850654f, 0.499995f),
		b3MakeVector3(0.052790f, -0.723612f, 0.688185f),
		b3MakeVector3(0.138199f, -0.894429f, 0.425321f),
		b3MakeVector3(0.262869f, -0.525738f, 0.809012f),
		b3MakeVector3(0.361805f, -0.723611f, 0.587779f),
		b3MakeVector3(0.531941f, -0.502302f, 0.681712f),
		b3MakeVector3(0.425323f, -0.850654f, 0.309011f),
		b3MakeVector3(0.812729f, -0.502301f, -0.295238f),
		b3MakeVector3(0.609547f, -0.657519f, -0.442856f),
		b3MakeVector3(0.850648f, -0.525736f, 0.000000f),
		b3MakeVector3(0.670817f, -0.723611f, -0.162457f),
		b3MakeVector3(0.670817f, -0.723610f, 0.162458f),
		b3MakeVector3(0.425323f, -0.850654f, -0.309011f),
		b3MakeVector3(0.447211f, -0.894428f, 0.000001f),
		b3MakeVector3(-0.753442f, -0.657515f, 0.000000f),
		b3MakeVector3(-0.525730f, -0.850652f, 0.000000f),
		b3MakeVector3(-0.638195f, -0.723609f, 0.262864f),
		b3MakeVector3(-0.361801f, -0.894428f, 0.262864f),
		b3MakeVector3(-0.688189f, -0.525736f, 0.499997f),
		b3MakeVector3(-0.447211f, -0.723610f, 0.525729f),
		b3MakeVector3(-0.483971f, -0.502302f, 0.716565f),
		b3MakeVector3(-0.232822f, -0.657519f, -0.716563f),
		b3MakeVector3(-0.162456f, -0.850654f, -0.499995f),
		b3MakeVector3(-0.447211f, -0.723611f, -0.525727f),
		b3MakeVector3(-0.361801f, -0.894429f, -0.262863f),
		b3MakeVector3(-0.688189f, -0.525736f, -0.499997f),
		b3MakeVector3(-0.638195f, -0.723609f, -0.262863f),
		b3MakeVector3(-0.831051f, -0.502299f, -0.238853f),
		b3MakeVector3(0.361804f, -0.723612f, -0.587779f),
		b3MakeVector3(0.138197f, -0.894429f, -0.425321f),
		b3MakeVector3(0.262869f, -0.525738f, -0.809012f),
		b3MakeVector3(0.052789f, -0.723611f, -0.688186f),
		b3MakeVector3(-0.029639f, -0.502302f, -0.864184f),
		b3MakeVector3(0.956626f, 0.251149f, 0.147618f),
		b3MakeVector3(0.956626f, 0.251149f, -0.147618f),
		b3MakeVector3(0.951058f, -0.000000f, 0.309013f),
		b3MakeVector3(1.000000f, 0.000000f, 0.000000f),
		b3MakeVector3(0.947213f, -0.276396f, 0.162458f),
		b3MakeVector3(0.951058f, 0.000000f, -0.309013f),
		b3MakeVector3(0.947213f, -0.276396f, -0.162458f),
		b3MakeVector3(0.155215f, 0.251152f, 0.955422f),
		b3MakeVector3(0.436007f, 0.251152f, 0.864188f),
		b3MakeVector3(-0.000000f, -0.000000f, 1.000000f),
		b3MakeVector3(0.309017f, 0.000000f, 0.951056f),
		b3MakeVector3(0.138199f, -0.276398f, 0.951055f),
		b3MakeVector3(0.587786f, 0.000000f, 0.809017f),
		b3MakeVector3(0.447216f, -0.276398f, 0.850648f),
		b3MakeVector3(-0.860698f, 0.251151f, 0.442858f),
		b3MakeVector3(-0.687159f, 0.251152f, 0.681715f),
		b3MakeVector3(-0.951058f, -0.000000f, 0.309013f),
		b3MakeVector3(-0.809018f, 0.000000f, 0.587783f),
		b3MakeVector3(-0.861803f, -0.276396f, 0.425324f),
		b3MakeVector3(-0.587786f, 0.000000f, 0.809017f),
		b3MakeVector3(-0.670819f, -0.276397f, 0.688191f),
		b3MakeVector3(-0.687159f, 0.251152f, -0.681715f),
		b3MakeVector3(-0.860698f, 0.251151f, -0.442858f),
		b3MakeVector3(-0.587786f, -0.000000f, -0.809017f),
		b3MakeVector3(-0.809018f, -0.000000f, -0.587783f),
		b3MakeVector3(-0.670819f, -0.276397f, -0.688191f),
		b3MakeVector3(-0.951058f, 0.000000f, -0.309013f),
		b3MakeVector3(-0.861803f, -0.276396f, -0.425324f),
		b3MakeVector3(0.436007f, 0.251152f, -0.864188f),
		b3MakeVector3(0.155215f, 0.251152f, -0.955422f),
		b3MakeVector3(0.587786f, -0.000000f, -0.809017f),
		b3MakeVector3(0.309017f, -0.000000f, -0.951056f),
		b3MakeVector3(0.447216f, -0.276398f, -0.850648f),
		b3MakeVector3(0.000000f, 0.000000f, -1.000000f),
		b3MakeVector3(0.138199f, -0.276398f, -0.951055f),
		b3MakeVector3(0.670820f, 0.276396f, 0.688190f),
		b3MakeVector3(0.809019f, -0.000002f, 0.587783f),
		b3MakeVector3(0.688189f, 0.525736f, 0.499997f),
		b3MakeVector3(0.861804f, 0.276394f, 0.425323f),
		b3MakeVector3(0.831051f, 0.502299f, 0.238853f),
		b3MakeVector3(-0.447216f, 0.276397f, 0.850649f),
		b3MakeVector3(-0.309017f, -0.000001f, 0.951056f),
		b3MakeVector3(-0.262869f, 0.525738f, 0.809012f),
		b3MakeVector3(-0.138199f, 0.276397f, 0.951055f),
		b3MakeVector3(0.029639f, 0.502302f, 0.864184f),
		b3MakeVector3(-0.947213f, 0.276396f, -0.162458f),
		b3MakeVector3(-1.000000f, 0.000001f, 0.000000f),
		b3MakeVector3(-0.850648f, 0.525736f, -0.000000f),
		b3MakeVector3(-0.947213f, 0.276397f, 0.162458f),
		b3MakeVector3(-0.812729f, 0.502301f, 0.295238f),
		b3MakeVector3(-0.138199f, 0.276397f, -0.951055f),
		b3MakeVector3(-0.309016f, -0.000000f, -0.951057f),
		b3MakeVector3(-0.262869f, 0.525738f, -0.809012f),
		b3MakeVector3(-0.447215f, 0.276397f, -0.850649f),
		b3MakeVector3(-0.531941f, 0.502302f, -0.681712f),
		b3MakeVector3(0.861804f, 0.276396f, -0.425322f),
		b3MakeVector3(0.809019f, 0.000000f, -0.587782f),
		b3MakeVector3(0.688189f, 0.525736f, -0.499997f),
		b3MakeVector3(0.670821f, 0.276397f, -0.688189f),
		b3MakeVector3(0.483971f, 0.502302f, -0.716565f),
		b3MakeVector3(0.077607f, 0.967950f, 0.238853f),
		b3MakeVector3(0.251147f, 0.967949f, 0.000000f),
		b3MakeVector3(0.000000f, 1.000000f, 0.000000f),
		b3MakeVector3(0.162456f, 0.850654f, 0.499995f),
		b3MakeVector3(0.361800f, 0.894429f, 0.262863f),
		b3MakeVector3(0.447209f, 0.723612f, 0.525728f),
		b3MakeVector3(0.525730f, 0.850652f, 0.000000f),
		b3MakeVector3(0.638194f, 0.723610f, 0.262864f),
		b3MakeVector3(-0.203181f, 0.967950f, 0.147618f),
		b3MakeVector3(-0.425323f, 0.850654f, 0.309011f),
		b3MakeVector3(-0.138197f, 0.894430f, 0.425320f),
		b3MakeVector3(-0.361804f, 0.723612f, 0.587778f),
		b3MakeVector3(-0.052790f, 0.723612f, 0.688185f),
		b3MakeVector3(-0.203181f, 0.967950f, -0.147618f),
		b3MakeVector3(-0.425323f, 0.850654f, -0.309011f),
		b3MakeVector3(-0.447210f, 0.894429f, 0.000000f),
		b3MakeVector3(-0.670817f, 0.723611f, -0.162457f),
		b3MakeVector3(-0.670817f, 0.723611f, 0.162457f),
		b3MakeVector3(0.077607f, 0.967950f, -0.238853f),
		b3MakeVector3(0.162456f, 0.850654f, -0.499995f),
		b3MakeVector3(-0.138197f, 0.894430f, -0.425320f),
		b3MakeVector3(-0.052790f, 0.723612f, -0.688185f),
		b3MakeVector3(-0.361804f, 0.723612f, -0.587778f),
		b3MakeVector3(0.361800f, 0.894429f, -0.262863f),
		b3MakeVector3(0.638194f, 0.723610f, -0.262864f),
		b3MakeVector3(0.447209f, 0.723612f, -0.525728f)};

bool b3FindSeparatingAxisEdgeEdge(const b3ConvexPolyhedronData* hullA, __global const b3ConvexPolyhedronData* hullB,
								  b3Float4ConstArg posA1,
								  b3QuatConstArg ornA,
								  b3Float4ConstArg posB1,
								  b3QuatConstArg ornB,
								  b3Float4ConstArg DeltaC2,
								  const b3Float4* verticesA,
								  const b3Float4* uniqueEdgesA,
								  const b3GpuFace* facesA,
								  const int* indicesA,
								  __global const b3Float4* verticesB,
								  __global const b3Float4* uniqueEdgesB,
								  __global const b3GpuFace* facesB,
								  __global const int* indicesB,
								  b3Float4* sep,
								  float* dmin,
								  bool searchAllEdgeEdge)
{
	b3Float4 posA = posA1;
	posA.w = 0.f;
	b3Float4 posB = posB1;
	posB.w = 0.f;

	//	int curPlaneTests=0;

	int curEdgeEdge = 0;
	// Test edges
	static int maxEdgeTests = 0;
	int curEdgeTests = hullA->m_numUniqueEdges * hullB->m_numUniqueEdges;
	if (curEdgeTests > maxEdgeTests)
	{
		maxEdgeTests = curEdgeTests;
		printf("maxEdgeTests = %d\n", maxEdgeTests);
		printf("hullA->m_numUniqueEdges = %d\n", hullA->m_numUniqueEdges);
		printf("hullB->m_numUniqueEdges = %d\n", hullB->m_numUniqueEdges);
	}

	if (searchAllEdgeEdge)
	{
		for (int e0 = 0; e0 < hullA->m_numUniqueEdges; e0++)
		{
			const b3Float4 edge0 = uniqueEdgesA[hullA->m_uniqueEdgesOffset + e0];
			b3Float4 edge0World = b3QuatRotate(ornA, edge0);

			for (int e1 = 0; e1 < hullB->m_numUniqueEdges; e1++)
			{
				const b3Float4 edge1 = uniqueEdgesB[hullB->m_uniqueEdgesOffset + e1];
				b3Float4 edge1World = b3QuatRotate(ornB, edge1);

				b3Float4 crossje = b3Cross(edge0World, edge1World);

				curEdgeEdge++;
				if (!b3IsAlmostZero(crossje))
				{
					crossje = b3Normalized(crossje);
					if (b3Dot(DeltaC2, crossje) < 0)
						crossje *= -1.f;

					float dist;
					bool result = true;
					{
						float Min0, Max0;
						float Min1, Max1;
						b3Project(hullA, posA, ornA, &crossje, verticesA, &Min0, &Max0);
						b3Project(hullB, posB, ornB, &crossje, verticesB, &Min1, &Max1);

						if (Max0 < Min1 || Max1 < Min0)
							return false;

						float d0 = Max0 - Min1;
						float d1 = Max1 - Min0;
						dist = d0 < d1 ? d0 : d1;
						result = true;
					}

					if (dist < *dmin)
					{
						*dmin = dist;
						*sep = crossje;
					}
				}
			}
		}
	}
	else
	{
		int numDirections = sizeof(unitSphere162) / sizeof(b3Vector3);
		//printf("numDirections =%d\n",numDirections );

		for (int i = 0; i < numDirections; i++)
		{
			b3Float4 crossje = unitSphere162[i];
			{
				//if (b3Dot(DeltaC2,crossje)>0)
				{
					float dist;
					bool result = true;
					{
						float Min0, Max0;
						float Min1, Max1;
						b3Project(hullA, posA, ornA, &crossje, verticesA, &Min0, &Max0);
						b3Project(hullB, posB, ornB, &crossje, verticesB, &Min1, &Max1);

						if (Max0 < Min1 || Max1 < Min0)
							return false;

						float d0 = Max0 - Min1;
						float d1 = Max1 - Min0;
						dist = d0 < d1 ? d0 : d1;
						result = true;
					}

					if (dist < *dmin)
					{
						*dmin = dist;
						*sep = crossje;
					}
				}
			}
		}
	}

	if ((b3Dot(-DeltaC2, *sep)) > 0.0f)
	{
		*sep = -(*sep);
	}
	return true;
}

inline int b3FindClippingFaces(b3Float4ConstArg separatingNormal,
							   __global const b3ConvexPolyhedronData_t* hullA, __global const b3ConvexPolyhedronData_t* hullB,
							   b3Float4ConstArg posA, b3QuatConstArg ornA, b3Float4ConstArg posB, b3QuatConstArg ornB,
							   __global b3Float4* worldVertsA1,
							   __global b3Float4* worldNormalsA1,
							   __global b3Float4* worldVertsB1,
							   int capacityWorldVerts,
							   const float minDist, float maxDist,
							   __global const b3Float4* verticesA,
							   __global const b3GpuFace_t* facesA,
							   __global const int* indicesA,
							   __global const b3Float4* verticesB,
							   __global const b3GpuFace_t* facesB,
							   __global const int* indicesB,

							   __global b3Int4* clippingFaces, int pairIndex)
{
	int numContactsOut = 0;
	int numWorldVertsB1 = 0;

	int closestFaceB = -1;
	float dmax = -FLT_MAX;

	{
		for (int face = 0; face < hullB->m_numFaces; face++)
		{
			const b3Float4 Normal = b3MakeFloat4(facesB[hullB->m_faceOffset + face].m_plane.x,
												 facesB[hullB->m_faceOffset + face].m_plane.y, facesB[hullB->m_faceOffset + face].m_plane.z, 0.f);
			const b3Float4 WorldNormal = b3QuatRotate(ornB, Normal);
			float d = b3Dot(WorldNormal, separatingNormal);
			if (d > dmax)
			{
				dmax = d;
				closestFaceB = face;
			}
		}
	}

	{
		const b3GpuFace_t polyB = facesB[hullB->m_faceOffset + closestFaceB];
		const int numVertices = polyB.m_numIndices;
		for (int e0 = 0; e0 < numVertices; e0++)
		{
			const b3Float4 b = verticesB[hullB->m_vertexOffset + indicesB[polyB.m_indexOffset + e0]];
			worldVertsB1[pairIndex * capacityWorldVerts + numWorldVertsB1++] = b3TransformPoint(b, posB, ornB);
		}
	}

	int closestFaceA = -1;
	{
		float dmin = FLT_MAX;
		for (int face = 0; face < hullA->m_numFaces; face++)
		{
			const b3Float4 Normal = b3MakeFloat4(
				facesA[hullA->m_faceOffset + face].m_plane.x,
				facesA[hullA->m_faceOffset + face].m_plane.y,
				facesA[hullA->m_faceOffset + face].m_plane.z,
				0.f);
			const b3Float4 faceANormalWS = b3QuatRotate(ornA, Normal);

			float d = b3Dot(faceANormalWS, separatingNormal);
			if (d < dmin)
			{
				dmin = d;
				closestFaceA = face;
				worldNormalsA1[pairIndex] = faceANormalWS;
			}
		}
	}

	int numVerticesA = facesA[hullA->m_faceOffset + closestFaceA].m_numIndices;
	for (int e0 = 0; e0 < numVerticesA; e0++)
	{
		const b3Float4 a = verticesA[hullA->m_vertexOffset + indicesA[facesA[hullA->m_faceOffset + closestFaceA].m_indexOffset + e0]];
		worldVertsA1[pairIndex * capacityWorldVerts + e0] = b3TransformPoint(a, posA, ornA);
	}

	clippingFaces[pairIndex].x = closestFaceA;
	clippingFaces[pairIndex].y = closestFaceB;
	clippingFaces[pairIndex].z = numVerticesA;
	clippingFaces[pairIndex].w = numWorldVertsB1;

	return numContactsOut;
}

__kernel void b3FindConcaveSeparatingAxisKernel(__global b3Int4* concavePairs,
												__global const b3RigidBodyData* rigidBodies,
												__global const b3Collidable* collidables,
												__global const b3ConvexPolyhedronData* convexShapes,
												__global const b3Float4* vertices,
												__global const b3Float4* uniqueEdges,
												__global const b3GpuFace* faces,
												__global const int* indices,
												__global const b3GpuChildShape* gpuChildShapes,
												__global b3Aabb* aabbs,
												__global b3Float4* concaveSeparatingNormalsOut,
												__global b3Int4* clippingFacesOut,
												__global b3Vector3* worldVertsA1Out,
												__global b3Vector3* worldNormalsA1Out,
												__global b3Vector3* worldVertsB1Out,
												__global int* hasSeparatingNormals,
												int vertexFaceCapacity,
												int numConcavePairs,
												int pairIdx)
{
	int i = pairIdx;
	/*	int i = get_global_id(0);
	if (i>=numConcavePairs)
		return;
	int pairIdx = i;
	*/

	int bodyIndexA = concavePairs[i].x;
	int bodyIndexB = concavePairs[i].y;

	int collidableIndexA = rigidBodies[bodyIndexA].m_collidableIdx;
	int collidableIndexB = rigidBodies[bodyIndexB].m_collidableIdx;

	int shapeIndexA = collidables[collidableIndexA].m_shapeIndex;
	int shapeIndexB = collidables[collidableIndexB].m_shapeIndex;

	if (collidables[collidableIndexB].m_shapeType != SHAPE_CONVEX_HULL &&
		collidables[collidableIndexB].m_shapeType != SHAPE_COMPOUND_OF_CONVEX_HULLS)
	{
		concavePairs[pairIdx].w = -1;
		return;
	}

	hasSeparatingNormals[i] = 0;

	//	int numFacesA = convexShapes[shapeIndexA].m_numFaces;
	int numActualConcaveConvexTests = 0;

	int f = concavePairs[i].z;

	bool overlap = false;

	b3ConvexPolyhedronData convexPolyhedronA;

	//add 3 vertices of the triangle
	convexPolyhedronA.m_numVertices = 3;
	convexPolyhedronA.m_vertexOffset = 0;
	b3Float4 localCenter = b3MakeFloat4(0.f, 0.f, 0.f, 0.f);

	b3GpuFace face = faces[convexShapes[shapeIndexA].m_faceOffset + f];
	b3Aabb triAabb;
	triAabb.m_minVec = b3MakeFloat4(1e30f, 1e30f, 1e30f, 0.f);
	triAabb.m_maxVec = b3MakeFloat4(-1e30f, -1e30f, -1e30f, 0.f);

	b3Float4 verticesA[3];
	for (int i = 0; i < 3; i++)
	{
		int index = indices[face.m_indexOffset + i];
		b3Float4 vert = vertices[convexShapes[shapeIndexA].m_vertexOffset + index];
		verticesA[i] = vert;
		localCenter += vert;

		triAabb.m_minVec = b3MinFloat4(triAabb.m_minVec, vert);
		triAabb.m_maxVec = b3MaxFloat4(triAabb.m_maxVec, vert);
	}

	overlap = true;
	overlap = (triAabb.m_minVec.x > aabbs[bodyIndexB].m_maxVec.x || triAabb.m_maxVec.x < aabbs[bodyIndexB].m_minVec.x) ? false : overlap;
	overlap = (triAabb.m_minVec.z > aabbs[bodyIndexB].m_maxVec.z || triAabb.m_maxVec.z < aabbs[bodyIndexB].m_minVec.z) ? false : overlap;
	overlap = (triAabb.m_minVec.y > aabbs[bodyIndexB].m_maxVec.y || triAabb.m_maxVec.y < aabbs[bodyIndexB].m_minVec.y) ? false : overlap;

	if (overlap)
	{
		float dmin = FLT_MAX;
		int hasSeparatingAxis = 5;
		b3Float4 sepAxis = b3MakeFloat4(1, 2, 3, 4);

		//	int localCC=0;
		numActualConcaveConvexTests++;

		//a triangle has 3 unique edges
		convexPolyhedronA.m_numUniqueEdges = 3;
		convexPolyhedronA.m_uniqueEdgesOffset = 0;
		b3Float4 uniqueEdgesA[3];

		uniqueEdgesA[0] = (verticesA[1] - verticesA[0]);
		uniqueEdgesA[1] = (verticesA[2] - verticesA[1]);
		uniqueEdgesA[2] = (verticesA[0] - verticesA[2]);

		convexPolyhedronA.m_faceOffset = 0;

		b3Float4 normal = b3MakeFloat4(face.m_plane.x, face.m_plane.y, face.m_plane.z, 0.f);

		b3GpuFace facesA[B3_TRIANGLE_NUM_CONVEX_FACES];
		int indicesA[3 + 3 + 2 + 2 + 2];
		int curUsedIndices = 0;
		int fidx = 0;

		//front size of triangle
		{
			facesA[fidx].m_indexOffset = curUsedIndices;
			indicesA[0] = 0;
			indicesA[1] = 1;
			indicesA[2] = 2;
			curUsedIndices += 3;
			float c = face.m_plane.w;
			facesA[fidx].m_plane.x = normal.x;
			facesA[fidx].m_plane.y = normal.y;
			facesA[fidx].m_plane.z = normal.z;
			facesA[fidx].m_plane.w = c;
			facesA[fidx].m_numIndices = 3;
		}
		fidx++;
		//back size of triangle
		{
			facesA[fidx].m_indexOffset = curUsedIndices;
			indicesA[3] = 2;
			indicesA[4] = 1;
			indicesA[5] = 0;
			curUsedIndices += 3;
			float c = b3Dot(normal, verticesA[0]);
			//	float c1 = -face.m_plane.w;
			facesA[fidx].m_plane.x = -normal.x;
			facesA[fidx].m_plane.y = -normal.y;
			facesA[fidx].m_plane.z = -normal.z;
			facesA[fidx].m_plane.w = c;
			facesA[fidx].m_numIndices = 3;
		}
		fidx++;

		bool addEdgePlanes = true;
		if (addEdgePlanes)
		{
			int numVertices = 3;
			int prevVertex = numVertices - 1;
			for (int i = 0; i < numVertices; i++)
			{
				b3Float4 v0 = verticesA[i];
				b3Float4 v1 = verticesA[prevVertex];

				b3Float4 edgeNormal = b3Normalized(b3Cross(normal, v1 - v0));
				float c = -b3Dot(edgeNormal, v0);

				facesA[fidx].m_numIndices = 2;
				facesA[fidx].m_indexOffset = curUsedIndices;
				indicesA[curUsedIndices++] = i;
				indicesA[curUsedIndices++] = prevVertex;

				facesA[fidx].m_plane.x = edgeNormal.x;
				facesA[fidx].m_plane.y = edgeNormal.y;
				facesA[fidx].m_plane.z = edgeNormal.z;
				facesA[fidx].m_plane.w = c;
				fidx++;
				prevVertex = i;
			}
		}
		convexPolyhedronA.m_numFaces = B3_TRIANGLE_NUM_CONVEX_FACES;
		convexPolyhedronA.m_localCenter = localCenter * (1.f / 3.f);

		b3Float4 posA = rigidBodies[bodyIndexA].m_pos;
		posA.w = 0.f;
		b3Float4 posB = rigidBodies[bodyIndexB].m_pos;
		posB.w = 0.f;

		b3Quaternion ornA = rigidBodies[bodyIndexA].m_quat;
		b3Quaternion ornB = rigidBodies[bodyIndexB].m_quat;

		///////////////////
		///compound shape support

		if (collidables[collidableIndexB].m_shapeType == SHAPE_COMPOUND_OF_CONVEX_HULLS)
		{
			int compoundChild = concavePairs[pairIdx].w;
			int childShapeIndexB = compoundChild;  //collidables[collidableIndexB].m_shapeIndex+compoundChild;
			int childColIndexB = gpuChildShapes[childShapeIndexB].m_shapeIndex;
			b3Float4 childPosB = gpuChildShapes[childShapeIndexB].m_childPosition;
			b3Quaternion childOrnB = gpuChildShapes[childShapeIndexB].m_childOrientation;
			b3Float4 newPosB = b3TransformPoint(childPosB, posB, ornB);
			b3Quaternion newOrnB = b3QuatMul(ornB, childOrnB);
			posB = newPosB;
			ornB = newOrnB;
			shapeIndexB = collidables[childColIndexB].m_shapeIndex;
		}
		//////////////////

		b3Float4 c0local = convexPolyhedronA.m_localCenter;
		b3Float4 c0 = b3TransformPoint(c0local, posA, ornA);
		b3Float4 c1local = convexShapes[shapeIndexB].m_localCenter;
		b3Float4 c1 = b3TransformPoint(c1local, posB, ornB);
		const b3Float4 DeltaC2 = c0 - c1;

		bool sepA = b3FindSeparatingAxis(&convexPolyhedronA, &convexShapes[shapeIndexB],
										 posA, ornA,
										 posB, ornB,
										 DeltaC2,
										 verticesA, uniqueEdgesA, facesA, indicesA,
										 vertices, uniqueEdges, faces, indices,
										 &sepAxis, &dmin);
		hasSeparatingAxis = 4;
		if (!sepA)
		{
			hasSeparatingAxis = 0;
		}
		else
		{
			bool sepB = b3FindSeparatingAxis(&convexShapes[shapeIndexB], &convexPolyhedronA,
											 posB, ornB,
											 posA, ornA,
											 DeltaC2,
											 vertices, uniqueEdges, faces, indices,
											 verticesA, uniqueEdgesA, facesA, indicesA,
											 &sepAxis, &dmin);

			if (!sepB)
			{
				hasSeparatingAxis = 0;
			}
			else
			{
				bool sepEE = b3FindSeparatingAxisEdgeEdge(&convexPolyhedronA, &convexShapes[shapeIndexB],
														  posA, ornA,
														  posB, ornB,
														  DeltaC2,
														  verticesA, uniqueEdgesA, facesA, indicesA,
														  vertices, uniqueEdges, faces, indices,
														  &sepAxis, &dmin, true);

				if (!sepEE)
				{
					hasSeparatingAxis = 0;
				}
				else
				{
					hasSeparatingAxis = 1;
				}
			}
		}

		if (hasSeparatingAxis)
		{
			hasSeparatingNormals[i] = 1;
			sepAxis.w = dmin;
			concaveSeparatingNormalsOut[pairIdx] = sepAxis;

			//now compute clipping faces A and B, and world-space clipping vertices A and B...

			float minDist = -1e30f;
			float maxDist = 0.02f;

			b3FindClippingFaces(sepAxis,
								&convexPolyhedronA,
								&convexShapes[shapeIndexB],
								posA, ornA,
								posB, ornB,
								worldVertsA1Out,
								worldNormalsA1Out,
								worldVertsB1Out,
								vertexFaceCapacity,
								minDist, maxDist,
								verticesA,
								facesA,
								indicesA,

								vertices,
								faces,
								indices,
								clippingFacesOut, pairIdx);
		}
		else
		{
			//mark this pair as in-active
			concavePairs[pairIdx].w = -1;
		}
	}
	else
	{
		//mark this pair as in-active
		concavePairs[pairIdx].w = -1;
	}
}

#endif  //B3_FIND_CONCAVE_SEPARATING_AXIS_H
