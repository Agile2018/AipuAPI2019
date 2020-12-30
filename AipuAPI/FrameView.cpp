#include "FrameView.h"


int widthWindow = 400;
int heightWindow = 300;
int indexCurrentFrame = 0;
GLint factorStipple = 10;
GLfloat lineWidth = 3.0f;
std::string labels[NUM_FRAMES] = { "Video 1", "Video 2", "Video 3", "Video 4" };

GLFrame* glFrame;
GLFrame* glFrameTwo = new GLFrame();
GLFrame* glFrameThree = new GLFrame();
GLFrame* glFrameFour = new GLFrame();
GLFWwindow* window;

FrameData* frames = new FrameData[NUM_FRAMES];

FrameView::FrameView()
{
    
}

FrameView::~FrameView()
{
    delete[] frames;
   
}

void FrameView::SetIndexCurrent(int value) {
    indexCurrentFrame = value;
}

void FrameView::SetWidthImage(int index, int value) {
    int idx = index - 1;
    frames[idx].SetWidthImage(value);
        
}

void FrameView::SetHeightImage(int index, int value) {
    int idx = index - 1;
    frames[idx].SetHeightImage(value);
            
}

void FrameView::SetElapse(int index, int value) {
    int idx = index - 1;
    frames[idx].SetElapse(value);

    //switch (index)
    //{
    //case 1:
    //    std::cout << "ELAPSE ONE: " << value << std::endl;
    //    frameOne->SetElapse(value);
    //    break;
    //case 2:
    //    std::cout << "ELAPSE TWO: " << value << std::endl;
    //    frameTwo->SetElapse(value);
    //    break;
    //case 3:
    //    std::cout << "ELAPSE THREE: " << value << std::endl;
    //    frameThree->SetElapse(value);
    //    break;
    //case 4:
    //    std::cout << "ELAPSE FOUR: " << value << std::endl;
    //    frameFour->SetElapse(value);
    //    break;
    //default:
    //    break;
    //}
       
}

void FrameView::SetFactorStipple(GLint value) {
    factorStipple = value;
}

void FrameView::SetLineWidth(GLfloat value) {
    lineWidth = value;
}

void FrameView::SetColourR(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetColourR(indexFigure, value);        
    
}

void FrameView::SetColourG(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetColourG(indexFigure, value);
    
}

void FrameView::SetColourB(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetColourB(indexFigure, value);    
    
}

void FrameView::SetRatioWidth(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetRatioWidth(indexFigure, value);

    //switch (indexFrame)
    //{
    //case 1:
    //    frameOne->SetRatioWidth(indexFigure, value);
    //    break;
    //case 2:
    //    frameTwo->SetRatioWidth(indexFigure, value);
    //    break;
    //case 3:
    //    frameThree->SetRatioWidth(indexFigure, value);
    //    break;
    //case 4:
    //    frameFour->SetRatioWidth(indexFigure, value);
    //    break;
    //default:
    //    break;
    //}
    
    
}

void FrameView::SetRatioHeight(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetRatioHeight(indexFigure, value);

    //switch (indexFrame)
    //{
    //case 1:
    //    frameOne->SetRatioHeight(indexFigure, value);
    //    break;
    //case 2:
    //    frameTwo->SetRatioHeight(indexFigure, value);
    //    break;
    //case 3:
    //    frameThree->SetRatioHeight(indexFigure, value);
    //    break;
    //case 4:
    //    frameFour->SetRatioHeight(indexFigure, value);
    //    break;
    //default:
    //    break;
    //}
    
}

void FrameView::SetPositionX(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetPositionX(indexFigure, value);

    //switch (indexFrame)
    //{
    //case 1:
    //    frameOne->SetPositionX(indexFigure, value);
    //    break;
    //case 2:
    //    frameTwo->SetPositionX(indexFigure, value);
    //    break;
    //case 3:
    //    frameThree->SetPositionX(indexFigure, value);
    //    break;
    //case 4:
    //    frameFour->SetPositionX(indexFigure, value);
    //    break;
    //default:
    //    break;
    //}
        
}
void FrameView::SetPositionY(int indexFrame, int indexFigure, float value) {
    int idx = indexFrame - 1;
    frames[idx].SetPositionY(indexFigure, value);

    //switch (indexFrame)
    //{
    //case 1:
    //    frameOne->SetPositionY(indexFigure, value);
    //    break;
    //case 2:
    //    frameTwo->SetPositionY(indexFigure, value);
    //    break;
    //case 3:
    //    frameThree->SetPositionY(indexFigure, value);
    //    break;
    //case 4:
    //    frameFour->SetPositionY(indexFigure, value);
    //    break;
    //default:
    //    break;
    //}
    
}

