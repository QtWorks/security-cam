
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

#ifndef SECURITYCAM__FRAMES_HPP__INCLUDED
#define SECURITYCAM__FRAMES_HPP__INCLUDED

// Qt include.
#include <QAbstractVideoSurface>


namespace SecurityCam {

//! Count of processed frames when key farme changes.
static const int c_keyFrameChangesOn = 10;
//! Threshold of motion detection.
static const double c_threshold = 0.015;

//
// Frames
//

//! Frames listener.
class Frames Q_DECL_FINAL
	:	public QAbstractVideoSurface
{
	Q_OBJECT

signals:
	//! New frame arrived.
	void newFrame( QImage );
	//! Motion detected.
	void motionDetected();
	//! No more motions.
	void noMoreMotions();

public:
	explicit Frames( QObject * parent );

	bool present( const QVideoFrame & frame ) Q_DECL_OVERRIDE;

	QList< QVideoFrame::PixelFormat > supportedPixelFormats(
		QAbstractVideoBuffer::HandleType type =
			QAbstractVideoBuffer::NoHandle ) const Q_DECL_OVERRIDE;

private:
	//! Detect motion.
	void detectMotion( const QImage & key, const QImage & image );

private:
	//! Key frame.
	QImage m_keyFrame;
	//! Frames counter.
	int m_counter;
	//! Motions was detected.
	bool m_motion;
}; // class Frames

} /* namespace SecurityCam */

#endif // SECURITYCAM__FRAMES_HPP__INCLUDED
