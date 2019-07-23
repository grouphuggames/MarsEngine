#pragma once
#include "Core.h"
#include "vec2.h"
#include "vec3.h"
#include "MVector.h"


namespace Mars
{
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
	};

	struct IndexPair
	{
		u32 index;
		vec3 value;
	};

	bool LoadOBJ(const char* path, MVector<Vertex>& verts, MVector<u32>& indices)
	{
		MVector<u32> vert_indices, uv_indices, normal_indices;
		MVector<vec3> tmp_verts;
		MVector<vec2> tmp_uvs;
		MVector<vec3> tmp_normals;

		MVector<vec3> out_verts;
		MVector<vec2> out_uvs;
		MVector<vec3> out_normals;

		FILE* file = fopen(path, "r");
		if (!file)
		{
			MARS_CORE_ERROR("Could not open file: ", path);
			return false;
		}

		while (true)
		{
			char header[128];

			int res = fscanf(file, "%s", header);
			if (res == EOF)
			{
				break;
			}

			if (strcmp(header, "v") == 0)
			{
				f32 x, y, z;
				fscanf(file, "%f %f %f", &x, &y, &z);
				tmp_verts.PushBack(vec3(x, y, z));
			}
			else if (strcmp(header, "vt") == 0)
			{
				f32 x, y;
				fscanf(file, "%f %f %f", &x, &y);
				tmp_uvs.PushBack(vec2(x, y));
			}
			else if (strcmp(header, "vn") == 0)
			{
				f32 x, y, z;
				fscanf(file, "%f %f %f", &x, &y, &z);
				tmp_normals.PushBack(vec3(x, y, z));
			}
			else if (strcmp(header, "f") == 0)
			{
				u32 vert_index[3], uv_index[3], normal_index[3];

				s32 matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vert_index[0], &uv_index[0], &normal_index[0], &vert_index[1], &uv_index[1], &normal_index[1], &vert_index[2], &uv_index[2], &normal_index[2]);
				if (matches != 9)
				{
					MARS_CORE_ERROR("File cannot be parsed correctly. Make sure you are exporting with the proper options!!");
					return false;
				}

				vert_indices.PushBack(vert_index[0]);
				vert_indices.PushBack(vert_index[1]);
				vert_indices.PushBack(vert_index[2]);
				uv_indices.PushBack(uv_index[0]);
				uv_indices.PushBack(uv_index[1]);
				uv_indices.PushBack(uv_index[2]);
				normal_indices.PushBack(normal_index[0]);
				normal_indices.PushBack(normal_index[1]);
				normal_indices.PushBack(normal_index[2]);
			}
		}

		for (u32 i = 0; i < vert_indices.Size(); i++)
		{
			u32 vert_index = vert_indices[i];
			vec3 vert = tmp_verts[vert_index - 1];
			out_verts.PushBack(vert);
		}

		for (u32 i = 0; i < uv_indices.Size(); i++)
		{
			u32 uv_index = uv_indices[i];
			vec2 uv = tmp_uvs[uv_index - 1];
			out_uvs.PushBack(uv);
		}

		for (u32 i = 0; i < normal_indices.Size(); i++)
		{
			u32 normal_index = normal_indices[i];
			vec3 normal = tmp_normals[normal_index - 1];
			out_normals.PushBack(normal);
		}

		s32 total_verts = Max(Max(out_verts.Size(), out_uvs.Size()), out_normals.Size());
		for (s32 i = 0; i < total_verts; i++)
		{
			Vertex vertex;
			vertex.position = out_verts[i];
			vertex.normal = out_normals[i];
			vertex.uv = out_uvs[i];
			verts.PushBack(vertex);
		}

		verts.Resize(verts.Size() + 1);

		MVector<IndexPair> other_vals;

		IndexPair tmp;
		tmp.index = 0;
		tmp.value = verts[0].position;
		other_vals.PushBack(tmp);
		indices.PushBack(0);

		for (u32 i = 1; i < verts.Size(); i++)
		{
			for (u32 j = 0; j < other_vals.Size(); j++)
			{
				if (verts[i].position == other_vals[j].value)
				{
					indices.PushBack(j);
				}
				else
				{
					indices.PushBack(i);
				}
			}
		}
	}
}
