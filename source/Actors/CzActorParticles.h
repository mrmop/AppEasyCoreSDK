// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

#if !defined(_CZ_ACTOR_PARTICLES_H_)
#define _CZ_ACTOR_PARTICLES_H_

#include "CzUtil.h"
#include "CzActor.h"
#include "CzBitmapSprite.h"
#include "CzAnim.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzBox2d.h"
#include "CzShapes.h"
#include "CzActions.h"

class CzScene;
class CzActorParticles;

/**
 @addtogroup Actors
 @{
 */
 
/**
 @struct	CzActorParticle

 @brief	A particle.

 */

struct CzActorParticle
{
	CzActorParticles*		Parent;					///< Particles parent emitter
	CzBitmapSprite*			Visual;					///< Visual element that represents the particle
	CzVec2					Position;				///< Position relative to particle system actor
	CzVec2					Velocity;				///< Velocity of particle
	CzVec2					VelocityDamping;		///< Velocity damping for particle
	CzVec2					Gravity;				///< Gravitational acceleration
	CzVec2					Scale;					///< Scale of particle
	CzVec2					ScaleVelocity;			///< Scale velocity of particle
	CzVec2					ScaleVelocityDamping;	///< Scale velocity of particle
	float					Angle;					///< Rotation of particle
	float					AngularVelocity;		///< Rotation of particle
	float					AngularVelocityDamping;	///< Rotation of particle
	CzVec4					Colour;					///< Particle colour
	CzVec4					ColourVelocity;			///< Particle colour velocity
	CzVec4					ColourVelocityDamping;	///< Particle colour velocity damping
	float					Depth;					///< Depth of particle
	float					DepthVelocity;			///< Depth velocity of particle
	float					DepthVelocityDamping;	///< Depth velocity damping of particle
	bool					Active;					///< Active state
	bool					Visible;				///< Visible state
	float					LifeSpan;				///< Total duration of particle in seconds
	float					SpawnDelay;				///< Amount of time to wait before spawning in seconds
	float					LifeTime;				///< Current life time in seconds
	int						Lives;					///< Number of lives for particle (how time it will be respawned)
	CzVec2					OPosition;				///< Original Position relative to particle system actor
	CzVec2					OVelocity;				///< Original Velocity of particle
	CzVec2					OScale;					///< Original Scale of particle
	CzVec2					OScaleVelocity;			///< Original Scale velocity of particle
	float					OAngle;					///< Original Rotation of particle
	float					OAngularVelocity;		///< Original Rotation veloxity of particle
	CzVec4					OColour;				///< Original Particle colour
	CzVec4					OColourVelocity;		///< Original Particle colour velocity
	float					ODepth;					///< Original Depth of particle
	float					ODepthVelocity;			///< Original Depth velocity of particle
	bool					Attached;				///< Attached particles will follow the particle actor

	CzActorParticle() :	Parent(NULL), Visual(NULL), VelocityDamping(1.0f, 1.0f), Scale(1.0f, 1.0f), ScaleVelocityDamping(1.0f, 1.0f), 
								Angle(0), AngularVelocity(0), AngularVelocityDamping(1.0f), Colour(255, 255, 255, 255), ColourVelocityDamping(1, 1, 1, 1), 
								LifeSpan(2), SpawnDelay(0), LifeTime(0), Lives(-1), Visible(false), Active(true), Depth(0), DepthVelocity(0),DepthVelocityDamping(1), Attached(true)
	{
	}
	virtual ~CzActorParticle();

	void	Update(float dt);
	void	CopyToOld();
	void	CopyFromOld();

	CzActorParticle* Clone();

	bool			setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool			setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool			getProperty(unsigned int property_name, CzXomlProperty& prop);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system)
