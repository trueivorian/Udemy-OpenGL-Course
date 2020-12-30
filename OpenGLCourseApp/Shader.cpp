#include "Shader.h"

Shader::Shader() : shaderID(0), uniformModel(0), uniformProjection(0){}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode){
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode){

	shaderID = glCreateProgram(); // Creates the program giving shader an ID

	if (!shaderID) {
		printf("Error creating shader program\n");
		return;
	}

	// When adding the shader, the graphics card needs to know what kind
	// of shader it will be. The Enum GL_VERTEX_SHADER helps us specify this
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	// Getting the error code from the creation of the shaders after linking
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Creates the executables on the graphics card, linking all the programs together
	glLinkProgram(shaderID);

	// Makes sure the program has been linked properly
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s \n", eLog);
		return;
	}

	// Checks if the created shader is valid in the current openGL context
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s \n", eLog);
		return;
	}

	// The uniforms in the shader are bound to the GLuints in the program
	uniformModel = glGetUniformLocation(shaderID, "model"); // Binds the model variable using a uniform location type
	uniformProjection = glGetUniformLocation(shaderID, "projection"); // projection variable
	uniformView = glGetUniformLocation(shaderID, "view"); // view variable
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	// parameters:-
	// GLuint shader: The shader we are modifying the code for
	// GLsizei count: The number of string arguments
	// const GLchar *const *string: The code itself
	// const GLint *length: The length of the code
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Makes sure the program has been compiled properly
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);

}

GLuint Shader::GetProjectionLocation(){
	return uniformProjection;
}

GLuint Shader::GetModelLocation(){
	return uniformModel;
}

GLuint Shader::GetViewLocation() {
	return uniformView;
}

void Shader::UseShader(){

	// Shader error checking
	if (!shaderID) {
		printf("Shader Error");
		return;
	}
	else {
		glUseProgram(shaderID);
	}
}

void Shader::ClearShader(){
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader(){
	ClearShader();
}
