/*
 * color_define.hpp
 *
 *  Created on: 14 lug 2021
 *      Author: stefanoc
 */

#ifndef TARGET_COLOR_DEFINE_HPP_
#define TARGET_COLOR_DEFINE_HPP_

//#ifdef __cplusplus
// extern "C" {
//#endif
/**************************************************************************************************
* INCLUDE
*************************************************************************************************/

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/Color.hpp>
/**************************************************************************************************
* DEFINE
*************************************************************************************************/

/**************************************************************************************************
* TYPEDEF
*************************************************************************************************/

/**************************************************************************************************
* PUBLIC VARIABLE
*************************************************************************************************/
//using namespace touchgfx;

class ColorDefined:public touchgfx::Color
{
public:
	ColorDefined(){};
	~ColorDefined(){};

	touchgfx::colortype setYellow()
	{
//		return getColorFromRGB(225,173 ,0);
		return getColorFromRGB(254,204,0);

	}
	touchgfx::colortype setBackground_Focus()
	{
//		return getColorFromRGB(225,173 ,0);
		return getColorFromRGB(102,179,46);
	}
	touchgfx::colortype setBackground_Unfocus()
	{
		//		return getColorFromRGB(225,173 ,0);
		return getColorFromRGB(128, 129, 132);
	}
	touchgfx::colortype setGray()
	{
		return getColorFromRGB(34,34,33);
	}
	touchgfx::colortype setWhite()
	{
		return getColorFromRGB(255,255,255);
	}
	touchgfx::colortype setBlack()
	{
		return getColorFromRGB(0, 0, 0);
	}
	protected:
};
//Yellow touchgfx::Color::getColorFromRGB(225,173 ,0 ));

/**************************************************************************************************
* PUBLIC FUNCTIONS DECLARATION
*************************************************************************************************/
//#ifdef __cplusplus
//}
//#endif

#endif /* TARGET_COLOR_DEFINE_HPP_ */
