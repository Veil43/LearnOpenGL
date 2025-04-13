#ifndef CH_COMMON_H
#define CH_COMMON_H

#include "types.h"
#include "object.h"
#include "shader.h"
#include "camera.h"
#include "math.h"

struct ch_return
{
    Object3D obj;
    Shader shader;
};

struct lighting_return
{
    Shader light_shader;
    Shader object_shader;
};
#endif // CH_COMMON_H