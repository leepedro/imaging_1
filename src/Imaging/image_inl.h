#if !defined(IMAGE_INL_H)
#define IMAGE_INL_H

namespace Imaging
{
	//////////////////////////////////////////////////////////////////////////
	// Image<T> class

	//////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Image<T>::Image(void) : data(data_), size(size_),
		size_(Size3D<SizeType>(0, 0, 0)), format(ImageFormat::UNKNOWN) {}

	template <typename T>
	Image<T>::Image(const Image<T> &src) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>(),
#else				// up to VS2012
		data(data_), size(size_),
#endif
		data_(src.data) { size_ = src.size; format = src.format; }

	template <typename T>
	Image<T> &Image<T>::operator=(const Image<T> &src)
	{
		this->data_ = src.data;
		this->size_ = src.size;
		this->format = src.format;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Image<T>::Image(const Size3D<SizeType> &sz, ImageFormat fmt) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>()
#else				// up to VS2012
		data(data_), size(size_)
#endif
	{
		this->resize(sz);
		this->format = fmt;
	}

	template <typename T>
	Image<T>::Image(SizeType width, SizeType height, SizeType depth, ImageFormat fmt) :
#if _MSC_VER > 1700	// from VS2013
		Image<T>(Size3D<SizeType>(width, height, depth), fmt) {}
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
	T &Image<T>::operator()(SizeType x, SizeType y, SizeType c)
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data_.at(this->GetOffset(x, y, c));
	}

