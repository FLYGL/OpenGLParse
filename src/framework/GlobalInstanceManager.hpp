#include <vector>
#include <assert.h>
class InstanceManager
{
    typedef void(DeconstructFunction)(void*);
    template<typename InstanceType>
    static void InstanceDeconstructor(void* pInstance)
    {
        InstanceType* pRealInstance = reinterpret_cast<InstanceType*>(pInstance);
        delete pRealInstance;
    }

    struct InstanceEntry
    {
        void* pInstance;
        DeconstructFunction* pFuncDeconstruct;

        InstanceEntry(void* a, DeconstructFunction* b) : pInstance{ a }, pFuncDeconstruct{ b }
        {}
    };
public:
    static InstanceManager& GetInstanceManager();

    template<typename InstanceType, typename ... InitArgs>
    InstanceType& RegisterIntance(InitArgs&&... args)
    {
        InstanceType* pResult = nullptr;

        pResult = new InstanceType(std::forward<InitArgs>(args)...);
        assert(pResult);
        m_instances.emplace_back(reinterpret_cast<void*>(pResult), &InstanceDeconstructor<InstanceType>);

        return *pResult;
    }
private:
    InstanceManager() = default;
    ~InstanceManager();
private:
    //Priority
    std::vector<InstanceEntry> m_instances;
};

