#include "Animation.h"


Vec3 AnimationSequence::Interpolate(Vec3 p1, Vec3 p2, float t)
{
	return Lerp(p1, p2, t);
}

Quaternion AnimationSequence::Interpolate(Quaternion q1, Quaternion q2, float t)
{
	return Quaternion::Slerp(q1, q2, t);
}

float AnimationSequence::Duration() const
{
	return ((float)frames.size() / ticksPerSecond);
}

void AnimationSequence::CalcFrame(float t, int& frame, float& interpolationFact) const
{
	interpolationFact = t * ticksPerSecond;
	frame = (int)floorf(interpolationFact);
	interpolationFact = interpolationFact - (float)frame;
	frame = min(frame, frames.size() - 1);
}

int AnimationSequence::NextFrame(int frame) const
{
	return min(frame + 1, frames.size() - 1);
}

Matrix AnimationSequence::InterpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
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

int Animation::BonesSize() const { return skeleton.bones.size(); }

void Animation::CalcFrame(std::string name, float t, int& frame, float& interpolationFact)
{
	animations[name].CalcFrame(t, frame, interpolationFact);
}

Matrix Animation::InterpolateBoneToGlobal(std::string name, Matrix* matrices,
	int baseFrame, float interpolationFact, int boneIndex)
{
	return animations[name].InterpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
}

void Animation::CalcFinalTransforms(Matrix* matrices)
{
	for (int i = 0; i < BonesSize(); i++)
		matrices[i] *= skeleton.bones[i].offset * skeleton.globalInverse;
}

void Animation::Print()
{
	for (auto& data : animations)
		std::cout << data.first << std::endl;
}

void AnimationInstance::ResetAnimationTime() { t = 0; }

bool AnimationInstance::AnimationFinished()
{
	if (t > animation->animations[currentAnimation].Duration())
	{
		return true;
	}

	return false;
}

void AnimationInstance::update(std::string name, float dt) {

	if (name == currentAnimation) {
		t += dt;
	}
	else {
		currentAnimation = name;  t = 0;
	}

	// check for reverse or forward animation ciculation
	if (t < 0)
		t = animation->animations[currentAnimation].Duration();
	else if (AnimationFinished() == true) { ResetAnimationTime(); }

	int frame = 0;
	float interpolationFact = 0;
	animation->CalcFrame(name, t, frame, interpolationFact);
	for (int i = 0; i < animation->BonesSize(); i++)
	{
		matrices[i] = animation->InterpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
	}
	animation->CalcFinalTransforms(matrices);
}
