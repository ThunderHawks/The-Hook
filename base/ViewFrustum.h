#ifndef VIEW_FRUSTUM_H_
#define VIEW_FRUSTUM_H_

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

/* input:   the center of a bounding sphere, the radius of the bounding sphere (put 0 for just a point), and the model view matrix
 * summary: Checks if a bounding sphere is inside the view frustum
 * output:  A number representing which plane it didn't pass. Still working on things other than 0. might just have it always be 1 if it fails
 *				0 = inside frustum
 *				1 = not in frustum
*/
int checkViewFrustum (glm::vec3 point, int rad, glm::mat4 mView);

#endif
