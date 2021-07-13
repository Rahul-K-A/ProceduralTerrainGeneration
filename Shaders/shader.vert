  #version 330

  layout (location = 0) in vec3 position;
  layout (location = 1) in vec3 DiffuseLightVec;

  //out vec4 vColour;
  out vec2 TextureData;
  out vec3 Normal;
  out vec3 FragmentPosition;
  out float YPos;

  uniform mat4 model;
  uniform mat4 projection; 
  uniform mat4 view;


  void main()
  {
     //Position of object in world space
    gl_Position= projection*view*model*vec4(position,1.f);
    //Posiiton of face with respect to the camera POV
    Normal=mat3(transpose(inverse(model)))*DiffuseLightVec;
    FragmentPosition=(model * vec4(position,1.0f)).xyz;
    YPos=position.y;

  }