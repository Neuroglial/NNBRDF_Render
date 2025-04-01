#include <windows.h>
#include <shobjidl.h>
#include <iostream>
#include <string>
#include <comdef.h>

// 将宽字符 (wchar_t*) 转换为 UTF-8 编码的 std::string
inline std::string WStringToUTF8(const wchar_t *wstr)
{
    if (wstr == nullptr)
        return "";

    int size_needed = WideCharToMultiByte(
        CP_UTF8,         // 目标编码：UTF-8
        0,               // 转换标志（无特殊要求）
        wstr,            // 输入的宽字符字符串
        -1,              // 自动计算输入长度（-1 表示以 null 结尾）
        nullptr,         // 输出缓冲区（nullptr 仅获取所需长度）
        0,               // 输出缓冲区大小（0 表示获取长度）
        nullptr, nullptr // 默认字符和是否使用默认字符（此处忽略）
    );

    if (size_needed <= 0)
    {
        return "";
    }

    std::string utf8_str(size_needed, 0);
    WideCharToMultiByte(
        CP_UTF8, 0, wstr, -1,
        &utf8_str[0], size_needed,
        nullptr, nullptr);

    // 移除末尾的 null 终止符（因为 std::string 会自行处理）
    utf8_str.pop_back();
    return utf8_str;
}

inline std::string GetFile(COMDLG_FILTERSPEC fileTypes[] = nullptr, UINT TypesSize = 0)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
    {
        std::cerr << "COM初始化失败。错误代码: 0x" << std::hex << hr << std::endl;
        return "";
    }

    IFileOpenDialog *pFileOpen = nullptr;
    hr = CoCreateInstance(
        CLSID_FileOpenDialog,
        nullptr,
        CLSCTX_ALL,
        IID_IFileOpenDialog,
        reinterpret_cast<void **>(&pFileOpen));

    if (FAILED(hr))
    {
        std::cerr << "创建文件对话框失败。错误: " << _com_error(hr).ErrorMessage() << std::endl;
        CoUninitialize();
        return "";
    }

    // 定义文件类型过滤器
    // COMDLG_FILTERSPEC fileTypes[] = {
    //     {L"(*.txt)", L"*.txt"},              // 仅允许 .txt
    //     {L"(*.jpg, *.png)", L"*.jpg;*.png"}, // 允许 .jpg 和 .png
    //     {L"(*.*)", L"*.*"}                   // 可选：允许所有文件
    // };

    // 设置选项：强制文件系统路径
    // 设置过滤器到对话框
    // hr = pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
    
    if (TypesSize > 0)
    {
        hr = pFileOpen->SetFileTypes(TypesSize, fileTypes);
        if (FAILED(hr))
        {
            std::cerr << "设置文件类型失败。错误: " << _com_error(hr).ErrorMessage() << std::endl;
            pFileOpen->Release();
            CoUninitialize();
            return "";
        }

        // 设置默认选中第一个过滤器（索引从1开始）
        hr = pFileOpen->SetFileTypeIndex(1);
        if (FAILED(hr))
        {
            std::cerr << "设置默认文件类型失败。错误: " << _com_error(hr).ErrorMessage() << std::endl;
        }
    }

    DWORD dwOptions;
    hr = pFileOpen->GetOptions(&dwOptions);
    if (SUCCEEDED(hr))
    {
        hr = pFileOpen->SetOptions(dwOptions | FOS_FORCEFILESYSTEM);
    }

    if (FAILED(hr))
    {
        std::cerr << "设置选项失败。错误: " << _com_error(hr).ErrorMessage() << std::endl;
        pFileOpen->Release();
        CoUninitialize();
        return "";
    }

    // 显示对话框
    hr = pFileOpen->Show(nullptr);
    if (SUCCEEDED(hr))
    {
        IShellItem *pItem = nullptr;
        hr = pFileOpen->GetResult(&pItem);
        if (SUCCEEDED(hr))
        {
            PWSTR pszFilePath = nullptr;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
            if (SUCCEEDED(hr))
            {
                // 将宽字符路径转换为 UTF-8 编码的 std::string
                std::string utf8_path = WStringToUTF8(pszFilePath);
                return utf8_path;
                CoTaskMemFree(pszFilePath);
            }
            pItem->Release();
        }
    }
    pFileOpen->Release();
    CoUninitialize();
    return 0;
}