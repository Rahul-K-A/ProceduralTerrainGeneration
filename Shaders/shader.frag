  #version 330
  //Incoming data
  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;
  in float YPos;
  //Output data
  out vec4 colour;

   //Superstructure which stores data common to all types of lights
  struct Light{
    vec3 LightColour;
    float AmbientIntensity;
    float DiffuseIntensity;
  };

  //Directional light structure
  struct DirectionalLight{
    Light LightData;
    vec3 Direction;
  };

   struct Material{
    float SpecularIntensity;
    float Shininess;
  };


  uniform DirectionalLight dLight;
  //Material
  uniform Material material;
  //Camera position vector
  uniform vec3 CameraViewPosition;
  uniform float MaxHeight;

  
vec3 color_from_height( const float height )
{
    vec3 terrain_colours[4];
    terrain_colours[0] = vec3(0.0,0.0,0.6);
    terrain_colours[1] = vec3(0.1, 0.3, 0.1);
    terrain_colours[2] =  vec3(0.4, 0.8, 0.4);
    terrain_colours[3] = vec3(1.0,1.0,1.0);
    //vec3 terrain_colours[1] = vec3[1]{ vec3(0,0,0.6)};
    if(height <= 0.0)
        return terrain_colours[0];
    else
    {
        float hscaled = height*2.0 - 1e-05; // hscaled should range in [0,2)
        int hi = int(hscaled); // hi should range in [0,1]
        float hfrac = hscaled-float(hi); // hfrac should range in [0,1]
        if( hi <1 && hi<2)
            return mix( terrain_colours[1],terrain_colours[2],1); // blends between the two colours    
        else 
            return mix( terrain_colours[2],terrain_colours[3],1); // blends between the two colours
    }
    return vec3(1.0,1.0,0.0);
}


//Calculates light from a given direction
   vec4 CalculateLightByDirection(Light Light,vec3 Direction)
  {
   //Calculates ambient lighting
    vec4 AmbientLight= vec4(Light.LightColour,1.0f)*Light.AmbientIntensity;

    //Amount of diffuse lighting for the given normal. Varies between 0 and 1
    float DiffuseFactor= max( dot( normalize(Normal), normalize(Direction))  , 0.0f);

    //Diffuse Lighting final calculation 
    vec4 DiffuseLight= vec4(Light.LightColour, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;


    //Default specular lighting
    vec4 SpecularLight=vec4(0.0f,0.0f,0.0f,0.0f);

    if(DiffuseFactor>0.0f)
     {
         //Direction from vector a to vector b is normalize(b-a)
         //We find the direction of fragment from the point of the camera view
        vec3 CameraViewToFragmentDirection= normalize(CameraViewPosition-FragmentPosition);
        // glsl reflect function doesn't directly give you the resulting direction we are after but the reversed one. So we take the negative of the reflected vector
        vec3 ReflectedVertexDirection= normalize(reflect(Direction,normalize(Normal) ) );
       //Calculates specular lighting
        float SpecularFactor= pow(max (dot(CameraViewToFragmentDirection, ReflectedVertexDirection), 0.0), material.Shininess);
            if(SpecularFactor>0.0f)
            {
                 SpecularFactor=pow(SpecularFactor, material.Shininess );
                SpecularLight=vec4( Light.LightColour * SpecularFactor * material.SpecularIntensity ,1.0f);
            }
      }
      //Returns overall lighting
     return (AmbientLight+DiffuseLight+SpecularLight);
  }


  //Caclulates lighting for a given directional light
  vec4 CalculateDirectionalLight(){
     return CalculateLightByDirection(dLight.LightData,dLight.Direction);
  }


  void main()
  {
    float col=clamp(YPos,0,10);
    
    if ( col<=0.5f)
      colour= vec4(0.1725, 0.0588, 0.8078, 1.0);

    if ((col>0.5f) && (col<5.f))
      colour= vec4(0.2, 0.8863, 0.0588, 1.0);

    if ((col>5.f) && (col<9.5f))
      colour= vec4(0.3882, 0.2706, 0.1176, 1.0);

    if ((col>=9.5f) && (col<=10.f))
      colour= vec4(0.8588, 0.8235, 0.8235, 1.0); 

      colour=colour*CalculateDirectionalLight();
  }