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
	T &Image<T>::operator()(SizeType x, SizeType y, SizeType c)
	{
		this->size.CheckRange(x, y, c);
		return this->data_.at(this->size.GetOffset(x, y, c));
	}

	template <typename T>
	const T &Image<T>::operator()(SizeType x, SizeType y, SizeType c) const
	{
		this->size.CheckRange(x, y, c);
		return this->data.at(this->size.GetOffset(x, y, c));
	}

	template <typename T>
	typename Image<T>::Iterator Image<T>::GetIterator(SizeType x, SizeType y, SizeType c)
	{
		this->size.CheckRange(x, y, c);
		return this->data_.begin() + this->size.GetOffset(x, y, c);
	}

	template <typename T>
	typename Image<T>::ConstIterator Image<T>::GetIterator(SizeType x, SizeType y, SizeType c) const
	{
		this->size.CheckRange(x, y, c);
		return this->data.cbegin() + this->size.GetOffset(x, y, c);
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
	If size is changed while the total number of elements are the same (reshaping),
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

	// This function is valid for only BIP format.
	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst, const Point2D<typename Image<T>::SizeType> &orgnDst)
	{
		// Check the depth of both images.
		imgSrc.size.CheckDepth(imgDst.size.depth);
		// Check source/destination ROI.
		imgSrc.size.CheckRange(roiSrc.origin, roiSrc.size);
		imgDst.size.CheckRange(orgnDst, roiSrc.size);

		// Copy line by line.
		auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
		auto it_dst = imgDst.GetIterator(orgnDst.x, orgnDst.y);
		auto nElemPerLineSrc = imgSrc.size.GetNumElemPerLine();
		auto nElemPerLineDst = imgDst.size.GetNumElemPerLine();
		auto nElemWidth = imgSrc.size.depth * roiSrc.size.width;
		for (auto H = 0; H != roiSrc.size.height; ++H)
		{
			std::copy(it_src, it_src + nElemWidth, it_dst);
			it_src += nElemPerLineSrc;
			it_dst += nElemPerLineDst;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// ImageNew<T> class

	//////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	ImageNew<T>::ImageNew(void) : data(data_), size(size_) {}

	template <typename T>
	ImageNew<T>::ImageNew(const ImageNew<T> &src) :
#if _MSC_VER > 1700	// from VS2013
		ImageNew<T>(),
#else				// up to VS2012
		data(data_), size(size_),
#endif
		data_(src.data), size_(src.size), format(src.format) {}

	template <typename T>
	ImageNew<T> &ImageNew<T>::operator=(const ImageNew<T> &src)
	{
		this->data_ = src.data;
		this->size_ = src.size;
		this->format = src.format;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	ImageNew<T>::ImageNew(const Size3D<SizeType> &sz, ImageFormat fmt) :
#if _MSC_VER > 1700	// from VS2013
		ImageNew<T>()
#else				// up to VS2012
		data(data_), size(size_)
#endif
	{
		this->resize(sz);
		this->format = fmt;
	}

	template <typename T>
	ImageNew<T>::ImageNew(SizeType width, SizeType height, SizeType depth, ImageFormat fmt) :
#if _MSC_VER > 1700	// from VS2013
		ImageNew<T>(Size3D<SizeType>(width, height, depth), fmt) {}
#else				// up to VS2012
		data(data_), size(size_)
	{
		this->resize(width, height, depth);
		this->format = fmt;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Accessors.

	// NOTE: Check the range for given position only if the method is declared as public.

	template <typename T>
	T &ImageNew<T>::operator()(SizeType x, SizeType y, SizeType c)
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data_.at(this->GetOffset(x, y, c));
	}

	template <typename T>
	const T &ImageNew<T>::operator()(SizeType x, SizeType y, SizeType c) const
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data.at(this->GetOffset(x, y, c));
	}

	template <typename T>
	typename ImageNew<T>::Iterator ImageNew<T>::GetIterator(SizeType x, SizeType y, SizeType c)
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data_.begin() + this->GetOffset(x, y, c);
	}

	template <typename T>
	typename ImageNew<T>::ConstIterator ImageNew<T>::GetIterator(SizeType x, SizeType y, SizeType c) const
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data.cbegin() + this->GetOffset(x, y, c);
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods.

	template <typename T>
	void ImageNew<T>::CheckDepth(const ImageNew<T> &img) const
	{
		if (this->size.depth != img.size.depth)
			throw std::runtime_error("Depth is not matched.");
	}

	/** Throws an exception instead of returning false because you have to throw an
	exception at a higher level any way. */
	template <typename T>
	void ImageNew<T>::CheckRange(SizeType c) const
	{
		if (c >= this->size.depth)
		{
			std::ostringstream errMsg;
			errMsg << "Channel c = " << c << " is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void ImageNew<T>::CheckRange(SizeType x, SizeType y) const
	{
		if (x < 0 || x >= this->size.width || y < 0 || y >= this->size.height)
		{
			std::ostringstream errMsg;
			errMsg << "(" << x << ", " << y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void ImageNew<T>::CheckRange(const Point2D<SizeType> &pt) const
	{
		this->CheckRange(pt.x, pt.y);
	}

	// The end points are the excluding end of an ROI, so it could be up to (width, height).
	template <typename T>
	void ImageNew<T>::CheckRange(const Point2D<SizeType> &orgn, const Size2D<SizeType> &sz) const
	{
		Point2D<SizeType> ptEnd = orgn + sz;
		if (orgn.x < 0 || ptEnd.x > this->size.width || orgn.y < 0 ||
			ptEnd.y > this->size.height)
		{
			std::ostringstream errMsg;
			errMsg << "[" << orgn.x << ", " << orgn.y << "] ~ (" << ptEnd.x << ", " <<
				ptEnd.y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void ImageNew<T>::CheckRange(const Region<SizeType, SizeType> &roi) const
	{
		this->CheckRange(roi.origin, roi.size);
	}

	template <typename T>
	void ImageNew<T>::clear(void)
	{
		this->data_.clear();
		this->size_ = Size3D<SizeType>(0, 0, 0);
	}

	template <typename T>
	typename ImageNew<T>::SizeType ImageNew<T>::GetOffset(SizeType x, SizeType y, SizeType c) const
	{
		switch (this->format)
		{
		case ImageFormat::BIP:
			return c + this->size.depth * x + this->size.depth * this->size.width * y;
		case ImageFormat::BSQ:
			return x + this->size.width * y + this->size.width * this->size.height * c;
		case ImageFormat::BIL:
			return x + this->size.width * c + this->size.width * this->size.depth * y;
		default:
			std::ostringstream errMsg;
			errMsg << "Image format " << static_cast<int>(this->format) <<
				" is not supported.";
			throw std::logic_error(errMsg.str());
		}
	}

	//template <typename T>
	//typename ImageNew<T>::SizeType ImageNew<T>::GetNumElem(void) const
	//{
	//	return this->size.width * this->size.height * this->size.depth;
	//}

	/** Resizes the std::vector<T> object only if necessary.
	If size is changed while the total number of elements are the same (reshaping),
	it does NOT run resize() function of the std::vector<T>. */
	template <typename T>
	void ImageNew<T>::resize(const Size3D<SizeType> &sz)
	{
		SizeType nElem = sz.width * sz.height * sz.depth;
		if (this->data.size() != nElem)
			this->data_.resize(nElem);
		this->size_ = sz;
	}

	template <typename T>
	void ImageNew<T>::resize(SizeType width, SizeType height, SizeType depth)
	{
		this->resize(Size3D<SizeType>(width, height, depth));
	}


	template <typename T>
	void Copy(const ImageNew<T> &imgSrc,
		const Region<typename ImageNew<T>::SizeType, typename ImageNew<T>::SizeType> &roiSrc,
		ImageNew<T> &imgDst, const Point2D<typename ImageNew<T>::SizeType> &orgnDst)
	{
		// Check image format.
		if (imgSrc.format != imgDst.format)
			throw std::runtime_error(
			"Image format of source/destination images should be identical.");

		// Check the depth of both images.
		imgSrc.CheckDepth(imgDst);
		// Check source/destination ROI.
		imgSrc.CheckRange(roiSrc);
		imgDst.CheckRange(orgnDst, roiSrc.size);

		// Copy line by line.
		switch (imgSrc.format)
		{
		case ImageFormat::BIP:
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
				auto it_dst = imgDst.GetIterator(orgnDst.x, orgnDst.y);
				auto nElemPerLineSrc = imgSrc.size.depth * imgSrc.size.width;
				auto nElemPerLineDst = imgDst.size.depth * imgDst.size.width;
				auto nElemWidth = imgSrc.size.depth * roiSrc.size.width;
				for (auto H = 0; H != roiSrc.size.height; ++H)
				{
					std::copy(it_src, it_src + nElemWidth, it_dst);
					it_src += nElemPerLineSrc;
					it_dst += nElemPerLineDst;
				}
			}
			break;
		case ImageFormat::BSQ:
			{
				auto nElemPerLineSrc = imgSrc.size.width;
				auto nElemPerLineDst = imgDst.size.width;
				auto nElemWidth = roiSrc.size.width;
				for (auto C = 0; C != imgSrc.size.depth; ++C)
				{
					auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y, C);
					auto it_dst = imgDst.GetIterator(orgnDst.x, orgnDst.y, C);
					for (auto H = 0; H != roiSrc.size.height; ++H)
					{
						std::copy(it_src, it_src + nElemWidth, it_dst);
						it_src += nElemPerLineSrc;
						it_dst += nElemPerLineDst;
					}
				}
			}
			break;
		case ImageFormat::BIL:
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
				auto it_dst = imgDst.GetIterator(orgnDst.x, orgnDst.y);
				auto nElemPerChSrc = imgSrc.size.width;
				auto nElemPerChDst = imgDst.size.width;
				auto nElemWidth = roiSrc.size.width;
				for (auto H = 0; H != roiSrc.size.height * imgSrc.size.depth; ++H)
				{
					std::copy(it_src, it_src + nElemWidth, it_dst);
					it_src += nElemPerChSrc;
					it_dst += nElemPerChDst;
				}
			}
			break;
		default:
			break;
		}
	}
}

#endif
