#include "include/boid.h"

#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include "Emitter.h"

Boid::Boid(glm::vec3 pos)
{
    m_pos = pos;
    m_acceleration = glm::vec3(RandomFloat(-1.0,1.0),RandomFloat(-1.0,1.0),RandomFloat(-1.0,1.0));
}

Boid::~Boid()
{

}



glm::vec3 Boid::seek(glm::vec3 target)
{
    glm::vec3 desired = target - m_pos;
    glm::normalize(desired);
    //std::cout<<desired;
    desired *= maxspeed;
    glm::vec3 steer = desired - m_vel;
    //limit
    if (glm::length(steer) > maxforce)
    {
        steer =  glm::normalize(steer) * maxforce;
    }
    return steer;

}

void Boid::Update()
{
    std::cout<<" updating ... ";
    m_vel += m_acceleration;
    if (glm::length(m_vel) > maxspeed)
    {
        m_vel =  glm::normalize(m_vel) * maxspeed;
    }

    m_pos += m_vel;
    /*
    if (m_pos.x > 3.0){std::cout<<"Yikes x big";}
    if (m_pos.x < -3.0){std::cout<<"Yikes x small";}
    if (m_pos.y > 3.0){std::cout<<"Yikes y big";}
    if (m_pos.y < -3.0){std::cout<<"Yikes y small";}
    if (m_pos.z > 3.0){std::cout<<"Yikes z big";}
    if (m_pos.z < -3.0){std::cout<<"Yikes z small";}
    std::cout<<"Pos "<<m_pos.x<<" "<<m_pos.y<<" "<<m_pos.z<<"...    ";
    std::cout<<"Vel "<<m_vel.x<<" "<<m_vel.y<<" "<<m_vel.z<<"...    ";

    std::cout<<"Acceleration "<<m_acceleration.x<<" "<<m_acceleration.y<<" "<<m_acceleration.z<<"...    ";
*/
    m_acceleration = glm::vec3(0.0,0.0,0.0);
}

void Boid::applyForce(glm::vec3 force)
{
    m_acceleration += force;
   // std::cout<<"applying force = "<<force.x<<" "<<force.y<<" "<<force.z<<"\n";
}

void Boid::draw(const ngl::Mat4 &_view, const ngl::Mat4 &_project)
{


    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    ngl::Transformation transform;
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use("Phong");
    transform.setPosition(m_pos);
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=transform.getMatrix();
    MV=_view*transform.getMatrix();

    MVP=_project *MV;
    normalMatrix=MV;
    normalMatrix.inverse().transpose();
    shader->setUniform("MV",MV);
    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
    shader->setUniform("M",M);
    prim->draw("sphere");
    //glm::vec3 right = glm::cross(m_vel,glm::vec3(0.0,0.0,1.0));
    //right = glm::normalize(right);
    //glm::vec3 left = -right;



/*

    glBegin(GL_LINES);
    glVertex3f((0.0)+m_pos.x,  (0.0)+m_pos.y, (0.0)+m_pos.z);
    glVertex3f((foward.x * m_size)+m_pos.x,  (foward.y * m_size)+m_pos.y, (foward.z * m_size)+m_pos.z);
    /*
    glVertex3f((foward.x * m_size)+m_pos.x,  (foward.y * m_size)+m_pos.y, (0.0 * m_size)+m_pos.z);
    glVertex3f((((foward.x * 0.8) + (right.x * 0.2)) * m_size)+m_pos.x,  (((foward.y * 0.8) + (right.y * 0.2)) * m_size)+m_pos.y, (0.0 * m_size)+m_pos.z);

    glVertex3f((foward.x * m_size)+m_pos.x,  (foward.y * m_size)+m_pos.y, (0.0 * m_size)+m_pos.z);
    glVertex3f((((foward.x * 0.8) + (left.x * 0.2)) * m_size)+m_pos.x,  (((foward.y * 0.8) + (left.y * 0.2)) * m_size)+m_pos.y, (0.0 * m_size)+m_pos.z);


    glEnd();
*/

}

