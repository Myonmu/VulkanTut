vec3 packNormal(vec3 normal){
    return ( normal + 1 ) / 2;
}

vec3 unpackNormal(vec3 packedNormal){
    return packedNormal * 2 - 1;
}