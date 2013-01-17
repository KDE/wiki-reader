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

import bb.cascades 1.0

Page {

    function loadUrl()
    {
        wikiWebView.url = searchModel.url
        wikiScrollView.visible = true
        searchBar.visible = false
    }

    actions: [
        // The page should be refactored later into two pages
        // 1) WebView + potentially the Action Bar
        // 2) SearchBar + ListView

        ActionItem {
            id: backActionItem
            title: "Back"
            imageSource: "asset:///previous.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                wikiWebView.goBack();
            }
        },

        ActionItem {
            id: nextActionItem
            title: "Next"
            imageSource: "asset:///next.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                wikiWebView.goForward();
            }
        },

        ActionItem {
            id: searchActionItem
            title: "Search"
            imageSource: "asset:///search.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                searchBar.visible = true;
            }
        },

        ActionItem {
            id: languageActionItem
            // title: selectionDialog.model.get(searchModel.language).name
            title: "Languages"
            imageSource: "asset:///images/language.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                // languageSelectionDialog.open();
            }
        },
        
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

        ActionItem {
            id: copyUrlActionItem
            title: "Copy Url"
            imageSource: "asset:///copy_link.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                searchModel.shareArticle(wikiWebView.url);
            }
        },
        
        ActionItem {
            id: emailActionItem
            title: "Email"
            imageSource: "asset:///email.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                searchModel.shareArticle(wikiWebView.url);
            }
        }

        // ActionItem {
            // id: findActionItem
            // title: "Find"
            // imageSource: "asset:///find.png"
            // ActionBar.placement: ActionBarPlacement.OnBar

            // onTriggered: {
                // searchModel.shareArticle(wikiWebView.url);
            // }
        // }
    ]

    Container {
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

            horizontalAlignment: HorizontalAlignment.Right
            visible: true

            TextField {
                id: searchTextField

                hintText: "Enter a Search Term"

                onTextChanging: {
                }

                input {
                    submitKey: SubmitKey.Search;

                    onSubmitted: {
                        searchModel.setSearchString(text);
                    }
                }
            }

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
                    // searchModel.language = selectedIndex
                    // langicon.text = selectionDialog.model.get(searchModel.p_language).name
                    // searchbar.focus = true;
                // }
            // }

            // onVisibleChanged: {
                // if(visible)
                    // searchbar.focus = true;
            // }
        }

        ListView {
            id: resultsListView

            dataModel: searchModel

            listItemComponents: [

                ListItemComponent {
                    type: "header"
                     
                    Label {
                        text: ListItemData.title
                    }
                },

                ListItemComponent {
                    type: "title"

                    StandardListItem {
                        title: ListItemData

                    }
                }

            ]

            onTriggered: {
                // if (title == "Search on google.com") {
                //     searchModel.searchGoogle(searchbar.text);
                // } else {
                //     searchModel.showArticle(title);
                // }
            }
                
            onSelectionChanged: {

            }
        }

        ScrollView {
            id: wikiScrollView

            verticalAlignment: VerticalAlignment.Fill
            visible: false

            WebView {
                id: wikiWebView

                url: "http://wikipedia.org"
                // z: 1

                settings.viewport: {
                    // "width" : "device-width"
                    // "height": "devide-height"
                    // defaultFontSize: searchModel.fontSize
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

            preferredWidth: 75
            preferredHeight: 75

            running: false
        }

    }
}
