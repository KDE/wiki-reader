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

#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <bb/cascades/DataModel>

#include <QTimer>
#include <QStringList>
#include <QNetworkAccessManager>

using namespace bb::cascades;

class HistoryModel;
class FavModel;

class SearchModel : public DataModel
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    SearchModel();
    ~SearchModel();

    virtual int childCount(const QVariantList &indexPath);
    virtual bool hasChildren(const QVariantList &indexPath);
    virtual QString itemType(const QVariantList &indexPath);
    virtual QVariant data(const QVariantList &indexPath);

    void setLanguage(int language);
    int language() const;

    QString languageToString() const;

    void setUrl(const QString& url);
    QString url() const;
    
    void setBusy(bool state);
    bool busy() const;

    Q_INVOKABLE QString googleSearchString() const;

public Q_SLOTS:

    void setSearchString(const QString& searchString);
    void showArticle(const QString& title);
    void showArticle(int index);
    void searchGoogle(const QString& str);

Q_SIGNALS:

    void urlChanged();
    void canQuit();
    void busyChanged();
    void languageChanged();

    void fetchFinished(const QString& searchString, QStringList data);

private Q_SLOTS:

    void httpFinished();
    void httpReadyRead();
    void fetchUrl();

    void handleSearchComplete(const QString& searchString, QStringList resultList, bool cachedresults = false);

private:

    QNetworkAccessManager m_networkAccessManager;
    QNetworkReply* m_searchNetworkReply;

    QByteArray m_searchResultData;
    QTimer m_timer;

    QVariantMap* m_resultsMap;
    QStringList m_searchResults;

    QString m_searchString;
    QString m_url;
    QString m_wikiUrlPrefix;

    int m_language;
    bool m_busy;
};

#endif // SEARCHMODEL_H
