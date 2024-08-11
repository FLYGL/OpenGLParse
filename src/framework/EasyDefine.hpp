#define JUMP_IF_SUCCESS(exp)    do { if((exp)) goto SUCCESS_STATE; } while(false)
#define JUMP_IF_FAIL(exp)       do { if(!(exp)) goto FAIL_STATE;} while(false)
#define DO_NOTHING()             {}