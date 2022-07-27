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

#pragma region FromJson

	static void FromJson(Json::Value& jsonString, Vec2& vec)
	{
		vec.x = jsonString["x"].asFloat();
		vec.y = jsonString["y"].asFloat();
	}

	static void FromJson(Json::Value& jsonString, Vec3& vec)
	{
		vec.x = jsonString["x"].asFloat();
		vec.y = jsonString["y"].asFloat();
		vec.z = jsonString["z"].asFloat();
	}


	static void FromJson(Json::Value& jsonString, Vec4& vec)
	{

		vec.x = jsonString["x"].asFloat();
		vec.y = jsonString["y"].asFloat();
		vec.z = jsonString["z"].asFloat();
		vec.w = jsonString["w"].asFloat();
	}

	static void FromJson(Json::Value& jsonString, KeyFrameInfo& keyFrameInfo)
	{
		keyFrameInfo.time = jsonString["Time"].asDouble();
		keyFrameInfo.frame = jsonString["Frame"].asInt();

		Utils::FromJson(jsonString["Scale"], keyFrameInfo.scale);
		Utils::FromJson(jsonString["Rotation"], keyFrameInfo.rotation);
		Utils::FromJson(jsonString["Translate"], keyFrameInfo.translate);
	}

	static void FromJson(Json::Value& jsonString, AnimClipInfo& animClipInfo)
	{
		animClipInfo.animName = Utils::Str2Wstr(jsonString["AnimName"].asString());
		animClipInfo.frameCount = jsonString["FrameCount"].asInt();
		animClipInfo.duration = jsonString["Duration"].asFloat();

		int32 iSize = jsonString["ClipKeyFrameSize"].asInt();
		animClipInfo.keyFrames.resize(iSize);

		for (int32 i = 0; i < iSize; ++i) {
			Json::Value vecJson = jsonString["MatrixKeyFrameInfo"][i];
			int32 jSize = vecJson["ClipKeyFrameSize"].asInt();
			for (int32 j = 0; j < jSize; ++j) {
				KeyFrameInfo info;
				Utils::FromJson(vecJson["KeyFrameInfo"][j], info);
				animClipInfo.keyFrames[i].push_back(info);
			}
		}

	}

	static void FromJson(Json::Value& jsonString, Matrix& mat)
	{
		for (int32 i = 0; i < 4; ++i) {
			Json::Value iValue = jsonString["Row"][i];
			for (int32 j = 0; j < 4; ++j) {
				mat.m[i][j] = iValue["Colum"][j].asFloat();
			}
		}
	}

	static void FromJson(Json::Value& jsonString, BoneInfo& bone)
	{
		bone.boneName = Utils::Str2Wstr(jsonString["BoneName"].asString());
		bone.parentIdx = jsonString["ParentIndex"].asInt();
		Utils::FromJson(jsonString["Offset"], bone.matOffset);
	}

#pragma endregion

