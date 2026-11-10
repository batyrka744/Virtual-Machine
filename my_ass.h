#ifndef STOP_ASSERTS
    #define my_assert(exp, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s]\n" RESET, __FILE__, __LINE__, #exp);\
    return ret;}    

    
    #define my_assert_tell(exp, text, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s]\n" RESET, __FILE__, __LINE__, text);\
    return ret;} 
#else
    #define my_assert(exp, ret) ;
    #define my_assert_tell(exp, text, ret) ;
#endif