#include "pch.h"
#include "Resources.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "PathManager.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "MeshData.h"

#include "DirectoryManager.h"
#include "MeshCollider.h"

void Resources::Init()
{


	CreateDefaultTexture();
	CreateDefaultShader();
	CreateDefaultLoad();
	CreateDefaultMaterial();
	CreateDefaultGameObject();
	CreateDefaultComponent();

}

void Resources::End()
{
	SaveObjectList(SCENE_PATH);
}

void Resources::SaveObjectList(const wstring& path)
{
	wstring pullPath = path + L"ResourcesList" + FILE_EXPENSION;

	std::ofstream file(pullPath);

	Json::Value resource;

//#pragma region GameObject
//	{
//		Json::Value info;
//		for (const pair<wstring, Ref<Object>>& obj : _resources[static_cast<uint8>(OBJECT_TYPE::GAMEOBJECT)]) {
//			info.append(Utils::Wstr2Str(obj.first));
//		}
//		resource["GameObject"] = info;
//	}
//#pragma endregion
//
//#pragma region Component
//	{
//		Json::Value info;
//		for (const pair<wstring, Ref<Object>>& obj : _resources[static_cast<uint8>(OBJECT_TYPE::COMPONENT)]) {
//			info.append(Utils::Wstr2Str(obj.first));
//		}
//		resource["Component"] = info;
//	}
//#pragma endregion
//
//#pragma region Material
//	{
//		Json::Value info;
//		for (const pair<wstring, Ref<Object>>& obj : _resources[static_cast<uint8>(OBJECT_TYPE::MATERIAL)]) {
//			info.append(Utils::Wstr2Str(obj.first));
//		}
//		resource["Material"] = info;
//	}
//#pragma endregion

#pragma region Mesh
	{
		Json::Value info;
		for (const pair<wstring, Ref<Object>>& obj : _resources[static_cast<uint8>(OBJECT_TYPE::MESH)]) {
			info.append(Utils::Wstr2Str(obj.first));
			//append(Utils::Wstr2Str(obj.first).data());		c_ptr
		}
		resource["Mesh"] = info;
	}
#pragma endregion


	Json::StreamWriterBuilder builder;
	unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(resource, &file);

	file.close();
}

void Resources::LoadObjectList(const wstring& path)
{
	wstring pullPath = path + L"ResourcesList" + FILE_EXPENSION;
	std::ifstream file(pullPath);

	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs; 
	Json::Value resource;
	if (parseFromStream(builder, file, &resource, &errs) != true) {
		return;
	}

#pragma region Mesh
	{
		vector<string> filePaths;

		Json::Value iterator = resource["Mesh"];
		for (Json::ValueIterator iter = iterator.begin(); iter != iterator.end(); iter++) {
			filePaths.push_back(iter->asCString());
		}

		
		for (int32 i = 0; i < filePaths.size(); ++i) {
			wstring key = Utils::Str2Wstr(filePaths[i]);
			Load<Mesh>(key, path);
		}
	}
#pragma endregion

	file.close();
}

Ref<Mesh> Resources::LoadRectangleMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Rectangle");
	if (findMesh) {
		return findMesh;
	}
	float w2 = 0.5f;
	float h2 = 0.5f;

	vector<Vertex> vec(4);

	// ¾Õ¸é
	vec[0] = Vertex(Vec3(-w2, -h2, 0), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, 0), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, 0), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, 0), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(6);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Rectangle", mesh);

	return mesh;
}

Ref<Mesh> Resources::LoadCircleMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Circle");
	if (findMesh) {
		return findMesh;
	}
	uint32 sliceCount = 20;

	vector<Vertex> vertices;
	vector<uint32> indices;

	// À­ ¶Ñ²±
	{
		// ¹öÅØ½º
		float y = 0.0f;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = 1.0f * ::cosf(i * phi);
			float z = 1.0f * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		}

		vertices.push_back(Vertex(Vec3(0.0f, y, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));


		// ÀÎµ¦½º
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);
		}
	}
	
	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vertices, indices);
	Add(L"Circle", mesh);

	return mesh;
}

