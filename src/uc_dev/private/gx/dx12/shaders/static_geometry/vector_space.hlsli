#ifndef __transform_hlsli__
#define __transform_hlsli__

/////////////////////////////////////////////////////////////////
struct point_3d
{
    float3 m_value;
};

struct point_ws : point_3d
{
    
};

struct point_vs : point_3d
{
};

struct point_os : point_3d
{
};

struct point_ps
{
    float4 m_value;
};

/////////////////////////////////////////////////////////////////
struct vector_3d
{
    float3 m_value;
};

struct vector_ws : vector_3d
{
};

struct vector_vs : vector_3d
{
};

struct vector_os : vector_3d
{
};

struct vector_ps
{
    float4 m_value;
};
/////////////////////////////////////////////////////////////////

struct euclidean_transform_3d
{
    float4x4 m_value;
};

struct affine_transform_3d
{
    float4x4 m_value;
};

struct projective_transform_3d
{
    float4x4 m_value;
};

/////////////////////////////////////////////////////////////////

point_3d make_point_3d(float3 v)
{
    point_3d r;
    r.m_value = v;
    return r;
}

point_ws make_point_ws(float3 v)
{
    point_ws r;
    r.m_value = v;
    return r;
}

point_vs make_point_vs(float3 v)
{
    point_ws r;
    r.m_value = v;
    return r;
}

point_ps make_point_ps(float4 v)
{
    point_ps r;
    r.m_value = v;
    return r;
}

point_os make_point_os(float3 v)
{
    point_ws r;
    r.m_value = v;
    return r;
}


vector_3d make_vector_3d(float3 v)
{
    vector_3d r;
    r.m_value = v;
    return r;
}

vector_ws make_vector_ws(float3 v)
{
    vector_ws r;
    r.m_value = v;
    return r;
}

vector_vs make_vector_vs(float3 v)
{
    vector_ws r;
    r.m_value = v;
    return r;
}

vector_ps make_vector_ps(float4 v)
{
    vector_ps r;
    r.m_value = v;
    return r;
}

vector_os make_vector_os(float3 v)
{
    vector_ws r;
    r.m_value = v;
    return r;
}

point_ps project_p_os(point_os v_os, euclidean_transform_3d world, euclidean_transform_3d view, projective_transform_3d perspective)
{
    //three muls for greater accuracy
    float4 result = mul(mul(mul(float4(v_os.m_value, 1.0f), world.m_value), view.m_value), perspective.m_value);
    return make_point_ps(result);
}

point_ps project_p_ws(point_ws v_os, euclidean_transform_3d view, projective_transform_3d perspective)
{
    //three muls for greater accuracy
    float4 result = mul(mul(float4(v_os.m_value, 1.0f), view.m_value), perspective.m_value);
    return make_point_ps(result);
}



point_3d transform_p_3d(point_3d p, euclidean_transform_3d t)
{
    float4 result = mul(float4(p.m_value, 1.0f), t.m_value);
    return make_point_3d(result.xyz);
}

vector_3d transform_v_3d(vector_3d v, euclidean_transform_3d world)
{
    float4 result = mul(float4(v.m_value, 0.0f), world.m_value);
    return make_vector_3d(result.xyz);
}

point_ws transform_p_os(point_os v_os, euclidean_transform_3d world)
{
    point_3d r0 = transform_p_3d(v_os, world);
    return make_point_ws(r0.m_value);
}

vector_ws transform_v_os(vector_os v_os, euclidean_transform_3d world)
{
    vector_3d r0 = transform_v_3d(v_os, world);
    return make_vector_ws(r0.m_value);
}


#endif
