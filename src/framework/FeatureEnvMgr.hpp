#pragma once
typedef void(*FeatureFunc)();

#define FeatureId			__LINE__
#define NameConcat(a,b)		a##b
#define UniqueName(a,b)	NameConcat(a,b)
#define RegisterFeaturetest(func)	static void* UniqueName(func,FeatureId) = _registerFeaturetest(func)
#define RegisterFrameFeaturetest(func) static void* UniqueName(func,FeatureId) = _registerFrameFeaturetest(func)

void* _registerFeaturetest(FeatureFunc);
void* _registerFrameFeaturetest(FeatureFunc);

class FeatureEnvWrapper
{
public:
    static bool RunFeatures();
    static bool RunFrameFeatures();
};
