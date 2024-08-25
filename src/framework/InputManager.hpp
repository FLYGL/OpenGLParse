#include "EasyDefine.hpp"
#include <vector>

typedef void (*KeyInputCallback)(int nKey, int nAction, void* pContext);
typedef void (*MousemoveCallback)(double xpos, double ypos, void* pContext);

class InputManager
{
    friend class InstanceManager;
public:
    ~InputManager() = default;
public:
    static size_t RegisterInputCallback(KeyInputCallback, MousemoveCallback, void* pContext);
    static void UnRegisterInputCallback(size_t uUniqueId);
    static void FireKeyInputCallback(int nKey, int nAction);
    static void FireMousemoveCallback(double xpos, double ypos);
private:
    static InputManager& GetInstance();
    InputManager() = default;
    struct InputCallbackInfo
    {
        size_t uUniqueId;
        KeyInputCallback    pKeyInputCallback;
        MousemoveCallback   pMousemoveCallback;
        void* pContext;
        InputCallbackInfo(size_t uUniqueId, KeyInputCallback pKeyInputCallback, MousemoveCallback pMousemoveCallback,void* pContext) :
            uUniqueId{uUniqueId}, pKeyInputCallback{pKeyInputCallback}, pMousemoveCallback{pMousemoveCallback}, pContext{pContext}
        {}
    };
private:
    std::vector<InputCallbackInfo> m_registeredCallback;
    size_t m_uUniqueId = 0;
};