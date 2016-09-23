#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include <map>
#include <unordered_map>

namespace GameEngine
{
  using AttribLocation = GLuint;
  using UniformLocation = GLuint;
  using ProgramID = GLuint;
  /** Shader structure to define a single shader object */
  struct Shader
  {
    /** Shader constructor needs only 3 values, because its ID is given to it by OpenGL
     * \param[in] _type The enum types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, or GL_GEOMETRY_SHADER.
     * \param[in] _filePath The file path to the shader file
     * \param[in] _name The name of the shader (can be anything, but preferably something to help you differentiate them)
     */
    Shader(GLenum _type, const std::string& _filePath, const std::string& _name)
    {
      type = _type;
      filePath = _filePath;
      name = _name;
    }
    Shader()
    {
    }
    GLenum type; ///< GLenum types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, or GL_GEOMETRY_SHADER. 
    ProgramID shaderID{ 0 }; ///< The ID of the shader which is gotten when it's created by glCreateShader
    std::string filePath{ "Default" }; ///< the filepath of the shader
    std::string name{ "Default" }; ///< the name of the shader which is declared from the user
  };

  /** This class handles the compilation, linking, and usage of a GLSL shader program. */
  class GLSLProgram
  {
  public:
    GLSLProgram();
    ~GLSLProgram();

    /** Compiles the passed shaders
     * \param[in] _shaders The shaders passed to the main program in order to be compiled
     */
    void CompileShaders(const std::vector<Shader>& _shaders);

    /** Compiling vertex and fragment shaders from source
     * \param[in] _vertexSource, _fragmentSource the source code of the vertex and fragment shader
     */
    void CompileShadersFromSource(const char* _vertexSource, const char* _fragmentSource);

    /** Links the shaders together */
    void LinkShaders();

    /** Returns the index of the named uniform block specified by uniformBlockName associated with the shader program.
    * If uniformBlockName is not a valid uniform block of the shader program, GL_INVALID_INDEX is returned
    * \param[in] _uniformBlockName The name of the requested uniform block
    * \return output is the GLint location of the uniform block in the shader
    */
    GLuint GetUniformBlockIndex(const std::string& _uniformBlockName);

    /** Returns the size of the block as generated by the compiler
    * \param[in] _index The index of the uniform block (Gotten from GetUniformBlockIndex)
    * \param[out] _params The GLint locations array of parameters in the specific uniform block
    */
    void GetUniformBlockDataSize(GLuint _index, GLint* _params);

    /** Returns the indices associated with the uniformCount uniform variables specified by name in the array uniformNames
    * in the array uniformIndices for this shader program
    * \param[in] _numUniforms The number of uniforms in an Uniform block
    * \param[in] _uniformNames The names of the uniform variables inside the uniform block
    * \param[out] _uniformIndices The outputted indices of the associated uniform variables
    */
    void GetUniformIndices(GLsizei _numUniforms, const char ** _uniformNames, GLuint * _uniformIndices);

    /**  Associate a buffer object with a specific uniform block
    *  If all the buffer storage is used for the uniform block, set offset to 0 and size to size of the buffer (equivalent to glBindBufferBase()).
    * \param[in] _target The target can either be GL_UNIFORM_BUFFER (for uniform blocks) or GL_TRANSFORM_FEEDBACK_BUFFER (for use with transform feedback)
    * \param[in] _index The index of the uniform block (Gotten from GetUniformBlockIndex)
    * \param[out] _buffer The buffer object that will get associated with the specific uniform block (index)
    * \param[in] _offset The specification of the starting index (can be 0 if the index is the starting one)
    * \param[in] _size The range of the buffer that is to be mapped to the uniform buffer. (can be the size of the buffer object)
    */
    void BindBufferRange(GLenum _target, GLuint _index, GLuint _buffer, GLintptr _offset, GLsizeiptr _size);

