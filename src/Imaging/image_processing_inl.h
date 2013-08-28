#if !defined(IMAGE_PROCESSING_INL_H)
#define IMAGE_PROCESSING_INL_H

namespace Imaging
{
	template <typename T>
	int GetOpenCvType(::size_t depth)
	{
		if (typeid(T) == typeid(unsigned char))
			return SafeCast_<int>(CV_MAKETYPE(CV_8U, depth));
		else if (typeid(T) == typeid(signed char))
			return SafeCast_<int>(CV_MAKETYPE(CV_8S, depth));
		else if (typeid(T) == typeid(unsigned short))
			return SafeCast_<int>(CV_MAKETYPE(CV_16U, depth));
		else if (typeid(T) == typeid(short))
			return SafeCast_<int>(CV_MAKETYPE(CV_16S, depth));
		else if (typeid(T) == typeid(int))
			return SafeCast_<int>(CV_MAKETYPE(CV_32S, depth));
		else if (typeid(T) == typeid(float))
			return SafeCast_<int>(CV_MAKETYPE(CV_32F, depth));
		else if (typeid(T) == typeid(double))
			return SafeCast_<int>(CV_MAKETYPE(CV_64F, depth));
		else
			throw std::invalid_argument("Unsupported type.");
	}

	/** The internal interpolation logic calls an OpenCV function, and this function creates
	a temporary OpenCV Mat objects from existing images without memory allocation. */
	template <typename T>
	void Resize(Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		const Point2D<double> &zm, Image<T> &imgDst, Interpolation interp)
	{
		if (roiSrc.size.width == imgSrc.size.width && roiSrc.size.height == imgSrc.size.height)
		//if (roiSrc.size == imgSrc.size)
		//if (roiSrc.Fit(imgSrc.size.width, imgSrc.size.height))
			Resize(imgSrc, zm, imgDst, interp);
		else
		{
			// Resize destination image.
			Region<typename Image<T>::SizeType, typename Image<T>::SizeType> roiDst =
				roiSrc * zm;
			imgDst.resize(roiDst.size.width, roiDst.size.height, imgSrc.size.depth);
			imgDst.format = imgSrc.format;

			// Create a temporary image for given source ROI to create OpenCV Mat objects.
			Image<T> imgTemp;
			Copy(imgSrc, roiSrc, imgTemp);

			// Prepare temporary OpenCV Mat objects without memory allocation.
			cv::Mat cvSrc(SafeCast_<int>(imgTemp.size.height), SafeCast_<int>(imgTemp.size.width),
				GetOpenCvType<T>(imgTemp.size.depth), imgTemp.GetPointer(0, 0));
			cv::Mat cvDst(SafeCast_<int>(imgDst.size.height), SafeCast_<int>(imgDst.size.width),
				GetOpenCvType<T>(imgDst.size.depth), imgDst.GetPointer(0, 0));

			cv::resize(cvSrc, cvDst, cvDst.size(), zm.x, zm.y, static_cast<int>(interp));
		}
	}

	/** The internal interpolation logic calls an OpenCV function, and this function creates
	a temporary OpenCV Mat objects from existing images without memory allocation. */
	template <typename T>
	void Resize(Image<T> &imgSrc,const Point2D<double> &zm, Image<T> &imgDst,
		Interpolation interp)
	{
		// Resize destination image.
		Region<typename Image<T>::SizeType, typename Image<T>::SizeType> roiDst =
			imgSrc.GetRoi() * zm;
		imgDst.resize(roiDst.size.width, roiDst.size.height, imgSrc.size.depth);
		imgDst.format = imgSrc.format;

		// Prepare temporary OpenCV Mat objects without memory allocation.
		cv::Mat cvDst(SafeCast_<int>(imgDst.size.height), SafeCast_<int>(imgDst.size.width),
			GetOpenCvType<T>(imgDst.size.depth), imgDst.GetPointer(0, 0));
		cv::Mat cvSrc(SafeCast_<int>(imgSrc.size.height), SafeCast_<int>(imgSrc.size.width),
			GetOpenCvType<T>(imgSrc.size.depth), imgSrc.GetPointer(0, 0));

		cv::resize(cvSrc, cvDst, cvDst.size(), zm.x, zm.y, static_cast<int>(interp));
	}

}
#endif
