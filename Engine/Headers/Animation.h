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

	Vec3 Interpolate(Vec3 p1, Vec3 p2, float t) {
		return Lerp(p1, p2, t);
	}

	Quaternion Interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::Slerp(q1, q2, t);
	}

	float Duration() const {
		return ((float)frames.size() / ticksPerSecond);
	}

	void CalcFrame(float t, int& frame, float& interpolationFact) const
	{
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame;
		frame = min(frame, frames.size() - 1);
	}

	int NextFrame(int frame) const
	{
		return min(frame + 1, frames.size() - 1);
	}

	Matrix InterpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
		int nextFrameIndex = NextFrame(baseFrame);

		Matrix scale = Matrix::Scaling(Interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrameIndex].scales[boneIndex], interpolationFact));
		Matrix rotation = Interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrameIndex].rotations[boneIndex], interpolationFact).ToMatrix();
		Matrix translation = Matrix::Translation(Interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrameIndex].positions[boneIndex], interpolationFact));

		Matrix local = translation * rotation * scale;

		if (skeleton->bones[boneIndex].parentIndex > -1) {
			Matrix global = matrices[skeleton->bones[boneIndex].parentIndex] * local;
			return global;
		}

		return local;
	}
};

class Animation
{
public:
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;

	int BonesSize() const { return skeleton.bones.size(); }

	void CalcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].CalcFrame(t, frame, interpolationFact);
	}

	Matrix InterpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].InterpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}

	void CalcFinalTransforms(Matrix* matrices)
	{
		for (int i = 0; i < BonesSize(); i++)
		{
			matrices[i] *= skeleton.bones[i].offset * skeleton.globalInverse;
		}
	}

	void Print()
	{
		for (auto& data : animations)
			std::cout << data.first << std::endl;
	}
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	float t;
	Matrix matrices[256];

	void ResetAnimationTime()
	{
		t = 0;
	}

	bool AnimationFinished()
	{
		if (t > animation->animations[currentAnimation].Duration())
		{
			return true;
		}
		return false;
	}

	void update(std::string name, float dt) {

		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}

		if (AnimationFinished() == true) { ResetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation->CalcFrame(name, t, frame, interpolationFact);
		for (int i = 0; i < animation->BonesSize(); i++)
		{
			matrices[i] = animation->InterpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation->CalcFinalTransforms(matrices);
	}

};
