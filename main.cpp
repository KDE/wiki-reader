/******************************************************************************
 * This file is part of the QWiki project
 * Copyright (c) 2013 Laszlo Papp <lpapp@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QtCore>

#include "wikimodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect clientRect = desktopWidget->screenGeometry();
    QDeclarativeView view;
    WikiModel wikiModel;

    // QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);

    view.rootContext()->setContextProperty("appModel", &wikiModel);
    view.setSource(QUrl("qrc:/ui.qml"));
    view.setSceneRect(clientRect);
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.showFullScreen();

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    QObject::connect(&wikiModel, SIGNAL(urlChanged()),(QObject*) view.rootObject(), SLOT(loadUrl()));
    view.show();

    return app.exec();
}
