/* Comment Rong Tao @Sylincom Beijing, 2020年 01月 19日 星期日 17:13:49 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2020年 01月 19日 星期日 17:11:44 CST. */
//解释器模式虽然听上去有些费解，但是如果用示例说明一下就不难理解了。
//我们知道在C语言中，关于变量的定义是这样的：一个不以数字开始的由字母、数字和下划线构成的字符串。
//这种形式的表达式可以用状态自动机解决，当然也可以用解释器的方式解决。

typedef struct _Interpret
{
    int type;
    void* (*process)(void* pData, int* type, int* result);	 
}Interpret;

// 上面的数据结构比较简单，但是很能说明问题。就拿变量来说吧，
// 这里就可以定义成字母的解释器、数字解释器、下划线解释器三种形式。
// 所以，我们可以进一步定义一下process的相关函数。

#define DIGITAL_TYPE 1
#define LETTER_TYPE  2
#define BOTTOM_LINE  3
 
void* digital_process(void* pData, int* type, int* result)
{
    UINT8* str;
    assert(NULL != pData && NULL != type && NULL != result);
 
    str = (UNT8*)pData;
    while (*str >= '0' && *str <= '9')
    {
        str ++;
    } 
 
    if(*str == '\0')
    {
        *result = TRUE;
        return NULL;
    }   
 
    if(*str == '_')
    {
        *result = TRUE;
        *type = BOTTOM_TYPE;
        return str;
    }
 
    if(*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
    {
        *result = TRUE;
        *type = LETTER_TYPE;
        return str;
    }
 
    *result = FALSE;
    return NULL;            
}    
 
void* letter_process(void* pData, int* type, int* result)
{
    UINT8* str;
    assert(NULL != pData && NULL != type && NULL != result);
 
    str = (UNT8*)pData;
    while (*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
    {
        str ++;
    } 
 
    if(*str == '\0')
    {
        *result = TRUE;
        return NULL;
    }   
 
    if(*str == '_')
    {
        *result = TRUE;
        *type = BOTTOM_TYPE;
        return str;
    }
 
    if(*str >= '0' && *str <= '9')
    {
        *result = TRUE;
        *type = DIGITAL_TYPE;
        return str;
    }
 
    *result = FALSE;
    return NULL;            
}          
 
void* bottom_process(void* pData, int* type, int* result)
{
    UINT8* str;
    assert(NULL != pData && NULL != type && NULL != result);
 
    str = (UNT8*)pData;
    while ('_' == *str )
    {
        str ++;
    } 
 
    if(*str == '\0')
    {
        *result = TRUE;
        return NULL;
    }   
 
    if(*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
    {
        *result = TRUE;
        *type = LETTER_TYPE;
        return str;
    }
 
    if(*str >= '0' && *str <= '9')
    {
        *result = TRUE;
        *type = DIGITAL_TYPE;
        return str;
    }
 
    *result = FALSE;
    return NULL;            
} 
