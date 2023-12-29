#include "CChunksManager.h"

CChunksManager::CChunksManager()
{
}

CChunksManager::~CChunksManager()
{
}

int CChunksManager::Init(int chunk_width, int chunk_height, int chunks_per_width, int chunks_per_height)
{
	m_nChunkWidth = chunk_width;
	m_nChunkHeight = chunk_height;
	m_nChunksPerWidth = chunks_per_width;
	m_nChunksPerHeight = chunks_per_height;
	m_nNumberOfChunks = chunks_per_width * chunks_per_width * chunks_per_height;
	m_pChunks = new CChunk[m_nNumberOfChunks];
	CChunk *p_current_chunk = NULL;
	for (int y = 0; y < chunks_per_height; y++) {
		for (int x = 0; x < chunks_per_width; x++) {
			for (int z = 0; z < chunks_per_width; z++) {
				p_current_chunk = &m_pChunks[COORD2OFFSET2(chunks_per_height, chunks_per_width, x, y, z)];
				p_current_chunk->Init(vec3int(x * chunk_width, y * chunk_height, z * chunk_width),
					chunk_width,
					chunk_height,
					VOXEL_FLAG_AIR);

				p_current_chunk->DebugDraw_ChunkBounds(false);
				p_current_chunk->MarkIdle(false);
				p_current_chunk->RebuildMesh();
			}
		}
	}
	return 0;
}

CVoxel *CChunksManager::GetRegionVoxel(int gx, int gy, int gz)
{
	CChunk *pchunk = GetRegionChunk(gx, gy, gz);
	if (!pchunk)
		return NULL;

	return pchunk->VoxelAt(gx % pchunk->GetChunkWidth(),
						   gy % pchunk->GetChunkHeight(),
					       gz % pchunk->GetChunkWidth());
}

CChunk *CChunksManager::GetRegionChunk(int gx, int gy, int gz)
{
	int xd = GLOBALCOORD2LOCAL(gx, m_nChunkWidth);
	int yd = GLOBALCOORD2LOCAL(gy, m_nChunkHeight);
	int zd = GLOBALCOORD2LOCAL(gz, m_nChunkWidth);

	if (gx == m_nChunkWidth * m_nChunksPerWidth || gy == m_nChunkHeight * m_nChunksPerHeight || gz == m_nChunkWidth * m_nChunksPerWidth) {
		if (gx == m_nChunkWidth * m_nChunksPerWidth) {
			xd--;
		}
		if (gy == m_nChunkHeight * m_nChunksPerHeight) {
			yd--;
		}
		if (gz == m_nChunkWidth * m_nChunksPerWidth) {
			zd--;
		}
	}

	//check bounds
	if (xd < 0 || yd < 0 || zd < 0)
		return NULL;

	if (xd >= m_nChunksPerWidth || yd >= m_nChunksPerHeight || zd >= m_nChunksPerWidth)
		return NULL;

	return &m_pChunks[COORD2OFFSET2(m_nChunksPerWidth, m_nChunksPerHeight, xd, yd, zd)];
}


