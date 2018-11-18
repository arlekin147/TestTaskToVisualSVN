#include <windows.h>
#include <iostream>

enum errors {canNotOpenInputFile = 1,
             canNotOpenOutputFile,
             canNotToGetSizeOfFile,
             invalidNumberOfArguments,
             invalidSetFilePointer,
             invalidReadFile,
             invalidWriteFile};

int main(int argc, const char **argv){
    setlocale(LC_ALL, "Rus");
    if(argc != 3){
        std::wcout << L"Invalid number of arguments" << std::endl;
        return invalidNumberOfArguments; 
    }
    wchar_t *name = new wchar_t[strlen(argv[1]) + 1];
    mbstowcs(name, argv[1], strlen(argv[1]) + 1); //Перевод из char в wchar_t
    HANDLE hInputFile = CreateFileW(name, GENERIC_READ, FILE_SHARE_READ, nullptr,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);//Открыть файл источник для чтения
    
    if(hInputFile == INVALID_HANDLE_VALUE){
        std::wcout << L"Сan not open input file" << std::endl;
        return canNotOpenInputFile; 
    }
                
    delete(name);
    name = new wchar_t[strlen(argv[2]) + 1];
    mbstowcs(name, argv[2], strlen(argv[2]) + 1);           
    HANDLE hOutputFile = CreateFileW(name, GENERIC_WRITE, FILE_SHARE_READ, nullptr,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);//Создать результирующий файл для записи
                
    if(hOutputFile == INVALID_HANDLE_VALUE){
        std::wcout << L"Сan not open output file" << std::endl;
        return canNotOpenOutputFile; 
    }
    //Получение размера файла
    DWORD sizeOfFile = GetFileSize(hInputFile, nullptr);
    if(sizeOfFile == INVALID_FILE_SIZE){
        std::wcout << L"Can not to get size of file" << std::endl;
        return canNotToGetSizeOfFile;
    }
    char buf;
    DWORD dwBytes; //Побайтное считывание
    for (size_t i = 0; i < sizeOfFile; i++)
    {
        
        if(SetFilePointer(hInputFile, -(i + 1), nullptr, FILE_END) == INVALID_SET_FILE_POINTER){
            std::wcout << L"Unexpected error" << std::endl;
            return invalidSetFilePointer;
        }
        if(!ReadFile(hInputFile, &buf, 1, &dwBytes, nullptr)){ //sizeof(char) по спецификации всегда = 1
            std::wcout << L"Unexpected error" << std::endl;
            return invalidReadFile;
        }
         
        if(SetFilePointer(hOutputFile, i, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER){
            std::wcout << L"Unexpected error" << std::endl;
            return invalidSetFilePointer;
        }
        if(!WriteFile(hOutputFile, &buf, 1, &dwBytes, nullptr)){
            std::wcout << L"Unexpected error" << std::endl;
            return invalidWriteFile;
        }
    }
    CloseHandle(hInputFile);
    CloseHandle(hOutputFile);
    std::wcout << L"Success!" << std::endl;
}