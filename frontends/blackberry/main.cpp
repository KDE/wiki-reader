/******************************************************************************
 * This file is part of the WikiReader project
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

#include "wikimodel.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    Application app(argc, argv);

    QTranslator translator;
    QString localeString = QLocale().name();
    QString filename = QString("WikiReader_%1").arg(localeString);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    WikiModel wikiModel;

    QmlDocument *qmlDocument = QmlDocument::create("asset:///main.qml").parent(&app);
    qmlDocument->setContextProperty("wikiModel", &wikiModel);

    AbstractPane *root= qmlDocument->createRootObject<AbstractPane>();

    app.setScene(root);

    // QObject::connect(&wikiModel, SIGNAL(urlChanged()),(QObject*) view.rootObject(), SLOT(loadUrl()));

    return app.exec();
}
