#include "shader_loader.h"
#include <direct.h>
ShaderLoader::ShaderLoader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
	string vertexShaderCode;
	string fragmentShaderCode;
	string geometryShaderCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream gShaderFile;
	// ȷ�� ������ ���׳��쳣
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	gShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
        string vPath = concatString(getcwd(NULL, 0),vertexPath);
        string fPath = concatString(getcwd(NULL, 0),fragmentPath);
		vShaderFile.open(vPath);
		fShaderFile.open(fPath);
		stringstream vShaderStream;
		stringstream fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();

		vertexShaderCode = vShaderStream.str();
		fragmentShaderCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryShaderCode = gShaderStream.str();
		}
	}
	catch (ifstream::failure& e)
	{
		cout << "����::Shader::�ļ���ȡʧ��" << endl;
	}

	// ����shader
	unsigned int vertexShader;
	unsigned int fragmentShader;

	const char* vShaderCode = vertexShaderCode.c_str();
	const char* fShaderCode = fragmentShaderCode.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	// �������geometry shader���� ����geometry shader
	unsigned int geometryShader;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryShaderCode.c_str();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &gShaderCode, NULL);
		glCompileShader(geometryShader);
		checkCompileErrors(fragmentShader, "GEOMETRY");
	}

	// shader����
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometryPath != nullptr)
	{
		glAttachShader(ID, geometryShader);
	}
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// shader�Ѿ����ӵ����� ������Ҫ
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryPath != nullptr)
	{
		glDeleteShader(geometryShader);
	}
}

// ����shader
void ShaderLoader::use()
{
	glUseProgram(ID);
}

// uniform���ߺ���
void ShaderLoader::setBool(const string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderLoader::setInt(const string& name, int value)const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderLoader::setFloat(const string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderLoader::setVec2(const string& name, const vec2& value)const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderLoader::setVec2(const string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void ShaderLoader::setVec3(const string& name, const vec3& value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderLoader::setVec3(const string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void  ShaderLoader::setVec4(const string& name, const vec4& value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderLoader::setVec4(const string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void ShaderLoader::setMat2(const string& name, const mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderLoader::setMat3(const string& name, const mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderLoader::setMat4(const string& name, const mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


// ���shader ���� ���Ӵ���
void  ShaderLoader::checkCompileErrors(GLuint shader, string type) {
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "����::Shader�������::Type:" << type << endl << infoLog << endl << "-- --------------------------------------------------- --" << endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "����::Shader�������Ӵ���::Type:" << type << endl << infoLog << endl << "-- --------------------------------------------------- --" << endl;
		}
	}
}

string ShaderLoader:: concatString(char* s1, const char* s2){
    string temp = subreplace(string(s1 ),"\\","/") + string(s2) + "\0";
    return temp;
}

string ShaderLoader:: subreplace(string resource_str, string sub_str, string new_str)
{
    string::size_type pos = 0;
    while((pos = resource_str.find(sub_str)) != string::npos)   //�滻����ָ���Ӵ�
    {
        resource_str.replace(pos, sub_str.length(), new_str);
    }
    return resource_str;
}