#if !defined(IMAGE_INL_H)
#define IMAGE_INL_H

namespace Imaging
{
	//////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Image<T>::Image(void) : data(data_), size(size_) {}

	template <typename T>
	Image<T>::Image(const Image<T> &src) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>(),
#else				// up to VS2012
		data(data_), size(size_),
#endif
		data_(src.data), size_(src.size) {}

	template <typename T>
	Image<T> &Image<T>::operator=(const Image<T> &src)
	{
		this->data_ = src.data;
		this->size_ = src.size;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Image<T>::Image(const ImageSize<SizeType> &sz) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>()
#else				// up to VS2012
		data(data_), size(size_)
#endif
	{
		this->resize(sz);
	}

	template <typename T>
	Image<T>::Image(SizeType width, SizeType height, SizeType depth) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>(ImageSize<SizeType>(width, height, depth)) {}
#else				// up to VS2012
		data(data_), size(size_)
	{
		this->resize(width, height, depth);
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Accessors.
	template <typename T>
	T &Image<T>::operator()(SizeType x, SizeType y)
	{
		this->size.CheckRange(x, y);
		return this->data_.at(this->size.GetOffset(x, y));
	}

	template <typename T>
	const T &Image<T>::operator()(SizeType x, SizeType y) const
	{
		this->size.CheckRange(x, y);
		return this->data.at(this->size.GetOffset(x, y));
	}

	template <typename T>
	typename Image<T>::Iterator Image<T>::GetIterator(SizeType x, SizeType y)
	{
		this->size.CheckRange(x, y);
		return this->data_.begin() + this->size.GetOffset(x, y);
	}

	template <typename T>
	typename Image<T>::ConstIterator Image<T>::GetIterator(SizeType x, SizeType y) const
	{
		this->size.CheckRange(x, y);
		return this->data.cbegin() + this->size.GetOffset(x, y);
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods.
	template <typename T>
	void Image<T>::clear(void)
	{
		this->data_.clear();
		this->size_ = ImageSize<SizeType>(0, 0, 0);
	}

	/** Resizes the std::vector<T> object only if necessary.
	If sizeension is changed while the total number of elements are the same (reshaping),
	it does NOT run resize() function of the std::vector<T>. */
	template <typename T>
	void Image<T>::resize(const ImageSize<SizeType> &sz)
	{
		if (this->data.size() != sz.GetNumElemPerFrame())
			this->data_.resize(sz.GetNumElemPerFrame());
		this->size_ = sz;
	}

	template <typename T>
	void Image<T>::resize(SizeType width, SizeType height, SizeType depth)
	{
		this->resize(ImageSize<SizeType>(width, height, depth));
	}


	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst, const Point2D<typename Image<T>::SizeType> &orgnDst)
	{
		// Check source/destination ROI.
		imgSrc.size.CheckRange(roiSrc.origin);
		Point2D<typename Image<T>::SizeType> ptEnd = roiSrc.origin + roiSrc.size;
		// NOTE: ptEnd is an excluding end of the ROI, so it could be up to (width, height).
		if (ptEnd.x > imgSrc.size.width || ptEnd.y > imgSrc.size.height)
		{
			std::ostringstream errMsg;
			errMsg << "(" << ptEnd.x << ", " << ptEnd.y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
		imgDst.size.CheckRange(orgnDst);
	}
}

#endif
