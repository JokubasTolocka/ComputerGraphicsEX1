#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "color.hpp"
#include "surface.hpp"

void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	for(int i = aMinCorner.x; i < aMaxCorner.x; i++ ) {
		for(int j = aMinCorner.y; j < aMaxCorner.y; j++ ) {
			aSurface.set_pixel_srgb(i,j,aColor);
		}
	}

}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	for(int i = aMinCorner.x; i < aMaxCorner.x; i++ ) {
		for(int j = aMinCorner.y; j < aMaxCorner.y; j++ ) {
			if ((i > aMinCorner.x && i < aMaxCorner.x -1) && (j > aMinCorner.y && j < aMaxCorner.y -1)) {
				continue;
			}
			aSurface.set_pixel_srgb(i,j,aColor);
		}
	}
}
