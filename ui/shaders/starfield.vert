#version 450

layout(location = 0) in vec4 qt_Vertex;
layout(location = 1) in vec2 qt_MultiTexCoord0;
layout(location = 0) out vec2 fragCoord;
layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec3 iResolution; // viewport resolution (in pixels)
    float iTime;      // shader playback time (in seconds)
};

void main()
{
    fragCoord = vec2(qt_Vertex);
    gl_Position = qt_Matrix * qt_Vertex;
}
