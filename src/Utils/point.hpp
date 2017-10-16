/*
 * point.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: kmolloy
 */

#ifndef SRC_UTILS_POINT_HPP_
#define SRC_UTILS_POINT_HPP_


namespace Antipatrea
{
class point
	{
		public:
        	point() {x=0.0;y=0.0;z=0.0;}
        	point(const double x, const double y, const double z) {this->x = x; this->y=y; this->z=z;}
        	void set(const double x, const double y, const double z) {this->x = x; this->y=y; this->z=z;}

        	double x;
        	double y;
        	double z;
	};
}
#endif /* SRC_UTILS_POINT_HPP_ */
