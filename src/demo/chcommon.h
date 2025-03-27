#ifndef CH_COMMON_H
#define CH_COMMON_H

#include "types.h"
#include "object.h"
#include "shader.h"

struct ch_return
{
    Object3D obj;
    Shader shader;
};

#endif // CH_COMMON_H