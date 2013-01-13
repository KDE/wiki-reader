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

#include "wikimodel.h"

#include <QDesktopServices>

#include <QDomDocument>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QUrl>
#include <QFile>
#include <QDebug>

WikiModel::WikiModel()
    : m_searchNetworkReply(0)
    , m_searchString("")
    , m_url("")
    , m_busy(false)
    , m_resultsMap(new QVariantMap())
    , m_errorString("")
    , m_language(0)
    , m_wikiUrlPrefix("http://en.")
{
    m_timer.setSingleShot(true);
    m_timer.setInterval(1000);

    QHash<int, QByteArray> roles;
    roles[0] = "title";

    setRoleNames(roles);
    roles.clear();

    QString fi("wikireader.conf");
    QFile file(fi.toLower());

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        int configfileversion = 0;
        stream >> configfileversion; //version of config file

        int lang = 0;
        stream >> lang;
        setLanguage(lang);
    }

    file.close();

    connect(&m_timer, SIGNAL(timeout()), SLOT(timerTimeout()));

    connect(this, SIGNAL(operationComplete(const QString&, QStringList, int)),
            SLOT(handleSearchComplete(const QString&, QStringList, int)));
}

WikiModel::~WikiModel()
{
    QFile file("wikireader.conf");
    if (file.open( QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream << 1; //config file version
        stream << m_language;
    } else {
        qDebug() << "Could not open the file for writing";
    }

    file.close();

    m_timer.stop();

    if (m_searchNetworkReply) {
        delete m_searchNetworkReply;
        m_searchNetworkReply = 0;
    }

    m_searchResults.clear();

    if (m_resultsMap) {
        m_resultsMap->clear();
        delete m_resultsMap;
        m_resultsMap = 0;
    }
}

int WikiModel::rowCount(const QModelIndex& index) const
{
    return m_searchResults.count();
}

QVariant WikiModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == 0) {
        return QVariant(m_searchResults[index.row()]);
    } else if (role == 2) {
    }
    else {
        return QVariant();
    }
}

int WikiModel::setSearchString(const QString& searchString)
{
    m_searchString = searchString.trimmed();

    //first see if we have any cached results.
    QMap<QString, QVariant>::const_iterator iter = m_resultsMap->find(m_searchString);
    if (iter != m_resultsMap->end())
    {
        const QVariant variant = iter.value();
        handleSearchComplete(iter.key(), variant.toStringList(), 0, true);
        return 0;
    }

    if (m_searchString.isEmpty())
    {
        emit beginResetModel();
        m_searchResults.clear();
        setBusy(false);
        emit endResetModel();
    } else {
        setBusy(true);
    }

    m_timer.start();
}

void WikiModel::handleSearchComplete(const QString& searchString, QStringList list, int networkReplyError, bool cachedResults)
{
    if (networkReplyError != QNetworkReply::NoError)
    {
        setErrorString("Internet not accessible.\nTry Again Later.");
        setBusy(false);
        return;
    } else {    
        if (QString::compare(searchString, m_searchString, Qt::CaseInsensitive) == 0) {
            emit beginResetModel();
            m_searchResults.clear();
            if (!list.count() == 1 || !list.at(0).size() != 0) {
                m_searchResults = list;
            }

            if (!m_searchString.isEmpty()) {
                m_searchResults.append(QString("Search in google.com").toAscii());
            }

            emit endResetModel();
        }
    }

    setBusy(false);

    if (!cachedResults) {
        if (list.count() == 1 && list.at(0).isEmpty()) {
            m_resultsMap->insert(searchString, QStringList());
        } else {
            m_resultsMap->insert(searchString, list);
        }
    }
}

void WikiModel::showArticle(const QString& title)
{
    // Convert title to url and set ita
    
    QString urlString = title;
    urlString = m_wikiUrlPrefix + "m.wikipedia.org/wiki/" + urlString.replace(' ', '_');
    setUrl(urlString);
}

void WikiModel::shareArticle(const QString& url)
{
    QString urlString = url;
    urlString = "mailto:?subject=Check this article&body=I found an interesting article. Check it out.\n "+ urlString;

    QDesktopServices::openUrl(QUrl(urlString));
}

void WikiModel::searchGoogle(const QString& string)
{
    QString trimmedString = string.trimmed();

    if (trimmedString.isEmpty())
        return;

    QByteArray percentEncodingByteArray = QUrl::toPercentEncoding(trimmedString, " ");

    QString urlString = "http://www.google.com/m?q=" + percentEncodingByteArray.replace(' ', '+');
    QDesktopServices::openUrl(QUrl(urlString));
}

