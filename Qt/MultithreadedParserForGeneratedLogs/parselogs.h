#ifndef PARSELOGS_H
#define PARSELOGS_H

#include <mutex>
#include <ctime>
#include <cstdlib>
#include <utility>

#include "safebuffer.h"

class ParseLogs
{
private:
    SafeBuffer currentSafeVector;
    SafeLogMap current_log_vector;
public:
    void parse(SafeBuffer& SafeVectorForParsedWriter, SafeLogMap& log_vector, SafeBuffer& SafeVectorForParse);
};

#endif // PARSELOGS_H