Ref<Mesh> Resources::LoadCubeMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Cube");
	if (findMesh) {
		return findMesh;
	}
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<Vertex> vec(24);

	// ¾Õ¸é
	vec[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// µÞ¸é
	vec[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// À­¸é
	vec[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// ¾Æ·§¸é
	vec[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ¿ÞÂÊ¸é
	vec[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// ¿À¸¥ÂÊ¸é
	vec[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	vector<uint32> idx(36);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	// µÞ¸é
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;

	// À­¸é
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;

	// ¾Æ·§¸é
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;

	// ¿ÞÂÊ¸é
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;

	// ¿À¸¥ÂÊ¸é
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Cube", mesh);

	return mesh;
}

Ref<Mesh> Resources::LoadSphereMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Sphere");
	if (findMesh) {
		return findMesh;
	}
	float radius = 0.5f; // ±¸ÀÇ ¹ÝÁö¸§
	uint32 stackCount = 20; // °¡·Î ºÐÇÒ
	uint32 sliceCount = 20; // ¼¼·Î ºÐÇÒ

	vector<Vertex> vec;

	Vertex v;

	// ºÏ±Ø
	v.pos = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 1.0f);
	vec.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// °í¸®¸¶´Ù µ¹¸é¼­ Á¤Á¡À» °è»êÇÑ´Ù (ºÏ±Ø/³²±Ø ´ÜÀÏÁ¡Àº °í¸®°¡ X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// °í¸®¿¡ À§Ä¡ÇÑ Á¤Á¡
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.pos;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vec.push_back(v);
		}
	}

	// ³²±Ø
	v.pos = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 0.0f);
	vec.push_back(v);

	vector<uint32> idx(36);

	// ºÏ±Ø ÀÎµ¦½º
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ¸öÅë ÀÎµ¦½º
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ³²±Ø ÀÎµ¦½º
	uint32 bottomIndex = static_cast<uint32>(vec.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Sphere", mesh);

	return mesh;
}

Ref<Mesh> Resources::LoadCylinderMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Cylinder");
	if (findMesh) {
		return findMesh;
	}
	float topRadius = 0.5f;
	float bottomRadius = 0.5f;

	uint32 sliceCount = 10;
	uint32 stackCount = 10;

	float height = 2.5f;

	vector<Vertex> vertices;

	// ½Ç¸°´õÀÇ ³ôÀÌ¸¸Å­À» ¸î°³ÀÇ »ç°¢ÇüÀ¸·Î ±¸¼ºÇÏÁö
	// Ex) stackCount¿¡ 2°³°¡ µé¾î¿À¸é ½Ç¸°´õÀÇ ÇÑ¸éÀÌ 2°³ÀÇ »ç°¢ÇüÀ¸·Î ±¸¼ºµÊ
	// (µÎ°³¸¦ ÇÕÄ£ »ç°¢Çü ³ôÀÌ = height)
	float stackHeight = height / static_cast<float>(stackCount);

	// À§¶Ñ²±°ú ¾Æ·¡¶Ñ²±ÀÇ ¹ÝÁö¸§ÀÌ ´Ù¸¦ °æ¿ì ¹ÝÁö¸§À» ¸ÂÃçÁÖ±â À§ÇÔ.
	float radiusStep = (topRadius - bottomRadius) / static_cast<float>(stackCount);

	uint32 ringCount = stackCount + 1;

	for (uint32 i = 0; i < ringCount; ++i) {
		float y = (-0.5f * height) + (i * stackHeight);
		float r = bottomRadius + (i * radiusStep);
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 k = 0; k <= sliceCount; ++k) {
			float x = r * ::cosf(k * phi);
			float z = r * ::sinf(k * phi);

			Vertex vertex;
			vertex.pos = Vec3(x, y, z);

			vertex.tangent = Vec3(-z, 0.0f, x);
			float dr = bottomRadius - topRadius;
			Vec3 biTangent = Vec3(dr * x, -height, dr * z);

			vertex.normal.Cross(vertex.tangent, biTangent);
			vertex.normal.Normalize();

			float u = k / static_cast<float>(sliceCount);
			float v = i / static_cast<float>(ringCount);
			vertex.uv = Vec2(u, v);

			vertices.push_back(vertex);
		}
	}

	vector<uint32> indices;
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount; ++y) {
		for (uint32 x = 0; x < sliceCount; ++x) {
			indices.push_back(y * ringVertexCount + x);
			indices.push_back((y + 1) * ringVertexCount + x);
			indices.push_back((y + 1) * ringVertexCount + (x + 1));

			indices.push_back(y * ringVertexCount + x);
			indices.push_back((y + 1) * ringVertexCount + (x + 1));
			indices.push_back(y * ringVertexCount + (x + 1));
		}
	}


	// À­ ¶Ñ²±
	{
		// ¹öÅØ½º
		float y = 0.5f * height;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = topRadius * ::cosf(i * phi);
			float z = topRadius * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		}

		vertices.push_back(Vertex(Vec3(0.0f, y, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));


		// ÀÎµ¦½º
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);
		}
	}

	// ¾Æ·¡¶Ñ²±
	{
		// ¹öÅØ½º
		float y = -0.5f * height;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = bottomRadius * ::cosf(i * phi);
			float z = bottomRadius * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)));
		}
		vertices.push_back(Vertex(Vec3(0.0f, y, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)));


		// ÀÎµ¦½º
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);

		}
	}
	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vertices, indices);
	Add(L"Cylinder", mesh);

	return mesh;
}