    /* Explicitly assigns uniformBlockIndex to uniformBlockBinding for the current shader program program.
    * Use when a specific uniform block is used in many shader programs, so that it avoids having the block be assigned a different index for each program
    * Must be called before calling LinkShaders
    * \param[in] _uniformBlockIndex The index that the uniform block will be assigned to
    * \param[in] _uniformBlockBinding The uniform block that will be explicitly bound to the uniformBlockIndex
    */
    void BlockUniformBinding(GLuint _uniformBlockIndex, GLuint _uniformBlockBinding);

    /* This function is used to get the offset, size and type of for an index of an Uniform block.
    * \param[in] _numUniforms The number of uniform variables in an Uniform block
    * \param[in] _uniformIndices The indices of the associated uniform variables
    * \param[in] _pname The possible enums: GL_UNIFORM_OFFSET, GL_UNIFORM_SIZE, GL_UNIFORM_TYPE (they specify the outputted attribute)
    * \param[out] _attribute The outputted attribute
    */
    void GetActiveUniformsIndexValues(GLsizei _numUniforms, GLuint * _uniformIndices, GLenum _pname, GLint * _attribute);

    /* Returns the location of the subroutine uniform named _name in the current shader program object for the shading stage specified by _shaderType.
    * \param[in] _shaderType Must be one of: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, or GL_GEOMETRY_SHADER.    
    * \param[in] _name The name of the uniform variable (null-terminated cstring
    * \return the location of the subroutine uniform named _name
    */
    GLint GetSubroutineUniformLocation(GLenum _shaderType, const std::string& _name);

    /* Determine the indices of the subroutines inside the shader
    * \param[in] _shaderType Must be one of: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, or GL_GEOMETRY_SHADER.    
    * \param[in] _name The name of the uniform variable (null-terminated cstring
    * \return the index of the shader function associated with _name from current shader program for the shading stage specified by _shaderType
    */
    GLuint GetSubroutineIndex(GLenum _shaderType, const std::string& _name);

    /* Specify which subroutine should be executed in the shader. All active subroutine uniforms for a shader stage must be initialized.
    * \param[in] _shaderType Must be one of: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, or GL_GEOMETRY_SHADER.    
    * \param[in] _numSubrUniforms The number of subroutine uniforms
    * \param[in] _indices The values which the shader subroutine uniforms are set to. The i'th subroutine uniform will be assigned the value indices[i]. 
    */
    void UniformSubroutinesuiv(GLenum _shaderType, GLsizei _numSubrUniforms, const GLuint * _indices);

    /** Begin using the shader*/
    void Use();

    /** Stop using the shader*/
    void UnUse();

    /** Dispose of the shader*/
    void Dispose();
    // Registers the location of an attribute in this shader (must be called after linking)
    void RegisterAttribute(const std::string& _attrib);
    // Registers the location of a uniform in this shader (must be called after linking)
    void RegisterUniform(const std::string& _uniform);
    ///accesses elements : shaders/uniforms;
    AttribLocation GetAttribLocation(const std::string& _attrib);
    UniformLocation GetUniformLocation(const std::string& _uniform);

  private:
    /// The number of attributes in a shader (the in values)
    //int m_numAttributes;
    /// Compile a single shader program
    void CompileShader(const char* _source, const std::string& _name, GLuint _id);

    /** Gets the location of a specific attribute in the shader program
    * \param[in] _attributeName The name of the searched attribute
    * \return the GLint location of the specified attribute
    */
    AttribLocation GetAttribLoc(const std::string& _attributeName);

    /** Get the uniform location
    * \param[in] _uniformName The name of the requested uniform
    * \return output is the GLint location of the uniform in the shader
    */
    UniformLocation GetUniformLoc(const std::string& _uniformName);

    /// The program ID of the whole shader program
    ProgramID m_programID;
    /// A vector containing all the shaders of 1 GLSL program object
    std::vector<Shader> m_shaders;

    std::unordered_map<std::string, AttribLocation> m_attribList;
    std::unordered_map<std::string, UniformLocation> m_unifLocationList;
  };
}