#if !defined(IMAGE_H)
#define IMAGE_H

#include <vector>

#include "dimension.h"

namespace Imaging
{
	/** Presents a pixel-based bitmap (raster) image.
	This class stores image data as a std::vector<T> object, so it does NOT need to release
	the memory at the destructor and it does NOT support padding bytes.
	Information about the dimension of the image is stored as a Dimension<SizeType> object.
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
		Image(const Dimension<SizeType>& d);
		Image(SizeType w, SizeType h, SizeType d = 1);

		//////////////////////////////////////////////////
		// Accessors.

		/** Accesses image data for given coordinate (x, y) by a reference. */
		T &operator()(SizeType x, SizeType y);
		const T &operator()(SizeType x, SizeType y) const;

		/** Accesses image data for given coordinate (x, y) by an iterator. */
		Iterator GetIterator(SizeType x, SizeType y);
		ConstIterator GetIterator(SizeType x, SizeType y) const;

		const SizeType &width, &height, &depth;
		const std::vector<T> &data;
		const Dimension<SizeType> &dim;

		//////////////////////////////////////////////////
		// Methods.
		void clear(void);
		void resize(const Dimension<SizeType>& dim);
		void resize(SizeType w, SizeType h, SizeType d = 1);

	protected:
		//////////////////////////////////////////////////
		// Data.
		std::vector<T> data_;
		Dimension<SizeType> dim_;
	};

	//////////////////////////////////////////////////////////////////////////
	// Default constructors.
	/** @NOTE Check if this is the right way to initialize width, height, and depth.
	Test it by changing dim. */
	template <typename T>
	Image<T>::Image(void) :
		width(dim_.width), height(dim_.height), depth(dim_.depth), data(data_), dim(dim_) {}

	template <typename T>
	Image<T>::Image(const Image<T> &src) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>(),
#else				// up to VS2012
		width(dim_.width), height(dim_.height), depth(dim_.depth), data(data_), dim(dim_),
#endif
		data_(src.data), dim_(src.dim) {}

	template <typename T>
	Image<T> &Image<T>::operator=(const Image<T> &src)
	{
		this->data_ = src.data;
		this->dim_ = src.dim;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Image<T>::Image(const Dimension<SizeType>& d) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>()
#else				// up to VS2012
		width(dim_.width), height(dim_.height), depth(dim_.depth), data(data_), dim(dim_)
#endif
	{
		this->resize(d);
	}

	template <typename T>
	Image<T>::Image(SizeType w, SizeType h, SizeType d) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>()
#else				// up to VS2012
		width(dim_.width), height(dim_.height), depth(dim_.depth), data(data_), dim(dim_)
#endif
	{
		this->resize(Dimension<SizeType>(w, h, d));
	}

	//////////////////////////////////////////////////////////////////////////
	// Accessors.
	template <typename T>
	T &Image<T>::operator()(SizeType x, SizeType y)
	{
		this->dim.CheckRange(x, y);
		return this->data_.at(this->dim.GetOffset(x, y));
	}

	template <typename T>
	const T &Image<T>::operator()(SizeType x, SizeType y) const
	{
		this->dim.CheckRange(x, y);
		return this->data.at(this->dim.GetOffset(x, y));
	}

	template <typename T>
	typename Image<T>::Iterator Image<T>::GetIterator(SizeType x, SizeType y)
	{
		this->dim.CheckRange(x, y);
		return this->data_.begin() + this->dim.GetOffset(x, y);
	}

	template <typename T>
	typename Image<T>::ConstIterator Image<T>::GetIterator(SizeType x, SizeType y) const
	{
		this->dim.CheckRange(x, y);
		return this->data.cbegin() + this->dim.GetOffset(x, y);
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods.
	template <typename T>
	void Image<T>::clear(void)
	{
		this->data_.clear();
		this->dim_.width = 0;
		this->dim_.height = 0;
		this->dim_.depth = 0;
	}

	/** Resizes the std::vector<T> object only if necessary.
	If dimension is changed while the total number of elements are the same (reshaping),
	it does NOT run resize() function of the std::vector<T>. */
	template <typename T>
	void Image<T>::resize(const Dimension<SizeType>& dim)
	{
		if (this->data_.size() != dim.GetNumElemPerFrame())
			this->data_.resize(dim.GetNumElemPerFrame());
		this->dim_ = dim;
	}

	template <typename T>
	void Image<T>::resize(SizeType w, SizeType h, SizeType d)
	{
		this->resize(Dimension<SizeType>(w, h, d));
	}

}

#endif
