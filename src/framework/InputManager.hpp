#include "EasyDefine.hpp"
#include <vector>

typedef void (*InputCallback)(int nKey, int nAction, void* pContext);

class InputManager
{
public:
    ~InputManager() = default;
public:
    static size_t RegisterInputCallback(InputCallback, void* pContext);
    static void UnRegisterInputCallback(size_t uUniqueId);
    static void FireInputCallback(int nKey, int nAction);
private:
    static InputManager& GetInstance();
    InputManager() = default;
    struct InputCallbackInfo
    {
        size_t uUniqueId;
        InputCallback   pCallback;
        void* pContext;
        InputCallbackInfo(size_t uUniqueId, InputCallback pCallback, void* pContext) :
            uUniqueId{uUniqueId}, pCallback{pCallback}, pContext{pContext}
        {}
    };
private:
    std::vector<InputCallbackInfo> m_registeredCallback;
    size_t m_uUniqueId = 0;
};