#if !defined(IMAGE2_H)
#define IMAGE2_H

#include "image.h"

namespace Imaging
{
	/*
	image = {band, column, row}
	*/
	//template <typename T>
	//class ImageStorage
	//{
	//public:
	//	//////////////////////////////////////////////////
	//	// Data.
	//	ImageFormat format;

	//protected:
	//	std::vector<T> data_;
	//};

	/*
	channel -> pixel -> line -> frame
	*/
	template <typename T>
	class ImageFrame
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef typename std::vector<T>::size_type SizeType;

		//////////////////////////////////////////////////
		// Default constructors.
		ImageFrame(void);
		ImageFrame(const ImageFrame<T> &src);
		ImageFrame(ImageFrame<T> &&src);
		ImageFrame<T> &operator=(const ImageFrame<T> &src);
		ImageFrame<T> &operator=(ImageFrame<T> &&src);

		//////////////////////////////////////////////////
		// Custom constructors.
		ImageFrame(const Size3D<SizeType> &sz);
		ImageFrame(SizeType width, SizeType height, SizeType depth = 1);

		//////////////////////////////////////////////////
		// Accessors.
		T &operator()(SizeType x, SizeType y, SizeType c = 0);
		const T &operator()(SizeType x, SizeType y, SizeType c = 0) const;
		const std::vector<T> &data;
		const Size3D<SizeType> &size;

		//////////////////////////////////////////////////
		// Methods.
		void Clear(void);
		void Resize(const Size3D<SizeType> &sz);
		void Resize(SizeType width, SizeType height, SizeType depth = 1);

	protected:
		//////////////////////////////////////////////////
		// Methods.
		SizeType GetOffset(SizeType x, SizeType y, SizeType c = 0) const;

		//////////////////////////////////////////////////
		// Data.
		std::vector<T> data_;
		Size3D<SizeType> size_;
	};

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

		//////////////////////////////////////////////////
		// Methods.
		void Clear(void);
		void Resize(SizeType n);
		void Resize(const Size3D<typename ImageFrame<T>::SizeType> &sz, SizeType n);
	protected:
		std::vector<ImageFrame<T> > frames_;
	};

	// case A: band <-> channel, either single-channel or multi-channel
	// case B: band <-> frame

	// Storage to processing
	/* ImageStorage to ImageFrame
	band -> channel, column -> pixel, row -> line (case A)
	*/
	/* ImageStorage to ImageBlock
	if band -> channel: band -> channel, column -> pixel, row -> line, frame = 1 (no need?)
	if band -> frame: channel = 1, column -> pixel, row -> line, band -> frame (case B)
	*/
	/* ImageStorage[t] to ImageBlock
	band -> channel, column -> pixel, row -> line, t -> frame (case A)
	*/
	/* ImageStorage[t] to ImageBlock[b]
	channel = 1, column -> pixel, row -> line, t -> frame, band -> b (for each band, case A)
	*/

	// Processing to storage
	/* ImageFrame to ImageStorage
	channel -> band, pixel -> column, line -> row (case A)
	*/
	/* ImageBlock to ImageStorage
	if frame = 1, channel -> band: channel -> band, pixel -> column, line -> row (for each frame, case A)
	if channel = 1, frame -> band: frame -> band, pixel -> column, line -> row (case B)
	*/
	/* ImageBlock to ImageStorage[t]
	channel -> band, pixel -> column, line -> row, frame -> t (case A)
	*/

	// a single data set to a single frame, band -> channel.
	template <typename T>
	void ConvertBand2Channel(const std::vector<T> &src, ImageFormat fmtSrc,
		ImageFrame<T> &dst);

	// multi data sets to a block (?), band -> channel, [t] -> frame
	template <typename T>
	void ConvertBand2Channel(const std::vector<std::vector<T> > &src, ImageFormat fmtSrc,
		ImageBlock<T> &dst);

	// a single data set to a block, band -> frame.
	template <typename T>
	void ConvertBand2Frame(const std::vector<T> &src, ImageFormat fmtSrc,
		ImageBlock<T> &dst);

	// a single frame to single data set, channel -> band.
	template <typename T>
	void ConvertChannel2Band(const ImageFrame<T> &src, std::vector<T> &dst,
		ImageFormat fmtDst);

	// a block to multi data sets, channel -> band, frame -> [t]
	template <typename T>
	void ConvertChannel2Band(const ImageBlock<T> &src, std::vector<std::vector<T> > &dst,
		ImageFormat fmtDst);

	// a block to a single data set, if channel = 1, frame -> band.
	template <typename T>
	void ConvertFrame2Band(const ImageBlock<T> &src, std::vector<T> &dst,
		ImageFormat fmtDst);
}

#include "image2_inl.h"

#endif
