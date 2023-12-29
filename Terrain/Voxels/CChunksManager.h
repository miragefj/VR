#pragma once
#include "voxel.h"
#include "SimplexNoise.h"
#include "thread.h"

//27.10.2021
#include "CPerfomanceCalculator.h"
#include "CommonUtils.h"

//OK: ����������� ������ �������� � ���������� ������� ������ �� ������� (�����������)
//xpos = min.x / chunk_width
//ypos = min.y / chunk_height
//zpos = min.z / chunk_width

extern CRITICAL_SECTION crit_section;
extern CThreadPool tp;

#define GLOBALCOORD2LOCAL(x, w) (x / w)


class CChunksManager;

struct chunk_information_s
{
	int xpos;
	int zpos;
	int chunk_width;
	int chunk_height;
	CChunk *p_chunk;
};

//Region Controller
class CChunksManager
{
public:
	CChunksManager();
	~CChunksManager();

	int Init(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height);

	CChunk *GetRegionChunk(int gx, int gy, int gz);
	CVoxel *GetRegionVoxel(int gx, int gy, int gz); //!!!!
	bool GetVoxelByRay(CChunk **ppchunk, CVoxel **ppvox, CRay &ray, vec3 *pos, bool *krai, float distance, int checkflag);
	bool RemoveSolidVoxel(CRay &ray, float distance, int newflag = VOXEL_FLAG_AIR);
	bool PlaceSolidVoxel(CRay &ray, float distance, int newflag = VOXEL_FLAG_SOLID);
	void SetVoxel(vec3int position, int flags);

	inline int GetRegionWidth() { return m_nChunkWidth * m_nChunksPerWidth; }
	inline int GetRegionHeight() { return m_nChunkHeight * m_nChunksPerHeight; }

	int m_nChunksPerWidth;
	int m_nChunksPerHeight;
	CChunk *m_pChunks;
	int m_nChunkWidth;
	int m_nChunkHeight;
	int m_nNumberOfChunks;
private:
	void iskl(vec3 *pos, int newflag = VOXEL_FLAG_AIR);
	void iskl_krai(vec3 *pos, int newflag = VOXEL_FLAG_AIR);
};

///////////////////////////////////////////////////////////////////////////////////
class CChunksGeneratorTest
{
public:
	CChunksGeneratorTest();
	~CChunksGeneratorTest();

	

	//int StartGeneration(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height); //gavna
	void InitProperties(int distance, int chunk_width, int chunk_height);
	int StartGeneration2(vec3int &playerpos, int distance, int chunk_width, int chunk_height);


	void Update(vec3int pos); //���������� ����������� ���� ��������� ������

	vec3int m_ChunksDir;
	vec3int m_nbag_min_chunk;
	vec3int m_nbag_max_chunk;

	int m_nDistanceInChunks;
	int m_nChunksPerWidth;
	int m_nChunksPerHeight;
	int m_nChunkWidth;
	int m_nChunkHeight;
	int m_nNumberOfChunks;

	//���������� �������
	//void OnChunksLoad(vec3int &playerpos); //����� ��������� ������
	
	CChunk * m_Chunks;
};