/*
 * Vector3Algebra.hpp
 *
 *  Created on: Oct 3, 2017
 *      Author: kmolloy
 */

#ifndef VECTOR3_ALGEBRA_H_
#define VECTOR3_ALGEBRA_H_

#include <stdio.h>
#include "Utils/PseudoRandom.hpp"
#include <math.h>

#ifndef EPSILON
#define EPSILON (1.0 / (1 << 22))
#endif

namespace Antipatrea
{
   /**
    *@author <a href="http:/www.cs.gmu.edu/~ashehu">Amarda Shehu</a>
    *@brief Functionality to measure various metrics on vectors in 3D
    */

    namespace Vector3Algebra
    {

	/**
         *@brief Data type for <tt>3</tt>-dimensional vectors.
         *
         *A <tt>3</tt>-dimensional vector <tt>v = (a, b, c)</tt> is a <tt>3</tt>-tuple,
         *where <tt>a, b</tt>, and \c c represent the first, second, and third coordinate of \c v, respectively.
         */
	typedef double* Vector3_t;

        /**
         *@name Declaration.
         *@{
         */

       /**
        *@brief Declare \c v to be a <tt>2</tt>-dimensional vector.
        */
       #define Vector3Declare(v) double v[3]
       #define Point3Declare(p)  double p[3]

      /**
       *END Declaration.
       *@}
       */

     /**
      *@name Dynamic memory management.
      *@{
      */

     /**
      *@brief Allocate memory.
      *
      *@returns
      *A contigous block of \c 3 elements of type \c double.
      */
	static inline
	Vector3_t Vector3Alloc(void)
	{
	    return (Vector3_t) calloc(3, sizeof(double));
	}

        /**
         *@brief Free memory.
         *
         *@param[in,out] v  a pointer of type  \c ::Vector3_t representing the memory address to be freed.
         *
         *@par Description:
         *Free the memory allocated to \c v.\n
         *No memory is freed if \c v is \c NULL.
         */
	static inline
	void Vector3Free(Vector3_t v)
	{
	    if(v)
		free(v);
	}

       /**
        *END Dynamic memory management.
        *@}
        */

       /**
        *@name Input/output functions.
        *@{
        */

       /**
        *@brief Read the elements of the vector \c v from the file stream \c in.
        *
        *@param[in,out] v  a pointer of type \c ::Vector3_t representing the vector to be read.
        *@param[in]     in a pointer to a file stream opened for reading.
        *
        *@par Description:
        *Read \c 3 consecutive \c double elements, <tt>v[0], v[1], v[2]</tt>,
        *from the file stream \c in points to.
        */
	static inline
	void Vector3Read(Vector3_t v, FILE *in)
	{
	    fscanf(in, "%lf %lf %lf", &(v[0]), &(v[1]), &(v[2]));
	}

       /**
        *@brief Print the elements of the vector \c v to the file stream \c out.
        *
        *@param[in] v   a pointer of type \c ::Vector3_t representing the vector to be printed.
        *@param[in] out a pointer to a file stream opened for writing.
        *
        *@par Description:
        *Write \c 3 consecutive \c double elements, <tt>v[0], v[1], v[2]</tt>,
        *separated by spaces to the file stream \c out points to.
        */
	static inline
	void Vector3Print(const Vector3_t v, FILE *out)
	{
	    fprintf(out, "%f %f %f", v[0], v[1], v[2]);
	}

        /**
         *END Input/output functions.
         *@}
         */

       /**
        *@name Access functions.
        *@{
        */

       /**
        *@brief  Access the <tt>i</tt>-th element of the vector \c v.
        *
        *@param[in] v a pointer of type  \c ::Vector3_t representing the vector to be accessed.
        *@param[in] i a \c int in the range <tt>0...2</tt> representing the index of the vector element.
        *
        *@returns
        *The element in position \c i of \c v.
        *
        *@warning
        *Errors may occur if the index is outside the allowed range due
        *to the inappropriate memory access.
        */
	static inline
	double Vector3GetElem(const Vector3_t v, const int i)
	{
	    return v[i];
	}

        /**
         *END Access functions.
         *@}
         */

        /**
         *@name Modify functions.
         *@{
         */

        /**
         *@brief  Set the <tt>i</tt>-th element of the vector \c v, i.e., <tt>v[i] <-- a</tt>.
         *
         *@param[in,out] v a pointer of type  \c ::Vector3_t representing the vector to be modified.
         *@param[in]     i a \c int in the range <tt>0...2</tt> representing the index of the vector element.
         *@param[in]     a a \c double representing the new value of the <tt>i</tt>-th vector element.
         *
         *@par Description:
         *Set the value of the element in position \c i of \c v to a.
         *
         *@warning
         *Errors may occur if the index is outside the allowed range due
         *to the inappropriate memory access.
         */
	static inline
	void Vector3SetElem(Vector3_t v, const int i, const double a)
	{
	    v[i] = a;
	}

