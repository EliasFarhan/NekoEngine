#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <mathematics/circle.h>
#include <mathematics/vector_nvec.h>
#include <mathematics/rect.h>

namespace neko
{	
    /*class  NCircle2D
    {
    public:
        std::array<Vec2f, 4> centers;
        std::array<float, 4> radius_array;
    	
        NCircle2D(): centers{}, radius_array{}
        {
	        
        }
    	
        explicit NCircle2D(Circle* circle)
        {
	        for (int i = 0; i < N; i++)
	        {
                centers[i] = circle[i].center;
                radius_array[i] = circle[i].radius;
	        }
        }
    	
        std::array<float, N> GetRadius(Circle* circle) const
        {
        	for (int i = 0; i < N; i++)
        	{
                radius_array[i] = circle[i].radius;
        	}
            return radius_array;
        }

        std::array<bool, N> Intersects(NCircle2D<C, N>* circle) const
        {
            std::array<bool, N> result;
            std::array<float, N> distanceX;
            std::array<float, N> distanceY;
            std::array<float, N> radiusSum;
        	
            for (int i = 0; i < N; i++)
            {
                distanceX[i] = centers[i].x - circle[i].centers.x;
                distanceY[i] = centers[i].y - circle[i].centers.y;
                radiusSum[i] = circle[i].radius + radius_array[i];

                result[i] = distanceX[i] * distanceX[i] + distanceY[i] * distanceY[i] <= radiusSum[i] * radiusSum[i];
            }
            return result;
        }

        bool IntersectsOther(NCircle2D<C,N> circle)
        {
            std::array<Vec2f, N> distanceVector;
        	std::array<float, N> magnitudes;
            std::array<bool, N> results;
        	
            for (int i = 0; i< N; i++)
            {
                distanceVector[i] = circle[i].center - centers[i];
                NVec2<float,N>::Dot(distanceVector[i], distanceVector[i]) * 2;
            	results[i] = magnitudes[i] <= circle[i].radius + radius_array[i];
            }
        	
            return results;
        }

        std::array<bool, N> RectCircleIntersects(std::array<Rect2f, N> rect) const
        {
            std::array<Vec2f, N> distanceVector;
            std::array<float, N> magnitudes;
            std::array<bool, N> results;
        	
            for (int i = 0; i < N; i++)
            {
                distanceVector[i] = rect[i].center - centers[i];
                magnitudes[i] = distanceVector[i].Magnitude();
            	
                    results[i] = magnitudes[i] <= rect[i].halfSize.x + radius_array[i] || magnitudes[i] <= rect[i].halfSize.y + radius_array[i];
                
            }
            return results;
        }
		using NCircle = NCircle2D<Vec2f, N>;
    };*/
    struct NSphere
    {
        std::array<float, 4> centerX;
        std::array<float, 4> centerY;
        std::array<float, 4> centerZ;
        std::array<float, 4> radius;
    	
        NSphere() = default;

    	template <typename T, int N>
    	explicit NSphere(std::array<Sphere, N> sphere)
        {
	        for (int i = 0; i < 4; i++)
	        {
                centerX[i] = sphere[i].center.x;
                centerY[i] = sphere[i].center.y;
                centerZ[i] = sphere[i].center.z;
                radius[i] = sphere[i].radius;
	        }
        }

    	std::array<float,4> GetRadius(std::array<Sphere, 4> sphere)
        {
	        for (int i = 0; i < 4; i++)
	        {
               radius[i] = sphere[i].radius;
	        }
            return radius;
        }

    	std::array<bool, 4> Intersect(std::array<Sphere, 4>& sphere)
        {
            std::array<float, 4> distanceX;
            std::array<float, 4> distanceY;
            std::array<float, 4> distanceZ;
            std::array<float, 4> radiusSum;
            std::array<bool, 4> results;
        	
	        for (int i = 0; i < 4; i++)
	        {
                distanceX[i] = centerX[i] - sphere[i].center.x;
                distanceY[i] = centerY[i] - sphere[i].center.y;
                distanceZ[i] = centerZ[i] - sphere[i].center.z;

                radiusSum[i] = radius[i] + sphere[i].radius;

                results[i] = distanceX[i] * distanceX[i] +
						  distanceY[i] * distanceY[i] +
						  distanceZ[i] * distanceZ[i] <= radiusSum[i] * radiusSum[i];
	        }
        	
            return results;
        }
    };
	
    
}