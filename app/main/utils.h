/*
 * utils.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef UTILS_H_
#define UTILS_H_


namespace Utils {
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
}


#endif /* UTILS_H_ */