        /**
         *@brief  Set the vector \c v to <tt>(a, b, c)</tt>.
         *
         *@param[in,out] v a pointer of type  \c ::Vector3_t representing the vector to be modified.
         *@param[in]     a a \c double representing the new value of the <tt>0</tt>-th vector element.
         *@param[in]     b a \c double representing the new value of the <tt>1</tt>-st vector element.
         *@param[in]     c a \c double representing the new value of the <tt>2</tt>-nd vector element.
         *
         *@par Description:
         *Set \c v to <tt>(a, b, c)</tt>.
         *
         */
	static inline
	void Vector3Set(Vector3_t v, const double a, const double b, const double c)
	{
	    v[0] = a;
	    v[1] = b;
	    v[2] = c;
	}

        /**
         *@brief Set every element of the vector \c v to \c 0.
         *
         *@param[in,out] v a pointer of type \c ::Vector3_t representing the vector to be modified.
         *
         *@returns
         *Vector \c v defined as <tt>(0, 0, 0)</tt>.
         */
	static inline
	Vector3_t Vector3Zero(Vector3_t v)
	{
	    v[0] = v[1] = v[2] = 0.0;

	    return v;
	}

        /**
         *END Modify functions.
         *@}
         */

        /**
         *@name Identity functions.
         *@{
         */

        /**
         *@brief Set the vector \c v to the <tt>3</tt>-dimensional additive identity vector.
         *
         *@param[in,out] v a pointer of type \c ::Vector3_t representing the vector to be modified.
         *
         *@returns
         *Vector \c v defined as
         *<CENTER><tt> v' = v + v' = v' + v</tt>,  for every <tt>3</tt>-dimensional vector <tt>v'</tt>. </CENTER>
         *It follows that all entries of \c v are \c 0.
         *
         *@attention
         *Equivalent to <tt>::Vector3Zero</tt>.
         */
	static inline
	Vector3_t Vector3AdditiveIdentity(Vector3_t v)
	{
	    return Vector3Zero(v);
	}

        /**
         *END Identity functions.
         *@}
         */

        /**
         *@name Utility functions.
         *@{
         */

        /**
         *@brief Copy the vector \c v1 into the vector \c v, i.e., <tt>v <-- COPY(v1)</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the vector to copy to.
         *@param[in]     v1 a pointer of type \c ::Vector3_t representing the vector to copy from.
         *
         *@returns
         *Vector \c v defined as the copy of the vector \c v1, i.e.,
         *<CENTER><tt>v[i] = v1[i]</tt>, for all <tt>i = 0, 1, 2</tt>.</CENTER>
         */
	static inline
	Vector3_t Vector3Copy(Vector3_t v, const Vector3_t v1)
	{
	    v[0] = v1[0];
	    v[1] = v1[1];
	    v[2] = v1[2];

	    return v;
	}

        /**
         *@brief Copy the elements of the vector \c v cast to \c float into the memory block to which \c vf points to.
         *
         *@param[in]     v  a pointer of type \c ::Vector3_t representing the vector to copy from.
         *@param[in,out] vf a pointer of type <tt>float *</tt> to a contigous block of <tt>3</tt> elements of type <tt>float</tt>.
         *
         *@returns
         *The pointer \c vf defined as the copy of the vector \c v.
         */
	static inline
	float* Vector3GetFloat(const Vector3_t v, float *vf)
	{
	    vf[0] = (float) v[0];
	    vf[1] = (float) v[1];
	    vf[2] = (float) v[2];

	    return vf;
	}

        /**
         *END Utility functions.
         *@}
         */

        /**
         *@name General math functions.
         *@{
         */

        /**
         *@brief Set the vector \c v to the sum of the vectors \c v1 and \c v2, i.e.,  <tt>v <-- v1 + v2</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the vector sum.
         *@param[in]     v1 a pointer of type \c ::Vector3_t representing the first operand.
         *@param[in]     v2 a pointer of type \c ::Vector3_t representing the second operand.
         *
         *@returns
         *Vector \c v defined as
         *<CENTER><tt>v[i] = v1[i] + v2[i]</tt>, for all <tt>i = 0, 1, 2</tt>.</CENTER>
         */
	static inline
	Vector3_t Vector3Add(Vector3_t v, const Vector3_t v1, const Vector3_t v2)
	{
	    v[0] = v1[0] + v2[0];
	    v[1] = v1[1] + v2[1];
	    v[2] = v1[2] + v2[2];

	    return v;
	}

