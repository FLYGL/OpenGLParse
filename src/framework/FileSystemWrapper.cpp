#include "EasyDefine.hpp"
#include "FileSystemWrapper.hpp"

#include <fstream>

#ifdef WIN32
bool LoadFileContent(const std::string& rAbsolutePath, void** ppBuffer, size_t* puFileSize)
{
    bool bRetCode = false;
    bool bResult = false;
    FILE* pFile = nullptr;
    void* pBuffer = nullptr;
    int64_t nFileSize = -1;
    size_t uFileSize = 0;

    bRetCode = fopen_s(&pFile, rAbsolutePath.c_str(), "rb") == 0;
    JUMP_IF_FAIL(bRetCode);

    JUMP_IF_FAIL(_fseeki64(pFile, 0, SEEK_END) == 0);
    nFileSize = _ftelli64(pFile);
    JUMP_IF_FAIL(_fseeki64(pFile, 0, SEEK_SET) == 0);

    JUMP_IF_FAIL(nFileSize >= 0);

    uFileSize = static_cast<size_t>(nFileSize);
    JUMP_IF_SUCCESS(uFileSize == 0);
    //fixed allocator
    pBuffer = malloc(uFileSize);
    JUMP_IF_FAIL(pBuffer);
    bRetCode = fread_s(pBuffer, uFileSize, 1, uFileSize, pFile) == uFileSize;
    JUMP_IF_FAIL(bRetCode);

SUCCESS_STATE:
    *ppBuffer = pBuffer;
    *puFileSize = uFileSize;
    pBuffer = nullptr;
    bResult = true;

FAIL_STATE:
    if (!bResult)
    {
        free(pBuffer);
    }
    return bResult;
}
#else

bool LoadFileContent(const std::string& rAbsolutePath, void** ppBuffer, size_t* puFileSize)
{
    bool bRetCode = false;
    bool bResult = false;
    FILE* pFile = nullptr;
    void* pBuffer = nullptr;
    off_t nFileSize = -1;
    size_t uFileSize = 0;

    pFile = fopen(rAbsolutePath.c_str(), "rb");
    JUMP_IF_FAIL(pFile);

    JUMP_IF_FAIL(fseeko(pFile, 0, SEEK_END) == 0);
    nFileSize = ftello(pFile);
    JUMP_IF_FAIL(fseeko(pFile, 0, SEEK_SET) == 0);

    JUMP_IF_FAIL(nFileSize >= 0);

    uFileSize = static_cast<size_t>(nFileSize);
    JUMP_IF_SUCCESS(uFileSize == 0);
    //fixed allocator
    pBuffer = malloc(uFileSize);
    JUMP_IF_FAIL(pBuffer);
    bRetCode = fread(pBuffer, uFileSize, 1, pFile) == uFileSize;
    JUMP_IF_FAIL(bRetCode);

SUCCESS_STATE:
    *ppBuffer = pBuffer;
    *puFileSize = uFileSize;
    pBuffer = nullptr;
    bResult = true;

FAIL_STATE:
    if (!bResult)
    {
        free(pBuffer);
    }
    return bResult;
}

#endif