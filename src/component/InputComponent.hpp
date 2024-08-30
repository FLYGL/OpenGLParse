#include <framework/EasyDefine.hpp>
#include <framework/InputManager.hpp>
class InputComponent
{
public:
    InputComponent(KeyInputCallback, MousemoveCallback, void*);
    InputComponent() = default;
    void SetCallback(KeyInputCallback, MousemoveCallback, void*);
    ~InputComponent();
private:
    size_t m_uUniqueId;
};