int FrameView::GetWidthImage(int index) {
    int idx = index - 1;
    return frames[idx].GetWidthImage();
    //switch (index)
    //{
    //case 1:
    //    return frameOne->GetWidthImage();
    //    break;
    //case 2:
    //    return frameTwo->GetWidthImage();
    //    break;
    //case 3:
    //    return frameThree->GetWidthImage();
    //    break;
    //case 4:
    //    return frameFour->GetWidthImage();
    //    break;
    //default:
    //    return 0;
    //    break;
    //}
    
}

int FrameView::GetHeightImage(int index) {
    int idx = index - 1;
    return frames[idx].GetHeightImage();

    //switch (index)
    //{
    //case 1:
    //    return frameOne->GetHeightImage();
    //    break;
    //case 2:
    //    return frameTwo->GetHeightImage();
    //    break;
    //case 3:
    //    return frameThree->GetHeightImage();
    //    break;
    //case 4:
    //    return frameFour->GetHeightImage();
    //    break;
    //default:
    //    return 0;
    //    break;
    //}
    
}

void FrameView::SetImageData(int indexFrame, unsigned char* data, int size) {
    int idx = indexFrame - 1;
    frames[idx].SetImageData(data, size);

    //switch (indexFrame)
    //{
    //case 1:
    //    frameOne->SetImageData(data, size);
    //    break;
    //case 2:
    //    frameTwo->SetImageData(data, size);
    //    break;
    //case 3:
    //    frameThree->SetImageData(data, size);
    //    break;
    //case 4:
    //    frameFour->SetImageData(data, size);
    //    break;
    //default:
    //    break;
    //}
    
}

void LoadImageOnTextureOne(int indexFrame) {
    if (frames[indexFrame].mtx.try_lock())
    {
        unsigned char* image = frames[indexFrame].LoadImageForTexture();
        if (image != nullptr)
        {
            glFrame->SetFlowImage(image, frames[indexFrame].GetWidthImage(), frames[indexFrame].GetHeightImage());
        }
        image = nullptr;
        free(image);
        frames[indexFrame].mtx.unlock();
    }   

}

void LoadImageOnTextureTwo(int indexFrame) {
    if (frames[indexFrame].mtx.try_lock())
    {
        unsigned char* image = frames[indexFrame].LoadImageForTexture();
        if (image != nullptr)
        {
            glFrameTwo->SetFlowImage(image, frames[indexFrame].GetWidthImage(), frames[indexFrame].GetHeightImage());
        }
        image = nullptr;
        free(image);
        frames[indexFrame].mtx.unlock();
    }

}

void LoadImageOnTextureThree(int indexFrame) {
    if (frames[indexFrame].mtx.try_lock())
    {
        unsigned char* image = frames[indexFrame].LoadImageForTexture();
        if (image != nullptr)
        {
            glFrameThree->SetFlowImage(image, frames[indexFrame].GetWidthImage(), frames[indexFrame].GetHeightImage());
        }
        image = nullptr;
        free(image);
        frames[indexFrame].mtx.unlock();
    }

}

void LoadImageOnTextureFour(int indexFrame) {
    if (frames[indexFrame].mtx.try_lock())
    {
        unsigned char* image = frames[indexFrame].LoadImageForTexture();
        if (image != nullptr)
        {
            glFrameFour->SetFlowImage(image, frames[indexFrame].GetWidthImage(), frames[indexFrame].GetHeightImage());
        }
        image = nullptr;
        free(image);
        frames[indexFrame].mtx.unlock();
    }

}

void FrameView::SetColourLabelFrame(int indexFrame, float red, float green, float blue) {
    frames[indexFrame].SetColourLabelR(red);
    frames[indexFrame].SetColourLabelG(green);
    frames[indexFrame].SetColourLabelB(blue);
}

void DrawOnewindow(int indexFrame) {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    double current_time = glfwGetTime();
    if ((current_time - glFrame->GetLastTime()) * 1000 > frames[indexFrame].GetElapse()) {                
        LoadImageOnTextureOne(indexFrame);        
        glFrame->SetLastTime(glfwGetTime());       
    }
    //clock_t timeStart1 = clock();
    //glFrame->DrawFrame(); // 3     3
    //clock_t duration1 = clock() - timeStart1;
    //int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);
    //printf("   LOAD FRAME time: %d\n", durationMs1);
    //glFrameTwo->DrawFrame();

    glFrame->DrawFrame();
    glfwSwapBuffers(window);
}

