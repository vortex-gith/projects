#ifndef RAWLOGSWRITER_H
#define RAWLOGSWRITER_H

#include <utility>
#include <mutex>
#include <filesystem>
#include <fstream>

#include "safebuffer.h"

class RawLogsWriter
{
private:
    SafeBuffer currentSafeVector;
public:
    void write(SafeBuffer& SafeVectorForParse, SafeBuffer& SafeVectorForSimpleWriter);
};

#endif // RAWLOGSWRITER_H
