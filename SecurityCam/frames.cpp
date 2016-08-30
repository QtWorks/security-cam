
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2016 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// SecurityCam include.
#include "frames.hpp"

// OpenCV include.
#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>


namespace SecurityCam {

//
// Frames
//

Frames::Frames( QObject * parent )
	:	QAbstractVideoSurface( parent )
	,	m_counter( 0 )
	,	m_motion( false )
{
}

bool
Frames::present( const QVideoFrame & frame )
{
	if( !isActive() )
		return false;

	QVideoFrame f = frame;
	f.map( QAbstractVideoBuffer::ReadOnly );

	QImage image( f.bits(), f.width(), f.height(), f.bytesPerLine(),
		QVideoFrame::imageFormatFromPixelFormat( f.pixelFormat() ) );

	f.unmap();

	if( m_counter == c_keyFrameChangesOn )
		m_counter = 0;

	if( m_counter == 0 )
	{
		if( !m_keyFrame.isNull() )
			detectMotion( m_keyFrame, image );

		m_keyFrame = image.copy( image.rect() );

		emit newFrame( m_keyFrame );
	}
	else
		emit newFrame( image.copy( image.rect() ) );

	++m_counter;

	return true;
}

inline cv::Mat QImageToCvMat( const QImage & inImage )
{
	switch ( inImage.format() )
	{
		// 8-bit, 4 channel
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
		{
			cv::Mat mat( inImage.height(), inImage.width(),
				CV_8UC4,
				const_cast< uchar* >( inImage.bits() ),
				static_cast< size_t >( inImage.bytesPerLine() ) );

			return mat;
		}

		// 8-bit, 3 channel
		case QImage::Format_RGB32:
		case QImage::Format_RGB888:
		{
			QImage swapped;

			if( inImage.format() == QImage::Format_RGB32 )
				swapped = inImage.convertToFormat( QImage::Format_RGB888 );

			swapped = inImage.rgbSwapped();

			return cv::Mat( swapped.height(), swapped.width(),
				CV_8UC3,
				const_cast< uchar* >( swapped.bits() ),
				static_cast< size_t >( swapped.bytesPerLine() ) ).clone();
		}

		// 8-bit, 1 channel
		case QImage::Format_Indexed8:
		{
			cv::Mat mat( inImage.height(), inImage.width(),
				CV_8UC1,
				const_cast< uchar* >( inImage.bits() ),
				static_cast< size_t >( inImage.bytesPerLine() ) );

			return mat;
		}

		default:
			break;
	}

	return cv::Mat();
}

void
Frames::detectMotion( const QImage & key, const QImage & image )
{
	bool detected = false;

	try {
		const cv::Mat A = QImageToCvMat( key );
		const cv::Mat B = QImageToCvMat( image );

		// Calculate the L2 relative error between images.
		const double errorL2 = norm( A, B, CV_L2 );
		// Convert to a reasonable scale, since L2 error is summed across
		// all pixels of the image.
		const double similarity = errorL2 / (double)( A.rows * A.cols );

		detected = similarity > c_threshold;
	}
	catch( const cv::Exception & )
	{
	}

	if( m_motion && !detected )
	{
		m_motion = false;

		emit noMoreMotions();
	}
	else if( !m_motion && detected )
	{
		m_motion = true;

		emit motionDetected();
	}
}

QList< QVideoFrame::PixelFormat >
Frames::supportedPixelFormats( QAbstractVideoBuffer::HandleType type ) const
{
	Q_UNUSED( type )

	return QList< QVideoFrame::PixelFormat > ()
		<< QVideoFrame::Format_ARGB32
		<< QVideoFrame::Format_ARGB32_Premultiplied
		<< QVideoFrame::Format_RGB32
		<< QVideoFrame::Format_RGB24
		<< QVideoFrame::Format_RGB565
		<< QVideoFrame::Format_RGB555
		<< QVideoFrame::Format_ARGB8565_Premultiplied;
}

} /* namespace SecurityCam */
