#include "FileWatcher.h"
#include "vendor/FileWatch.h"

void FileWatcher::WatchFile(std::string&& filePath, std::function<void()> callback)
{
    filewatch::FileWatch<std::string> watch{
            filePath,
            [callback](const std::string& path, const filewatch::Event event) {
                  callback();
            }
    };
}
