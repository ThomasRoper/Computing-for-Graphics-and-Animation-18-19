#include "include/flock.h"
#include "iostream"
#include "stdlib.h"
Flock::Flock()
{

}



Flock::~Flock()
{

}

void Flock::addNewBoids(int amount)
{
    float spawnArea = 6.0;
    for(int i = 0; i < amount; i++)
    {
        Boid dummy(glm::vec3(RandomFloat(-spawnArea,spawnArea),RandomFloat(-spawnArea,spawnArea),RandomFloat(-spawnArea,spawnArea)));

        m_Boids.push_back(dummy);
        std::cout<<"just pushed back Boid: "<<i+1<<".\n";
    }
}
void Flock::deleteBoid()
{
if (m_Boids.size() > 1)
{
    m_Boids.pop_back();
}
}

void Flock::draw(const ngl::Mat4 &_view, const ngl::Mat4 &_project)
{
    unsigned int sizeOfFlock = m_Boids.size();

    for(int i = 0; i < sizeOfFlock; i++)
    {
        m_Boids[i].draw(_view,_project);
    }
}


void Flock::setWeights()
{

}
void Flock::update()
{

    applyBehavior();
    unsigned int sizeOfFlock = m_Boids.size();

    for(int i = 0; i < sizeOfFlock; i++)
    {
        std::cout<<"Boid "<<i+1<<" ";
        m_Boids[i].Update();
        m_Boids[i].stayInWalls();
        std::cout<<"updated\n";
    }
    std::cout<<"\n\n";



}
void Flock::setBoidView(float theta, float r)
{

}
float Flock::RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


void Flock::applyBehavior()
{
    unsigned int sizeOfFlock = m_Boids.size();

    for(int i = 0; i < sizeOfFlock; i++)
    {
        //123
        glm::vec3 seperation = m_Boids[i].Seperation(m_Boids);
        glm::vec3 alignment = m_Boids[i].Alignment(m_Boids);
        glm::vec3 cohesion = m_Boids[i].Cohesion(m_Boids);

        //scale as you like
        seperation *=  seperationScale;
        alignment *=   alignmentScale;
        cohesion *=    cohesionScale;
       // stayIn *= 0.1;

       m_Boids[i].applyForce(seperation);
       m_Boids[i].applyForce(alignment);
       m_Boids[i].applyForce(cohesion);
       //m_Boids[i].applyForce(stayIn);

    }

}


