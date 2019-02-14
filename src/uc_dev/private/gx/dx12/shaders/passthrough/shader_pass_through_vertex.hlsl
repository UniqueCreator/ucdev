#include "shader_pass_through.h"
#include "../default_signature.hlsli"

struct vertex
{
    float4 position     : POSITION0;
};

struct interpolants
{
    float4 position     : SV_POSITION0;
};

[RootSignature( MyRS1 ) ]
interpolants main( vertex v )
{
    return v;
}
