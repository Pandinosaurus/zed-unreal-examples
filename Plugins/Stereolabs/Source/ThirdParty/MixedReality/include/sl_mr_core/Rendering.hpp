//======= Copyright (c) Stereolabs Corporation, All rights reserved. ===============

#ifndef __RENDERING_H__
#define __RENDERING_H__

#include "sl_mr_core/defines.hpp"

#include <sl/types.hpp>
#include <sl/core.hpp>

namespace sl {
	namespace mr {

		/**
		 * \brief Compute rendering plane size
		 * @param verticalFOV		Zed vertical FOV
		 * @param planeDistance	    Plane rendering distance from camera
		 * @param imageResolution   Zed image resolution
		 * @return size width/height of the plane
		 */
		SLMRCORE_API sl::float2 computeRenderPlaneSize(const sl::Resolution& imageResolution, float verticalFOV, float planeDistance);

		/**
		 * \brief Compute rendering plane size using gamma
		 * @param gamma			  The gamma
		 * @param HMDFocal        HMD focal
		 * @param planeDistance   Plane rendering distance from camera
		 * @param imageResolution Zed image resolution
		 * @return the width/height of the plane
		 */
		SLMRCORE_API sl::float2 computeRenderPlaneSizeWithGamma(const sl::Resolution& imageResolution, float perceptionDistance, float eyeToZedDistance, float planeDistance, float HMDFocal, float zedFocal);

		/**
		 * \brief Compute optical center offsets for left/right images
		 * @param calibrationParameters Zed calibration parameters
		 * @param imageResolution       Zed image resolution
		 * @param distance				Distance from camera
		 * @return the x/y offset for each eye
		 */
		SLMRCORE_API sl::float4 computeOpticalCentersOffsets(const sl::CalibrationParameters& calibrationParameters, const sl::Resolution& imageResolution, float distance);

		/**
		 * \brief Compute HMD focal
		 * @param renderTargetSize Render target size used by the HMD
		 * @param w				   Projection matrix [0][0] case
		 * @param h				   Projection matrix [1][1] case
		 */
		SLMRCORE_API float computeHMDFocal(const sl::Resolution& renderTargetSize, float w, float h);

		/**
		 * \brief Return the distance from the eyes to the Zed
		 * @param HMDDeviceType The HMD type
		 */
		SLMRCORE_API float getEyeToZEDDistance(sl::mr::HMD_DEVICE_TYPE HMDDeviceType);

		/**
		 * \brief Return factors for generating noise
		 * @param gain The camera gain
		 */
		SLMRCORE_API sl::mr::noiseFactors computeNoiseFactors(float gain);

	}
}

#endif // __RENDERING_H__