        /**
         *@brief Set the vector \c v to the difference of the vectors \c v1 and \c v2, i.e.,  <tt>v <-- v1 - v2</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the vector difference.
         *@param[in]     v1 a pointer of type \c ::Vector3_t representing the first operand.
         *@param[in]     v2 a pointer of type \c ::Vector3_t representing the second operand.
         *
         *@returns
         *Vector \c v defined as
         *<CENTER><tt>v[i] = v1[i] - v2[i]</tt>, for all <tt>i = 0, 1, 2</tt>.</CENTER>
         */
	static inline
	Vector3_t Vector3Sub(Vector3_t v, const Vector3_t v1, const Vector3_t v2)
	{
	    v[0] = v1[0] - v2[0];
	    v[1] = v1[1] - v2[1];
	    v[2] = v1[2] - v2[2];

	    return v;
	}

       /**
        *@brief Set the vector \c v to the product of the vector \c v1 with the scalar \c d,  i.e.,  <tt>v <-- v1 * d</tt>.
        *
        *@param[in,out] v  a pointer of type \c ::Vector3_t representing the vector-scalar product.
        *@param[in]     v1 a pointer of type \c ::Vector3_t representing the vector operand.
        *@param[in]     d  a \c double representing the scalar operand.
        *
        *@returns
        *Vector \c v defined as
        *<CENTER><tt>v[i] = v1[i] * d</tt>, for all <tt>i = 0, 1, 2</tt>.</CENTER>
        */
	static inline
	Vector3_t Vector3MultiplyScalar(Vector3_t v, const Vector3_t v1, const double d)
	{
	    v[0] = v1[0] * d;
	    v[1] = v1[1] * d;
	    v[2] = v1[2] * d;

	    return v;
	}

        /**
         *@brief Find a vector that is perpendicular to \c from.
         *
         *@param[out] normal a pointer of type \c ::Vector3_t .
         *@param[in]  from a pointer of type \c ::Vector3_t .
         *
         *@returns
         *The vector \c normal that is perpendicular to vector \c from
         *
         */
	static inline
	void Vector3FindNormal(Vector3_t normal, const Vector3_t from)
	{
	    int imax = 0;
	    if(fabs(from[1]) > fabs(from[imax]))
		imax = 1;
	    if(fabs(from[2]) > fabs(from[imax]))
		imax = 2;

	    normal[(imax + 1) % 3] = 0;
	    normal[(imax + 2) % 3] = 1;
	    normal[imax] = -from[(imax + 2) % 3] / from[imax];
	}


        /**
         *@brief Set the vector \c v to the linear combination of the vectors \c v1 and \c v2 with
         *the scalars \c d1 and \c d2, i.e., <tt>v <-- v1 * d1 + v2 * d2</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the result.
         *@param[in]     v1 a pointer of type \c ::Vector3_t representing the first vector to be linearly combined.
         *@param[in]     d1 a \c double representing the scalar factor for the first vector.
         *@param[in]     v2 a pointer of type \c ::Vector3_t representing the second vector to be linearly combined.
         *@param[in]     d2 a \c double representing the scalar factor for the second vector.
         *
         *@returns
         *Vector \c v defined as
         *<CENTER> <tt>v[i] = v1[i] * d1 + v2[i] * d2</tt>, for all <tt>i = 0, 1, 2</tt>.</CENTER>
         *
         *@see
         *<tt>::Vector3Add, ::Vector3MultiplyScalar</tt>.
         */
	static inline
	Vector3_t Vector3LinearCombination(Vector3_t v, const Vector3_t v1, const double d1, const Vector3_t v2, const double d2)
	{
	    v[0] = v1[0] * d1 + v2[0] * d2;
	    v[1] = v1[1] * d1 + v2[1] * d2;
	    v[2] = v1[2] * d1 + v2[2] * d2;

	    return v;
	}

