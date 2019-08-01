#ifndef BOID_H
#define BOID_H


#include <ngl/Vec3.h>
#include <ngl/Mat4.h>


class Boid
{
public :
    Boid(glm::vec3 pos);
    ~Boid();
    void Update();
    glm::vec3 Seperation(std::vector<Boid>& Flock);
    glm::vec3 Cohesion(std::vector<Boid>& Flock);
    glm::vec3 Alignment(std::vector<Boid>& Flock);

    void applyForce(glm::vec3 force);
    void draw(const ngl::Mat4 &_view, const ngl::Mat4 &_project);
    void stayInWalls();

    float RandomFloat(float a, float b);
    glm::vec3 getPos(){return m_pos;}
    glm::vec3 getVel(){return m_vel;}

    float getSpeed(){return maxspeed;}
    void setSpeed(float newSpeed){if (maxspeed > 0.009){maxspeed = newSpeed;}}

    float getForce(){return maxforce;}
    void setForce(float newForce){if (maxforce > 0.000001){maxforce = newForce;}}

    float getSight(){return m_sight;}
    void setSight(float newSight){if (m_sight > 0.1){m_sight = newSight;}}

    float getSepSight(){return m_sepAmount;}
    void setSepSight(float newSight){if (m_sepAmount > 0.1){m_sepAmount = newSight;}}


private :
    glm::vec3 seek (glm::vec3 target);
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec3 m_rot;
    glm::vec3 m_acceleration;
    //maxspeed
    float maxspeed = 0.1;
    //max turning force
    float maxforce = 0.0001;
    float m_boundingbox = 6.0;
    float pointx = 1.25;
    float pointy = 1.25;

    float m_size = 0.05;
    float m_sepAmount = 1.5;
    float m_sight = 3.0;



};



#endif // BOID_H
