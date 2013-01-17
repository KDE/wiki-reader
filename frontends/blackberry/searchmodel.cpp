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

#include "searchmodel.h"

#include <QDesktopServices>

#include <QDomDocument>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QUrl>
#include <QFile>
#include <QDebug>

SearchModel::SearchModel()
    : m_searchNetworkReply(0)
    , m_resultsMap(new QVariantMap())
    , m_searchString("")
    , m_url("")
    , m_errorString("")
    , m_wikiUrlPrefix("http://en.")
    , m_language(0)
    , m_busy(false)
{
    m_timer.setSingleShot(true);
    m_timer.setInterval(1000);

    // QHash<int, QByteArray> roles;
    // roles[0] = "title";

    // setRoleNames(roles);
    // roles.clear();

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

    connect(&m_timer, SIGNAL(timeout()), SLOT(fetchUrl()));

    connect(this, SIGNAL(fetchFinished(const QString&, QStringList)),
            SLOT(handleSearchComplete(const QString&, QStringList)));
}

SearchModel::~SearchModel()
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

int SearchModel::childCount(const QVariantList &indexPath)
{
    if (indexPath.isEmpty()) {
        return m_searchResults.count();
    }

    return 0;
}

bool SearchModel::hasChildren(const QVariantList &indexPath)
{
    if (indexPath.empty()) {
        return true;
    }

    return false;
}

QString SearchModel::itemType(const QVariantList &indexPath)
{
    return "title";
}

QVariant SearchModel::data(const QVariantList &indexPath)
{
    return QVariant(m_searchResults.at(indexPath.first().toInt()));
}

void SearchModel::setSearchString(const QString& searchString)
{
    m_searchString = searchString.trimmed();

    //first see if we have any cached results.
    QMap<QString, QVariant>::const_iterator iter = m_resultsMap->find(m_searchString);
    if (iter != m_resultsMap->end())
    {
        const QVariant variant = iter.value();
        handleSearchComplete(iter.key(), variant.toStringList(), true);
    }

    if (m_searchString.isEmpty())
    {
        m_searchResults.clear();
        setBusy(false);
    } else {
        setBusy(true);
    }

    m_timer.start();
}

void SearchModel::handleSearchComplete(const QString& searchString, QStringList list, bool cachedResults)
{
    m_searchResults.clear();
    if (!list.count() == 1 || !list.first().isEmpty()) {
        m_searchResults = list;
    }

    if (!m_searchString.isEmpty()) {
        m_searchResults.append(QString("Search on google.com").toAscii());
    }

    setBusy(false);

    if (!cachedResults) {
        if (list.count() == 1 && list.first().isEmpty()) {
            m_resultsMap->insert(searchString, QStringList());
        } else {
            m_resultsMap->insert(searchString, list);
        }
    }

    emit itemsChanged(DataModelChangeType::AddRemove, QSharedPointer<DataModel::IndexMapper>());
}

void SearchModel::showArticle(const QString& title)
{
    // Convert title to url and set it
    
    QString urlString = title;
    urlString = m_wikiUrlPrefix + "m.wikipedia.org/wiki/" + urlString.replace(' ', '_');

    setUrl(urlString);
}

void SearchModel::shareArticle(const QString& url)
{
    QString urlString = url;
    urlString = "mailto:?subject=Check this article&body=I found an interesting article. Check it out.\n "+ urlString;

    QDesktopServices::openUrl(QUrl(urlString));
}

void SearchModel::searchGoogle(const QString& string)
{
    QString trimmedString = string.trimmed();

    if (trimmedString.isEmpty())
        return;

    QByteArray percentEncodingByteArray = QUrl::toPercentEncoding(trimmedString, " ");

    QString urlString = "http://www.google.com/m?q=" + percentEncodingByteArray.replace(' ', '+');
    QDesktopServices::openUrl(QUrl(urlString));
}

QString SearchModel::languageToString() const
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

int SearchModel::language() const
{
    return m_language;
}   

void SearchModel::setLanguage(int language)
{
    m_language = language;
    m_wikiUrlPrefix = languageToString();

    //clear the history so that previous language results are not stored.
    if (m_resultsMap)
        m_resultsMap->clear();

    emit languageChanged();
}   


QString SearchModel::url() const
{
    return m_url;
}   

void SearchModel::setUrl(const QString& url)
{   
    m_url = url;
    emit urlChanged();
}

bool SearchModel::busy() const
{
    return m_busy;
}   

void SearchModel::setBusy(bool state)
{
    m_busy = state;
    emit busyChanged();
}

void SearchModel::setErrorString(const QString& errorString)
{
    m_errorString = errorString;
    emit errorChanged();
}   

QString SearchModel::errorString() const
{
    return m_errorString;
}

void SearchModel::fetchUrl()
{
    if (m_searchNetworkReply != NULL) {
        // Search in progress so do not open request a new one
        m_searchNetworkReply->deleteLater();
        m_searchNetworkReply = 0;
    }   

    if (m_searchString.isEmpty())
        return;

    QString searchString = m_wikiUrlPrefix + "wikipedia.org/w/api.php?action=query&list=search&format=xml&srwhat=text&srlimit=20&srsearch=";
    searchString.append(QUrl::toPercentEncoding(m_searchString, "/?&=:+"));

    // QNetworkConfigurationManager m;
    //  qDebug() << "Online:" << m.isOnline();

    m_searchNetworkReply = m_networkAccessManager.get(QNetworkRequest(QUrl(searchString)));

    connect(m_searchNetworkReply, SIGNAL(finished()), SLOT(httpFinished()));
    connect(m_searchNetworkReply, SIGNAL(readyRead()), SLOT(httpReadyRead()));
}

void SearchModel::httpReadyRead()
{
    if (m_searchString.isEmpty())
        return;

    m_searchResultData += m_searchNetworkReply->readAll();
}

void SearchModel::httpFinished()
{
    Q_ASSERT(m_searchNetworkReply);

    if (m_searchString.isEmpty()) {
        emit fetchFinished(m_searchString, QStringList());
        m_searchNetworkReply->deleteLater();
        m_searchNetworkReply = 0;
        return;
    }

    int searchNetworkReplyError = m_searchNetworkReply->error();

    if (searchNetworkReplyError != QNetworkReply::NoError) {
        m_searchNetworkReply->deleteLater();
        m_searchNetworkReply = 0;
        m_searchResultData.clear();

        if (searchNetworkReplyError == QNetworkReply::OperationCanceledError) {
            m_timer.start();
            return;
        } else {
            setErrorString("Internet not accessible.\nTry Again Later.");
            setBusy(false);
            return;
        }
    }

    QStringList final;

    QDomDocument domDocument;
    if (domDocument.setContent(m_searchResultData, true)) {
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

    m_searchResultData.clear();

    m_searchNetworkReply->deleteLater();
    m_searchNetworkReply = 0;

    emit fetchFinished(m_searchString, final);

    final.clear();
}

#include <searchmodel.moc>