Ref<Mesh> Resources::LoadConeMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Cone");
	if (findMesh) {
		return findMesh;
	}
	uint32 sliceCount = 20;

	vector<Vertex> vertices;
	vector<uint32> indices;
	float height = 2.0f;

	// À­ ¶Ñ²±
	{
		// ¹öÅØ½º
		float y = 0.0f;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = 1.0f * ::cosf(i * phi);
			float z = 1.0f * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		}

		vertices.push_back(Vertex(Vec3(0.0f, height, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));


		// ÀÎµ¦½º
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);

			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);

		}
	}

	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vertices, indices);
	Add(L"Cone", mesh);
	return mesh;
}

Ref<Mesh> Resources::LoadPointMesh()
{
	Ref<Mesh> findMesh = Get<Mesh>(L"Point");
	if (findMesh)
		return findMesh;

	vector<Vertex> vec(1);
	vec[0] = Vertex(Vec3(0, 0, 0), Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> indices(1);
	indices[0] = 0;

	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, indices);
	Add(L"Point", mesh);

	return mesh;
}

Ref<Mesh> Resources::LoadTerrainMesh(int32 sizeX, int32 sizeZ)
{
	vector<Vertex> vertexs;

	Vertex vtx;
	for (int32 z = 0; z < sizeZ + 1; ++z) {
		for (int32 x = 0; x < sizeX + 1; ++x) {
			vtx.pos = Vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));
			vtx.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			vtx.normal = Vec3(0.0f, 1.0f, 0.0f);
			vtx.tangent = Vec3(0.0f, 1.0f, 0.0f);

			vertexs.push_back(vtx);
		}
	}


	vector<uint32> indices;

	for (int32 z = 0; z < sizeZ; ++z) {
		for (int32 x = 0; x < sizeX; ++x) {
			indices.push_back((sizeX + 1) * (z + 1) + (x));
			indices.push_back((sizeX + 1) * (z) + (x + 1));
			indices.push_back((sizeX + 1) * (z) + (x));

			indices.push_back((sizeX + 1) * (z) + (x + 1));
			indices.push_back((sizeX + 1) * (z + 1) + (x));
			indices.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	Ref<Mesh> findMesh = Get<Mesh>(L"Terrain");
	if (findMesh) {
		findMesh->Create(vertexs, indices);
		return findMesh;
	}

	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vertexs, indices);
	Add(L"Terrain", mesh);

	return mesh;
}

Ref<MeshData> Resources::LoadFBX(const wstring& path)
{
	wstring key = path;

	Ref<MeshData> meshData = Get<MeshData>(key);
	if (meshData) {
		return meshData;
	}

	//meshData->Load(SCENE_PATH + path);
	if (meshData == nullptr) {
		meshData = MeshData::LoadFromFBX(FBX_PATH + path);
	}
	meshData->name = key;
	Add(key, meshData);


	return meshData;
}

Ref<Texture> Resources::CreateTexture(const wstring& name, DXGI_FORMAT format, uint32 width, uint32 height,
	const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
	D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	Ref<Texture> texture = make_shared<Texture>();
	texture->Create(format, width, height, heapProperty, heapFlags, resFlags, clearColor);
	Add(name, texture);

	return texture;
}