void CChunksManager::iskl_krai(vec3 *pos, int newflag) {
	CChunk *pchunk;
	CVoxel *pvoxel;
	vec3int ipos = vec3int((int)pos->x, (int)pos->y, (int)pos->z);
	int sx = ipos.x % m_nChunkWidth;
	int sy = ipos.y % m_nChunkHeight;
	int sz = ipos.z % m_nChunkWidth;
	if (ipos.z == (m_nChunkWidth * m_nChunksPerWidth) && ipos.y == (m_nChunkHeight * m_nChunksPerHeight) && ipos.x == (m_nChunkWidth * m_nChunksPerWidth)) {
		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}

	//
	//	y z
	//
	if (ipos.z == (m_nChunkWidth * m_nChunksPerWidth) && ipos.y == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}

	//
	//	y x
	//
	if (ipos.x == (m_nChunkWidth * m_nChunksPerWidth) && ipos.y == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sz) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}

	//
	//	x z
	//
	if (ipos.x == (m_nChunkWidth * m_nChunksPerWidth) && ipos.z == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z - m_nChunkWidth);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		return;
	}

	//
	//	x
	//
	if (ipos.x == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz && !sy) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}

	//
	//	y
	//
	if (ipos.y == (m_nChunkHeight * m_nChunksPerHeight)) {
		if (!sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz) {
			pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sz && !sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, sz);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}

	//
	//	z
	//
	if (ipos.z == (m_nChunkWidth * m_nChunksPerWidth)) {
		if (!sy) {
			pchunk = GetRegionChunk(ipos.x, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		if (!sy && !sx) {
			pchunk = GetRegionChunk(ipos.x - m_nChunkWidth, ipos.y - m_nChunkHeight, ipos.z - m_nChunkWidth);
			if (pchunk) {
				pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
				pvoxel->SetFlag(newflag);
				pchunk->RebuildMesh();
			}
		}

		pchunk = GetRegionChunk(ipos.x, ipos.y, ipos.z - m_nChunkWidth);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(sx, sy, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
}

void CChunksManager::iskl(vec3 *pos, int newflag)
{
	CChunk *pchunk;
	CVoxel *pvoxel;
	int sx = int(pos->x) % m_nChunkWidth;
	int sy = int(pos->y) % m_nChunkHeight;
	int sz = int(pos->z) % m_nChunkWidth;
	if (!sx) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y, (int)pos->z);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sy) {
		pchunk = GetRegionChunk((int)pos->x, (int)pos->y - m_nChunkHeight, (int)pos->z);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sz) {
		pchunk = GetRegionChunk((int)pos->x, (int)pos->y, (int)pos->z - m_nChunkWidth);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(sx, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sx && !sy) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y - m_nChunkHeight, (int)pos->z);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, sz);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sz && !sy) {
		pchunk = GetRegionChunk((int)pos->x, (int)pos->y - m_nChunkHeight, (int)pos->z - m_nChunkWidth);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(sx, m_nChunkHeight, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sx && !sz) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y, (int)pos->z - m_nChunkWidth);
		if (pchunk) {
			pvoxel = pchunk->VoxelAt(m_nChunkWidth, sy, m_nChunkWidth);
			pvoxel->SetFlag(newflag);
			pchunk->RebuildMesh();
		}
	}

	if (!sz && !sy && !sx) {
		pchunk = GetRegionChunk((int)pos->x - m_nChunkWidth, (int)pos->y - m_nChunkHeight, (int)pos->z - m_nChunkWidth);
		if (!(pchunk))
			return;

		pvoxel = pchunk->VoxelAt(m_nChunkWidth, m_nChunkHeight, m_nChunkWidth);
		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
	}
}

bool CChunksManager::GetVoxelByRay(CChunk **ppchunk, CVoxel **ppvox, CRay &ray, vec3 *pos, bool *krai, float distance, int checkflag)
{
	float dist = 0.f;
	while (dist < distance) {
		*(pos) = ray.m_origin + ray.m_direction * dist;
		round_vector(*(pos), *(pos), 1.0f);
		*(ppchunk) = GetRegionChunk((int)pos->x, (int)pos->y, (int)pos->z);
		if (!*(ppchunk))
			goto __end;

		*krai = (int(pos->x) == m_nChunkWidth * m_nChunksPerWidth || int(pos->y) == m_nChunkHeight * m_nChunksPerHeight || int(pos->z) == m_nChunkWidth * m_nChunksPerWidth);

		*(ppvox) = (*ppchunk)->VoxelAt(
			int(pos->x) == m_nChunkWidth * m_nChunksPerWidth ? m_nChunkWidth : (int(pos->x) % m_nChunkWidth),
			int(pos->y) == m_nChunkHeight * m_nChunksPerHeight ? m_nChunkHeight : (int(pos->y) % m_nChunkHeight),
			int(pos->z) == m_nChunkWidth * m_nChunksPerWidth ?  m_nChunkWidth : (int(pos->z) % m_nChunkWidth)
		);
		
		if (!*(ppvox))
			goto __end;

		if ((*ppvox)->GetFlags() & checkflag) {
			return true;
		}
	__end:
		dist++;
	}
	return false;
}

