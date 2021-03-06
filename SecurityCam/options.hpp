
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

#ifndef SECURITYCAM__OPTIONS_HPP__INCLUDED
#define SECURITYCAM__OPTIONS_HPP__INCLUDED

// Qt include.
#include <QDialog>
#include <QScopedPointer>

// SecurityCam include.
#include "cfg.hpp"


namespace SecurityCam {

//
// Options
//

class OptionsPrivate;

//! Options of the application.
class Options Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	Options( const Cfg::Cfg & cfg, QWidget * parent );
	~Options();

	//! \return Cfg.
	Cfg::Cfg cfg() const;

public slots:
	//! Img difference.
	void imgDiff( qreal value );

private slots:
	//! Choose folder.
	void chooseFolder();

private:
	friend class OptionsPrivate;

	Q_DISABLE_COPY( Options )

	QScopedPointer< OptionsPrivate > d;
}; // class Options

} // namespace SecurityCam

#endif // SECURITYCAM__OPTIONS_HPP__INCLUDED
