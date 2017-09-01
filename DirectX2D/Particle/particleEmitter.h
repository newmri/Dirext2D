// Programming 2D Games
// Copyright (c) 2014 by: 
// Charles Kelly
// particleEmitter.h v1.1
// Last modification: December-25-2014  Merry Christmas

#ifndef _PARTICLE_EMITTER_H     // Prevent multiple definitions if this 
#define _PARTICLE_EMITTER_H     // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class ParticleEmitter;

#include <vector>
#include <ctime>
using namespace std;
#include "particle.h"
#include "game.h"
#include "textureManager.h"

namespace particleEmitterNS
{
    const int MAX_PARTICLES = 1000;
    const int MIN_SPEED = 100;
    const int MAX_SPEED = 200;
    const int PARTICLE_SIZE = 4;    // size in pixels
    const int PARTICLE_COLS = 16;   // columns in texture image
    const int PARTICLE_START_FRAME = 0;
    const int PARTICLE_END_FRAME = 255;
    const float PARTICLE_FRAME_DELAY = 0.005f;  // delay between frames of animation
    const float MIN_LIFE = 1.0f;    // minimum life time of a particle (seconds)
    const float MAX_LIFE = 4.0f;    // maximum life time of a particle (sedonds)
    const float CREATION_RATE = 100;    // particles per second
}

class ParticleEmitter
{
  private:
    // properties
    bool initialized;
    float gravity;
    float minLife, maxLife;     // time each particle lives (seconds)
    float creationDelay;        // seconds per particle
    float creationTimer;
    float x,y;                  // emitter location
    vector<Particle*> particles;
    VECTOR2 cv;                 // collision vector

  public:
    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////
    float getGravity() {return gravity;}
    float getMinLife() {return minLife;}
    float getMaxLife() {return maxLife;}
    float getCreationDelay() {return creationDelay;}
    float getX() {return x;}
    float getY() {return y;}

    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////
    void setGravity(float g);
    void setMinLife(float m) {minLife = m;}
    void setMaxLife(float m) {maxLife = m;}
    void setCreationDelay(float d) {creationDelay = d;}
    void setX(float x1) {x = x1;}
    void setY(float y1) {y = y1;}

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // ParticleEmitter Constructor
    // screenX, screenY of emitter
    // grav is Y axis gravity, + pulls particles down
    // minLife, maxLife is time each particle lives (seconds)
    // creationRate is number of particles to create per second
	ParticleEmitter(float screenX=100, float screenY=100, float grav = 0, 
        float minLife = particleEmitterNS::MIN_LIFE, 
        float maxLife = particleEmitterNS::MAX_LIFE, 
        float creationRate = particleEmitterNS::CREATION_RATE);

    // Destructor
	virtual ~ParticleEmitter();

    // Update
    virtual void update(float frameTime);

    // Initialize
    // Pre: *gamePtr = pointer to Game object
    virtual bool initialize(Game *gamePtr, TextureManager *textureM);

    // Draw the particles
    virtual void draw();

    // Do the particles collide with ent?
    virtual bool collidesWith(Entity &ent);

};

#endif