protected:
	static CzXomlClassDef*	ActorParticleClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPosition(IzXomlResource* target);
	static bool				_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSize(IzXomlResource* target);
	static bool				_setVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVelocity(IzXomlResource* target);
	static bool				_setVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVelocityDamping(IzXomlResource* target);
	static bool				_setGravity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getGravity(IzXomlResource* target);
	static bool				_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScale(IzXomlResource* target);
	static bool				_setScaleVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScaleVelocity(IzXomlResource* target);
	static bool				_setScaleVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScaleVelocityDamping(IzXomlResource* target);
	static bool				_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngle(IzXomlResource* target);
	static bool				_setAngularVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngularVelocity(IzXomlResource* target);
	static bool				_setAngularVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngularVelocityDamping(IzXomlResource* target);
	static bool				_setColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getColour(IzXomlResource* target);
	static bool				_setColourVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getColourVelocity(IzXomlResource* target);
	static bool				_setColourVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getColourVelocityDamping(IzXomlResource* target);
	static bool				_setDepth(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDepth(IzXomlResource* target);
	static bool				_setDepthVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDepthVelocity(IzXomlResource* target);
	static bool				_setDepthVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDepthVelocityDamping(IzXomlResource* target);
	static bool				_setActive(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getActive(IzXomlResource* target);
	static bool				_setVisible(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVisible(IzXomlResource* target);
	static bool				_setLifeSpan(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getLifeSpan(IzXomlResource* target);
	static bool				_setSpawnDelay(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSpawnDelay(IzXomlResource* target);
	static bool				_setLifeTime(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getLifeTime(IzXomlResource* target);
	static bool				_setLives(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getLives(IzXomlResource* target);
	static bool				_setOPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOPosition(IzXomlResource* target);
	static bool				_setOVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOVelocity(IzXomlResource* target);
	static bool				_setOScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOScale(IzXomlResource* target);
	static bool				_setOScaleVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOScaleVelocity(IzXomlResource* target);
	static bool				_setOAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOAngle(IzXomlResource* target);
	static bool				_setOAngularVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOAngularVelocity(IzXomlResource* target);
	static bool				_setOColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOColour(IzXomlResource* target);
	static bool				_setOColourVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOColourVelocity(IzXomlResource* target);
	static bool				_setODepth(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getODepth(IzXomlResource* target);
	static bool				_setODepthVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getODepthVelocity(IzXomlResource* target);
	static bool				_setAttached(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAttached(IzXomlResource* target);
	static bool				_setSrcRect(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSrcRect(IzXomlResource* target);

};

/**
 @class	CzActorParticles

 @brief	A particle system based actor.

 Many games and in fact some apps make use of a special effects. One method of generating special effects is using a particle system. A particle system is a system that generates a 
 group of particles over time using a particle generator. Particle systems are great for producing effects such as fireworks, sparkles, fire and smoke trails etc..

 In XOML a particle actor is declared using the ActorParticles tag. The ActorParticles example has been provided to show how they work. Lets take a quick look at some example XOML:

  @code
<ActorParticles Name="SnowyParticles" Image="Particle" Position="0, 0" Scale="1.0" Depth="1.0" Layer="1" PosMode="random" AngVelMode="random" VelMode="random" AngMode="random" 
  ScaleMode="random" DepthMode="random" DepthVelMode="random" PositionRange="1000, 1000" AngleRange="0, 360" AngVelRange="-25, 25" ScaleRange="0.25, 0.5" DepthRange="0.1, 2.0" 
  VelRange="-4, 4, -14, 0" ScaleVelRange="0, -0.1" DepthVelRange="-0.03, 0.01">
	<Particle Count="1000" Position="0, 0" VelocityDamping="0.99, 0.99" SrcRect="0, 0, 128, 128" ColourVelocity="0, 0, 0, -2" Duration="3" Repeat="-1" SpawnDelay="0.01" Gravity="15" />
</ActorParticles>
  @endcode

 In this example we create a particle actor generator that generates 1000 random spinning / flying particles over time that fall to the ground under the affect of gravity.
 In addition to the basic actor properties particle actors have a number of new properties, which include:

 <b>General</b>
 - Size (x, y) - Size of the particle actor (used to clip the actor)
 - Anchor (topleft or centre) - Sets the draw anchor (topleft causes the actor to be displayed relative to its top-left cornr whilst centre will use the actors centre)

 <b>Regeneration Modes</b>
 - PosMode (mode) - Determines how the position property of the particle will be regenerated when the particle comes to the end of its life.
 - AngMode (mode) - Determines how the angle property of the particle will be regenerated when the particle comes to the end of its life.
 - ScaleMode (mode) - Determines how the scale property of the particle will be regenerated when the particle comes to the end of its life.
 - DepthMode (mode) - Determines how the depth property of the particle will be regenerated when the particle comes to the end of its life.
 - VelMode (mode) - Determines how the velocity property of the particle will be regenerated when the particle comes to the end of its life.
 - AngVelMode (mode) - Determines how the angular velocity property of the particle will be regenerated when the particle comes to the end of its life.
 - ScaleVelMode (mode) - Determines how the scale velocity property of the particle will be regenerated when the particle comes to the end of its life.
 - DepthVelMode (mode) - Determines how the depth velocity property of the particle will be regenerated when the particle comes to the end of its life. 
 
 The mode parameter can be either:
 - random - The property will be randomly generated
 - normal - The property will not reset back to its original value Random Generation Ranges:

 <b>Random Generation Ranges</b>
 - PositionRange (x, y) - Sets the range for which positional coordinates can be randomly generated on the x and y axis. For example, setting a value of 50, 100 will generate random 
 coordinates between -50 and 50 for the x axis as well as -100 to 100 for the y axis
 - AngleRange (lower, upper) - Sets the range for which angle values can be generated from lower degrees to upper degrees. For example setting values of 10, 50 will generate random angles 
 between 10 and 50 degrees.
 - ScaleRange (lower, upper) - Sets the range for which scale values can be generated from lower to upper. For example setting values of 0.1, 2.0 will generate random scaling factors 
 between 0.1 and 2.0
 - DepthRange (lower, upper) - Sets the range for which depth values can be generated from lower to upper depth. For example setting values of 0.1, 10.0 will generate random depths 
 between 0.1 and 10.0
 - VelRange (lower x, upper x, lower y, upper y) - Sets the range for which velocity values can be generated from lower x to upper x, lower y to upper y. For example setting values 
 of -1, 1, -2, 2 will generate random velocities between -1 and 1 on the x axis and -2 and 2 on the y axis
 - AngVelRange - (lower, upper) - Sets the range for which angular velocity values can be generated from lower degrees/sec to upper degrees/sec. For example setting values of -0.1, 0.1 
 will generate random angular velocities between -0.1 and 0.1
 - ScaleVelRange (lower, upper) - Sets the range for which scale velocity values can be generated from lower to upper. For example setting values of 0.1, 0.2 will generate random scale 
 velocities between 0.1 and 0.2
 - DepthVelRange (lower, upper) - Sets the range for which depth velocity values can be generated from lower to upper. For example setting values of -0.01, 0.02 will generate random depth 
 velocities between -0.01 and 0.02 

Random generators are used when particular modes are set to random. For example, when PosMode=”random” PositionRange will be used to provide the range between which positional coordinates are generated. 

Individual particles are added as children of the particle actor. In addition, a number of particles can be automatically generated for you. The inner Particle tag has the following properties:
 - Attached (boolean) – When set to true particles will be attached to the emitter and will follow it, when set to false particles will remain where they are created (default is true)
 - Count (number) - The number of particles to generate
 - Position (x, y) - The position of the particle relative to the generator
 - Angle (degrees) - The angle of the particle
 - Scale (x, y) - The scale of the particle
 - Depth (depth) - The depth of the particle, the greater the value of depth the farther away the actor appears
 - Velocity (x, y) - Initial velocity of the particle
 - Colour (r, g, b, a) - Initial colour and opacity of the particle
 - VelocityDamping (x, y) - The rate at which to slow velocity down or speed it up. Values greater than 1 will increase velocity over time whilst values less than 1.0 will reduce the 
 velocity over time • AngularVelocity (number) - Initial angular velocity of the particle (degrees / sec)
 - AngularVelocityDamping (number) - The rate at which to slow angular velocity down or speed it up. Values greater than 1 will increase angular velocity over time whilst values less 
 than 1.0 will reduce the angular velocity over time
 - DepthVelocity (number) - Initial depth velocity of the particle
 - DepthVelocityDamping (number) - The rate at which to slow depth velocity down or speed it up. Values greater than 1 will increase depth velocity over time whilst values less than 1.0 
 will reduce the depth velocity over time
 - ScaleVelocity (x, y) - Initial scale velocity of the particle
 - ScaleVelocityDamping (x, y) - The rate at which to slow scale velocity down or speed it up. Values greater than 1 will increase scale velocity over time whilst values less than 1.0 will 
 reduce the scale velocity over time
 - ColourVelocity (r, g, b, a) - Initial colour velocity of the particle
 - ColourVelocityDamping (r, g, b, a) - The rate at which to slow colour velocity down or speed it up. Values greater than 1 will increase colour velocity over time whilst values less 
 than 1.0 will reduce the colour velocity over time
 - SrcRect (x, y, width, height) - Sets a part of the image to draw instead of the whole image (used in sprite atlases where multiple images are contained within the same image)
 - Repeat (number) - The number of times to repeat a particle (can be thought of its number of lives). Particles that run out of lives will be removed from the generator. Using a value 
 of -1 will regenerate the particle whilst the generator is active
 - SpawnDelay (seconds) - Sets an amount of time to wait before spawning this particle. If count is more than 1 then then the spawn delay of each consecutive particle will be compounded. 
 For example if you generate 3 particles with a spawn delay of 1.0 then the first particle will be generated after 1 second, the secnod particle after 2 seconds and the 3rd particle 
 after seconds.
 - Duration (seconds) - This represents the life time of the particle. For example, if you set the duration to 5 seconds, it will be regenerated after 5 seconds (deleted if repeat is 1)
 - Gravity (number) - The gravity property is used to add a gravitational affect to particles.

 */

class CzActorParticles : public CzActor
{
public:
	enum eParticleMode
	{
		PAM_Normal,
		PAM_Random,
	};

	// Properties
protected:
	CzImage*						Image;				///< Image used by the particles
	CzSlotArray<CzActorParticle*>*	Particles;			///< Particles in this system
	eParticleMode					PositionMode;		///< Mode of operatiuon for position
	eParticleMode					AngleMode;			///< Mode of operatiuon for angle
	eParticleMode					ScaleMode;			///< Mode of operatiuon for scale
	eParticleMode					DepthMode;			///< Mode of operatiuon for depth
	eParticleMode					VelocityMode;		///< Mode of operatiuon for velocity
	eParticleMode					AngVelocityMode;	///< Mode of operatiuon for angular velocity
	eParticleMode					ScaleVelocityMode;	///< Mode of operatiuon for scale velocity
	eParticleMode					DepthVelocityMode;	///< Mode of operatiuon for depth velocity
	CzVec2							PositionRange;		///< Range of spawn positions
	CzVec2							AngleRange;			///< Range of spawn angles - x = min angle, y = max angle
	CzVec2							ScaleRange;			///< Range of scales - x = min scale, y = max scale
	CzVec2							DepthRange;			///< Range of depth - x = min depth, y = max depth
	CzVec4							VelocityRange;		///< Range of spawn velocities - min_x, max_y, min_y, max_y
	CzVec2							AngVelocityRange;	///< Range of angular velocities - x = min ang velocity, y = max ang velocity
	CzVec2							ScaleVelocityRange;	///< Range of scale velocities - x = min scale velocity, y = max scale velocity
	CzVec2							DepthVelocityRange;	///< Range of depth velocities - x = min depth velocity, y = max depth velocity
	bool							AutoDelete;			///< Wehn set to true the actor will be automatically deleted when the particle system finishes running

public:
	bool			addParticle(CzActorParticle* particle, const CzIRect& src_rect, int count = 0, float spawn_delay = 0.0f, CzSprite::eAnchor anchor = CzSprite::Centre);
	void			removeParticle(CzActorParticle* particle)		{ Particles->remove(particle); delete particle; }
	void			removeAllParticles();

	int				getParticleCount() const						{ return Particles->getSize(); }
	CzActorParticle* getParticle(int index);
	CzImage*		getImage()										{ return Image; }
	void			setImage(CzImage* image)						{ Image = image; }
	void			setPositionMode(eParticleMode mode)				{ PositionMode = mode; }
	void			setAngleMode(eParticleMode mode)				{ AngleMode = mode; }
	void			setScaleMode(eParticleMode mode)				{ ScaleMode = mode; }
	void			setVelocityMode(eParticleMode mode)				{ VelocityMode = mode; }
	void			setAngVelocityMode(eParticleMode mode)			{ AngVelocityMode = mode; }
	void			setScaleVelocityMode(eParticleMode mode)		{ ScaleVelocityMode = mode; }
	void			setDepthMode(eParticleMode mode)				{ DepthMode = mode; }
	void			setDepthVelocityMode(eParticleMode mode)		{ DepthVelocityMode = mode; }
	void			setPositionRange(CzVec2& range)					{ PositionRange = range; }
	CzVec2			getPositionRange() const						{ return PositionRange; }
	void			setAngleRange(CzVec2& range)					{ AngleRange = range; }
	CzVec2			getAngleRange() const							{ return AngleRange; }
	void			setScaleRange(CzVec2& range)					{ ScaleRange = range; }
	CzVec2			getScaleRange() const							{ return ScaleRange; }
	void			setDepthRange(CzVec2& range)					{ DepthRange = range; }
	CzVec2			getDepthRange() const							{ return DepthRange; }
	void			setVelocityRange(CzVec4& range)					{ VelocityRange = range; }
	CzVec4			getVelocityRange() const						{ return VelocityRange; }
	void			setAngVelocityRange(CzVec2& range)				{ AngVelocityRange = range; }
	CzVec2			getAngVelocityRange() const						{ return AngVelocityRange; }
	void			setScaleVelocityRange(CzVec2& range)			{ ScaleVelocityRange = range; }
	CzVec2			getScaleVelocityRange() const					{ return ScaleVelocityRange; }
	void			setDepthVelocityRange(CzVec2& range)			{ DepthVelocityRange = range; }
	CzVec2			getDepthVelocityRange() const					{ return DepthVelocityRange; }
	void			SetDestSize(int w, int h)						{ Size.x = w; Size.y = h; }
	bool			setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool			setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool			getProperty(unsigned int property_name, CzXomlProperty& prop);
	void			setAutoDelete(bool enable)						{ AutoDelete = enable; }
	bool			isAutoDelete() const							{ return AutoDelete; }
	// Properties end
protected:
	void			setParticlesVisibility(bool visible);
	bool			UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzActorParticles() :	CzActor(), Particles(NULL), Image(NULL), PositionMode(PAM_Normal), AngleMode(PAM_Normal), ScaleMode(PAM_Normal), VelocityMode(PAM_Normal), 
								AngVelocityMode(PAM_Normal), ScaleVelocityMode(PAM_Normal), DepthMode(PAM_Normal), DepthVelocityMode(PAM_Normal), DepthRange(0, 0), 
								PositionRange(0, 0), AngleRange(0, 360), ScaleRange(1.0, 1.0), VelocityRange(0, 0, 0, 0), AngVelocityRange(0, 0), ScaleVelocityRange(0, 0), DepthVelocityRange(0, 0), 
								AutoDelete(true) { setActualClassType("actorparticles"); }
	virtual ~CzActorParticles()
	{
		if (Particles != NULL)
		{
			for (int t = 0; t < Particles->getSize(); t++)
			{
				CzActorParticle* particle = Particles->element_at(t);
				if (particle != NULL)
					delete particle;
			}
			SAFE_DELETE(Particles)
		}
	}

	bool				Init(int max_particles);
	bool				Update(float dt);
	bool				UpdateVisual();

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Generates a bunch of randmo particles using the ranges and modes previously set
	void				GenerateRandomParticles(int count, const CzIRect& src_rect, const CzVec4& colour, const CzVec4& colour_velocity, CzVec2& gravity, float duration, int repeat_count = 0, float spawn_delay_change = 0);
	// Generates a bunch of randmo particles using the ranges and modes previously set, using particle as a basis
	void				GenerateRandomParticles(int count, CzActorParticle* particle, const CzIRect& src_rect, float duration, int repeat_count = 0, float spawn_delay_change = 0);

	// Utility
	void				setVelocityAll(float x, float y, bool delta);

	// Internal
	void				RegenParticle(CzActorParticle* particle);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	ActorParticlesClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();

	static CzXomlProperty	_getParticleCount(IzXomlResource* target);
	static bool				_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSize(IzXomlResource* target);
	static bool				_setImage(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getImage(IzXomlResource* target);
	static bool				_setPosMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAngMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setScaleMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setDepthMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAngVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setScaleVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setDepthVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setPositionRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAngleRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setScaleRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setDepthRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAngVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setScaleVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setDepthVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setVelocityAll(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAutoDelete(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAutoDelete(IzXomlResource* target);
};

/// @}

/**
 @class	CzActorParticlesCreator

 @brief	Creates an instance of a particles based actor object.

 Used by the XOML system to instantiate a CzActorParticles object.


 */

class CzActorParticlesCreator : public IzXomlClassCreator
{
public:
	CzActorParticlesCreator()
	{
		setClassName("actorparticles");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzActorParticles(); }
};


#endif // _CZ_ACTOR_PARTICLES_H_