        /**
         *@brief Set the vector \c v to the cross product of the vectors \c v1 and \c v2, i.e., <tt>v <-- CROSS(v1, v2)</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the cross product.
         *@param[in]     v1 a pointer of type \c ::Vector3_t representing the first operand.
         *@param[in]     v2 a pointer of type \c ::Vector3_t representing the second operand.
         *
         *@returns
         *The cross product of the vectors <tt>v1 = (a1, b1, c1)</tt> and <tt>v2 = (a2, b2, c2)</tt> defined as
         *<CENTER><tt>v = v1 x v2 = (b1 * c2 - c1 * b2, c1 * a2 - a1 * c2, a1 * b2 - b1 * a2)</tt>.</CENTER>
         *
         *@warning
         *Vector cross product is not commutative.
         */
	static inline
	Vector3_t Vector3CrossProduct(Vector3_t v, const Vector3_t v1, const Vector3_t v2)
	{
	    double a[3];
	    a[0] = v1[1] * v2[2] - v1[2] * v2[1];
	    a[1] = v1[2] * v2[0] - v1[0] * v2[2];
	    a[2] = v1[0] * v2[1] - v1[1] * v2[0];

	    v[0] = a[0];
	    v[1] = a[1];
	    v[2] = a[2];

	    return v;
	}