void DrawTwoWindow() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    double current_time = glfwGetTime();
    if ((current_time - glFrame->GetLastTime()) * 1000 > frames[0].GetElapse()) {        
        LoadImageOnTextureOne(0);
        glFrame->SetLastTime(glfwGetTime());
    }

    if ((current_time - glFrameTwo->GetLastTime()) * 1000 > frames[1].GetElapse()) {        
        LoadImageOnTextureTwo(1);
        glFrameTwo->SetLastTime(glfwGetTime());
    }
    glFrame->DrawFrame();
    glFrameTwo->DrawFrame();
    glfwSwapBuffers(window);

}

void DrawThreeWindow() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    double current_time = glfwGetTime();
    if ((current_time - glFrame->GetLastTime()) * 1000 > frames[0].GetElapse()) {        
        LoadImageOnTextureOne(0);
        glFrame->SetLastTime(glfwGetTime());
    }

    if ((current_time - glFrameTwo->GetLastTime()) * 1000 > frames[1].GetElapse()) {        
        LoadImageOnTextureTwo(1);
        glFrameTwo->SetLastTime(glfwGetTime());
    }

    if ((current_time - glFrameThree->GetLastTime()) * 1000 > frames[2].GetElapse()) {        
        LoadImageOnTextureThree(2);
        glFrameThree->SetLastTime(glfwGetTime());
    }

    glFrame->DrawFrame();
    glFrameTwo->DrawFrame();
    glFrameThree->DrawFrame();
    glfwSwapBuffers(window);

}

void DrawFourWindow() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    double current_time = glfwGetTime();
    if ((current_time - glFrame->GetLastTime()) * 1000 > frames[0].GetElapse()) {        
        LoadImageOnTextureOne(0);
        glFrame->SetLastTime(glfwGetTime());
    }

    if ((current_time - glFrameTwo->GetLastTime()) * 1000 > frames[1].GetElapse()) {        
        LoadImageOnTextureTwo(1);
        glFrameTwo->SetLastTime(glfwGetTime());
    }

    if ((current_time - glFrameThree->GetLastTime()) * 1000 > frames[2].GetElapse()) {        
        LoadImageOnTextureThree(2);
        glFrameThree->SetLastTime(glfwGetTime());
    }

    if ((current_time - glFrameFour->GetLastTime()) * 1000 > frames[3].GetElapse()) {
        
        LoadImageOnTextureFour(3);
        glFrameFour->SetLastTime(glfwGetTime());
    }

    glFrame->DrawFrame();
    glFrameTwo->DrawFrame();
    glFrameThree->DrawFrame();
    glFrameFour->DrawFrame();
    glfwSwapBuffers(window);

}
void framebuffer_size_callback_one_window(GLFWwindow* window, int width, int height)
{    
    glFrame->Viewport(0, 0, width, height);    
    DrawOnewindow(indexCurrentFrame);
}

void framebuffer_size_callback_two_window(GLFWwindow* window, int width, int height)
{
    glFrame->Viewport(0, 0, width / 2, height);
    glFrameTwo->Viewport(width / 2, 0, width / 2, height);
    DrawTwoWindow();
}

void framebuffer_size_callback_three_window(GLFWwindow* window, int width, int height)
{
    glFrame->Viewport(0, height / 2, width / 2, height / 2);
    glFrameTwo->Viewport(width / 2, height / 2, width / 2, height / 2);
    glFrameThree->Viewport(0, 0, width / 2, height / 2);
    DrawThreeWindow();
}

void framebuffer_size_callback_four_window(GLFWwindow* window, int width, int height)
{
    glFrame->Viewport(0, height / 2, width / 2, height / 2);
    glFrameTwo->Viewport(width / 2, height / 2, width / 2, height / 2);
    glFrameThree->Viewport(0, 0, width / 2, height / 2);
    glFrameFour->Viewport(width / 2, 0, width / 2, height / 2);
    DrawThreeWindow();
}

