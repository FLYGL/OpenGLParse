#include <framework/EasyDefine.hpp>
#include <framework/InputManager.hpp>
class InputComponent
{
public:
    InputComponent(InputCallback, void*);
    ~InputComponent();
private:
    size_t m_uUniqueId;
};