bool CChunksManager::RemoveSolidVoxel(CRay &ray, float distance, int newflag)
{
	vec3 *pos;
	bool krai;
	pos = (vec3*)&vec3(-1, -1, -1);
	CChunk *pchunk;
	CVoxel *pvoxel;
	if (GetVoxelByRay(&pchunk, &pvoxel, ray, pos, &krai, distance, VOXEL_FLAG_SOLID)) {
		if (!pchunk || !pvoxel)
			return false;

		pvoxel->SetFlag(newflag);
		pchunk->RebuildMesh();
		
		if (krai) {
			iskl_krai(pos, newflag);
		}
		else if(!krai){
			iskl(pos, newflag);
		}

		return true;
	}
	return false;
}

bool CChunksManager::PlaceSolidVoxel(CRay &ray, float distance, int newflag)
{
	return false;
}

void CChunksManager::SetVoxel(vec3int position, int flags)
{
	//check is not valid coords
	if(position.x < 0 || position.y < 0 || position.z < 0)
		return;

	vec3 voxpos(position.x, position.y, position.z);
	if (position.x == GetRegionWidth() || position.y == GetRegionHeight() || position.z == GetRegionWidth()) {
		iskl_krai(&voxpos, flags);
	}
	else
	{
		CVoxel *pvox = GetRegionVoxel(position.x, position.y, position.z);
		if (!pvox)
			return;

		pvox->SetFlag(flags);
		iskl(&voxpos, flags);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChunksGeneratorTest::CChunksGeneratorTest()
{
}

CChunksGeneratorTest::~CChunksGeneratorTest()
{
}

void CChunksGeneratorTest::InitProperties(int distance, int chunk_width, int chunk_height)
{
	m_nChunkWidth = chunk_width;
	m_nChunkHeight = chunk_height;
	m_nDistanceInChunks = distance;

	int layer_area = (m_nDistanceInChunks * 2) + 1;
	m_nNumberOfChunks = (layer_area * layer_area);
}

UINT peremennaya;

static void LoadChankLOGIC(threadpool_information threadpoolinf, void *parameter) {
	//printf("%d\n", threadpoolinf.total_objects_in_queue);
	CChunk * chunk = (CChunk *)parameter;
	
	//threadpoolinf.cs->Lock();
	if (!chunk->m_pVoxels) chunk->AllocVoxels(chunk->m_nMeshgenClipWidth, chunk->m_nMeshgenClipHeight, VOXEL_FLAG_AIR);
	//else 
	//threadpoolinf.cs->Unlock();

	SimplexNoise perlin;
	float this_height;
	float global_noid;
	//float freq = 0.025f;

	for (int x = chunk->m_ChunkPos.x; x < chunk->m_vecMax.x+1; x++) {
		for (int z = chunk->m_ChunkPos.z; z < chunk->m_vecMax.z+1; z++) {

			double  nx = x / 1000.f, nz = z / 1000.f;
			this_height = (1.00f *  perlin.noise(1.f *	nx, 1.f *  nz)
						  + 0.50f * perlin.noise(2.f *  nx, 2.f *  nz)
						  + 0.25f * perlin.noise(4.f *  nx, 4.f *  nz)
						  + 0.13f * perlin.noise(8.f *  nx, 8.f *  nz)
						  + 0.06f * perlin.noise(16.f * nx, 16.f * nz)
						  + 0.03f * perlin.noise(32.f * nx, 32.f * nz));
			this_height /= (1.00f + 0.50f + 0.25f + 0.13f + 0.06f + 0.03f);
			this_height = pow(this_height, 2.00f);
			this_height *= 300;
			this_height += 63;
			chunk->vox_max = max(chunk->vox_max, this_height);
			chunk->vox_min = min(chunk->vox_min, this_height);
			for (int y = chunk->m_ChunkPos.y; y < chunk->m_vecMax.y; y++) {
				if (abs(y) <= this_height )
					chunk->m_pVoxels[
						COORD2OFFSET2(chunk->m_nMeshgenClipWidth, chunk->m_nMeshgenClipHeight, abs(x - chunk->m_ChunkPos.x),
							abs(y - chunk->m_ChunkPos.y), abs(z - chunk->m_ChunkPos.z))].m_Flags=VOXEL_FLAG_SOLID;
			}
		}
	}
	chunk->RebuildMesh();
	InterlockedExchange8((char *)&chunk->has_chank, false);
	//printf("%d %d \n",chunk->m_vertices.size(),chunk->m_vertices.size());
	InterlockedDecrement(&peremennaya);
}

int CChunksGeneratorTest::StartGeneration2(vec3int &playerpos, int distance, int chunk_width, int chunk_height)
{
	InitProperties(distance, chunk_width, chunk_height);
	m_Chunks = (CChunk *)calloc(m_nNumberOfChunks , sizeof CChunk);
	
	peremennaya = m_nNumberOfChunks;
	int indx = 0;
	CMutex m_cs;
	m_cs.Init();
	for (int x = -m_nDistanceInChunks; x <= m_nDistanceInChunks; x++) {
		for (int z = -m_nDistanceInChunks; z <= m_nDistanceInChunks; z++) {

			m_Chunks[indx].has_chank = true;

			//m_Chunks[indx].m_vertices.Init_Cs(&m_cs);
			//m_Chunks[indx].m_normals.Init_Cs(&m_cs);
			//m_Chunks[indx].m_uvs.Init_Cs(&m_cs);
			//m_Chunks[indx].m_indices.Init_Cs(&m_cs);

			m_Chunks[indx].m_nWidth = m_nChunkWidth;
			m_Chunks[indx].m_nHeight = m_nChunkHeight;
			m_Chunks[indx].m_nMeshgenClipWidth = m_nChunkWidth + 1;
			m_Chunks[indx].m_nMeshgenClipHeight = m_nChunkHeight + 1;
			m_Chunks[indx].m_nDDBounds = 1;
			
			m_Chunks[indx].m_ChunkPos = vec3int(x * m_nChunkWidth, 0, z * m_nChunkWidth);
				
			m_Chunks[indx].m_vecMax.x = m_Chunks[indx].m_ChunkPos.x + m_nChunkWidth;
			m_Chunks[indx].m_vecMax.y = m_Chunks[indx].m_ChunkPos.y + m_nChunkHeight;
			m_Chunks[indx].m_vecMax.z = m_Chunks[indx].m_ChunkPos.z + m_nChunkWidth;

			m_Chunks[indx].vox_max = m_Chunks[indx].m_vecMax.y;
			m_Chunks[indx].vox_min = 0;

			
			tp.AddTask(LoadChankLOGIC, &m_Chunks[indx]);


			CChunk *p_curr_chunk = &m_Chunks[indx];
			//if (!p_curr_chunk->m_ChunkPos.x && !p_curr_chunk->m_ChunkPos.y && !p_curr_chunk->m_ChunkPos.z) //так же проверка по центру мира а не по месту спавна игрока
			//	m_pMainChunk = p_curr_chunk;
			indx++;
		}
	}
	while(peremennaya) {
		Sleep(100);
	}
	return 1;
}

void CChunksGeneratorTest::Update(vec3int pos)
{
	//bool * mat_chunk = (bool *)calloc(441, sizeof(bool));
	std::vector<int> id_chunks;
	//std::vector<vec3int> zeros;
	vec3int chunk_pos = vec3int((round(pos.x / m_nChunkWidth) * m_nChunkWidth), 0, (round(pos.z / m_nChunkWidth) * m_nChunkWidth));
	if (chunk_pos.z != m_nbag_min_chunk.z || chunk_pos.x != m_nbag_min_chunk.x) {

		vec3int new_chunk_dir;
		new_chunk_dir = vec3int((chunk_pos.x - m_nbag_min_chunk.x), 0, (chunk_pos.z - m_nbag_min_chunk.z));
		printf("%d %d %d\n", new_chunk_dir.x, new_chunk_dir.y, new_chunk_dir.z);
		m_nbag_min_chunk = chunk_pos;
		for (int h = m_nNumberOfChunks - 1; h >= 0; h--)
		{
			if (abs(m_Chunks[h].m_ChunkPos.x - chunk_pos.x) > m_nDistanceInChunks * m_nChunkWidth || abs(m_Chunks[h].m_ChunkPos.z - chunk_pos.z) > m_nDistanceInChunks * m_nChunkWidth)
			{
				memset(m_Chunks[h].m_pVoxels, VOXEL_FLAG_AIR, ((m_Chunks[h].m_nMeshgenClipWidth * m_Chunks[h].m_nMeshgenClipWidth) * m_Chunks[h].m_nMeshgenClipHeight) * sizeof CVoxel);

				id_chunks.push_back(h);
			}
		}
		//vec3int _x[21];

		if (new_chunk_dir.x != 0 && new_chunk_dir.z != 0) {

			// (1)
			if (new_chunk_dir.x > 0 && new_chunk_dir.z > 0) {
				for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks - 1; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();

					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + (i * m_nChunkWidth);

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
				for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();
					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + (i * m_nChunkWidth);
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
			}
			// (2)
			if (new_chunk_dir.x < 0 && new_chunk_dir.z > 0) {
				for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks - 1; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();

					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + (i * m_nChunkWidth);

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
				for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();
					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + (i * m_nChunkWidth);
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
			}
			// (3)
			if (new_chunk_dir.x < 0 && new_chunk_dir.z < 0) {
				for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();

					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + (i * m_nChunkWidth);

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
				for (int i = -m_nDistanceInChunks + 1 ; i <= m_nDistanceInChunks; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();
					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + (i * m_nChunkWidth);
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
			}
			// (4)
			if (new_chunk_dir.x > 0 && new_chunk_dir.z < 0) {
				for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();

					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + (i * m_nChunkWidth);

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
				for (int i = -m_nDistanceInChunks + 1; i <= m_nDistanceInChunks; i++) {
					int id = id_chunks.back();
					id_chunks.pop_back();
					m_Chunks[id].has_chank = true;
					m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + (i * m_nChunkWidth);
					m_Chunks[id].m_ChunkPos.y = 0;
					m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;

					m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
					//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
					tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
				}
			}
		}
		else if (new_chunk_dir.x != 0) {
			for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
				int id = id_chunks.back();
				id_chunks.pop_back();

  				m_Chunks[id].has_chank = true;
				m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
				m_Chunks[id].m_ChunkPos.y = 0;
				m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + (i * m_nChunkWidth);

				m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
				m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
				m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;

				//_x[i + m_nDistanceInChunks] = m_Chunks[id].m_ChunkPos;
				//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
				tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
			}
		}

		///
		//
		///
		else if (new_chunk_dir.z != 0) {
			for (int i = -m_nDistanceInChunks; i <= m_nDistanceInChunks; i++) {
				int id = id_chunks.back();
				id_chunks.pop_back();
				m_Chunks[id].has_chank = true;
				m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + (i * m_nChunkWidth);
				m_Chunks[id].m_ChunkPos.y = 0;
				m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;

				m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
				m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
				m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
				//LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
				tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
			}
		}
		id_chunks.clear();
	}
}





/*

		//int x = -m_nDistanceInChunks;
		//int z = -m_nDistanceInChunks;
		//for (int h = 0; h < id_chunks.size(); h++)
		//{
		//	int id = id_chunks.back();
		//	id_chunks.pop_back();
		//	if (new_chunk_dir.x != 0 && x <= m_nDistanceInChunks) {
		//		m_Chunks[id].has_chank = true;
		//		m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
		//		m_Chunks[id].m_ChunkPos.y = 0;
		//		m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + (x * m_nChunkWidth);
		//
		//		m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
		//		m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
		//		m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
		//
		//		LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
		//		//tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
		//		x++;
		//		printf("x = %d\n", x + m_nDistanceInChunks);
		//	}
		//	else if (new_chunk_dir.z != 0 && z <= m_nDistanceInChunks) {
		//
		//		for (int i = 0; i < m_nNumberOfChunks; i++) {
		//			if (m_Chunks[i].m_ChunkPos.x == m_nbag_min_chunk.x + (z * m_nChunkWidth) && m_Chunks[i].m_ChunkPos.z == m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks) {
		//				h--;
		//				z++;
		//				//printf("gop stop\n");
		//				goto skip;
		//			}
		//		}
		//		m_Chunks[id].has_chank = true;
		//		m_Chunks[id].m_ChunkPos.x = m_nbag_min_chunk.x + (z * m_nChunkWidth);
		//		m_Chunks[id].m_ChunkPos.y = 0;
		//		m_Chunks[id].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;
		//
		//		m_Chunks[id].m_vecMax.x = m_Chunks[id].m_ChunkPos.x + m_nChunkWidth;
		//		m_Chunks[id].m_vecMax.y = m_Chunks[id].m_ChunkPos.y + m_nChunkHeight;
		//		m_Chunks[id].m_vecMax.z = m_Chunks[id].m_ChunkPos.z + m_nChunkWidth;
		//
		//		LoadChankLOGIC(threadpool_information(), &m_Chunks[id]);
		//		//tp.AddTask(LoadChankLOGIC, &m_Chunks[id]);
		//		z++;
		//	}
		//skip:
		//	if (1);
		//}
//for (int z = -m_nDistanceInChunks; z <= m_nDistanceInChunks; z++) {
		//	for (int x = -m_nDistanceInChunks; x <= m_nDistanceInChunks; x++) {
		//
		//		if (!mat_chunk[(z + m_nDistanceInChunks) * m_nDistanceInChunks + (x + m_nDistanceInChunks)]) {
		//			if (id_chunks.size()) {
		//				int h = id_chunks.back();
		//				id_chunks.pop_back();
		//
		//				memset(m_Chunks[h].m_pVoxels, VOXEL_FLAG_AIR, ((m_Chunks[h].m_nMeshgenClipWidth * m_Chunks[h].m_nMeshgenClipWidth) * m_Chunks[h].m_nMeshgenClipHeight) * sizeof CVoxel);
		//
		//				m_Chunks[h].has_chank = true;
		//
		//				m_Chunks[h].m_ChunkPos = vec3int(m_nbag_min_chunk.x + (x * m_nChunkWidth), 0, m_nbag_min_chunk.z + (z * m_nChunkWidth));
		//
		//				m_Chunks[h].m_vecMax.x = m_Chunks[h].m_ChunkPos.x + m_nChunkWidth;
		//				m_Chunks[h].m_vecMax.y = m_Chunks[h].m_ChunkPos.y + m_nChunkHeight;
		//				m_Chunks[h].m_vecMax.z = m_Chunks[h].m_ChunkPos.z + m_nChunkWidth;
		//
		//				tp.AddTask(LoadChankLOGIC, &m_Chunks[h]);
		//				mat_chunk[(z + m_nDistanceInChunks) * m_nDistanceInChunks + (x + m_nDistanceInChunks)] = true;
		//			}
		//		}
		//	}
		//}
	if (chunk_pos.z != m_nbag_min_chunk.z || chunk_pos.x != m_nbag_min_chunk.x) {
		vec3int new_chunk_dir;
		new_chunk_dir = vec3int((chunk_pos.x - m_nbag_min_chunk.x), 0, (chunk_pos.z - m_nbag_min_chunk.z));

		m_nbag_min_chunk = chunk_pos;

		m_nbag_max_chunk.x = chunk_pos.x + m_nChunkWidth;
		m_nbag_max_chunk.y = chunk_pos.y + m_nChunkHeight;
		m_nbag_max_chunk.z = chunk_pos.z + m_nChunkWidth;

		//printf("%d %d %d \n", new_chunk_dir.x, new_chunk_dir.y, new_chunk_dir.z);
		int j = -m_nDistanceInChunks;
		int k = -m_nDistanceInChunks;
		for (int h = 0; h < m_nNumberOfChunks; h++) {
			int ax = (abs(m_Chunks[h].m_ChunkPos.x - chunk_pos.x) / m_nChunkWidth);
			int az = (abs(m_Chunks[h].m_ChunkPos.z - chunk_pos.z) / m_nChunkWidth);
			if (ax > m_nDistanceInChunks && j <= m_nDistanceInChunks || az > m_nDistanceInChunks && k <= m_nDistanceInChunks && !m_Chunks[h].has_chank) {
				//printf("ID = %-3d, coord (ax = %-3d, az = %-3d), dir ( %-3d %-3d %-3d) (", h, ax, az, new_chunk_dir);
				memset(m_Chunks[h].m_pVoxels, VOXEL_FLAG_AIR, ((m_Chunks[h].m_nMeshgenClipWidth * m_Chunks[h].m_nMeshgenClipWidth) * m_Chunks[h].m_nMeshgenClipHeight) * sizeof CVoxel);
				if (ax > m_nDistanceInChunks && j <= m_nDistanceInChunks) {
					//printf("ax = ID = %-3d) ", h);
					for (int i = 0; i < m_nNumberOfChunks; i++) {
						if (m_Chunks[i].m_ChunkPos.x == m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks && m_Chunks[i].m_ChunkPos.z == m_nbag_min_chunk.z + (j * m_nChunkWidth)) {
							h--;
							j++;
							//printf("gop stop\n");
							goto skip;
						}
					}

					m_Chunks[h].m_ChunkPos.x = m_nbag_min_chunk.x + new_chunk_dir.x * m_nDistanceInChunks;
					m_Chunks[h].m_ChunkPos.y = 0;
					m_Chunks[h].m_ChunkPos.z = m_nbag_min_chunk.z + (j * m_nChunkWidth);

					m_Chunks[h].m_vecMax.x = m_Chunks[h].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[h].m_vecMax.y = m_Chunks[h].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[h].m_vecMax.z = m_Chunks[h].m_ChunkPos.z + m_nChunkWidth;

					m_Chunks[h].has_chank = true;
					LoadChankLOGIC(threadpool_information(), &m_Chunks[h]);
					//tp.AddTask(LoadChankLOGIC, &m_Chunks[h]);

					//printf("( new ax = %-3d ) ( new az = %-3d ) \n", (abs(m_Chunks[h].m_ChunkPos.z - chunk_pos.z) / m_nChunkWidth), (abs(m_Chunks[h].m_ChunkPos.x - chunk_pos.x) / m_nChunkWidth));
					j++;
				}
				else if (az > m_nDistanceInChunks && k <= m_nDistanceInChunks) {
					//printf("az = ID = %-3d ) ", h);
					for (int i = 0; i < m_nNumberOfChunks; i++) {
						if (m_Chunks[i].m_ChunkPos.x == m_nbag_min_chunk.x + (k * m_nChunkWidth) && m_Chunks[i].m_ChunkPos.z == m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks) {
							h--;
							k++;
							//printf("gop stop\n");
							goto skip;
						}
					}

					m_Chunks[h].m_ChunkPos.x = m_nbag_min_chunk.x + (k * m_nChunkWidth);
					m_Chunks[h].m_ChunkPos.y = 0;
					m_Chunks[h].m_ChunkPos.z = m_nbag_min_chunk.z + new_chunk_dir.z * m_nDistanceInChunks;

					m_Chunks[h].m_vecMax.x = m_Chunks[h].m_ChunkPos.x + m_nChunkWidth;
					m_Chunks[h].m_vecMax.y = m_Chunks[h].m_ChunkPos.y + m_nChunkHeight;
					m_Chunks[h].m_vecMax.z = m_Chunks[h].m_ChunkPos.z + m_nChunkWidth;

					m_Chunks[h].has_chank = true;

					LoadChankLOGIC(threadpool_information(), &m_Chunks[h]);
					//tp.AddTask(LoadChankLOGIC, &m_Chunks[h]);

					//printf("( new ax = %-3d ) ( new az = %-3d ) \n", (abs(m_Chunks[h].m_ChunkPos.z - chunk_pos.z) / m_nChunkWidth), (abs(m_Chunks[h].m_ChunkPos.x - chunk_pos.x) / m_nChunkWidth));
					k++;
				}
			skip:
				if (0) {}
			}
		}
*/