Ref<Texture> Resources::CreateTextureFromResource(const wstring& name, ComPtr<ID3D12Resource> tex2D)
{
	Ref<Texture> texture = make_shared<Texture>();
	texture->CreateFromResource(tex2D);
	Add(name, texture);

	return texture;
}

void Resources::CreateDefaultShader()
{
	/* ----- Skybox ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_EQUAL
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"skybox.fx", info);
		Add<Shader>(L"Skybox", shader);
	}

	/* ----- Deferred (Deferred) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"deferred.fx", info);
		Add<Shader>(L"Deferred", shader);
	}

	/* ----- Forward (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"forward.fx", info);
		Add<Shader>(L"Forward", shader);
	}

	/* ----- Wirefram (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::WIREFRAME,
			DEPTH_STENCIL_TYPE::LESS_EQUAL,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_LINELIST
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"wirefram.fx", info);
		Add<Shader>(L"Wirefram", shader);
	}

	/* ----- Collider (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::WIREFRAME,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_LINELIST
		};
		ShaderArg arg =
		{
			"VS_Main",
			"",
			"",
			"",
			"PS_Main"
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"wirefram.fx", info, arg);
		Add<Shader>(L"Collider", shader);
	}

	/* ----- Texture (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE
		};
		ShaderArg arg =
		{
			"VS_Tex",
			"",
			"",
			"",
			"PS_Tex"
		};
		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"forward.fx", info, arg);
		Add<Shader>(L"Texture", shader);
	}

	/* ----- DirLight ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		ShaderArg arg =
		{
			"VS_DirLight",
			"",
			"",
			"",
			"PS_DirLight"
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"lighting.fx", info, arg);
		Add<Shader>(L"DirLight", shader);
	}

	/* ----- PointLight ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		ShaderArg arg =
		{
			"VS_PointLight",
			"",
			"",
			"",
			"PS_PointLight"
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"lighting.fx", info, arg);
		Add<Shader>(L"PointLight", shader);
	}

	/* ----- Final ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
		};

		ShaderArg arg =
		{
			"VS_Final",
			"",
			"",
			"",
			"PS_Final"
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"lighting.fx", info, arg);
		Add<Shader>(L"Final", shader);
	}


	/* ----- ComputeShader ----- */
	{
		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"compute.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeShader", shader);
	}


	/* ----- GraphicsParticle ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::PARTICLE,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS_NO_WRITE,
			BLEND_TYPE::ALPHA_BLEND,
			D3D_PRIMITIVE_TOPOLOGY_POINTLIST
		};

		ShaderArg arg =
		{
			"VS_Main",
			"",
			"",
			"GS_Main",
			"PS_Main"
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"particle.fx", info, arg);
		Add<Shader>(L"Particle", shader);
	}
	/* ----- ComputeParticle ----- */
	{
		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"particle.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeParticle", shader);
	}

	/* ----- Shadow ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::SHADOW,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"shadow.fx", info);
		Add<Shader>(L"Shadow", shader);
	}

	/* ----- Pick ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::PICK,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
		};


		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"pick.fx", info);
		Add<Shader>(L"Pick", shader);
	}

	/* ----- Tessellation ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::WIREFRAME,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
		};

		ShaderArg arg =
		{
			"VS_Main",
			"HS_Main",
			"DS_Main",
			"",
			"PS_Main"
		};

		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"tessellation.fx", info, arg);
		Add<Shader>(L"Tessellation", shader);
	}

	/* ----- Terrain ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED,
			RASTERIZER_TYPE::CULL_BACK ,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
		};
		ShaderArg arg =
		{
			"VS_Main",
			"HS_Main",
			"DS_Main",
			"",
			"PS_Main"
		};


		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"terrain.fx", info, arg);
		Add<Shader>(L"Terrain", shader);
	}

	/* ----- ComputeAnimation ----- */
	{
		Ref<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"animation.fx", "CS_Main");
		Add<Shader>(L"ComputeAnimation", shader);
	}
}

