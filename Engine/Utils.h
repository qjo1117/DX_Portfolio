#pragma once

#include "PMath.h"
#include "Random.h"

#include <string>

class Utils
{
public:
	static vector<string> Split(const string& str, char base)
	{
		vector<string> ret;
		int32 size = static_cast<int32>(str.size());
		string strTemp = "";
		for (int32 i = 0; i < size; ++i) {
			if (str[i] == base) {
				ret.push_back(strTemp);
				strTemp = "";
			}
			else {
				strTemp.push_back(str[i]);
			}
		}

		if (strTemp.empty() == false) {
			ret.push_back(strTemp);
		}

		return ret;
	}

	static vector<wstring> Split(const wstring& p_str, char base)
	{
		vector<wstring> ret;
		int32 size = static_cast<int32>(p_str.size());
		wstring strTemp = L"";
		for (int32 i = 0; i < size; ++i) {
			if (p_str[i] == base) {
				ret.push_back(strTemp);
				strTemp = L"";
			}
			else {
				strTemp.push_back(p_str[i]);
			}
		}

		if (strTemp.empty() == false) {
			ret.push_back(strTemp);
		}

		return ret;
	}

	static wstring Str2Wstr(const string& str)
	{
		int32 len;
		int32 slength = static_cast<int32>(str.length()) + 1;
		len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
		wstring ret(buf);
		delete[] buf;
		return ret;
	}
	static string Wstr2Str(const wstring& str)
	{
		int32 len;
		int32 slength = static_cast<int32>(str.length()) + 1;
		len = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, 0, 0, 0, 0);
		string r(len, '\0');
		::WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, &r[0], len, 0, 0);
		return r;
	}

	static void JsonSaveToVec2(Json::Value& vecInfo, const Vec2& vec)
	{
		Json::Value vector;
		vector.append(vec.x);
		vector.append(vec.y);

		vecInfo["Vector2"] = vector;
	}

	static void JsonLoadToVec2(Json::Value& info, Vec2& vec)
	{
		vector<float> arr;

		Json::Value iterator = info["Vector2"];
		for (Json::ValueIterator iter = iterator.begin(); iter != iterator.end(); iter++) {
			arr.push_back(iter->asFloat());
		}
		vec.x = arr[0];
		vec.y = arr[1];
	}

	static void JsonSaveToVec3(Json::Value& vecInfo, const Vec3& vec) 
	{
		Json::Value vector;
		vector.append(vec.x);
		vector.append(vec.y);
		vector.append(vec.z);

		vecInfo["Vector3"] = vector;
	}

	static void JsonLoadToVec3(Json::Value& info, Vec3& vec)
	{
		vector<float> arr;
		
		Json::Value iterator = info["Vector3"];
		for (Json::ValueIterator iter = iterator.begin(); iter != iterator.end(); iter++) {
			arr.push_back(iter->asFloat());
		}
		vec.x = arr[0];
		vec.y = arr[1];
		vec.z = arr[2];
	}

	static void JsonSaveToVec4(Json::Value& vecInfo, const Vec4& vec)
	{
		Json::Value vector;
		vector.append(vec.x);
		vector.append(vec.y);
		vector.append(vec.z);
		vector.append(vec.w);

		vecInfo["Vector4"] = vector;
	}

	static void JsonLoadToVec4(Json::Value& info, Vec4& vec)
	{
		vector<float> arr;

		Json::Value iterator = info["Vector4"];
		for (Json::ValueIterator iter = iterator.begin(); iter != iterator.end(); iter++) {
			arr.push_back(iter->asFloat());
		}
		vec.x = arr[0];
		vec.y = arr[1];
		vec.z = arr[2];
		vec.w = arr[3];
	}

	static void JsonSaveToMatrix(Json::Value& matInfo, const Matrix& mat)
	{
		Json::Value vector;
		for (int32 i = 0; i < 4; ++i) {
			for (int32 j = 0; j < 4; ++j) {
				vector.append(mat.m[i][j]);
			}
		}

		matInfo["Matrix"] = vector;
	}

	static void JsonSaveToVertex(Json::Value& vertexInfo, const Vertex& vertex)
	{
		Json::Value info;

		/* ----- Position ----- */
		Utils::JsonSaveToVec3(info, vertex.pos);
		vertexInfo["Position"] = info;
		info.clear();

		/* ----- UV ----- */
		Utils::JsonSaveToVec2(info, vertex.uv);
		vertexInfo["UV"] = info;
		info.clear();

		/* ----- Normal ----- */
		Utils::JsonSaveToVec3(info, vertex.normal);
		vertexInfo["Normal"] = info;
		info.clear();

		/* ----- Tangent ----- */
		Utils::JsonSaveToVec3(info, vertex.tangent);
		vertexInfo["Tangent"] = info;
		info.clear();

		/* ----- Weights ----- */
		Utils::JsonSaveToVec4(info, vertex.weights);
		vertexInfo["Weights"] = info;
		info.clear();

		/* ----- Indices ----- */
		Utils::JsonSaveToVec4(info, vertex.indices);
		vertexInfo["Indices"] = info;
		info.clear();
	}

	static void JsonLoadToVertex(Json::Value& vertexInfo, Vertex& vertex)
	{
		/* ----- Position ----- */
		Utils::JsonLoadToVec3(vertexInfo["Position"], vertex.pos);

		/* ----- UV ----- */
		Utils::JsonLoadToVec2(vertexInfo["UV"], vertex.uv);

		/* ----- Normal ----- */
		Utils::JsonLoadToVec3(vertexInfo["Normal"], vertex.normal);

		/* ----- Tangent ----- */
		Utils::JsonLoadToVec3(vertexInfo["Tangent"], vertex.tangent);

		/* ----- Weights ----- */
		Utils::JsonLoadToVec4(vertexInfo["Weights"], vertex.weights);

		/* ----- Indices ----- */
		Utils::JsonLoadToVec4(vertexInfo["Indices"], vertex.indices);

	}

	static void JsonBoneInfo(Json::Value& boneInfo, const BoneInfo& bone)
	{
		Json::Value info;

		/* ----- BoneName | ParentIndex ----- */
		info["BoneName"] = Utils::Wstr2Str(bone.boneName);
		info["ParentIndex"] = bone.parentIdx;
		
		/* ----- ParentOffset Matrix ----- */
		Utils::JsonSaveToMatrix(info, bone.matOffset);
		info["ParentOffsetMatrix"] = info;
	}
};