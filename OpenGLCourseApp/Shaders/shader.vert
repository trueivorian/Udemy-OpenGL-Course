	#version 330								             
												             
	layout (location = 0) in vec3 pos;			             
                                                             
    out vec4 vCol;                                           
                                                             
    uniform mat4 model;                                      
    uniform mat4 projection;
    uniform mat4 view;                                 
                                                             
	void main()                                              
    {                                                        
		gl_Position = projection * view * model * vec4(pos, 1.0);   
                                                             
        // The clamp means that negative values are placed   
        // back in the range of 0 and 1                      
        vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);           
	}   