void Resources::CreateDefaultMaterial()
{
	/* ----- Skybox ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Skybox");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Skybox", material);
	}

	/* ----- DirLight ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"DirLight");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, Get<Texture>(L"PositionTarget"));
		material->SetTexture(1, Get<Texture>(L"NormalTarget"));
		Add<Material>(L"DirLight", material);
	}

	/* ----- PointLight ----- */
	{
		const WindowInfo& window = GEngine->GetWindow();
		Vec2 resolution = { static_cast<float>(window.width), static_cast<float>(window.height) };

		Ref<Shader> shader =Get<Shader>(L"PointLight");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, Get<Texture>(L"PositionTarget"));
		material->SetTexture(1, Get<Texture>(L"NormalTarget"));
		material->SetVec2(0, resolution);
		Add<Material>(L"PointLight", material);
	}

	/* ----- Final ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Final");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);

		Ref<Texture> tex1 = Get<Texture>(L"DiffuseTarget");
		Ref<Texture> tex2 = Get<Texture>(L"DiffuseLightTarget");
		Ref<Texture> tex3 = Get<Texture>(L"SpecularLightTarget");

		material->SetTexture(0, tex1);
		material->SetTexture(1, tex2);
		material->SetTexture(2, tex3);
		Add<Material>(L"Final", material);
	}

	/* ----- Compute Shader ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"ComputeShader");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"ComputeShader", material);
	}

	/* ----- Particle ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Particle");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Particle", material);
	}

	/* ----- Compute Shader ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"ComputeParticle");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"ComputeParticle", material);
	}

	/* ----- Shadow ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Shadow");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Shadow", material);
	}

	/* ----- Pick ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Pick");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Pick", material);
	}

	/* ----- Tessellation ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Tessellation");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Tessellation", material);
	}

	/* ----- Collider ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Collider");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Collider", material);
	}

	/* ----- Terrain ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Terrain");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Terrain", material);
	}

	/* ----- Wirefram ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"Tessellation");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Wirefram", material);
	}

	/* ----- ComputeAnimation ----- */
	{
		Ref<Shader> shader = Get<Shader>(L"ComputeAnimation");
		Ref<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"ComputeAnimation", material);
	}

	/* ----- Test Material(Player) ----- */
	{
		Ref<Material> material = make_shared<Material>();

		material->SetTexture(0, GET_SINGLE(Resources)->Load<Texture>(L"Marble", TEXTURE_PATH + L"Marble.jpg"));
		material->SetTexture(1, GET_SINGLE(Resources)->Load<Texture>(L"MarbleNormal", TEXTURE_PATH + L"Marble_Normal.jpg"));
		material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));


		Add<Material>(L"Player", material);
	}

	/* ----- Test Material(Ground) ----- */
	{
		Ref<Material> material = make_shared<Material>();
		material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));
		Add<Material>(L"Ground", material);
	}

	/* ----- Test Material(Defulat) ----- */
	{
		Ref<Material> material = make_shared<Material>();
		material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));
		Add<Material>(L"Defualt", material);
	}

	/* ----- Test Material(Deferred) ----- */
	{
		Ref<Material> material = make_shared<Material>();
		material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));
		Add<Material>(L"Deferred", material);
	}
}

void Resources::CreateDefaultGameObject()
{
	const wstring& texPath = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH_KEY);
	const wstring& shaderPath = GET_SINGLE(PathManager)->FindPath(SHADER_PATH_KEY);

#pragma region SkyBox
	{
		Ref<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());

		Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			meshRenderer->mesh = (GET_SINGLE(Resources)->LoadCubeMesh());
		}
		{
			Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Skybox");
			Ref<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Skybox", texPath + L"SkyBox_1.jpg");
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		skybox->isFrustum = false;
		skybox->SetLayer(LAYER_TYPE::SKYBOX);
		Add<GameObject>(L"Skybox", skybox);
	}
#pragma endregion

#pragma region Main Camera
	{
		Ref<GameObject> camera = make_shared<GameObject>();
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		camera->name = (L"MainCamera");

		camera->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, true); // UI´Â ¾ÈÂïÀ½
		Add<GameObject>(L"MainCamera", camera);
	}
#pragma endregion

#pragma region UI Camera
	{
		Ref<GameObject> camera = make_shared<GameObject>();

		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());

		camera->name = (L"UI_Camera");

		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		camera->GetCamera()->CameraType = PROJECTION_TYPE::ORTHOGRAPHIC;

		camera->GetCamera()->SetCullingMaskAll();
		camera->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, false);	// UI¸¸ ÂïÀ½

		Add<GameObject>(L"UICamera", camera);
	}
