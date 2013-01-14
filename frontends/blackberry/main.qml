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

import bb.cascades 1.0

Page {

    actions: [
        // The page should be refactored later into two pages
        // 1) WebView + potentially the Action Bar
        // 2) SearchBar + ListView

        ActionItem {
            id: backActionItem
            title: "Back"
            imageSource: "asset:///images/back.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                wikiWebView.goBack();
            }
        }

        ActionItem {
            id: nextActionItem
            title: "Next"
            imageSource: "asset:///images/next.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                wikiWebView.goForward();
            }
        }

        ActionItem {
            id: searchActionItem
            title: "Search"
            imageSource: "asset:///images/search.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                searchBar.visible = true;
            }
        }

        ActionItem {
            id: languageActionItem
            title: selectionDialog.model.get(wikiModel.language).name
            imageSource: "asset:///images/language.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                languageSelectionDialog.open();
            }
        }
        
        // This is to ponder later whether it is a common enough use case to
        // rearrange the layout. There could be a group here because "Find in
        // page" can also be a useful action. There could could be an ActionSet
        // used with context menu. That could be really awesome! So, in short:
        //
        // - Copy
        // - Language
        // - Find in
        // - Download the content to read offline?
        // - Send in email
        // - Go home?
        //
        // Unfortunately, the "Cancel" has to stay at the top because the search
        // cannot be dismissed otherwise from the Action Bar for instance as the
        // Virtual Keyboard may well cover that part intentionally for good.

        // ActionItem {
            // id: copyUrlActionItem
            // title: "Copy Url"
            // imageSource: "asset:///images/copy.png"
            // ActionBar.placement: ActionBarPlacement.OnBar

            // onTriggered: {
                // wikiModel.shareArticle(webitem.url);
            // }
        // }
    ]

    content:Container {
        id: rootContainer
    
        topPadding: 20.0
        background: Color.Black

        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }

        onCreationCompleted: {
            OrientationSupport.supportedDisplayOrientation =
                SupportedDisplayOrientation.DisplayPortrait;
        }

        Container {
            id: searchBar

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            HorizontalAlignment: HorizontalAlignment::Right
            visible: false

            TextField {
                id: searchTextField

                hintText: "Search the text"

                onTextChanging: {
                    wikiModel.setSearchString(text);
                }
            }

            // Probably, the activity indicator should be used for this progress
            // as well instead of too much customization. There is
            // ProgressDialog for instance inside Cascades.

            // ImageView {
                id: busyicon
                // anchors.right: searchbar.right
                // anchors.rightMargin: 8
                // anchors.verticalCenter: searchbar.verticalCenter
                // imageSource: "asset:///images/progress.png"
                // width: 30
                // height: 30
                // visible: wikiModel.busy ? true : false
                // PropertyAnimation on rotation { to: 360; duration: 1000; loops: Animation.Infinite}
            // }

            Button {
                id: cancelButton

                text: "Cancel"

                onClicked: {
                    searchBar.visible = false;
                }
            }

            // No language changing support for now, but it is already possible
            // to type the search word in any language. This could now be done
            // with a language ListView on a new page for instance or in a
            // dialog, or even on the same page as a workaround.

            // SelectionDialog {
                // id: language selectionDialog
                // titleText: "Select your language"
                // model: ListModel {
                    // ListElement { name: "English";  value: "0";}
                    // ListElement { name: "Catala";   value: "1";}
                    // ListElement { name: "Cesky";    value: "2";}
                    // ListElement { name: "Dansk";    value: "3";}
                    // ListElement { name: "Deutsch";  value: "4";}
                    // ListElement { name: "Espanol";  value: "5";}
                    // ListElement { name: "Francais"; value: "6";}
                    // ListElement { name: "Italiano"; value: "7";}
                    // ListElement { name: "Magyar";   value: "8";}
                    // ListElement { name: "Nederlands"; value: "9";}
                    // ListElement { name: "Norsk";    value: "10";}
                    // ListElement { name: "Portugues";value: "11";}
                    // ListElement { name: "Polski";   value: "12";}
                    // ListElement { name: "Suomi";    value: "13";}
                    // ListElement { name: "Svenska";  value: "14";}
                    // ListElement { name: "Russian";  value: "15";}
                // }

                // onRejected: {
                    // searchbar.focus = true;
                // }

                // onAccepted: {
                    // wikiModel.language = selectedIndex
                    // langicon.text = selectionDialog.model.get(wikiModel.p_language).name
                    // searchbar.focus = true;
                // }
            // }

            // onVisibleChanged: {
                // if(visible)
                    // searchbar.focus = true;
            // }
        }

        ListView {
            id: completionListView

            rootIndexPath: [1]
            dataModel: wikiModel
            ListItemComponents: [
                ListItemComponent {
                    type: "item"

                    Label {
                        text: ListItem.data

                        onClicked: {
                            if (title == "Search on google.com") {
                                wikiModel.searchGoogle(searchbar.text);
                            } else {
                                wikiModel.showArticle(title);
                            }
                        }
                    }
                
            ]

            onSelectionChanged: {

            }
        }

        ScrollView {
            id: wikiScrollView

            verticalAlignment: VerticalAlignment::Fill
            visible: true

            WebView {
                id: wikiWebView

                url: "http://wikipedia.org"
                // z: 1

                settings.viewport: {
                    // "width" : "device-width"
                    // "height": "devide-height"
                    // defaultFontSize: wikiModel.fontSize
                    "initial-scale" : 1.0
                }
                
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Started) {
                        activityIndicator.start();
                    }
                    else if (loadRequest.status == WebLoadStatus.Succeeded) {
                        activityIndicator.stop();
                    }
                    else if (loadRequest.status == WebLoadStatus.Failed) {
                        activityIndicator.stop();
                    }
                }
            }

            scrollViewProperties {
                scrollMode: ScrollMode.Both
            }
        }

        ActivityIndicator {
            id: activityIndicator

            anchors.centerIn: parent

            preferredWidth: 75
            preferredHeight: 75

            running: false
        }

    }
}

function loadUrl()
{
    wikiWebView.url = wikiModel.url
    wikiScrollView.visible = true
    searchBar.visible = false
}
