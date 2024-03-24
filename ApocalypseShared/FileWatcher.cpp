#include "FileWatcher.h"
#include "vendor/FileWatch.h"
#include "Logger.h"

FileWatcher::FileWatcher(const std::string& filePath, std::function<void()> callback)
{
    watch = new filewatch::FileWatch<std::string>(
            filePath,
            [callback](const std::string& path, const filewatch::Event event) {
                callback();
            }
    );
}

FileWatcher::~FileWatcher()
{
    delete (filewatch::FileWatch<std::string>*)watch;
}
