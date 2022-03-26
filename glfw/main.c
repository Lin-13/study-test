#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLfloat vertices[] = {
    -0.5f,-0.5f, 0.0f,
     0.5f,-0.5f, 0.0f,
     0.0f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
     0.0f,-0.5f, 0.0f
};
const GLchar* const VertexShaderSource= {//顶点着色器
    "#version 330 core\n"
    "layout (location =0) in vec3 position;\n"
    "void main(){"
    "gl_Position=vec4(position.x,position.y,position.z,1.0);"
    "}"
};
const GLchar* const FragmentShaderSource = {//片段着色器
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main(){\n"
    "color=vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}"
};
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "fail to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
   // std::cout << glGetString(GL_VERSION) << std::endl;
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "fail to initialize glew" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    std::cout << "width:\t" << width << "\theight:\t" << height << std::endl;
    glViewport(width/2, height/2, width/2, height/2);
    /*****************************shader***************************/
    GLint vertexshader;
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &VertexShaderSource, nullptr);
    glCompileShader(vertexshader);
    GLint success;
    GLchar log[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cout << "vertex shader compile fail" << std::endl;
        glGetShaderInfoLog(vertexshader, 512, NULL, log);
        std::cout << log << std::endl;
    }
    else {
        std::cout << "vertex shader compile sucessfully" << std::endl;
    }

    GLint fragmentshader;
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &FragmentShaderSource, NULL);
    glCompileShader(fragmentshader);
    GLint frag_success;
    GLchar f_log[512];
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &frag_success);
    if (!frag_success) {
        std::cout << "fragment shader conpile fail" << std::endl;
        glGetShaderInfoLog(fragmentshader, 512, NULL, f_log);
        std::cout << f_log << std:: endl;

    }
    else {
        std::cout << "fragment shader conpile successfully" << std::endl;
    }
    //着色器程序
    GLuint shaderProg;
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexshader);
    glAttachShader(shaderProg, fragmentshader);
    glLinkProgram(shaderProg);
    GLint link_success;
    GLchar linkLog[512];
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        std::cout << "Link Error:";
        glGetProgramInfoLog(shaderProg, 512, NULL, linkLog);
        std::cout << linkLog << std::endl;
    }
    else {
        std::cout << "link shader sucessfully" << std::endl;
        glUseProgram(shaderProg);
        //删除着色器对象，因为我们不再需要它们
        glDeleteShader(vertexshader);
        glDeleteShader(fragmentshader);
    }
    /*********************************创建VBO——buffer********************************/
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    /***********************************************创建VAO顶点数组对象************************/
        //初始化代码，只运行一次，除非物体频繁改变
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);//绑定VAO
    //先前代码已经绑定VBO并完成设置顶点属性，故不再设置
 
    /********************************************链接顶点属性***********************************/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    /*
    0:顶点属性位置值
    3：顶点属性大小
    GL_FLOAT:数据类型
    GL_FALSE:不进行归一化
    3*sizeof(GLfloat):步长
    0(GLvoid*)：偏移量
    */
    glEnableVertexAttribArray(0);
    /************************************************callback*********************************/
    glfwSetKeyCallback(window, &key_callback);
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
