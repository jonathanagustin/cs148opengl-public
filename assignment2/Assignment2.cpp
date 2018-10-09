#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <ostream>
#include <fstream>
#include <sstream>

namespace
{
    const int SHADER_ERROR_LOG_SIZE = 500;
    bool VerifyShaderCompile(GLuint newShaderObject)
    {
        GLint compileStatus;
        OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
        if (compileStatus != GL_TRUE) {
            char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
            OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
            std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
            return false;
        }
        return true;
    }
    
    bool VerifyProgramLink(GLuint shaderProgram)
    {
        GLint linkStatus;
        OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
        if (linkStatus != GL_TRUE) {
            char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
            OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
            std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
            return false;
        }
        return true;
    }
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
Application(std::move(inputScene), std::move(inputCamera))
{
    vertexPositions =
    std::initializer_list<glm::vec4>({
        // Triangle 1
        {0.f, 0.f, 0.f, 1.f},
        {1.f, 0.f, 0.f, 1.f},
        {1.f, 1.f, 0.f, 1.f},
        // Triangle 2
        {0.f, 0.f, 0.f, 1.f},
        {-1.f, 1.f, 0.f, 1.f},
        {-1.f, 0.f, 0.f, 1.f}
    });
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

void Assignment2::SetupExample1()
{
    const std::string VERT_FILENAME = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
    const std::string FRAG_FILENAME = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
    
    std::ifstream vert_shader_file;
    std::ifstream frag_shader_file;
    
    std::string vert_code;
    std::string frag_code;
    
    try {
        /* OPEN FILES */
        vert_shader_file.open(VERT_FILENAME);
        frag_shader_file.open(FRAG_FILENAME);
        std::stringstream vert_stream, frag_stream;
        
        /* READ FILE BUFFER CONTENTS INTO STREAMS */
        vert_stream << vert_shader_file.rdbuf();
        frag_stream << frag_shader_file.rdbuf();
        
        /* CLOSE FILES */
        vert_shader_file.close();
        frag_shader_file.close();
        
        /* CONVERT STREAM INTO STRING */
        vert_code = vert_stream.str();
        frag_code = frag_stream.str();
        
    } catch (std::ifstream::failure e) {
        std::cout << "Error with file operations." << std::endl;
    }
    
    /* CONVERT TO C STRING */
    const char* vert_shader_code = vert_code.c_str();
    const char * frag_shader_code = frag_code.c_str();
    
    /* VERTEX SHADER */
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vert_shader_code, NULL);
    glCompileShader(vertex);
    
    /* FRAGMENT SHADER */
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &frag_shader_code, NULL);
    glCompileShader(fragment);
    
    /* SHADER PROGRAM */
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.
    const GLuint vertexShaderId = vertex;
    const GLuint fragmentShaderId = fragment;
    const GLuint shaderProgramId = program;
    
    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }
    
    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    
    // Insert "Setup Buffers" code here.
    
    /* CREATE VERTEX ARRAY OBJECT ID */
    glGenVertexArrays(1, &(Assignment2::vao));
    
    /* BIND THE VERTEX ARRAY OBJECT */
    glBindVertexArray(Assignment2::vao);
    
    /* GENERATE BUFFER ID FOR VERTEX POSITIONS */
    glGenBuffers(1, &(Assignment2::vertexbuffer));
    
    /* BIND THE BUFFER */
    glBindBuffer(GL_ARRAY_BUFFER, Assignment2::vertexbuffer);
    
    /* PASS THE VERTEX POSITION DATA */
    /* 24 VERTICES * 4 BYTES/VERTEX = 96 BYTES */
    glBufferData(GL_ARRAY_BUFFER, 96, &(vertexPositions[0]), GL_STATIC_DRAW);
    
    /* LET OPENGL KNOW HOW THE CURRENTLY BOUND BUFFER'S DATA SHOULD BE USED */
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    /* MAKES THE CURRENT BUFFER OBJECT TO BE PASSED TO OPENGL ALONG WITH THE CURRENT VERTEX ARRAY OBJECT */
    glEnableVertexAttribArray(0);
}

void Assignment2::Tick(double deltaTime)
{
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.
    
    /* INCREMENT TIME */
    time += deltaTime;
    
    /* SEND BUFFERS TO GPU */
    glUseProgram(program);
    
    /* FIGURE OUT LOCATION OF THE SHADER UNIFORM */
    GLuint timeuniform = glGetUniformLocation(program, "inputTime");
    
    /* SET TIME TO UNIFORM VARIABLE IN THE SHADER */
    glUniform1f(timeuniform, time);
    
    /* BIND THE VERTEX ARRAY OBJECT */
    glBindVertexArray(Assignment2::vao);
    
    /* SEND DRAW COMMAND TO OPEN GL */
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
