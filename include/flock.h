#ifndef FLOCK_H
#define FLOCK_H
#include <vector>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include "Particle.h"
#include "boid.h"

class Flock
{
public :
    Flock();

    ~Flock();
    void addNewBoids(int amount);
    void deleteBoid();
    void setWeights();
    void draw(const ngl::Mat4 &_view, const ngl::Mat4 &_project);
    void update();
    void applyBehavior();
    void setBoidView(float theta, float r);
    float RandomFloat(float a, float b);
    unsigned int getSize(){return m_Boids.size();}

    float getSepScale(){return seperationScale;}
    void setSepScale(float newSepScale){seperationScale = newSepScale;}

    float getAlignScale(){return alignmentScale;}
    void setAlignScale(float newAlignScale){alignmentScale = newAlignScale;}

    float getCohScale(){return cohesionScale;}
    void setCohScale(float newcohScale){cohesionScale = newcohScale;}

    float getBoidSpeed(){return m_Boids[0].getSpeed();}
    void setBoidSpeed(float newSpeed){for(int i = 0; i < m_Boids.size(); i++){m_Boids[i].setSpeed(newSpeed);}}

    float getBoidForce(){return m_Boids[0].getForce();}
    void setBoidForce(float newForce){for(int i = 0; i < m_Boids.size(); i++){m_Boids[i].setForce(newForce);}}

    float getBoidSight(){return m_Boids[0].getSight();}
    void setBoidSight(float newSight){for(int i = 0; i < m_Boids.size(); i++){m_Boids[i].setSight(newSight);}}

    float getBoidSepSight(){return m_Boids[0].getSepSight();}
    void setBoidSepSight(float newSight){for(int i = 0; i < m_Boids.size(); i++){m_Boids[i].setSepSight(newSight);}}



private :
    glm::vec3 m_flockAveragePos;
    glm::vec3 m_leaderPos;
    glm::vec3 m_flockAverageVel;
    //arrays to store all the positions


    std::vector<Boid> m_Boids;
    float seperationScale = 8.0;
    float alignmentScale  = 0.5;
    float cohesionScale   = 3.5;
};

#endif // FLOCK_H
