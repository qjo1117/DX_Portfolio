#pragma once

#include <string>

struct WindowInfo
{
	HWND	hWnd; // 출력 윈도우
	int32	width; // 너비
	int32	height; // 높이
	bool	windowed; // 창모드 or 전체화면
	bool	loop = true;
};

struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t, Vec4 w = Vec4(0.0f, 0.0f, 0.0f, 0.0f), Vec4 i = Vec4(0.0f, 0.0f, 0.0f, 0.0f))
		: pos(p), uv(u), normal(n), tangent(t), weights(w), indices(i)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec4 weights;
	Vec4 indices;

};

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
	Matrix matViewInv;
};

/* ------- Animation Struct ------- */

struct KeyFrameInfo
{
	double	time;
	int32	frame;
	Vec3	scale;
	Vec4	rotation;
	Vec3	translate;;
};

struct BoneInfo
{
	wstring					boneName;
	int32					parentIdx;
	Matrix					matOffset;
};

struct AnimClipInfo
{
	wstring			animName;
	int32			frameCount;
	double			duration;
	vector<vector<KeyFrameInfo>>	keyFrames;
};

struct AnimFrameParams
{
	Vec4	scale;
	Vec4	rotation; // Quaternion
	Vec4	translation;
};