        /**
         *@brief Compute the dot product of the vectors \c v1 and \c v2.
         *
         *@param[in] v1 a pointer of type \c ::Vector3_t representing the first operand.
         *@param[in] v2 a pointer of type \c ::Vector3_t representing the second operand.
         *
         *@returns
         *The dot product of the vectors <tt>v1 = (a1, b1, c1)</tt> and <tt>v2 = (a2, b2, c2)</tt> defined as
         *<CENTER><tt>a1 * a2 + b1 * b2 + c1 * c2</tt>.</CENTER>
         */
	static inline
	double Vector3DotProduct(const Vector3_t v1, const Vector3_t v2)
	{
	    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

        /**
         *@brief Compute the norm of the vector \c v.
         *
         *@param[in] v a pointer of type \c ::Vector3_t representing the vector to be accessed.
         *
         *@returns
         *The norm of the vector <tt>v = (a, b, c)</tt> defined as
	 *<CENTER><tt>
         *norm(v) = sqrt(a * a + b * b + c * c).
         *</tt></CENTER>
         */
	static inline
	double Vector3Norm(const Vector3_t v)
	{
	    return sqrt(Vector3DotProduct(v, v));
	}

        /**
         *@brief Normalize the vector \c v, i.e., <tt>v <-- UNIT(v)</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the vector to be accessed and then modified.
         *
         *@returns
         *The unit vector \c v defined as
         *<CENTER><tt>v / norm(v)</tt>.</CENTER>
         *
         *@warning
         *If <tt>norm(v) = 0</tt>, then \c v is not modified.
         *
         *@see
         *<tt>::Vector3Norm</tt>.
         */
	static inline
	Vector3_t Vector3Unit(Vector3_t v)
	{
	    double d = Vector3Norm(v);

	    if(d != 0)
	    {
		v[0] /= d;
		v[1] /= d;
		v[2] /= d;
	    }

	    return v;
	}

        /**
         *@brief Set every element of the vector \c v to random numbers between \c min and \c max.
         *
         *@param[in,out] v a pointer of type \c ::Vector3_t representing the vector to be modified.
         *@param[in] min, real representing the beginning of interval to randomly generate elements
         *@param[in] max, real representing the end of interval to randomly generate elements
         *
         *@returns
         *Vector \c v, with randomly generated elements from min to max, and normalized.
         */
	static inline
	void Vector3Random(Vector3_t v, double min, double max)
	{
	    v[0] = Antipatrea::RandomUniformReal(min, max);
	    v[1] = Antipatrea::RandomUniformReal(min, max);
	    v[2] = Antipatrea::RandomUniformReal(min, max);

	    Vector3Unit(v);
	}


        /**
         *@brief Set the vector \c v to the negation of the vector \c v1, i.e., <tt>v <-- NEGATE(v1)</tt>.
         *
         *@param[in,out] v  a pointer of type \c ::Vector3_t representing the negated vector.
         *@param[in]     v1 a pointer of type \c ::Vector3_t representing the vector to be negated.
         *
         *@returns
         *Vector \c v defined as
         *<CENTER><tt>v = -v1 = (-a1, -b1, -c1)</tt>, where <tt>v1 = (a1, b1, c1)</tt>.</CENTER>
         */
	static inline
	Vector3_t Vector3Negate(Vector3_t v, const Vector3_t v1)
	{
	    v[0] = -v1[0];
	    v[1] = -v1[1];
	    v[2] = -v1[2];

	    return v;
	}

        /**
         *@brief Compute the angle between the vectors \c v1 and \c v2.
         *
         *@param[in] v1 a pointer of type \c ::Vector3_t representing the first vector.
         *@param[in] v2 a pointer of type \c ::Vector3_t representing the second vector.
         *
         *@returns
         *The angle \c theta between the vectors \c v1 and \c v2 defined as
         *<CENTER><tt>theta = acos(dot(v1, v2) / (norm(v1) * norm(v2))</tt>.</CENTER>
         *The computed angle \c theta is in the range
         *<CENTER><tt>[0, Pi]</tt>.</CENTER>
         *
         *@warning
         *If <tt>norm(v1)</tt> or <tt>norm(v2)</tt> is close to \c 0, then \c 0 is
         *returned as the angle between \c v1 and \c v2.
         */
	static inline
	double Vector3Angle(const Vector3_t v1, const Vector3_t v2)
	{
	    double d = Vector3DotProduct(v1, v1) * Vector3DotProduct(v2, v2);

	    if(d == 0.0)
		return 0;
	    return acos(Vector3DotProduct(v1, v2) / sqrt(d));
	}

	static inline
	double Vector3AnglePlane(const Vector3_t v1, const Vector3_t v2,
				 const Vector3_t v3)
	{
	    Vector3Declare(normal);
	    Vector3CrossProduct(normal, v2, v3);

	    const double alpha = 0.5 * M_PI - Vector3Angle(v1, normal);
	    return alpha < 0 ? -alpha : alpha;
	}

        /**
         *END General math functions.
         *@}
         */

         /**
          *@name Linear interpolation functions.
          *@{
          */

         /**
          *@brief Set the vector \c v to the linear interpolation of the vectors \c v1 and \c v2.
          *
          *@param[in,out] v   a pointer of type \c ::Vector3_t representing the interpolated vector.
          *@param[in]     v1  a pointer of type \c ::Vector3_t representing the source vector.
          *@param[in]     v2  a pointer of type \c ::Vector3_t representing the destination vector.
          *@param[in]     t   a \c double in the interval <tt>[0, 1]</tt> representing the interpolation factor.
          *
          *@returns
          *Vector \c v defined as
          *<CENTER>
          *<tt> v = v(t) = (1-t) * v1  + t * v2</tt>.
          *</CENTER>
          */
	static inline
	Vector3_t Vector3lerp(Vector3_t v, const Vector3_t v1, const Vector3_t v2, const double t)
	{
	    return Vector3LinearCombination(v, v1, (1 - t), v2, t);
	}

        /**
         *END Linear interpolation functions.
         *@}
         */

        /**
         *@name Distance functions.
         *@{
         */

        /**
         *@brief  Compute the square of the Euclidean distance between the vectors \c v1 and \c v2.
         *
         *@param[in] v1  a pointer of type \c ::Vector3_t representing the first vector.
         *@param[in] v2  a pointer of type \c ::Vector3_t representing the second vector.
         *
         *@returns
         *The square of the distance between \c v1 and \c v2 defined as
         *<CENTER><tt>dot(v1 - v2, v1 - v2)</tt>.</CENTER>
         */
	static inline
	double Vector3SquareDistance(const Vector3_t v1, const Vector3_t v2)
	{
	    return
		(v1[0] - v2[0]) * (v1[0] - v2[0]) +
		(v1[1] - v2[1]) * (v1[1] - v2[1]) +
		(v1[2] - v2[2]) * (v1[2] - v2[2]);
	}

       /**
        *@brief  Compute the Euclidean distance between the vectors \c v1 and \c v2.
        *
        *@param[in] v1  a pointer of type \c ::Vector3_t representing the first vector.
        *@param[in] v2  a pointer of type \c ::Vector3_t representing the second vector.
        *
        *@returns
        *The distance between \c v1 and \c v2 defined as
        *<CENTER><tt>norm(v1 - v2) = sqrt(dot(v1 - v2, v1 - v2))</tt>.</CENTER>
        */
	static inline
	double Vector3Distance(const Vector3_t v1, const Vector3_t v2)
	{
	    return sqrt(Vector3SquareDistance(v1, v2));
	}

        /**
         *@name Distance functions.
         *@}
         */

       /**
        *@author <a href="http:/www.cs.gmu.edu/~ashehu">Amarda Shehu</a>
        *@brief  Compute the parallel projection of the vector <em>u</em> unto the vector <em>v</em>
        *
        *@param u vector
        *@param v vector
        *@param proj parallel projection of <em>u</em> onto <em>v</em>
        *
        *@par Description:
        *<em>proj <-- v * dot(u, v) / dot(v, v)</em>
        */
	static inline
	double Vector3ParallelProjection(const Vector3_t u, const Vector3_t v)
	{
	    return Vector3DotProduct(u, v) / Vector3DotProduct(v, v);
	}

    };
}

#endif
