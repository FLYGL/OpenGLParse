#define JUMP_IF_SUCCESS(exp)    do { if((exp)) goto SUCCESS_STATE; } while(false)
#define JUMP_IF_FAIL(exp)       do { if(!(exp)) goto FAIL_STATE;} while(false)
#define DO_NOTHING()             {}
#define OUT
#define ANONYMOUS_SCOPE_BEGIN   namespace{
#define ANONYMOUS_SCOPE_END     }


//supplement
#define GLFW_KEY_NONE -1