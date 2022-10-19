/* Inline functions need to have a definition that is visible to the compiler
 * whenever the function is used. THey could be define in the header that
 * declares the function. However, to keep the definitions and declarations
 * somewhat apart, it is a common practice to move them to an "inline" file
 * such as this one (*.inl extension). This file is then #include:ed at the end
 * of the header, to ensure that whoever includes the header also automatically
 * includes the inline file.
 *
 * Inlining allows us to avoid any overheads related to call (when building in
 * "release mode" / with optimizations enabled). This makes functions like
 * set_pixel_srgb() zero overhead abstractions.
 *
 */
inline
void Surface::set_pixel_srgb( std::size_t aX, std::size_t aY, ColorU8_sRGB const& aColor )
{
	assert( aX < mWidth && aY < mHeight ); // IMPORTANT! This line must remain the first line in this function!

	size_t i = get_linear_index(aX, aY);
	mSurface[i] = aColor.r;
	mSurface[i + 1] = aColor.g;
	mSurface[i + 2] = aColor.b;
	mSurface[i + 3] = 0;
}

inline 
std::size_t Surface::get_width() const noexcept
{
	return mWidth;
}
inline
std::size_t Surface::get_height() const noexcept
{
	return mHeight;
}

inline
std::size_t Surface::get_linear_index( std::size_t aX, std::size_t aY ) const noexcept
{
	size_t index = (mWidth * aX + aY) * 4;
	return index;
}
