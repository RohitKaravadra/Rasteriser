#pragma once
#include "GMath.h"
#include <vector>
#include <map>
#include <iostream>

struct Bone
{
	std::string name;
	Matrix offset;
	int parentIndex;
};

struct Skeleton
{
	std::vector<Bone> bones;
	Matrix globalInverse;
};

struct AnimationFrame
{
	std::vector<Vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<Vec3> scales;
};


class AnimationSequence
{
public:
	std::vector<AnimationFrame> frames;
	float ticksPerSecond;

	Vec3 Interpolate(Vec3 p1, Vec3 p2, float t);

	Quaternion Interpolate(Quaternion q1, Quaternion q2, float t);

	float Duration() const;

	void CalcFrame(float t, int& frame, float& interpolationFact) const;
	int NextFrame(int frame) const;

	Matrix InterpolateBoneToGlobal(Matrix* matrices, int baseFrame,
		float interpolationFact, Skeleton* skeleton, int boneIndex);
};

class Animation
{
public:
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;

	int BonesSize() const;

	void CalcFrame(std::string name, float t, int& frame, float& interpolationFact);

	Matrix InterpolateBoneToGlobal(std::string name, Matrix* matrices,
		int baseFrame, float interpolationFact, int boneIndex);

	void CalcFinalTransforms(Matrix* matrices);

	void Print();
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	float t;
	Matrix matrices[256];

	void ResetAnimationTime();

	bool AnimationFinished();

	void update(std::string name, float dt);
};