#pragma region ToJson

	static void ToJson(Json::Value& jsonString, Vec2& vec)
	{

		jsonString["x"] = vec.x;
		jsonString["y"] = vec.y;

	}

	static void ToJson(Json::Value& jsonString, Vec3& vec)
	{
		jsonString["x"] = vec.x;
		jsonString["y"] = vec.y;
		jsonString["z"] = vec.z;

	}

	static void ToJson(Json::Value& jsonString, Vec4& vec)
	{
		jsonString["x"] = vec.x;
		jsonString["y"] = vec.y;
		jsonString["z"] = vec.z;
		jsonString["w"] = vec.z;
	}

	static void ToJson(Json::Value& jsonValue, KeyFrameInfo& keyFrameInfo)
	{
		Json::Value json;

		json["Time"] = keyFrameInfo.time;
		json["Frame"] = keyFrameInfo.frame;

		Utils::ToJson(json["Scale"], keyFrameInfo.scale);
		Utils::ToJson(json["Rotation"], keyFrameInfo.rotation);
		Utils::ToJson(json["Translate"], keyFrameInfo.translate);

		jsonValue.append(json);
	}

	static void ToJson(Json::Value& jsonString, Matrix& mat)
	{
		Json::Value json;
		
		for (int32 i = 0; i < 4; ++i) {
			Json::Value vecJson;
			for (int32 j = 0; j < 4; ++j) {
				vecJson["Colum"].append(mat.m[i][j]);
			}
			json["Row"].append(vecJson);
		}

		jsonString = json;
	}

	static void ToJson(Json::Value& jsonValue, AnimClipInfo& animClipInfo)
	{
		Json::Value json;

		json["AnimName"] = Utils::Wstr2Str(animClipInfo.animName);
		json["FrameCount"] = animClipInfo.frameCount;
		json["Duration"] = animClipInfo.duration;

		int32 iSize = animClipInfo.keyFrames.size();
		json["ClipKeyFrameSize"] = iSize;
		for (int32 i = 0; i < iSize; ++i) {
			Json::Value vecJson;
			int32 jSize = animClipInfo.keyFrames[i].size();
			vecJson["ClipKeyFrameSize"] = jSize;
			for (int32 j = 0; j < jSize; ++j) {
				Utils::ToJson(vecJson["KeyFrameInfo"], animClipInfo.keyFrames[i][j]);
			}
			json["MatrixKeyFrameInfo"].append(vecJson);
		}

		jsonValue = json;
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

	static void ToJson(Json::Value& jsonString, BoneInfo& info)
	{
		Json::Value json;

		json["BoneName"] = Utils::Wstr2Str(info.boneName);
		json["ParentIndex"] = info.parentIdx;
		Utils::ToJson(json["Offset"], info.matOffset);

		jsonString = json;
	}

	static void ToJson(Json::Value& jsonString, FbxKeyFrameInfo& keyFrameInfo)
	{
		Json::Value json;

		json["Time"] = keyFrameInfo.time;
		Matrix mat = Utils::FbxMatrixToMatrix(keyFrameInfo.matTransform);
		Utils::ToJson(json["Transform"], mat);

		jsonString.append(json);
	}

	static void ToJson(Json::Value& jsonString, FbxAnimClipInfo& animClipInfo)
	{
		Json::Value json;
		
		json["Name"] = Utils::Wstr2Str(animClipInfo.name);
		
		int32 startFrame = static_cast<int32>(animClipInfo.startTime.GetFrameCount(animClipInfo.mode));
		int32 endFrame = static_cast<int32>(animClipInfo.endTime.GetFrameCount(animClipInfo.mode));

		int32 iSize = animClipInfo.keyFrames.size();
		json["KeyFrameSize_I"] = iSize;
		for (int32 i = 0; i < iSize; ++i) {
			Json::Value vecJson = json["KeyFrame_I"];
			int32 jSize = animClipInfo.keyFrames[i].size();
			vecJson["KeyFrameSize_J"] = jSize;
			for (int32 j = 0; j < jSize; ++j) {
				Utils::ToJson(vecJson, animClipInfo.keyFrames[i][j]);
			}
			json["KeyFrame_IJ"].append(vecJson);
		}

		jsonString.append(json);
	}

	static void ToJson(Json::Value& jsonString, FbxBoneInfo& boneInfo)
	{

	}

#pragma endregion

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

	//static void JsonSaveToVertex(Json::Value& vertexInfo, Vertex& vertex)
	//{
	//	Json::Value info;

	//	/* ----- Position ----- */
	//	Utils::ToJson(info, vertex.pos);
	//	vertexInfo["Position"] = info;
	//	info.clear();

	//	/* ----- UV ----- */
	//	Utils::ToJson(info, vertex.uv);
	//	vertexInfo["UV"] = info;
	//	info.clear();

	//	/* ----- Normal ----- */
	//	Utils::ToJson(info, vertex.normal);
	//	vertexInfo["Normal"] = info;
	//	info.clear();

	//	/* ----- Tangent ----- */
	//	Utils::ToJson(info, vertex.tangent);
	//	vertexInfo["Tangent"] = info;
	//	info.clear();

	//	/* ----- Weights ----- */
	//	Utils::ToJson(info, vertex.weights);
	//	vertexInfo["Weights"] = info;
	//	info.clear();

	//	/* ----- Indices ----- */
	//	Utils::ToJson(info, vertex.indices);
	//	vertexInfo["Indices"] = info;
	//	info.clear();
	//}

	//static void JsonLoadToVertex(Json::Value& vertexInfo, Vertex& vertex)
	//{
	//	/* ----- Position ----- */
	//	Utils::JsonLoadToVec3(vertexInfo["Position"], vertex.pos);
	//	/* ----- UV ----- */
	//	Utils::JsonLoadToVec2(vertexInfo["UV"], vertex.uv);
	//	/* ----- Normal ----- */
	//	Utils::JsonLoadToVec3(vertexInfo["Normal"], vertex.normal);
	//	/* ----- Tangent ----- */
	//	Utils::JsonLoadToVec3(vertexInfo["Tangent"], vertex.tangent);
	//	/* ----- Weights ----- */
	//	Utils::JsonLoadToVec4(vertexInfo["Weights"], vertex.weights);
	//	/* ----- Indices ----- */
	//	Utils::JsonLoadToVec4(vertexInfo["Indices"], vertex.indices);
	//}


	static Matrix FbxMatrixToMatrix(const FbxMatrix& fbxMat)
	{
		Matrix mat;

		for (int32 i = 0; i < 4; ++i) {
			for (int32 j = 0; j < 4; ++j) {
				mat.m[i][j] = fbxMat[i][j];
			}
		}

		return mat;
	}
};