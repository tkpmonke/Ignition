#include "window.hpp"
#include "stdio.h"

#include "utils/io.hpp"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
   if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
      return;

  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

float quad[30] = {
      -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
       1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

      -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
       1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

const char* quadVertex = 
"#version 430 core\n"
"layout(location=0) in vec3 pos;"
"layout(location=1) in vec2 uv;"
"out vec2 UV;"
"void main() {"
"  UV=uv;"
"  gl_Position = vec4(pos, 1);"
"}\0";

const char* quadFragment =
"#version 430 core\n"
"out vec4 fragColor;"
"in vec2 UV;"
"uniform sampler2D tex;"
"void main() {"
"  fragColor = vec4(texture(tex, UV).rgb, 1);"
"}\0";

namespace Ignition {
   bool windowOpen;
   void Window::Resize(int x, int y)
   {
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      glViewport(0, 0, x, y);
   
      glBindTexture(GL_TEXTURE_2D, this->color);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);

      glBindTexture(GL_TEXTURE_2D, this->depth);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, x, y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth, 0);

   }

   Window::Window(int w, int h, const char* c, bool* b)
      : isopen(b)
   {
      windowOpen = true;

      Ignition::IO::DebugPrint("Initilizing GLFW");

      glfwInit();
      glfwWindowHint(GLFW_SAMPLES, 8);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      this->window = glfwCreateWindow(w,h,c,NULL,NULL);

      Ignition::IO::DebugPrint("Creating OpenGL Context");
      glfwMakeContextCurrent(this->window);

      Ignition::IO::DebugPrint("Loading OpenGL Functions");
      glewInit();

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS); 

      glEnable(GL_BLEND);  
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      glEnable(GL_MULTISAMPLE); 

      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      glFrontFace(GL_CW);

#ifdef DEBUG
      //glEnable(GL_DEBUG_OUTPUT);
      //glDebugMessageCallback(MessageCallback, 0);
#endif
      
      Ignition::IO::DebugPrint("Creating FrameBuffer Shader");
      s = Rendering::Shader(quadVertex, quadFragment, Rendering::ShaderType::Unlit);

      Ignition::IO::DebugPrint("Creating FrameBuffer Quad");
      unsigned int VBO;
      glGenVertexArrays(1, &this->vao);
      glGenBuffers(1, &VBO);
      glBindVertexArray(this->vao);
      
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), quad, GL_STATIC_DRAW);
      
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

      Ignition::IO::DebugPrint("Creating FrameBuffer");
      
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);

      glGenFramebuffers(1, &framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

      glGenTextures(1, &color);
      glBindTexture(GL_TEXTURE_2D, color);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      //glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, true);

      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);

      glGenTextures(1, &depth);
      glBindTexture(GL_TEXTURE_2D, depth);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

      if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	      std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

      Ignition::IO::DebugPrint("Binding FrameBuffer 0");
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   bool Window::IsOpen() { 
      if (glfwWindowShouldClose(this->window)) { 
         Close();
         return false;
      }
      return windowOpen; 
   }

   void Window::Update() { glfwPollEvents(); }

   void Window::Bind() {
      glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
   }

   void Window::Shutdown()
   {

      Ignition::IO::DebugPrint("Shutting Down");
      glfwDestroyWindow(this->window);
      glfwTerminate();
   }

   void Window::Close()
   {

      Ignition::IO::DebugPrint("Closing");
      *isopen = false;
      windowOpen = false;
      glfwWindowShouldClose(window);
   }

   void Window::Restart()
   {
      Ignition::IO::DebugPrint("Restarting");
      windowOpen = false;
      glfwWindowShouldClose(window);
   }
}
