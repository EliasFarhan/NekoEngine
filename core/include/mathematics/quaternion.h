#include <engine/entity.h>
#include <mathematics/matrix.h>
#include "engine/component.h"
#include "mathematics/vector.h"
//TEST
namespace neko
{
	class Quaternion
	{
	public:
		float Dot(Quaternion a, Quaternion b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}
		float Euler(float x, float y, float z)
		{

		}
		Quaternion Inverse(Quaternion quaternion)
		{

		}
		Quaternion Normalize(Quaternion quaternion)
		{

		}

	private:
		float x;
		float y;
		float z;
		float w;
	};
}