#if !defined(IMAGE_H)
#define IMAGE_H

#include <vector>

#include "coordinates.h"

namespace Imaging
{
	/** Presents image format by how the data is stored.
	
	BIP: band-interleaved by pixel or pixel interleaved, e.g., most of RGB bitmap images
	Data is stored as channel -> column -> row.
	BSQ: band-sequential or band interleaved, e.g., planar RGB images, time-sequenced image
	frames
	Data is stored as column -> row -> channel.
	BIL: band-interleaved by line or row interleaved, e.g., hyper-spectral images with a
	line sensor
	Data is stored as column -> channel -> row.
	*/
	enum class ImageFormat {UNKNOWN, BIP, BSQ, BIL};

	/** Presents a pixel-based bitmap (raster) image.

	This class stores image data as a std::vector<T> object, so it does NOT need to release
	the memory at the destructor and it does NOT support padding bytes.
	The value of image data can be changed externally by references and iterators of the
	std::vector<T> object.
	The dimension of image data can be changed externally by designated member functions. 

	@NOTE The number of element is NOT the same as the number of bytes.
	It is identical to the number of bytes only if the image is 1 byte data type and the
	image does not have any padding bytes.	*/
	template <typename T>
	class Image
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef typename std::vector<T>::size_type SizeType;
		typedef typename std::vector<T>::iterator Iterator;
		typedef typename std::vector<T>::const_iterator ConstIterator;

		//////////////////////////////////////////////////
		// Default constructors.
		Image(void);
		Image(const Image<T> &src);
		Image<T> &operator=(const Image<T> &src);

		//////////////////////////////////////////////////
		// Custom constructors.
		Image(const Size3D<SizeType> &sz, ImageFormat fmt = ImageFormat::BIP);
		Image(SizeType width, SizeType height, SizeType depth = 1,
			ImageFormat fmt = ImageFormat::BIP);

		//////////////////////////////////////////////////
		// Accessors.

		/** Accesses image data for given coordinate (x, y, c) by a reference. */
		T &operator()(SizeType x, SizeType y, SizeType c = 0);
		const T &operator()(SizeType x, SizeType y, SizeType c = 0) const;
		
		/** Accesses image data for given coordinate (x, y, c) by an iterator. */
		Iterator GetIterator(SizeType x, SizeType y, SizeType c = 0);
		ConstIterator GetIterator(SizeType x, SizeType y, SizeType c = 0) const;

		/** Accesses image data for given coordinate (x, y, c) by a pointer. */
		T *GetPointer(SizeType x, SizeType y, SizeType c = 0);
		const T *GetPointer(SizeType x, SizeType y, SizeType c = 0) const;

		const std::vector<T> &data;
		const Size3D<SizeType> &size;

		//////////////////////////////////////////////////
		// Methods.
		void CheckDepth(SizeType c) const;
		void CheckRange(SizeType c) const;
		void CheckRange(SizeType x, SizeType y) const;
		void CheckRange(const Point2D<SizeType> &pt) const;
		void CheckRange(const Point2D<SizeType> &orgn, const Size2D<SizeType> &sz) const;
		void CheckRange(const Region<SizeType, SizeType> &roi) const;
		void clear(void);
		Region<typename Image<T>::SizeType, typename Image<T>::SizeType> GetRoi(void) const;
		void resize(const Size3D<SizeType> &sz);
		void resize(SizeType width, SizeType height, SizeType depth = 1);

		//////////////////////////////////////////////////
		// Data.
		ImageFormat format;

	protected:
		//////////////////////////////////////////////////
		// Methods.
		SizeType GetOffset(SizeType x, SizeType y, SizeType c = 0) const;

		//////////////////////////////////////////////////
		// Data.
		std::vector<T> data_;
		Size3D<SizeType> size_;
	};

	enum class ImageDataType {UINT8, INT8, UINT16, INT16, UINT32, INT32, UINT64, INT64,
		FLOAT32, FLOAT64};

	class ImageRaw
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef ::size_t SizeType;
		//typedef std::vector<unsigned char>::iterator Iterator;
		//typedef std::vector<unsigned char>::const_iterator ConstIterator;

		//////////////////////////////////////////////////
		// Accessors.
		/** Accesses image data for given coordinate (x, y, c) by a reference. */
		template <typename T>
		T &operator()(SizeType x, SizeType y, SizeType c = 0);
		template <typename T>
		const T &operator()(SizeType x, SizeType y, SizeType c = 0) const;

		/** Accesses image data for given coordinate (x, y, c) by a pointer. */
		template <typename T>
		T *GetPointer(SizeType x, SizeType y, SizeType c = 0);
		template <typename T>
		const T *GetPointer(SizeType x, SizeType y, SizeType c = 0) const;

		const Size3D<SizeType> &size;
		//const Size3D<SizeType> &size;

		//////////////////////////////////////////////////
		// Methods.
		void CheckDepth(SizeType c) const;
		void CheckRange(SizeType c) const;
		void CheckRange(SizeType x, SizeType y) const;
		void CheckRange(const Point2D<SizeType> &pt) const;
		void CheckRange(const Point2D<SizeType> &orgn, const Size2D<SizeType> &sz) const;
		void CheckRange(const Region<SizeType, SizeType> &roi) const;
		void Clear(void);
		Region<typename ImageRaw::SizeType, typename ImageRaw::SizeType> GetRoi(void) const;
		void Resize(const Size3D<SizeType> &sz);
		void Resize(SizeType width, SizeType height, SizeType depth = 1);
		::size_t GetBytesPerCh(void) const;

		//////////////////////////////////////////////////
		// Data.
		ImageFormat format;
		ImageDataType datatype;

	protected:
		//////////////////////////////////////////////////
		// Methods.
		SizeType GetOffset(SizeType x, SizeType y, SizeType c = 0) const;

		//////////////////////////////////////////////////
		// Data.
		void *data_;
		//std::vector<unsigned char> data_;
		Size3D<SizeType> size_;
		//Size3D<SizeType> size_;
		
	};

#if _MSC_VER > 1700	// from VS2013
	template <typename T>
	using ROI = Region<typename Image<T>::SizeType, typename Image<T>::SizeType>;
#endif

	/** Copies image data of an ROI to another ROI.

	@NOTE destination image must already have been allocated. */
	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst, const Point2D<typename Image<T>::SizeType> &orgnDst);

	/** Copies image data of an ROI to another image.

	@NOTE destination image will be resized based on the size of the source ROI. */
	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst);

	/* TODO: Copies image data of an ROI to another image while converting image type from
	one to another. */
	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst, ImageFormat fmt);

	/** Copies an entire image from a raw data block with zero padding.

	@NOTE destination image will be reallocated based on the size of source image. */
	template <typename T>
	void Copy(const T *src, const Size3D<typename Image<T>::SizeType> &sz,
		::size_t bytesPerLine, Image<T> &imgDst, ImageFormat fmt = ImageFormat::BIP);

	/** Copies an entire image from a raw data block without zero padding.
	
	@NOTE destination image will be reallocated based on the size of source image. */
	template <typename T>
	void Copy(const T *src, const Size3D<typename Image<T>::SizeType> &sz,
		Image<T> &imgDst, ImageFormat fmt = ImageFormat::BIP);

	/** Copies image data of an ROI to a raw data block.
	
	@NOTE The range of destination data block will NOT be checked.
	Users are responsible to ensure the memory is already allocated at the destination. */
	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		T *dst);

	// Copy the whole data at once instead of copying lines.
	template <typename T>
	void Copy(const Image<T> &imgSrc, T *dst);

}

#include "image_inl.h"

#endif