#pragma endregion

#pragma region Directional Light
	{
		Ref<GameObject> light = make_shared<GameObject>();
		light->name = (L"Directional Light");
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.0f, -1.0f, 0.0f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.7f, 0.5f, 0.6f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		Add<GameObject>(L"DirectionalLight", light);
	}
#pragma endregion

#pragma region Point Light
	{
		Ref<GameObject> light = make_shared<GameObject>();
		light->name = (L"Point Light");
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		light->GetLight()->SetAmbient(Vec4(0.1f, 0.1f, 0.1f, 1.0f));
		light->GetLight()->SetSpecular(Vec4(0.1f, 0.1f, 0.1f, 1.0f));
		light->GetLight()->SetLightRange(500.0f);

		Add<GameObject>(L"PointLight", light);
	}
#pragma endregion

#pragma region Animation Mesh
	{
		Ref<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"Cat\\Cartoon Cat.fbx");
		vector<Ref<GameObject>> gameObjects = meshData->Instantiate();
		
		gameObjects[0]->name = wstring(L"OrangeBot");
		gameObjects[0]->isFrustum = false;
		
		gameObjects[0]->AddComponent(make_shared<MeshCollider>());
		gameObjects[0]->GetTransform()->SetLocalPosition(Vec3(100.0f, 0.f, -300.f));
		gameObjects[0]->GetTransform()->localRotation = (Vec3(3.14f / 2.0f, 0.f, -3.14f));
		gameObjects[0]->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));

		Add<GameObject>(L"OrangeBot", gameObjects[0]);
	}

	//{
	//	Ref<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"IronMan.fbx");
	//	vector<Ref<GameObject>> gameObjects = meshData->Instantiate();

	//	gameObjects[0]->name = wstring(L"IronMan");
	//	gameObjects[0]->isFrustum = false;

	//	gameObjects[0]->AddComponent(make_shared<MeshCollider>());
	//	gameObjects[0]->GetTransform()->SetLocalPosition(Vec3(100.0f, 0.f, -300.f));
	//	gameObjects[0]->GetTransform()->localRotation = (Vec3(3.14f / 2.0f, 0.f, -3.14f));
	//	gameObjects[0]->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));

	//	Add<GameObject>(L"IronMan", gameObjects[0]);
	//}
#pragma endregion

}

void Resources::CreateDefaultComponent()
{
	Add<Transform>(L"Transform", make_shared<Transform>());
	Add<Light>(L"Light", make_shared<Light>());
	Add<Camera>(L"Camera", make_shared<Camera>());
	{
		Ref<MeshRenderer> meshRenderer =  make_shared<MeshRenderer>();
		Ref<Material> material = make_shared <Material>();
		Ref<Mesh> mesh = LoadCubeMesh();
		material->SetShader(Get<Shader>(L"Deferred"));
		meshRenderer->SetMaterial(material);
		meshRenderer->mesh = (mesh);
		Add<MeshRenderer>(L"MeshRenderer", meshRenderer);
	}

	Add<ParticleSystem>(L"ParticleSystem", make_shared<ParticleSystem>());
}

void Resources::CreateDefaultTexture()
{
	wstring path = TEXTURE_PATH;

	vector<Ref<FileInfo>> fileInfos;
	GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), fileInfos, ".jpg");
	GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), fileInfos, ".png");
	GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), fileInfos, ".dds");
	for (Ref<FileInfo> info : fileInfos) {
		Load<Texture>(Utils::Str2Wstr(Utils::Split(info->Name, '.')[0]), info->GetPathInfo().wstring().data());
	}

	/* ---- Nullptr ----- */
	{
		_resources[static_cast<uint8>(OBJECT_TYPE::TEXTURE)].insert(make_pair(L"Null", nullptr));
	}
	/* ---- Defulat ----- */
	{
		Load<Texture>(L"Default", path + L"DefaultImage.png");
	}
}

void Resources::CreateDefaultLoad()
{
	LoadCubeMesh();
	LoadSphereMesh();
	LoadRectangleMesh();
	LoadCircleMesh();
	LoadCylinderMesh();
	LoadConeMesh();
	LoadPointMesh();
}