QString WikiModel::languageToString() const
{
    QString languageString = "http://";
    switch (m_language)
    {
    case 0:
        languageString.append("en.");
        break;
    case 1:
        languageString.append("pa.");
        break;
    case 2:
        languageString.append("cs.");
        break;
    case 3:
        languageString.append("da.");
        break;
    case 4:
        languageString.append("de.");
        break;
    case 5:
        languageString.append("es.");
        break;
    case 6:
        languageString.append("fr.");
        break;
    case 7:
        languageString.append("it.");
        break;
    case 8:
        languageString.append("hu.");
        break;
    case 9:
        languageString.append("nl.");
        break;
    case 10:
        languageString.append("no.");
        break;
    case 11:
        languageString.append("pt.");
        break;
    case 12:
        languageString.append("pl.");
        break;
    case 13:
        languageString.append("fi.");
        break;
    case 14:
        languageString.append("sv.");
        break;
    case 15:
        languageString.append("ru.");
        break;
    default:
        languageString.append("en.");
        break;
    }

    return languageString;
}

int WikiModel::language() const
{
    return m_language;
}   

void WikiModel::setLanguage(int language)
{
    m_language = language;
    m_wikiUrlPrefix = languageToString();

    //clear the history so that previous language results are not stored.
    if (m_resultsMap)
        m_resultsMap->clear();

    emit languageChanged();
}   


QString WikiModel::url() const
{
    return m_url;
}   

void WikiModel::setUrl(const QString& url)
{   
    m_url = url;
    emit urlChanged();
}

bool WikiModel::busy() const
{
    return m_busy;
}   

void WikiModel::setBusy(bool state)
{
    m_busy = state;
    emit busyChanged();
}

void WikiModel::setErrorString(const QString& errorString)
{
    m_errorString = errorString;
    emit errorChanged();
}   

QString WikiModel::errorString() const
{
    return m_errorString;
}

void WikiModel::timerTimeout()
{
    if (m_searchNetworkReply != NULL) {
        // Some search is already in progress
        m_searchNetworkReply->deleteLater();
        m_searchNetworkReply = 0;
    }   

    if (m_searchString.isEmpty())
        return;

    QString searchString = m_wikiUrlPrefix + "wikipedia.org/w/api.php?action=query&list=search&format=xml&srwhat=text&srlimit=20&srsearch=";

    searchString.append(QUrl::toPercentEncoding(m_searchString, "/?&=:+"));

    m_searchNetworkReply = m_networkAccessManager.get(QNetworkRequest(QUrl(searchString)));

    connect(m_searchNetworkReply, SIGNAL(finished()), SLOT(httpFinished()));
    connect(m_searchNetworkReply, SIGNAL(readyRead()), SLOT(httpReadyRead()));
}

void WikiModel::httpReadyRead()
{
    if (m_searchString.isEmpty())
        return;

    m_xmlResult += m_searchNetworkReply->readAll();
}

void WikiModel::httpFinished()
{
    Q_ASSERT(m_searchNetworkReply);

    if (m_searchString.isEmpty()) {
        emit operationComplete(m_searchString, QStringList(), 0);
        //does controler every come here. incase it comes emit operation complete, release resources and quit.
        m_searchNetworkReply->deleteLater();
        m_searchNetworkReply = 0;
        return;
    }

    int searchNetworkReplyError = m_searchNetworkReply->error();

    if (searchNetworkReplyError != QNetworkReply::NoError) {
        m_searchNetworkReply->deleteLater();
        m_searchNetworkReply = 0;
        m_xmlResult.clear();

        if (searchNetworkReplyError == QNetworkReply::OperationCanceledError) {
            m_timer.start();
            return;
        } else {
            emit operationComplete(m_searchString, QStringList(), searchNetworkReplyError);
            return;
        }
    }

    QStringList final;

    QDomDocument domDocument;
    if (domDocument.setContent(m_xmlResult, true)) {
        QDomNodeList list = domDocument.elementsByTagName("p");

        for (int i = 0; i < list.count(); ++i) {
            QDomNode node = list.at(i);
            if (node.isElement()) {   
                QString value = node.toElement().attribute("title");
                if (!value.isEmpty()) {
                    final.append(value);
                }   
            }   
        }   
    }

    m_xmlResult.clear();

    m_searchNetworkReply->deleteLater();
    m_searchNetworkReply = 0;

    emit operationComplete(m_searchString, final, 0);

    final.clear();
}

#include <wikimodel.moc>
