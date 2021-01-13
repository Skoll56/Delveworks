#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>
#include "Component.h"

namespace Engine
{
	class ShadowMap;
	class ShadowCube;

	/* !This has been modified as part of the GRAPHICS UNIT to support shadows! */
	/* !The ability to cast lights (but not shadows) was already a feature that has been submitted before! */

	/** \brief  The Light Object Base Class.*/
	/** This is the base class for any Light source. */
	class Light : public Component
	{
		public:		
		/** \brief Returns the diffuse (Color) property of the light*/
		glm::vec3 getDif() { return m_diffuse; }

		/** \brief Returns the Specular Intensity of the light*/
		float getSpec() { return m_specIntens; }	

		protected:		
		glm::vec3 m_diffuse;
		float m_specIntens;				
		Light() {};
	};

	/** \brief  Point Lights*/
	/** These are lights which shine in all directions from a single point */
	class PointLight : public Light
	{
		public:
		PointLight() {};

		/** \brief Iniitialise the light with a Diffuse Color, Specular Intensity, Radius of the light and Brightness.*/
		void onInitialise(glm::vec3 _diffuse, float _specular, float _radius, float _brightness);

		/** \brief Returns the ShadowCube texture which the PointLight uses for its ShadowMap*/
		std::shared_ptr<ShadowCube> getShadowCube() { return m_SC; }

		/** \brief Returns the radius of the light*/
		float getRadius() { return m_radius; }

		/** \brief Sets the brightness of the light*/
		void setBrightness(float _brightness) { m_brightness = _brightness; }

		/** \brief Gets the brightness of the light*/
		float getBrightness() { return m_brightness; }
		
		/** \brief Update*/
		void update(int _i);

		/** \brief A "this" pointer*/
		std::weak_ptr<PointLight> m_self;

		protected:
		/** \brief The ShadowCube that this light uses*/
		std::shared_ptr<ShadowCube> m_SC;

		/** \brief The radius of the light*/
		float m_radius;

		/** \brief The brightness of the light*/
		float m_brightness;

		/** \brief A math variable used in the lighting calculation*/
		float m_quadratic;
		
	};

	/** \brief This is a Directional Light source. It covers the whole scene like a "Sun".*/
	class DirLight : public Light
	{
	public:

		/** \brief Initialise the source with a color, specular intensity and an ambient light value*/
		void onInitialise(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient);

		/** \brief Returns the Diffuse lighting*/
		glm::vec3 getDif() { return m_diffuse; }

		/** \brief Returns the specular intensity*/
		float getSpec() { return m_specIntens; }

		/** \brief Returns the Shadowmap attached to this object*/
		/** The Shadowmap must have a location to draw from. It uses the default Camera attached to the Core as a centre for this purpose */
		std::shared_ptr<ShadowMap> getShadowMap() { return m_SM; }

		/** \brief Returns the ambient light value*/
		glm::vec3 getAmb() { return m_ambient; }

		/** \brief Update*/
		void update(int _i);

		/** \brief A "this" pointer*/
		std::weak_ptr<DirLight> m_self;

	private:		
		/** \brief The ambient light amount*/
		glm::vec3 m_ambient;

		/** \brief The diffuse color of the light*/
		glm::vec3 m_diffuse;

		/** \brief The specular intensity of the light*/
		float m_specIntens;

		/** \brief The ShadowMap attached to the light*/
		std::shared_ptr<ShadowMap> m_SM;
	};


	/** \brief A spotlight is a light in a cone shape, like a Flashlight.*/
	/** It has an inner angle at full brightness, and an outer angle which gets less bright gradually */
	class SpotLight : public PointLight
	{
		public:
		/** \brief Initialise with a diffuse color, specular intensity, angle of the cone, an outer faded angle, radius of the light cone and brightness*/
		void onInitialise(glm::vec3 _color, float _specular, float _angle, float _fadeAngle, float _radius, float _brightness);

		/** \brief Update*/
		void update(int _i);

		/** \brief Get the angle of the cone*/
		float getAngle() { return m_angle; }

		/** \brief Get the outer fade angle*/
		float getFangle() { return m_fadeAngle; }

		/** \brief Returns the Shadowmap of this object*/
		std::shared_ptr<ShadowMap> getShadowMap() { return m_SM; }

		/** \brief A "this" pointer*/
		std::weak_ptr<SpotLight> m_self;

		private:
		/** \brief The angle of the cone*/
		float m_angle;

		/** \brief The ShadowMap attached to this Light*/
		std::shared_ptr<ShadowMap> m_SM;

		/** \brief The outer faded angle of the light. The light source will fade linearly between the 'Angle' and 'FadeAngle' values*/
		float m_fadeAngle;		
	};
}
#endif