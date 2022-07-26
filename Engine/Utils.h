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

		vecInfo["Vec2"].append(vector);
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

	static void ToJson(Json::Value& jsonString, Vec3& vec) 
	{
		Json::Value json;

		json.append(vec.x);
		json.append(vec.y);
		json.append(vec.z);

		jsonString["Vec3"].append(json);
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

	static void ToJson(Json::Value& jsonString, Vec4& vec)
	{
		Json::Value json;

		json.append(vec.x);
		json.append(vec.y);
		json.append(vec.z);
		json.append(vec.w);

		jsonString["Vec4"].append(json);
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

	static void JsonSaveToMatrix(Json::Value& matInfo, Matrix& mat)
	{
		Json::Value vector;
		for (int32 i = 0; i < 4; ++i) {
			for (int32 j = 0; j < 4; ++j) {
				vector.append(mat.m[i][j]);
			}
		}

		matInfo["Matrix"] = vector;
	}

	static void JsonSaveToVertex(Json::Value& vertexInfo, Vertex& vertex)
	{
		Json::Value info;

		/* ----- Position ----- */
		Utils::ToJson(info, vertex.pos);
		vertexInfo["Position"] = info;
		info.clear();

		/* ----- UV ----- */
		Utils::JsonSaveToVec2(info, vertex.uv);
		vertexInfo["UV"] = info;
		info.clear();

		/* ----- Normal ----- */
		Utils::ToJson(info, vertex.normal);
		vertexInfo["Normal"] = info;
		info.clear();

		/* ----- Tangent ----- */
		Utils::ToJson(info, vertex.tangent);
		vertexInfo["Tangent"] = info;
		info.clear();

		/* ----- Weights ----- */
		Utils::ToJson(info, vertex.weights);
		vertexInfo["Weights"] = info;
		info.clear();

		/* ----- Indices ----- */
		Utils::ToJson(info, vertex.indices);
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

	static void JsonBoneInfo(Json::Value& boneInfo, BoneInfo& bone)
	{
		Json::Value info;

		/* ----- BoneName | ParentIndex ----- */
		info["BoneName"] = Utils::Wstr2Str(bone.boneName);
		info["ParentIndex"] = bone.parentIdx;
		
		/* ----- ParentOffset Matrix ----- */
		Utils::JsonSaveToMatrix(info, bone.matOffset);
		info["ParentOffsetMatrix"] = info;
	}

	static void ToJson(Json::Value& jsonValue, KeyFrameInfo& keyFrameInfo)
	{
		Json::Value json;

		json.append(keyFrameInfo.time);
		json.append(keyFrameInfo.frame);

		Utils::ToJson(json, keyFrameInfo.scale);
		Utils::ToJson(json, keyFrameInfo.rotation);
		Utils::ToJson(json, keyFrameInfo.translate);

		jsonValue["KeyFrameInfo"].append(json);
	}

	static void ToJson(Json::Value& jsonValue, AnimClipInfo& animClipInfo)
	{
		Json::Value json;

		json.append(Utils::Wstr2Str(animClipInfo.animName));
		json.append(animClipInfo.frameCount);
		json.append(animClipInfo.duration);

		int32 iSize = animClipInfo.keyFrames.size();
		for (int32 i = 0; i < iSize; ++i) {
			for (int32 j = 0; j < animClipInfo.keyFrames[i].size(); ++j) {
				Utils::ToJson(json, animClipInfo.keyFrames[i][j]);
			}
		}

		jsonValue["AnimClipInfo"].append(json);
	}

	static void ToJson(Json::Value& jsonString, const vector<vector<float>>& vec)
	{
		Json::Value json;

		for (auto& iVec : vec) {
			Json::Value iValue;
			for (auto& value : iVec) {
				iValue["colum"].append(value);
			}
			json["row"].append(iValue);
		}

		jsonString["Vector<Vector<float>>"].append(json);
	}
};