void Boid::stayInWalls()
{
     //std::cout<<"Stay in walls called\n";
    glm::vec3 desired;
    glm::vec3 steer;
    if (m_pos.x > m_boundingbox )
    {

/*
        //std::cout<<"i am over the line\n";
        desired = glm::vec3(-maxspeed,m_vel.y,m_vel.z);

        steer = desired - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
*/
        m_pos.x = -m_boundingbox;


    }else if(m_pos.x < -m_boundingbox)
    {
        std::cout<<"im fucked";
        /*
        //std::cout<<"i am over the line\n";
        desired = glm::vec3(maxspeed,m_vel.y,m_vel.z);

        steer = desired - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
        */
        m_pos.x = m_boundingbox;


    }
    if (m_pos.y > m_boundingbox )
    {
        /*
        //std::cout<<"i am over the line\n";
        desired = glm::vec3(m_vel.x,-maxspeed,m_vel.z);

        steer = desired - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
        */
        m_pos.y = -m_boundingbox;


    }else if(m_pos.y < -m_boundingbox)
    {
        /*
        //std::cout<<"i am over the line\n";
        desired = glm::vec3(m_vel.x,maxspeed,m_vel.z);

        steer = desired - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
        */
        m_pos.y = m_boundingbox;

    }

    if (m_pos.z > m_boundingbox )
    {
        /*
        //std::cout<<"i am over the line\n";
        desired = glm::vec3(m_vel.x,m_vel.y,-maxspeed);

        steer = desired - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
        */
        m_pos.z = -m_boundingbox;


    }else if(m_pos.z < -m_boundingbox)
    {
        /*
        //std::cout<<"i am over the line\n";
        desired = glm::vec3(m_vel.x,m_vel.y,maxspeed);

        steer = desired - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
        */
        m_pos.z = m_boundingbox;


    }

}
/*
void Boid::seekTest()
{

    if (pointx - 0.1 < m_pos.x && m_pos.x < pointx + 0.1)
    {
        if(pointy - 0.1 < m_pos.y && m_pos.y < pointy + 0.1)
        {
            pointx = RandomFloat(-m_boundingbox,m_boundingbox);
            pointy = RandomFloat(-m_boundingbox,m_boundingbox);
        }
    }

    glm::vec3 point = glm::vec3(pointx,pointy,0);
    //cross
    glBegin(GL_LINES);
    glVertex3f(point.x-0.1,point.y-0.1,0.0);
    glVertex3f(point.x+0.1,point.y+0.1,0.0);
    glVertex3f(point.x+0.1,point.y-0.1,0.0);
    glVertex3f(point.x-0.1,point.y+0.1,0.0);
    glEnd();


    applyForce(seek(point));



}
*/


float Boid::RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


glm::vec3 Boid::Seperation(std::vector<Boid>& Flock)
{
    unsigned int sizeOfFlock = Flock.size();
    glm::vec3 sum;
    int count = 0;
    float d;
    for(int i = 0; i < sizeOfFlock; i++)
    {
        if(m_pos == Flock[i].getPos())
        {
            //fail case incase it finds itself
           // std::cout<<"Yikes ";
        }else
        {
            d = glm::distance(m_pos,Flock[i].getPos());

            if (d > 0 && d < m_sepAmount)
            {
                glm::vec3 diff = m_pos - Flock[i].getPos();
                glm::normalize(diff);
                sum += diff;
                count++;
            }
        }
    }
    //std::cout<<"\n\n";
    //sum should now be the total of all the vectors
    glm::vec3 steer = glm::vec3(0.0,0.0,0.0);

    if (count)
    {
        sum /= count;
        glm::normalize(sum);
        sum *=maxspeed;
        steer = sum - m_vel;
        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }


    }
    return steer;
}

glm::vec3 Boid::Alignment(std::vector<Boid>& Flock)
{
    unsigned int sizeOfFlock = Flock.size();
    glm::vec3 sum = glm::vec3(0.0,0.0,0.0);
    int count = 0;
    float d = 0.0;
    for(int i = 0; i < sizeOfFlock; i++)
    {
        if(m_pos == Flock[i].getPos())
        {
            //fail case incase it finds itself
          //  std::cout<<"Aikes ";
        }else
        {
            d = glm::distance(m_pos,Flock[i].getPos());

            if (d > 0 && d < m_sight)
            {
                sum += Flock[i].getVel();
                count++;
            }
        }
    }
    if (count)
    {
        sum /= count;
        glm::normalize(sum);
        sum *= maxspeed;

        glm::vec3 steer = sum - m_vel;

        //limit
        if (glm::length(steer) > maxforce)
        {
            steer =  glm::normalize(steer) * maxforce;
        }
        return steer;
    }else{
        return glm::vec3(0.0,0.0,0.0);
    }

}

glm::vec3 Boid::Cohesion(std::vector<Boid>& Flock)
{
    unsigned int sizeOfFlock = Flock.size();
    glm::vec3 sum = glm::vec3(0.0,0.0,0.0);
    int count = 0;
    float d = 0.0;


    for(int i = 0; i < sizeOfFlock; i++)
    {
        if(m_pos == Flock[i].getPos())
        {
            //fail case incase it finds itself
           // std::cout<<"Cikes ";
        }else
        {
            d = glm::distance(m_pos,Flock[i].getPos());

            if (d > 0 && d < m_sight)
            {
                sum += Flock[i].getPos();
                count++;
            }
        }
    }


    if (count)
    {
        sum /= count;
        return seek(sum);
    }else{
        return glm::vec3(0.0,0.0,0.0);
    }

}

