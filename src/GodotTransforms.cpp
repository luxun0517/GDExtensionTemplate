//
// Created by Harris.Lu on 2024/2/3.
//

#include "GodotTransforms.h"

namespace CesiumForGodot {

	Vector3 GodotTransforms::toGodot( const glm::dvec3 &vector )
    {
        return Vector3( vector.x, vector.y, vector.z );
    }

	Transform3D GodotTransforms::toGodotMathematics( const glm::dmat4 &transform )
    {
        return Transform3D( transform[0][0], transform[0][1], transform[0][2], transform[0][3],
                            transform[1][0], transform[1][1], transform[1][2], transform[1][3],
                            transform[2][0], transform[2][1], transform[2][2], transform[2][3] );
    }

    glm::dmat4 GodotTransforms::fromGodot( const Transform3D &transform )
	{
		return glm::dmat4( transform.basis[0][0], transform.basis[0][1], transform.basis[0][2], 0.0,
						   transform.basis[1][0], transform.basis[1][1], transform.basis[1][2], 0.0,
						   transform.basis[2][0], transform.basis[2][1], transform.basis[2][2], 0.0,
						   transform.origin.x, transform.origin.y, transform.origin.z, 1.0 );
	}

} // CesiumForGodot