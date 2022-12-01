#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <ngl/Random.h>
#include <iostream>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("ParticleNGL");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

const std::string_view ParticleShader="ParticleShader";



void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  m_emitter = std::make_unique<Emitter>(10'000);
  ngl::ShaderLib::use(ngl::nglColourShader);
  // MVP is the model view project uiform
  // Colour 4floats
  m_view = ngl::lookAt({10,10,10},{0,0,0},{0,1,0});
  m_project = ngl::perspective(45.0f, 1.0f,0.01f,50.0f);
  ngl::ShaderLib::setUniform("MVP", m_project * m_view);

  ngl::ShaderLib::loadShader(ParticleShader, "shaders/ParticleVertex.glsl","shaders/ParticleFragment.glsl");
  ngl::ShaderLib::use(ParticleShader);
  ngl::ShaderLib::setUniform("MVP", m_project * m_view);
  
  auto dist = std::uniform_int_distribution<int>(10,100);
  ngl::Random::addIntGenerator("particleLife", dist);
  startTimer(10);
}


void NGLScene::timerEvent(QTimerEvent *_event)
{
  m_emitter->update();
  update();
}


void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
  auto rotY = ngl::Mat4::rotateX(m_win.spinYFace);
  m_mouseGlobalTX = rotX * rotY;
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX);
  m_emitter->render();
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
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;

  case Qt::Key_1:
    m_emitter->addParticles(100);
  break;
  case Qt::Key_2:
    std::cout<<"plus 1000\n";
    m_emitter->addParticles(1000);
  break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
