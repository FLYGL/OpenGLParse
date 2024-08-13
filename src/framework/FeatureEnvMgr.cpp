#include<iostream>
#include <vector>

#include "FeatureEnvMgr.hpp"
class FeatureMgr
{
public:
	FeatureMgr() = default;
	~FeatureMgr() = default;

	FeatureMgr(const FeatureMgr&) = delete;
	FeatureMgr& operator =(const FeatureMgr&) = delete;
	FeatureMgr(FeatureMgr&&) = delete;
	FeatureMgr& operator =(FeatureMgr&&) = delete;
public:
	//no init and uninit
	bool AddFeatureFunc(FeatureFunc);
	bool RunAllFeatures();
	bool ClearAllFeatureFunc();
public:
	static FeatureMgr* CreateInstance();
private:
	std::vector<FeatureFunc> m_registeredFeatures;
};

//no dealloc
static FeatureMgr& GetFeatureMgr()
{
	static FeatureMgr s_featureMgr;
	return s_featureMgr;
}

bool s_bUniqueFrameFeatureTestOpen = false;
static FeatureMgr& GetFrameFeatureMgr()
{
	static FeatureMgr s_frameFeatureMgr;
	return s_frameFeatureMgr;
}

bool FeatureMgr::AddFeatureFunc(FeatureFunc featureFunc)
{
	m_registeredFeatures.emplace_back(featureFunc);
	return true;
}

bool FeatureMgr::ClearAllFeatureFunc()
{
	m_registeredFeatures.clear();
	return true;
}

bool FeatureMgr::RunAllFeatures()
{
	for (auto featureFunc : m_registeredFeatures)
	{
		featureFunc();
	}
	return true;
}

FeatureMgr* FeatureMgr::CreateInstance()
{
	return new FeatureMgr;
}

void* _registerFeaturetest(FeatureFunc featureFunc)
{
	GetFeatureMgr().AddFeatureFunc(featureFunc);
	return (void*)0;
}

void* _registerFrameFeaturetest(FeatureFunc featureFunc)
{
	if (!s_bUniqueFrameFeatureTestOpen)
	{
		GetFrameFeatureMgr().AddFeatureFunc(featureFunc);
	}
	return (void*)0;
}

void* _registerUniqueFrameFeaturetest(FeatureFunc featureFunc)
{
	s_bUniqueFrameFeatureTestOpen = true;
	GetFrameFeatureMgr().ClearAllFeatureFunc();
	GetFrameFeatureMgr().AddFeatureFunc(featureFunc);
	return (void*)0;
}

bool FeatureEnvWrapper::RunFeatures()
{
	return GetFeatureMgr().RunAllFeatures();
}

bool FeatureEnvWrapper::RunFrameFeatures()
{
	return GetFrameFeatureMgr().RunAllFeatures();
}