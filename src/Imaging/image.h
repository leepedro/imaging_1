#if !defined(IMAGE_H)
#define IMAGE_H

#include <vector>

#include "dimension.h"

namespace Imaging
{
	/* TODO: This class covers only BIP format, i.e., channel -> column -> row.
	Should think about BSQ format, i.e., column -> row -> channel and BIL format, i.e.,
	column -> channel -> row if considering remote sensing applications. */
	/** Presents a pixel-based bitmap (raster) image.

	This class stores image data as a std::vector<T> object, so it does NOT need to release
	the memory at the destructor and it does NOT support padding bytes.
	Information about the dimension of the image is stored as an ImageSize object.
	The value of image data can be changed externally by references and iterators of the
	std::vector<T> object.
	The dimension of image data can be changed externally by designated member functions. 
	*/
	template <typename T>
	class Image
	{
	public:
		//////////////////////////////////////////////////
		// Types and constants.
		typedef T ValueType;
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
		Image(const ImageSize<SizeType> &sz);
		Image(SizeType width, SizeType height, SizeType depth = 1);

		//////////////////////////////////////////////////
		// Accessors.

		/** Accesses image data for given coordinate (x, y) by a reference. */
		T &operator()(SizeType x, SizeType y, SizeType c = 0);
		const T &operator()(SizeType x, SizeType y, SizeType c = 0) const;
		
		/** Accesses image data for given coordinate (x, y) by an iterator. */
		Iterator GetIterator(SizeType x, SizeType y, SizeType c = 0);
		ConstIterator GetIterator(SizeType x, SizeType y, SizeType c = 0) const;

		//const SizeType &width, &height, &depth;
		const std::vector<T> &data;
		const ImageSize<SizeType> &size;

		//////////////////////////////////////////////////
		// Methods.
		void clear(void);
		void resize(const ImageSize<SizeType> &sz);
		void resize(SizeType width, SizeType height, SizeType depth = 1);

	protected:
		//////////////////////////////////////////////////
		// Data.
		std::vector<T> data_;
		ImageSize<SizeType> size_;
	};

	// Copy ROI to ROI.
	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst, const Point2D<typename Image<T>::SizeType> &orgnDst);

	template <typename T>
	void Copy(const T *src, const ImageSize<typename Image<T>::SizeType> &sz,
		Image<T> &imgDst, const Point2D<typename Image<T>::SizeType> &orgnDst);
}

#include "image_inl.h"

#endif
