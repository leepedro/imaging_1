#if !defined(IMAGE2_H)
#define IMAGE2_H

#include "image.h"

namespace Imaging
{
	/*
	image = {band, column, row}
	*/
	template <typename T>
	class ImageStorage
	{
	public:
		//////////////////////////////////////////////////
		// Data.
		ImageFormat format;

	protected:
		std::vector<T> data_;
	};

	/*
	channel -> pixel (column) -> line (row) -> frame
	*/
	template <typename T>
	class ImageFrame
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef typename std::vector<T>::size_type SizeType;

		//////////////////////////////////////////////////
		// Accessors.
		T &operator()(SizeType x, SizeType y, SizeType c = 0);
		const T &operator()(SizeType x, SizeType y, SizeType c = 0) const;

	protected:
		std::vector<T> data_;
	};

	// Storage to processing
	/* ImageStorage to ImageFrame
	band -> channel, column -> pixel, row -> line
	*/
	/* ImageStorage to ImageBlock
	if band -> channel: band -> channel, column -> pixel, row -> line, frame = 1
	if band -> frame: channel = 1, column -> pixel, row -> line, band -> frame
	*/
	/* ImageStorage[t] to ImageBlock
	band -> channel, column -> pixel, row -> line, t -> frame
	*/

	// Processing to storage
	/* ImageFrame to ImageStorage
	channel -> band, pixel -> column, line -> row
	*/
	/* ImageBlock to ImageStorage
	if frame = 1, channel -> band: channel -> band, pixel -> column, line -> row
	if channel = 1, frame -> band: frame -> band, pixel -> column, line -> row
	*/
	/* ImageBlock to ImageStorage[t]
	channel -> band, pixel -> column, line -> row, frame -> t
	*/

	/*
	BIP: band -> channel, column -> pixel, row -> line
	BSQ: band -> frame, column -> pixel, row -> line, channel = 1
	BIL: 
	*/
	template <typename T>
	void Copy(const std::vector<T> &src, ImageFormat fmtSrc, ImageFrame<T> &dst);

	template <typename T>
	void Copy(const ImageFrame<T> &src, std::vector<T> &dst, ImageFormat fmtDst);

	/*
	frame -> block
	*/
	template <typename T>
	class ImageBlock
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef typename std::vector<ImageFrame<T> >::size_type SizeType;

		//////////////////////////////////////////////////
		// Accessors.
		ImageFrame<T> &operator[](SizeType pos);
		const ImageFrame<T> &operator[](SizeType pos) const;

	protected:
		std::vector<ImageFrame<T> > frames_;
	};
}
#endif
