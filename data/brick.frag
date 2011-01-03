//import: spherical_harmonics
//import: util

uniform sampler2DArray material;
varying vec3 texture_coordinate;

void main(){
    vec4 texel;
    texel = texture2DArray(material, texture_coordinate);
    gl_FragColor = texel;

    //vec3 outside = sh_light(data.normal, beach);
    //vec3 inside = sh_light(data.normal, groove)*0.004;
    //vec3 ambient = mix(outside, inside, data.occlusion);

    //vec3 color = material_color*ambient;
    //fragment = gamma(color);
}
