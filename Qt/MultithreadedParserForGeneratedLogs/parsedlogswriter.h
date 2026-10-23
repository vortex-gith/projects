#ifndef PARSEDLOGSWRITER_H
#define PARSEDLOGSWRITER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdlib>

#include "safebuffer.h"

class ParsedLogWriter
{
private:
    std::unordered_map<std::string, logInfo> current_log_vector;
public:
    void write(SafeLogMap& log_vector);
};

#endif // PARSEDLOGSWRITER_H
