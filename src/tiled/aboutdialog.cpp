/*
 * aboutdialog.cpp
 * Copyright 2008-2009, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2009, Dennis Honeyman <arcticuno@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "aboutdialog.h"

#include <QCoreApplication>

using namespace Tiled::Internal;

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setMaximumSize(432, 460);

    const QString html = QCoreApplication::translate(
            "AboutDialog",
            "<p align=\"center\"><font size=\"+2\"><b>Tiled - Sydoria Map Editor</b></font><br><i>Version %1</i></p>\n"
            "<p align=\"center\">Copyright 2008-2014 Thorbj&oslash;rn Lindeijer<br>Copyright 2014-2015 Sydoria Games<br><br>see the AUTHORS file for a full list of contributors<br>see the COPYING for license information</p>\n"
            "<p align=\"center\"><a href=\"http://www.mapeditor.org/\">http://www.mapeditor.org/</a><br><a href=\"http://www.sydoria.fr/\">http://www.sydoria.fr/</a></p>\n")
            .arg(QApplication::applicationVersion());

    textBrowser->setHtml(html);
}
