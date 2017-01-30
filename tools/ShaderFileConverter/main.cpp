/*
 * Shader file converter tool
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifdef _WIN32


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <Windows.h>


std::string ExtractFileName(const std::string& filename)
{
    /* Find position with filename only */
    size_t Pos = filename.find_last_of("/\\");

    if (Pos == std::string::npos)
        return filename;

    /* Return filename only */
    return filename.substr(Pos + 1);
}

std::string ExtractFilePath(const std::string& filename)
{
    /* Return file path only */
    auto pos = filename.find_last_of("\\/");
    return pos != std::string::npos ? filename.substr(0, pos) : ".";
}

bool ConvertSingleShaderFile(const std::string& filename, std::ofstream& outFile)
{
    /* Open input file for reading */
    std::ifstream inFile(filename, std::ios_base::binary);

    if (!inFile.good())
    {
        std::cerr << "Reading file \"" << filename << "\" failed" << std::endl;
        return false;
    }

    /* Scan each input file */
    while (!inFile.eof())
    {
        /* Read line */
        std::string line;
        std::getline(inFile, line);

        /* Check for include directive */
        if (line.substr(0, 8) == "#include")
        {
            auto start = line.find('\"');
            if (start != std::string::npos)
            {
                auto end = line.find('\"', start + 1);

                if (end != std::string::npos)
                {
                    /* Extract include filename */
                    auto includeFilename = ExtractFilePath(filename) + "/" + line.substr(start + 1, end - start - 1);

                    /* Read include file */
                    ConvertSingleShaderFile(includeFilename, outFile);
                    continue;
                }
            }
        }

        /* Remove new-line character */
        while ( !line.empty() && ( line.back() == '\n' || line.back() == '\r' ) )
            line.pop_back();

        /* Replace '\"' quotation marks */
        for (auto it = line.begin(); it != line.end(); ++it)
        {
            if (*it == '\"')
            {
                it = line.insert(it, '\\');
                ++it;
            }
        }

        /* Convert line */
        line = "\"" + line + "\\n\"";

        /* Write line to output file */
        outFile << line << std::endl;
    }

    return true;
}

bool ConvertShaderFile(const std::string& filename, const std::string& outputPath)
{
    /* Extract file part from filename */
    const auto filePart = ExtractFileName(filename);

    /* Open output file for writing */
    const auto outFilename = outputPath + filePart + ".h";
    std::ofstream outFile(outFilename, std::ios_base::binary);

    if (!outFile.good())
    {
        std::cerr << "Writing file \"" << outFilename << "\" failed" << std::endl;
        return false;
    }

    /* Print information */
    std::cout << "Convert file: \"" << filePart << "\"" << std::endl;

    return ConvertSingleShaderFile(filename, outFile);
}

bool ListFiles(std::string path, std::vector<std::string>& files, bool searchRecursive = false)
{
    HANDLE findHandle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA findData;
    std::string spec;
    std::stack<std::string> directories;

    directories.push(path);
    files.clear();

    while (!directories.empty())
    {
        path = directories.top();
        spec = path + "*";
        directories.pop();

        /* Find first file in directory */
        findHandle = FindFirstFileA(spec.c_str(), &findData);
        if (findHandle == INVALID_HANDLE_VALUE)
            return false;

        do
        {
            auto filename = std::string(findData.cFileName);

            if (filename != "." && filename != "..")
            {
                if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
                {
                    if (searchRecursive)
                    {
                        /* Add path to directory list */
                        directories.push(path + filename + "/");
                    }
                }
                else
                {
                    /* Get file extension */
                    std::string ext = (filename.size() >= 5 ? filename.substr(filename.size() - 5) : "");

                    if (ext == ".glsl" || ext == ".hlsl")
                    {
                        /* Add filename to list */
                        files.push_back(path + filename);
                    }
                }
            }
        }
        while (FindNextFileA(findHandle, &findData) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES)
        {
            FindClose(findHandle);
            return false;
        }

        FindClose(findHandle);
        findHandle = INVALID_HANDLE_VALUE;
    }

    return true;
}

void Wait()
{
    system("pause");
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Not enough arguments -> nothing to do" << std::endl;
        Wait();
        return 0;
    }

    size_t numSuccess = 0, numFailed = 0;

    /* Collect program arguments */
    for (int i = 1; i < argc; ++i)
    {
        /* Get current path */
        std::string path = argv[i];

        if (path.back() != '/' && path.back() != '\\')
            path += '/';

        std::cout << "Searching path: " << path << std::endl;

        /* List all filenames */
        std::vector<std::string> inputFilenames;
        if (!ListFiles(path, inputFilenames))
            std::cerr << "Listing files in path \"" + path + "\" failed" << std::endl;

        /* Convert files */
        for (const auto& filename : inputFilenames)
        {
            if (ConvertShaderFile(filename, path + "Resources/"))
                ++numSuccess;
            else
                ++numFailed;
        }
    }

    /* Print final message */
    std::cout << "Successful conversions: " << numSuccess << std::endl;
    if (numFailed > 0)
        std::cout << "Failed conversions: " << numFailed << std::endl;

    //Wait();

    return 0;
}


#else


#include <iostream>


int main()
{
    std::cerr << "This tool is only supported on MS/Windows" << std::endl;
    return 0;
}


#endif



// ========================

