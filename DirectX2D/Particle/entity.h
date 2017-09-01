// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// entity.h v3.1
// Last modification: December-27-2014

#ifndef _ENTITY_H               // Prevent multiple definitions if this 
#define _ENTITY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Entity;

#include "image.h"
#include "input.h"
#include "game.h"

namespace entityNS
{
    enum COLLISION_TYPE {NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT};
    const float GRAVITY = 6.67428e-11f;         // gravitational constant
}

class Entity : public Image
{
    // Entity properties
  protected:
    entityNS::COLLISION_TYPE collisionType;
    VECTOR2 center;         // center of entity
    float   oldX, oldY;     // the location of the entity in the previous frame
    float   radius;         // radius of collision circle
    VECTOR2 distSquared;    // used for calculating circle collision
    float   sumRadiiSquared;
    // edge specifies the collision box relative to the center of the entity.
    // left and top are typically negative numbers
    RECT    edge;           // for BOX and ROTATED_BOX collision detection
    VECTOR2 corners[4];     // for ROTATED_BOX collision detection
    VECTOR2 edge01,edge03;  // edges used for projection
    // min and max projections for this entity
    float   entA01min, entA01max, entA03min, entA03max;
    // min and max projections for other entity
    float   entB01min, entB01max, entB03min, entB03max;
    VECTOR2 velocity;       // velocity
    VECTOR2 deltaV;         // added to velocity during next call to update()
    float   rotationRate;   // current rotation rate (radians/second)
    float   mass;           // Mass of entity
    float   health;         // health 0 to 100
    float   rr;             // Radius squared variable
    float   force;          // Force of gravity
    float   gravity;        // gravitational constant of the game universe
    Input   *input;         // pointer to the input system
    Audio   *audio;         // pointer to audio system
    HRESULT hr;             // standard return type
    bool    active;         // only active entities may collide
    bool    rotatedBoxReady;    // true when rotated collision box is ready
    bool    intersecting;   // true when this entity is intersecting another entity
    // The bounce function will perform an extra move of the entity when embedded is true.
    bool    embedded;       // true when this entity is completely contained within the collision area of another entity.
    float   embeddedDistance;   // how deeply this entity is embedded (pixels)
    DWORD   pixelsColliding;    // number of pixels colliding in pixel perfect collision

