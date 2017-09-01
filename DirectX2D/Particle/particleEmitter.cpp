// Programming 2D Games
// Copyright (c) 2014 by: 
// Charles Kelly
// particleEmitter.cpp v1.1
// Last modification: December-25-2014  Merry Christmas

#include "particleEmitter.h"
using namespace particleEmitterNS;

//=============================================================================
// constructor, default arguments in prototype
//=============================================================================
ParticleEmitter::ParticleEmitter(float screenX, float screenY, float grav, float minL, float maxL, float creationR) :
                                x(screenX), y(screenY),             // screen location
                                gravity(grav), 
                                minLife(minL), maxLife(maxL)        // time each particle lives (seconds)
{
    if (creationR > 0)
        creationDelay = 1.0f / creationR;                           // seconds per particle
    creationTimer = 0;
    initialized = false;
}

//=============================================================================
// Destructor
//=============================================================================
ParticleEmitter::~ParticleEmitter()
{
    for(int i=0; i<MAX_PARTICLES; i++)
    {
        if(particles[i])
            delete particles[i];
    }
}

//=============================================================================
// Initialize
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//      *textureM = pointer to TextureManager object
// Post: returns true if successful, false if failed
//=============================================================================
bool ParticleEmitter::initialize(Game *gamePtr, TextureManager *textureM)
{
    srand(static_cast<UINT>(time(NULL)));                      // seed random numbers
    rand();                                 // discard 1st number for increased randomization
    particles.resize(MAX_PARTICLES);
    for(int i=0; i<MAX_PARTICLES; i++)
    {
        particles[i] = new Particle(gravity);
        //                      ( Game*,      width,        height,        columns,  TextureManager*)
        particles[i]->initialize(gamePtr, PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_COLS, textureM);
        particles[i]->setVisible(false);
        particles[i]->setFrames(PARTICLE_START_FRAME,PARTICLE_END_FRAME);
        particles[i]->setFrameDelay(PARTICLE_FRAME_DELAY);
        particles[i]->setCurrentFrame(PARTICLE_START_FRAME);
        particles[i]->setLoop(false);
    }
    initialized = true;
    return true;
}

//=============================================================================
// setGravity
//=============================================================================
void ParticleEmitter::setGravity(float g)
{
    for(UINT i=0; i<particles.size(); i++)
        particles[i]->setGravity(g);
}

//=============================================================================
// update
//=============================================================================
void ParticleEmitter::update(float frameTime)
{
    UINT i;
    float vX, vY;

    if(!initialized)
        return;

    creationTimer += frameTime;
    while (creationTimer >= creationDelay)      // if time to create particle(s)
    {
        // find the 1st unused(invisible) particle (if any)
        for(i=0; i<particles.size() && particles[i]->getVisible(); i++)
        {}

        if(i<particles.size())          // if we found an unused particle
        {
            // setup particle
            particles[i]->setX(x);
            particles[i]->setY(y);
            int speed = rand()%(MAX_SPEED-MIN_SPEED)+MIN_SPEED;   // random speed
            float radians = static_cast<float>((rand()%360) * PI/180.0);    // random angle in radians
            particles[i]->setRadians(radians);
            //particles[i]->setDegrees(90);
            vX = (float)cos(radians) * speed;               // random velocity X
            //vX = 0;               // random velocity X
            vY = (float)sin(radians) * speed;               // random velocity Y
            particles[i]->setVelocity(VECTOR2(vX,vY));
            particles[i]->setVisible(true);
            particles[i]->setCurrentFrame(PARTICLE_START_FRAME);
            int r = rand()%(static_cast<int>((maxLife-minLife)*10000)) + static_cast<int>(minLife*10000);
            particles[i]->setLife(r/10000.0f);
        }
        // when the particles list gets full uncreated particles are discarded
        creationTimer -= creationDelay;     // reduce creationTimer even if no particle was created
    }

    // Move particles
    for(i=0; i<particles.size(); i++)
    {
        if(particles[i]->getVisible())  // if visible particle
            particles[i]->update(frameTime);    // move particle
    }
}

//=============================================================================
// Do the particles collide with ent?
// Returns true if any particles collide
//=============================================================================
bool ParticleEmitter::collidesWith(Entity &ent)
{
    bool result = false;
    for(int i=0; i<MAX_PARTICLES; i++)
    {
        if(particles[i]->getVisible() && particles[i]->collidesWith(ent, cv))
        {
            particles[i]->bounce(cv, ent);
            result = true;
        }
    }
    return result;
}

//=============================================================================
// Draw the particles
//=============================================================================
void ParticleEmitter::draw()
{
    if(!initialized)
        return;

    for(int i=0; i<MAX_PARTICLES; i++)
        particles[i]->draw();
}
