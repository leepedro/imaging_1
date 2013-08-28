#if !defined(IMAGE2_INL_H)
#define IMAGE2_INL_H

namespace Imaging
{
	//////////////////////////////////////////////////////////////////////////
	// ImageFrame<T> class

	//////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	ImageFrame<T>::ImageFrame(void) : data(data_), size(size_),
		size_(Size3D<SizeType>(0, 0, 0)) {}

	template <typename T>
	ImageFrame<T>::ImageFrame(const ImageFrame<T> &src) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>(),
#else				// up to VS2012
		data(data_), size(size_),
#endif
		data_(src.data) { size_ = src.size; }

//	template <typename T>
//	ImageFrame<T>::ImageFrame(ImageFrame<T> &&src) :
//#if _MSC_VER > 1700	// from VS2013
//		Image<T>(),
//#else				// up to VS2012
//		data(data_), size(size_),
//#endif
//		// TODO: Next line won't work because src.data is const &!!!
//		data_(std::move(src.data)) { size_ = src.size; }

	template <typename T>
	ImageFrame<T> &ImageFrame<T>::operator=(const ImageFrame<T> &src)
	{
		this->data_ = src.data;
		this->size_ = src.size;
		return *this;
	}

	//template <typename T>
	//ImageFrame<T> &ImageFrame<T>::operator=(ImageFrame<T> &&src)
	//{
	//	this->data_ = std::move(src.data);
	//	this->size_ = std::move(src.size);
	//	return *this;
	//}

	//////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	ImageFrame<T>::ImageFrame(const Size3D<SizeType> &sz) :
#if _MSC_VER > 1700	// from VS2013
		ImageFrame<T>()
#else				// up to VS2012
		data(data_), size(size_)
#endif
	{
		this->Resize(sz);
	}

	template <typename T>
	ImageFrame<T>::ImageFrame(SizeType width, SizeType height, SizeType depth) :
#if _MSC_VER > 1700	// from VS2013
		ImageFrame<T>(Size3D<SizeType>(width, height, depth), fmt) {}
#else				// up to VS2012
		data(data_), size(size_)
	{
		this->Resize(width, height, depth);
	}
#endif

	/** Resizes the std::vector<T> object only if necessary.
	If size is changed while the total number of elements are the same (reshaping),
	it does NOT run resize() function of the std::vector<T>. */
	template <typename T>
	void ImageFrame<T>::Resize(const Size3D<SizeType> &sz)
	{
		SizeType nElem = sz.width * sz.height * sz.depth;
		if (this->data.size() != nElem)
			this->data_.resize(nElem);
		this->size_ = sz;
	}

	template <typename T>
	void ImageFrame<T>::Resize(SizeType width, SizeType height, SizeType depth)
	{
		this->Resize(Size3D<SizeType>(width, height, depth));
	}

}
#endif