    // --- The following functions are protected because they are not intended to be
    // --- called from outside the class.
    // Circular collision detection 
    // Pre: &ent = Other entity
    // Post: &collisionVector is from center to center
    virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);
    // Axis aligned box collision detection
    // Pre: &ent = Other entity
    // Post: &collisionVector is from center to center
    virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between boxes
    // Pre: &ent = Other entity
    // Post: &collisionVector is perpendicular to collision edge
    virtual bool collideRotatedBox(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between box and circle
    // Pre: &ent = Other entity
    // Post: &collisionVector is perpendicular to collision edge or
    //       center to corner when collision is with box corner.
    virtual bool collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection helper functions
    void computeRotatedBox();
    bool projectionsOverlap(Entity &ent, VECTOR2 &collisionVector);
    bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);
    // Pixel Perfect collision detection. If the graphics card does not support
    // a stencil buffer then CIRCLE collision detection is used.
    // This function waits for the graphics card to render the last frame and return
    // the collision query pixel count. To avoid slowing down your game, use a
    // simple collison test first to eliminate entities that are not colliding.
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collidePixelPerfect(Entity &ent, VECTOR2 &collisionVector);

  public:
    // Constructor
    Entity();
    // Destructor
    virtual ~Entity() {}

    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////

    // Return center of scaled Entity as screen x,y.
    virtual const VECTOR2* getCenter()   
    {
        center = VECTOR2(getCenterX(),getCenterY());
        return &center;
    }

    // Return radius of collision circle.
    virtual float getRadius() const     {return radius;}

    // Return RECT structure used for BOX and ROTATED_BOX collision detection.
    virtual const RECT& getEdge() const {return edge;}

    // Return corner c of ROTATED_BOX
    virtual const VECTOR2* getCorner(UINT c) const
    {
        if(c>=4) 
            c=0;
        return &corners[c]; 
    }

    // Return velocity vector.
    virtual const VECTOR2 getVelocity() const {return velocity;}

    // Returns rotation rate
    float getRotationRate() { return rotationRate; }

    // Return active.
    // Only active entities may collide.
    virtual bool  getActive()   const {return active;}

    // Return intersecting. 
    // Intersecting is true if this entity is intersecting another entity.
    virtual bool getIntersecting() const {return intersecting;}

    // Return embedded
    // embedded is true if this entity is completely contained within the collision area of another entity.
    virtual bool getEmbedded() const {return embedded;}

    // Return mass.
    virtual float getMass()     const {return mass;}

    // Return gravitational constant.
    virtual float getGravity()  const {return gravity;}

    // Return health;
    virtual float getHealth()   const {return health;}

    // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT)
    virtual entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

    // Return number of pixels colliding in pixel perfect collision
    virtual DWORD getPixelsColliding() const {return pixelsColliding;}

    // Return oldX
    virtual float getOldX()     const {return oldX;}

    // Return oldY
    virtual float getOldY()     const {return oldY;}

    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////

    // Set velocity.
    virtual void  setVelocity(VECTOR2 v)    {velocity = v;}

    // Set delta velocity. Added to velocity in update().
    virtual void  setDeltaV(VECTOR2 dv)     {deltaV = dv;}

    // Set rotation rate
    void setRotationRate(float r)   { rotationRate = r; }

    // Set active.
    virtual void  setActive(bool a)         {active = a;}

    // Set health.
    virtual void setHealth(float h)         {health = h;}

    // Set mass.
    virtual void  setMass(float m)          {mass = m;}

    // Set gravitational constant. Default is 6.67428e-11
    virtual void  setGravity(float g)       {gravity = g;}

    // Set radius of collision circle.
    virtual void setCollisionRadius(float r)    {radius = r;}

    // Set collision type (NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT)
    virtual void setCollisionType(entityNS::COLLISION_TYPE ctype)
    {collisionType = ctype;}

    // Set embedded
    // embedded is true if this entity is completely contained within the collision area of another entity.
    virtual void setEmbedded(bool e) { embedded = e; }

    // Set RECT structure used for BOX and ROTATED_BOX collision detection.
    virtual void setEdge(RECT e)    {edge = e;}

    // Set rotatedBoxReady. Set to false to force recalculation.
    virtual void setRotatedBoxReady(bool r) {rotatedBoxReady = r;}

    ////////////////////////////////////////
    //          Move functions            //
    ////////////////////////////////////////

    // Rotate using specified rotation rate
    // Pre: frameTime = Elapsed time in seconds
    //      rotateRate = radians/second, + clockwise, - counterclockwise
    // Post: Current rotation rate is unchanged (no change in momentum)
    virtual void rotate(float frameTime, float rotateRate)
    {
        spriteData.angle += frameTime * rotateRate;   // apply rotation
    }

    // Rotate using current rotationRate
    // Pre: frameTime = Elapsed time in seconds
    // Post: Current rotation rate is unchanged (no change in momentum)
    // Call this from the game's update method to give the Entity momentum.
    virtual void rotate(float frameTime)
    {
        spriteData.angle += frameTime * rotationRate;   // apply rotation
    }

    // Change rotationRate (angular momentum)
    // Pre: frameTime = Elapsed time in seconds
    //      rotateRate = radians/second, + clockwise, - counterclockwise
    // Post: Current rotation rate is changed (change in momentum)
    // The Entity does not turn until the rotate method is called.
    virtual void turn(float frameTime, float rotateRate)
    {
        rotationRate += frameTime * rotateRate;
    }

    // Angle to target
    // Pre:  x,y = Target center
    // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
    virtual float angleToTarget(float x, float y);

    // Angle to target
    // Pre:  target = Target center
    // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
    virtual float angleToTarget(VECTOR2 target)
    {
        return angleToTarget(target.x, target.y);
    }

    // Aim at target
    // Pre: frameTime = Elapsed time in seconds
    //      rotationRate = radians/second
    //      x,y = Target center
    // Post: Current rotation rate is unchanged (no change in momentum)
    // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
    virtual float aimAtTarget(float frameTime, float rotationRate, float x, float y);

    // Aim at target
    // Pre: frameTime = Elapsed time in seconds
    //      rotationRate = radians/second
    //      target = Target center
    // Post: Current rotation rate is unchanged (no change in momentum)
    // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
    virtual float aimAtTarget(float frameTime, float rotationRate, VECTOR2 target)
    {
        return aimAtTarget(frameTime, rotationRate, target.x, target.y);
    }

    // Move in X direction using current velocity
    // Pre: frameTime = Elapsed time in seconds
    // Post: Current velocity vector is unchanged (no change in momentum)
    virtual void moveX(float frameTime)
    {
        spriteData.x += frameTime * velocity.x;     // move X 
    }

    // Move in X direction
    // Pre: frameTime = Elapsed time in seconds
    //      speed = Pixels/second (+ right, - left)
    // Post: Current velocity vector is unchanged (no change in momentum)
    virtual void moveX(float frameTime, float speed)
    {
        spriteData.x += frameTime * speed;          // move X 
    }

    // Move in Y direction using current velocity
    // Pre: frameTime = Elapsed time in seconds
    // Post: Current velocity vector is unchanged (no change in momentum)
    virtual void moveY(float frameTime)
    {
        spriteData.y += frameTime * velocity.y;     // move Y
    }

    // Move in Y direction
    // Pre: frameTime = Elapsed time in seconds
    //      speed = Pixels/second (+ down, - up)
    // Post: Current velocity vector is unchanged (no change in momentum)
    virtual void moveY(float frameTime, float speed)
    {
        spriteData.y += frameTime * speed;          // move Y
    }

    // Move in X,Y direction using current velocity
    // Pre: frameTime = Elapsed time in seconds
    // Post: Current velocity vector is unchanged (no change in momentum)
    // Call this from the game's update method to give the Entity momentum.
    virtual void move(float frameTime)
    {
        moveX(frameTime, velocity.x);
        moveY(frameTime, velocity.y);
    }

    // Move in X,Y direction using specified velocity
    // Pre: frameTime = Elapsed time in seconds
    //      vel = Velocity vector
    // Post: Current velocity vector is unchanged (no change in momentum)
    virtual void move(float frameTime, VECTOR2 vel)
    {
        moveX(frameTime, vel.x);
        moveY(frameTime, vel.y);
    }

    // MoveTo X,Y at speed. Ignores current velocity.
    // Pre: frameTime = Elapsed time in seconds
    //      speed = Movement speed in pixels/second
    //      x,y = Destination location (center)
    // Post: Current velocity vector is unchanged (no change in momentum)
    virtual void moveTo(float frameTime, float speed, float x, float y)
    {
        float distX = x - getCenterX();             // destination X - current X
        float distY = y - getCenterY();             // destination Y - current Y
        float distance = sqrt(distX*distX + distY*distY);
        float moveTime = distance / speed;          // time required to complete move
        moveX(frameTime, distX/moveTime);           // move in X direction
        moveY(frameTime, distY / moveTime);         // move in Y direction
    }

    // Change the velocity
    // Pre: frameTime = Elapsed time in seconds
    //      speed = Movement speed in pixels/second
    //      vel = Velocity vector
    // Post: Current velocity vector is changed (change in momentum)
    virtual void accelerate(float frameTime, float speed, VECTOR2 deltaV)
    {
        //velocity.x += deltaV.x * speed * frameTime;
        //velocity.y += deltaV.y * speed * frameTime;
        velocity += deltaV * speed * frameTime;
    }

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // Update Entity.
    // typically called once per frame
    // frameTime is used to regulate the speed of movement and animation
    virtual void update(float frameTime);

    // Initialize Entity
    // Pre: *gamePtr = pointer to Game object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
    //      *textureM = pointer to TextureManager object
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    // Activate Entity.
    virtual void activate();

    // Empty ai function to allow Entity objects to be instantiated.
    virtual void ai(float frameTime, Entity &ent);

    // Is this Entity outside the specified rectangle
    // Post: returns true if outside rect, false otherwise
    virtual bool outsideRect(RECT rect);

    // Does this entity intersect with ent?
    // Each entity must use a single collision type. Complex shapes that require
    // multiple collision types may be done by treating each part as a separate
    // entity or by using PIXEL_PERFECT collision.
    // Post: Returns true if the entities are intersecting. 
    //       Sets collisionVector to indicate the direction of intersect.
    //       Sets intersecting true if this entity is intersecting another entity
    virtual bool intersects(Entity &ent, VECTOR2 &collisionVector);

    // Perform collision detection between this entity and the other Entity.
    // Each entity must use a single collision type. Complex shapes that require
    // multiple collision types may be done by treating each part as a separate
    // entity or by using PIXEL_PERFECT collision. A collision is occuring if the
    // entities are intersecting and moving toward each other. If the entities
    // are intersecting but moving apart false is returned.
    // Typically called once per frame.
    // The collision types: CIRCLE, BOX, ROTATED_BOX or PIXEL_PERFECT.
    // Post: returns true if collision, false otherwise
    //       sets collisionVector if collision
    //       sets intersecting true if collision
    //   The collisionVector points in the direction of force that would be applied
    //   to this entity as a result of the collision. (e.g. If this entity is a ball
    //   that is dropped onto a box, the collision vector would point up (-Y).
    virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

    // damage
    // This entity has been damaged by a weapon.
    // Override this function in the inheriting class.
    virtual void damage(int weapon);

    // Entity bounces after collision with other Entity
    void bounce(VECTOR2 &collisionVector, Entity &ent);

    // Move this entity to its oldX, oldY
    void moveToOldXY()
    {
        setX(oldX);
        setY(oldY);
    }

    // Force of gravity on this entity from other entity
    // Adds the gravitational force to the velocity vector of this entity
    // force = GRAVITY * m1 * m2 / r*r
    //                    2              2
    //  r*r  =   (Ax - Bx)   +  (Ay - By)
    void gravityForce(Entity *other, float frameTime);
};

#endif