	template <typename T>
	const T &Image<T>::operator()(SizeType x, SizeType y, SizeType c) const
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data.at(this->GetOffset(x, y, c));
	}

	template <typename T>
	typename Image<T>::Iterator Image<T>::GetIterator(SizeType x, SizeType y, SizeType c)
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data_.begin() + this->GetOffset(x, y, c);
	}

	template <typename T>
	typename Image<T>::ConstIterator Image<T>::GetIterator(SizeType x, SizeType y, SizeType c) const
	{
		this->CheckRange(c);
		this->CheckRange(x, y);
		return this->data.cbegin() + this->GetOffset(x, y, c);
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods.

	template <typename T>
	void Image<T>::CheckDepth(SizeType c) const
	{
		if (this->size.depth != c)
			throw std::runtime_error("Depth is not matched.");
	}

	/** Throws an exception instead of returning false because you have to throw an
	exception at a higher level any way. */
	template <typename T>
	void Image<T>::CheckRange(SizeType c) const
	{
		if (c >= this->size.depth)
		{
			std::ostringstream errMsg;
			errMsg << "Channel c = " << c << " is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void Image<T>::CheckRange(SizeType x, SizeType y) const
	{
		if (x < 0 || x >= this->size.width || y < 0 || y >= this->size.height)
		{
			std::ostringstream errMsg;
			errMsg << "(" << x << ", " << y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void Image<T>::CheckRange(const Point2D<SizeType> &pt) const
	{
		this->CheckRange(pt.x, pt.y);
	}

	// The end points are the excluding end of an ROI, so it could be up to (width, height).
	template <typename T>
	void Image<T>::CheckRange(const Point2D<SizeType> &orgn, const Size2D<SizeType> &sz) const
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
	void Image<T>::CheckRange(const Region<SizeType, SizeType> &roi) const
	{
		this->CheckRange(roi.origin, roi.size);
	}

	template <typename T>
	void Image<T>::clear(void)
	{
		this->data_.clear();
		this->size_ = Size3D<SizeType>(0, 0, 0);
	}

	template <typename T>
	typename Image<T>::SizeType Image<T>::GetOffset(SizeType x, SizeType y, SizeType c) const
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

	/** Resizes the std::vector<T> object only if necessary.
	If size is changed while the total number of elements are the same (reshaping),
	it does NOT run resize() function of the std::vector<T>. */
	template <typename T>
	void Image<T>::resize(const Size3D<SizeType> &sz)
	{
		SizeType nElem = sz.width * sz.height * sz.depth;
		if (this->data.size() != nElem)
			this->data_.resize(nElem);
		this->size_ = sz;
	}

	template <typename T>
	void Image<T>::resize(SizeType width, SizeType height, SizeType depth)
	{
		this->resize(Size3D<SizeType>(width, height, depth));
	}


	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		Image<T> &imgDst, const Point2D<typename Image<T>::SizeType> &orgnDst)
	{
		// Check image format.
		if (imgSrc.format != imgDst.format)
			throw std::runtime_error(
			"Image format of source/destination images should be identical.");

		// Check the depth of both images.
		imgSrc.CheckDepth(imgDst.size.depth);
		const auto depth = imgSrc.size.depth;	// common for both src/dst.

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
				CopyLines<T>(it_src, depth * imgSrc.size.width, it_dst,
					depth * imgDst.size.width, depth * roiSrc.size.width, roiSrc.size.height);
			}
			break;
		case ImageFormat::BSQ:
			for (auto C = 0; C != imgSrc.size.depth; ++C)
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y, C);
				auto it_dst = imgDst.GetIterator(orgnDst.x, orgnDst.y, C);
				CopyLines<T>(it_src, imgSrc.size.width, it_dst, imgDst.size.width,
					roiSrc.size.width, roiSrc.size.height);
			}
			break;
		case ImageFormat::BIL:
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
				auto it_dst = imgDst.GetIterator(orgnDst.x, orgnDst.y);
				CopyLines<T>(it_src, imgSrc.size.width, it_dst, imgDst.size.width,
					roiSrc.size.width,  depth * roiSrc.size.height);
			}
			break;
		default:
			std::ostringstream errMsg;
			errMsg << "Image format " << static_cast<int>(imgSrc.format) <<
				" is not supported.";
			throw std::logic_error(errMsg.str());
		}
	}

	template <typename T>
	void Copy(const T *src, const Size3D<typename Image<T>::SizeType> &sz,
		::size_t bytesPerLine, Image<T> &imgDst, ImageFormat fmt)
	{		
		if (bytesPerLine == sz.width * sizeof(T))	// no zero padding
			Copy(src, sz, imgDst, fmt);
		else
		{ 	// zero padding
			// Reset destination image for given dimension.
			imgDst.resize(sz);
			imgDst.format = fmt;

			::size_t nElemWidthSrc = bytesPerLine / sizeof(T);
			switch (fmt)
			{
			case Imaging::ImageFormat::BIP:
				{
					auto it_dst = imgDst.GetIterator(0, 0);
					CopyLines<T>(src, nElemWidthSrc, it_dst,
						depth * imgDst.size.width, depth * sz.width, sz.height);
				}
				break;
			case Imaging::ImageFormat::BSQ:
				for (auto C = 0; C != sz.depth; ++C)
				{
					auto it_dst = imgDst.GetIterator(0, 0, C);
					CopyLines<T>(src + nElemWidthSrc * sz.height * C, nElemWidthSrc, it_dst,
						imgDst.size.width, sz.width, sz.height);
				}
				break;
			case Imaging::ImageFormat::BIL:
				{
					auto it_dst = imgDst.GetIterator(0, 0);
					CopyLines<T>(src, nElemWidthSrc, it_dst, imgDst.size.width,
						sz.width, depth * sz.height);
				}
				break;
			default:
				std::ostringstream errMsg;
				errMsg << "Image format " << static_cast<int>(imgSrc.format) <<
					" is not supported.";
				throw std::logic_error(errMsg.str());
			}
		}
	}

	template <typename T>
	void Copy(const T *src, const Size3D<typename Image<T>::SizeType> &sz,
		Image<T> &imgDst, ImageFormat fmt)
	{
		// Reset destination image for given dimension.
		imgDst.resize(sz);
		imgDst.format = fmt;
		
		auto it_dst = imgDst.GetIterator(0, 0);
		std::copy(src, src + sz.depth * sz.width * sz.height, it_dst);
	}

	template <typename T>
	void Copy(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		T *dst)
	{
		// Check source/destination ROI.
		imgSrc.CheckRange(roiSrc);

		// Copy line by line.
		switch (imgSrc.format)
		{
		case ImageFormat::BIP:
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
				CopyLines<T>(it_src, imgSrc.size.depth * imgSrc.size.width, dst,
					imgSrc.size.depth * roiSrc.size.width,
					imgSrc.size.depth * roiSrc.size.width, roiSrc.size.height);
			}
			break;
		case ImageFormat::BSQ:
			for (auto C = 0; C != imgSrc.size.depth; ++C)
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y, C);
				CopyLines<T>(it_src, imgSrc.size.width, dst + roiSrc.size.width * roiSrc.size.height * C,
					roiSrc.size.width, roiSrc.size.width, roiSrc.size.height);
			}
			break;
		case ImageFormat::BIL:
			{
				auto it_src = imgSrc.GetIterator(roiSrc.origin.x, roiSrc.origin.y);
				CopyLines<T>(it_src, imgSrc.size.width, dst, roiSrc.size.width,
					roiSrc.size.width,  imgSrc.size.depth * roiSrc.size.height);
			}
			break;
		default:
			std::ostringstream errMsg;
			errMsg << "Image format " << static_cast<int>(imgSrc.format) <<
				" is not supported.";
			throw std::logic_error(errMsg.str());
		}

	}
}

#endif
