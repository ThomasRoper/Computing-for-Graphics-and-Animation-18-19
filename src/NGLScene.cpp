#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>


NGLScene::NGLScene()
{
    setTitle("Flocking Demo");
}


NGLScene::~NGLScene()
{
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL( int _w, int _h )
{
    m_project=ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
    m_win.width  = static_cast<int>( _w * devicePixelRatio() );
    m_win.height = static_cast<int>( _h * devicePixelRatio() );
}



void NGLScene::initializeGL()
{
    // we must call this first before any other GL commands to load and link the
    // gl commands from the lib, if this is not done program will crash
    ngl::NGLInit::instance();

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);
    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from(0,5,18);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);
    m_view=ngl::lookAt(from,to,up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_project=ngl::perspective(40,720.0f/576.0f,0.5f,150.0f);
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    // we are creating a shader called Phong
    shader->createShaderProgram("Phong");
    // now we are going to create empty shaders for Frag and Vert
    shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
    // attach the source
    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
    // compile the shaders
    shader->compileShader("PhongVertex");
    shader->compileShader("PhongFragment");
    // add them to the program
    shader->attachShaderToProgram("Phong","PhongVertex");
    shader->attachShaderToProgram("Phong","PhongFragment");

    // now we have associated this data we can link the shader
    shader->linkProgramObject("Phong");
    // and make it active ready to load values
    (*shader)["Phong"]->use();
    shader->setUniform("Normalize",1);
    ngl::Vec4 lightPos(0.0f,12.0f,-18.0f,1.0f);
    shader->setUniform("light.position",lightPos);
    shader->setUniform("light.ambient",0.0f,0.0f,0.0f,1.0f);
    shader->setUniform("light.diffuse",1.0f,1.0f,1.0f,1.0f);
    shader->setUniform("light.specular",0.8f,0.8f,0.8f,1.0f);
    // gold like phong material
    shader->setUniform("material.ambient",0.274725f,0.1995f,0.0745f,0.0f);
    shader->setUniform("material.diffuse",0.75164f,0.60648f,0.22648f,0.0f);
    shader->setUniform("material.specular",0.628281f,0.555802f,0.3666065f,0.0f);
    shader->setUniform("material.shininess",51.2f);



    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    prim->createSphere("sphere",0.1f,10);

    // m_emitter.reset(new Emitter(ngl::Vec3(0,0,0),400,&m_wind));

    //flock stuff

    //m_flock->addNewBoids(10);
    m_test.addNewBoids(30);
    m_text.reset( new ngl::Text(QFont("Arial",14)));
    m_text->setScreenSize(width(),height());
    // as re-size is not explicitly called we need to do this.
    glViewport(0,0,width(),height());
    m_flockTimer=startTimer(20);

}


#include "Particle.h"

void NGLScene::paintGL()
{
    // grab an instance of the shader manager
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_win.width,m_win.height);
    //-------------edit stuff
    //m_emitter->draw(m_view,m_project);

    // m_flock->update();
    // m_flock->draw(m_view,m_project);
    m_test.update();
    m_test.draw(m_view,m_project);

    // Boid test(glm::vec3(0.0,0.0,0.0));
    //test.draw(m_view,m_project);

    //--------------------------
    m_text->setColour(1,1,1);
    QString text=QString("Flock Size  %1").arg(m_test.getSize());
    m_text->renderText(10,20,text);
    text=QString("Weight Amounts:      ");
    m_text->renderText(10,40,text);
    text=QString("Seperation      :%1").arg(m_test.getSepScale());
    m_text->renderText(10,60,text);
    text=QString("Alignment       :%1").arg(m_test.getAlignScale());
    m_text->renderText(10,80,text);
    text=QString("Cohesion        :%1").arg(m_test.getCohScale());
    m_text->renderText(10,100,text);
    text=QString("Speed           :%1").arg(m_test.getBoidSpeed());
    m_text->renderText(10,120,text);
    text=QString("Turning force   :%1").arg(m_test.getBoidForce());
    m_text->renderText(10,140,text);
    text=QString("Align/coh Sight :%1").arg(m_test.getBoidSight());
    m_text->renderText(10,160,text);
    text=QString("Seperation Sight:%1").arg(m_test.getBoidSepSight());
    m_text->renderText(10,180,text);
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
    // this method is called every time the main window recives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    switch (_event->key())
    {
    // escape key to quite
    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
    case Qt::Key_Plus : m_test.addNewBoids(1); break;
    case Qt::Key_Minus : m_test.deleteBoid(); break;
    case Qt::Key_Q : m_test.setSepScale(m_test.getSepScale()+0.05); break;
    case Qt::Key_A : m_test.setSepScale(m_test.getSepScale()-0.05); break;
    case Qt::Key_W : m_test.setAlignScale(m_test.getAlignScale()+0.05); break;
    case Qt::Key_S : m_test.setAlignScale(m_test.getAlignScale()-0.05); break;
    case Qt::Key_E : m_test.setCohScale(m_test.getCohScale()+0.05); break;
    case Qt::Key_D : m_test.setCohScale(m_test.getCohScale()-0.05); break;
    case Qt::Key_R : m_test.setBoidSpeed(m_test.getBoidSpeed()+0.01); break;
    case Qt::Key_F : m_test.setBoidSpeed(m_test.getBoidSpeed()-0.01); break;
    case Qt::Key_T : m_test.setBoidForce(m_test.getBoidForce()+0.00001); break;
    case Qt::Key_G : m_test.setBoidForce(m_test.getBoidForce()-0.00001); break;
    case Qt::Key_Y : m_test.setBoidSight(m_test.getBoidSight()+0.1); break;
    case Qt::Key_H : m_test.setBoidSight(m_test.getBoidSight()-0.1); break;
    case Qt::Key_U : m_test.setBoidSepSight(m_test.getBoidSepSight()+0.05); break;
    case Qt::Key_J : m_test.setBoidSepSight(m_test.getBoidSepSight()-0.05); break;

    }
    // finally update the GLWindow and re-draw
    update();
}

void NGLScene::timerEvent(QTimerEvent *_event )
{
    if(_event->timerId() ==   m_flockTimer)
    {
        //m_emitter->update();
        update();
    }

    // re-draw GL
}
