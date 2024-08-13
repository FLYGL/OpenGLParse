#pragma once
typedef void(*FeatureFunc)();

#define FeatureId			__LINE__
#define NameConcat(a,b)		a##b
#define UniqueName(a,b)	NameConcat(a,b)
#define RegisterFeaturetest(func)	static void* UniqueName(func,FeatureId) = _registerFeaturetest(func)
#define RegisterFrameFeaturetest(func) static void* UniqueName(func,FeatureId) = _registerFrameFeaturetest(func)
#define RegisterUniqueFrameFeaturetest(func) static void* UniqueName(func,FeatureId) = _registerUniqueFrameFeaturetest(func)

void* _registerFeaturetest(FeatureFunc);
void* _registerFrameFeaturetest(FeatureFunc);
void* _registerUniqueFrameFeaturetest(FeatureFunc);

class FeatureEnvWrapper
{
public:
    static bool RunFeatures();
    static bool RunFrameFeatures();
};
