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

#include <QTimer>
#include <QAbstractListModel>
#include <QStringList>
#include <QNetworkAccessManager>

class HistoryModel;
class FavModel;

class SearchModel: public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorChanged)
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    SearchModel();
    ~SearchModel();

    virtual int rowCount(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role) const;

    void setLanguage(int language);
    int language() const;

    QString languageToString() const;

    void setUrl(const QString& url);
    QString url() const;
    
    void setBusy(bool state);
    bool busy() const;

    void setErrorString(const QString& errorString);
    QString errorString() const;

public Q_SLOTS:

    void setSearchString(const QString& searchString);
    void showArticle(const QString& title);
    void searchGoogle(const QString& str);
    void shareArticle(const QString& url);

Q_SIGNALS:

    void urlChanged();
    void canQuit();
    void busyChanged();
    void errorChanged();
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
    QString m_errorString;
    QString m_wikiUrlPrefix;

    int m_language;
    bool m_busy;
};

#endif // SEARCHMODEL_H