int FrameView::RunOne() {
    glFrame = new GLFrame();
    frames[indexCurrentFrame].ClearImageData();    
    frames[indexCurrentFrame].SetPositionX(0, 0.0f);
    frames[indexCurrentFrame].SetPositionY(0, 0.0f);
    frames[indexCurrentFrame].SetRatioWidth(0, 0.0f);
    frames[indexCurrentFrame].SetRatioHeight(0, 0.0f);
    widthWindow = 400;
    heightWindow = 300;
    if (!glfwInit())
    {
        return -1;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    window = glfwCreateWindow(widthWindow, heightWindow, nameWindow.c_str(), nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << glGetString(GL_VERSION) << std::endl;
    GLCall(glViewport(0, 0, screenWidth, screenHeight));

    glFrame->Viewport(0, 0, screenWidth, screenHeight);
    glFrame->SetLabel(labels[indexCurrentFrame]);
    glFrame->SetOrthographicProjection((float)frames[indexCurrentFrame].GetWidthImage(),
        (float)frames[indexCurrentFrame].GetHeightImage());
    glFrame->SetFrameData(&frames[indexCurrentFrame]);
    glFrame->InitFrame();  
    glFrame->SetLastTime(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_one_window);
        DrawOnewindow(indexCurrentFrame);

    }
    
    glfwTerminate();
    delete glFrame;
    return EXIT_SUCCESS;
}

int FrameView::RunTwo() {
    glFrame = new GLFrame();
    glFrameTwo = new GLFrame();
    frames[0].ClearImageData();
    frames[0].SetPositionX(0, 0.0f);
    frames[0].SetPositionY(0, 0.0f);
    frames[0].SetRatioWidth(0, 0.0f);
    frames[0].SetRatioHeight(0, 0.0f);

    frames[1].ClearImageData();
    frames[1].SetPositionX(0, 0.0f);
    frames[1].SetPositionY(0, 0.0f);
    frames[1].SetRatioWidth(0, 0.0f);
    frames[1].SetRatioHeight(0, 0.0f);

    widthWindow = 800;
    heightWindow = 300;
    if (!glfwInit())
    {
        return -1;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(widthWindow, heightWindow, nameWindow.c_str(), nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << glGetString(GL_VERSION) << std::endl;

    glFrame->Viewport(0, 0, screenWidth / 2, screenHeight);    
    glFrame->SetLabel(labels[0]);
    glFrame->SetOrthographicProjection((float)frames[0].GetWidthImage(),
        (float)frames[0].GetHeightImage());
    glFrame->SetFrameData(&frames[0]);
    glFrame->InitFrame();
    glFrame->SetLastTime(glfwGetTime());

    glFrameTwo->Viewport(screenWidth / 2, 0, screenWidth / 2, screenHeight);
    glFrameTwo->SetLabel(labels[1]);
    glFrameTwo->SetOrthographicProjection((float)frames[1].GetWidthImage(),
        (float)frames[1].GetHeightImage());
    glFrameTwo->SetFrameData(&frames[1]);
    glFrameTwo->InitFrame();
    glFrameTwo->SetLastTime(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_two_window);
        DrawTwoWindow();

    }

    glfwTerminate();
    delete glFrame;
    delete glFrameTwo;
    return EXIT_SUCCESS;
}

int FrameView::RunThree() {
    glFrame = new GLFrame();
    glFrameTwo = new GLFrame();
    glFrameThree = new GLFrame();
    frames[0].ClearImageData();
    frames[0].SetPositionX(0, 0.0f);
    frames[0].SetPositionY(0, 0.0f);
    frames[0].SetRatioWidth(0, 0.0f);
    frames[0].SetRatioHeight(0, 0.0f);

    frames[1].ClearImageData();
    frames[1].SetPositionX(0, 0.0f);
    frames[1].SetPositionY(0, 0.0f);
    frames[1].SetRatioWidth(0, 0.0f);
    frames[1].SetRatioHeight(0, 0.0f);

    frames[2].ClearImageData();
    frames[2].SetPositionX(0, 0.0f);
    frames[2].SetPositionY(0, 0.0f);
    frames[2].SetRatioWidth(0, 0.0f);
    frames[2].SetRatioHeight(0, 0.0f);

    widthWindow = 800;
    heightWindow = 600;
    if (!glfwInit())
    {
        return -1;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(widthWindow, heightWindow, nameWindow.c_str(), nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << glGetString(GL_VERSION) << std::endl;

    glFrame->Viewport(0, screenHeight / 2, screenWidth / 2, screenHeight / 2);
    glFrame->SetLabel(labels[0]);
    glFrame->SetOrthographicProjection((float)frames[0].GetWidthImage(),
        (float)frames[0].GetHeightImage());
    glFrame->SetFrameData(&frames[0]);
    glFrame->InitFrame();
    glFrame->SetLastTime(glfwGetTime());

    glFrameTwo->Viewport(screenWidth / 2, screenHeight / 2, screenWidth / 2, screenHeight / 2);
    glFrameTwo->SetLabel(labels[1]);
    glFrameTwo->SetOrthographicProjection((float)frames[1].GetWidthImage(),
        (float)frames[1].GetHeightImage());
    glFrameTwo->SetFrameData(&frames[1]);
    glFrameTwo->InitFrame();
    glFrameTwo->SetLastTime(glfwGetTime());

    glFrameThree->Viewport(0, 0, screenWidth / 2, screenHeight / 2);
    glFrameThree->SetLabel(labels[2]);
    glFrameThree->SetOrthographicProjection((float)frames[2].GetWidthImage(),
        (float)frames[2].GetHeightImage());
    glFrameThree->SetFrameData(&frames[2]);
    glFrameThree->InitFrame();
    glFrameThree->SetLastTime(glfwGetTime());


    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_three_window);
        DrawThreeWindow();

    }

    glfwTerminate();
    delete glFrame;
    delete glFrameTwo;
    delete glFrameThree;
    return EXIT_SUCCESS;
}

int FrameView::RunFour() {
    glFrame = new GLFrame();
    glFrameTwo = new GLFrame();
    glFrameThree = new GLFrame();
    glFrameFour = new GLFrame();

    frames[0].ClearImageData();
    frames[0].SetPositionX(0, 0.0f);
    frames[0].SetPositionY(0, 0.0f);
    frames[0].SetRatioWidth(0, 0.0f);
    frames[0].SetRatioHeight(0, 0.0f);

    frames[1].ClearImageData();
    frames[1].SetPositionX(0, 0.0f);
    frames[1].SetPositionY(0, 0.0f);
    frames[1].SetRatioWidth(0, 0.0f);
    frames[1].SetRatioHeight(0, 0.0f);

    frames[2].ClearImageData();
    frames[2].SetPositionX(0, 0.0f);
    frames[2].SetPositionY(0, 0.0f);
    frames[2].SetRatioWidth(0, 0.0f);
    frames[2].SetRatioHeight(0, 0.0f);

    frames[3].ClearImageData();
    frames[3].SetPositionX(0, 0.0f);
    frames[3].SetPositionY(0, 0.0f);
    frames[3].SetRatioWidth(0, 0.0f);
    frames[3].SetRatioHeight(0, 0.0f);


    widthWindow = 800;
    heightWindow = 600;
    if (!glfwInit())
    {
        return -1;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(widthWindow, heightWindow, nameWindow.c_str(), nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << glGetString(GL_VERSION) << std::endl;

    glFrame->Viewport(0, screenHeight / 2, screenWidth / 2, screenHeight / 2);
    glFrame->SetLabel(labels[0]);
    glFrame->SetOrthographicProjection((float)frames[0].GetWidthImage(),
        (float)frames[0].GetHeightImage());
    glFrame->SetFrameData(&frames[0]);
    glFrame->InitFrame();
    glFrame->SetLastTime(glfwGetTime());


    glFrameTwo->Viewport(screenWidth / 2, screenHeight / 2, screenWidth / 2, screenHeight / 2);
    glFrameTwo->SetLabel(labels[1]);
    glFrameTwo->SetOrthographicProjection((float)frames[1].GetWidthImage(),
        (float)frames[1].GetHeightImage());
    glFrameTwo->SetFrameData(&frames[1]);
    glFrameTwo->InitFrame();
    glFrameTwo->SetLastTime(glfwGetTime());

    glFrameThree->Viewport(0, 0, screenWidth / 2, screenHeight / 2);
    glFrameThree->SetLabel(labels[2]);
    glFrameThree->SetOrthographicProjection((float)frames[2].GetWidthImage(),
        (float)frames[2].GetHeightImage());
    glFrameThree->SetFrameData(&frames[2]);
    glFrameThree->InitFrame();
    glFrameThree->SetLastTime(glfwGetTime());

    glFrameFour->Viewport(screenWidth / 2, 0, screenWidth / 2, screenHeight / 2);
    glFrameFour->SetLabel(labels[3]);
    glFrameFour->SetOrthographicProjection((float)frames[3].GetWidthImage(),
        (float)frames[3].GetHeightImage());
    glFrameFour->SetFrameData(&frames[3]);
    glFrameFour->InitFrame();
    glFrameFour->SetLastTime(glfwGetTime());


    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_four_window);
        DrawFourWindow();

    }

    glfwTerminate();
    delete glFrame;
    delete glFrameTwo;
    delete glFrameThree;
    delete glFrameFour;
    return EXIT_SUCCESS;
}
void FrameView::DestroyWindow() {    
    glfwDestroyWindow(window);
}

