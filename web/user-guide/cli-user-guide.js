/*
    Copyright (c) 2006-2013, Alexis Royer, http://alexis.royer.free.fr/CLI

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
          and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
          without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


//! @brief Dynamic menu for the user-guide.
function UserGuideMenu() {

    var userGuideMenu = this;

    //! Generic dynamic menu reference.
    this.menu = null;
    //! Menu item receiving the table of content.
    this.toc = null;
    //! Menu item receiving example references.
    this.examples = null;
    //! Menu item receiving figure references.
    this.figures = null;
    //! Menu item receiving table references.
    this.tables = null;
    //! Manu item receiving document changes.
    this.changes = null;

    //! @brief Create main menu items
    this.create = function() {
        try {
            trace.profile("UserGuideMenu.create()");

            userGuideMenu.menu = new DynamicMenu("nav");

            // The main items of the menu are created right away, but sub-items are populated on mouseover event.
            userGuideMenu.toc = userGuideMenu.menu.createItem(null, "Table of content", "#");
            userGuideMenu.toc.setId("navtoc");
            userGuideMenu.toc.setColor("#ff0000"); // red
            userGuideMenu.populateToc();

            userGuideMenu.examples = userGuideMenu.menu.createItem(null, "Examples", "#");
            userGuideMenu.examples.setId("navexamples");
            userGuideMenu.examples.setColor("#006600"); // green
            userGuideMenu.populateExamples();

            userGuideMenu.figures = userGuideMenu.menu.createItem(null, "Figures", "#");
            userGuideMenu.figures.setId("navfigures");
            userGuideMenu.figures.setColor("#000066"); // blue
            userGuideMenu.populateFigures();

            userGuideMenu.tables = userGuideMenu.menu.createItem(null, "Tables", "#");
            userGuideMenu.tables.setId("navtables");
            userGuideMenu.tables.setColor("#ff8800"); // orange
            userGuideMenu.populateTables();

            userGuideMenu.changes = userGuideMenu.menu.createItem(null, "Changes", "#");
            userGuideMenu.changes.setId("navchanges");
            userGuideMenu.changes.setColor("#880088"); // purple
            userGuideMenu.populateChanges();

            trace.profile("UserGuideMenu.create() done");
        } catch (err) {
            trace.error("UserGuideMenu.create(): " + err); throw err;
        }
    }

    //! @brief Create TOC menu items.
    this.populateToc = function() {
        try {
            trace.profile("UserGuideMenu.populateToc()");
            var ar_TocItems = new Array(userGuideMenu.toc);
            $(".titlepage").each(function() {
                try {
                    var p_Node = new XmlNode(this);
                    var str_Title = p_Node.child("div").child("div").child(null).fullText();
                    var str_Anchor = p_Node.child("div").child("div").child(null).child("a").attribute("name");
                    var i_HeaderLevel = p_Node.child("div").child("div").child(null).name().substring(1);
                    if (i_HeaderLevel >= 2) {
                        var p_NewItem = userGuideMenu.menu.createItem(ar_TocItems[i_HeaderLevel - 2], str_Title, "#" + str_Anchor);
                        if (/Frequently Asked Questions/.exec(str_Title)) {
                            p_NewItem.setId("faq-menu");
                            p_NewItem.setChildWidth("600");
                        }
                        ar_TocItems[i_HeaderLevel - 1] = p_NewItem;
                    }
                } catch (err) {
                    trace.warn("UserGuideMenu.populateToc(): title processing error: this = " + this + ", err = " + err);
                }
            });
            trace.profile("UserGuideMenu.populateToc() done");
        } catch (err) {
            trace.error("UserGuideMenu.populateToc(): " + err); throw err;
        }
    }

    //! @brief Create examples menu items.
    this.populateExamples = function() {
        try {
            trace.profile("UserGuideMenu.populateExamples()");
            $(".example").each(function() {
                try {
                    var p_Node = new XmlNode(this);
                    var str_Title = p_Node.child("p").fullText();
                    var str_Anchor = p_Node.child("a").attribute("name");
                    var p_NewItem = userGuideMenu.menu.createItem(userGuideMenu.examples, str_Title, "#" + str_Anchor);
                } catch (err) {
                    trace.warn("UserGuideMenu.populateExamples(): example processing error: this = " + this + ", err = " + err);
                }
            });
            userGuideMenu.examples.setChildWidth("600");
            trace.profile("UserGuideMenu.populateExamples() done");
        } catch (err) {
            trace.error("UserGuideMenu.populateExamples(): " + err); throw err;
        }
    }

    //! @brief Create figure menu items.
    this.populateFigures = function() {
        try {
            trace.profile("UserGuideMenu.populateFigures()");
            var i_FigureCount = 0;
            $(".informalfigure").each(function() {
                try {
                    i_FigureCount ++;
                    var p_Node = new XmlNode(this);
                    var str_Title = "Figure_" + i_FigureCount; {
                        var p_Div = p_Node.child("div").child("div");
                        // Figure tables are not enumerated by docbook XSL transformation.
                        p_Div.node.insertBefore(document.createTextNode("Figure " + i_FigureCount + ". "), p_Div.node.firstChild);
                        // Eventually retrieve the full title.
                        str_Title = p_Div.fullText();
                    }
                    var str_Anchor = "Figure_" + i_FigureCount;
                    if ((p_Node.child("a") != null) && (p_Node.child("a").attribute("name") != null)) {
                        str_Anchor = p_Node.child("a").attribute("name");
                    } else {
                        // Runtime A element creation.
                        var p_a = createXmlNode("a"); {
                            // Create @name attribute.
                            p_a.setAttribute("name", str_Anchor);
                        } p_Node.node.insertBefore(p_a.node, p_Node.node.firstChild);
                    }
                    userGuideMenu.menu.createItem(userGuideMenu.figures, str_Title, "#" + str_Anchor);
                } catch (err) {
                    trace.warn("UserGuideMenu.populateFigures(): figure processing error: this = " + this + ", err = " + err);
                }
            });
            userGuideMenu.figures.setChildWidth("600");
            trace.profile("UserGuideMenu.populateFigures() done");
        } catch (err) {
            trace.error("UserGuideMenu.populateExamples(): " + err); throw err;
        }
    }

    //! @brief Create table menu items.
    this.populateTables = function() {
        try {
            trace.profile("UserGuideMenu.populateTables()");
            $(".table").each(function() {
                try {
                    var p_Node = new XmlNode(this);
                    var str_Title = p_Node.child("p").fullText();
                    var str_Anchor = p_Node.child("a").attribute("name");
                    userGuideMenu.menu.createItem(userGuideMenu.tables, str_Title, "#" + str_Anchor);
                } catch (err) {
                    trace.warn("UserGuideMenu.populateTables(): table processing error: this = " + this + ", err = " + err);
                }
            });
            trace.profile("UserGuideMenu.populateTables() done");
        } catch (err) {
            trace.error("UserGuideMenu.populateTables(): " + err); throw err;
        }
    }

    //! @brief Create change menu items.
    this.populateChanges = function() {
        try {
            trace.profile("UserGuideMenu.populateChanges()");
            $(".revhistory").each(function() {
                var xml_trs = new XmlNode(this).child("table").node.getElementsByTagName("tr");
                // Scan changes from the end of the table, in order to restore a chronological order.
                for (var i = 1; i + 1 < xml_trs.length; i += 2) {
                    try {
                        var xml_tr1 = xml_trs[i];
                        var xml_tr1tds = xml_tr1.getElementsByTagName("td");
                        var xml_tr2 = xml_trs[i+1];
                        var xml_tr2tds = xml_tr2.getElementsByTagName("td");
                        if ((xml_tr1tds.length >= 2) && (xml_tr2tds.length >= 1)) {
                            var str_Title = new XmlNode(xml_tr1tds[0]).fullText();
                            var str_Date = new XmlNode(xml_tr1tds[1]).fullText();
                            var str_Details = new XmlNode(xml_tr2tds[0]).fullText();
                            var p_Item = userGuideMenu.menu.createItem(userGuideMenu.changes, str_Title, "#");

                            // Create a dialogbox
                            p_Item.a.dlg = new DynamicDialog("change" + i, "change", str_Title); {
                                // Add date
                                var p_p = createXmlNode("p"); {
                                    var xml_Text = document.createTextNode(str_Date);
                                    p_p.node.appendChild(xml_Text);
                                } p_Item.a.dlg.div.appendChild(p_p.node);

                                // Copy content
                                for (var j = 0; j < xml_tr2tds[0].childNodes.length; j++) {
                                    var xml_Child = xml_tr2tds[0].childNodes[j];
                                    xml_Child = xml_Child.cloneNode(true);
                                    p_Item.a.dlg.div.appendChild(xml_Child);
                                }
                            }
                            // Make it visible on click
                            p_Item.a.showDlg = function() {
                                var xml_a = this;
                                // Differ the dialog display a little bit,
                                // in order not to have it hidden when the menu hides because of being clicked.
                                window.setTimeout(function() { xml_a.dlg.open(); }, 10);
                            }
                            $(p_Item.a).click(function() { this.showDlg(); });
                        }
                    } catch (err) {
                        trace.warn("UserGuideMenu.populateChanges(): change processing error: i = " + i + ", xml_trs[i] = " + xml_trs[i] + ", err = " + err);
                    }
                }

                // Eventually remove the change table from the body.
                this.parentNode.removeChild(this);
            });
            userGuideMenu.changes.setChildWidth("150");
            trace.profile("UserGuideMenu.populateChanges() done");
        } catch (err) {
            trace.error("UserGuideMenu.populateChanges(): " + err); throw err;
        }
    }

    // Initialization calls.
    try {
        userGuideMenu.create();
        userGuideMenu.menu.show("main");
    } catch (err) {
        trace.error("UserGuideMenu.&lt;init&gt;: " + err); throw err;
    }
}

//! @brief Info object (note or tip).
//! @param xml_div Note or tip DIV element.
//! @param str_Icon jquery UI icon identifier.
//! @param str_DlgTitle Title of the dialog attached to the info.
function UserGuideInfo(xml_div, str_Icon, str_DlgTitle) {

    var info = this;

    //! Short text.
    this.shortText = null;

    //! Dialog attached with the info.
    this.dlg = null;

    //! DIV element of the info.
    this.div = null;
    // UL element of the info.
    this.ul = null;
    //! LI element of the info.
    this.li = null;
    // SPAN(icon) element of the info.
    this.icon = null;

    //! @brief Info creation.
    this.create = function() {
        // First of all, remove the H3 title (only contains "Note").
        var xml_h3 = xml_div.removeChild(new XmlNode(xml_div).child("h3").node);

        // Set short text.
        info.shortText = new XmlNode(xml_div).fullText();
        if (info.shortText.length > 100) {
            info.shortText = info.shortText.substr(0, 100) + "...";
        }

        // Create the dialog.
        info.dlg = new DynamicDialog(null, null, str_DlgTitle);
        while (xml_div.childNodes.length > 0) {
            var xml_FirstChild = xml_div.firstChild;
            xml_div.removeChild(xml_FirstChild);
            info.dlg.div.appendChild(xml_FirstChild);
        }

        // Create XML elements.
        info.div = xml_div;
        var p_ul = createXmlNode("ul"); info.ul = p_ul.node; {
            var p_li = createXmlNode("li"); info.li = p_li.node; {
                p_li.setAttribute("class", "ui-state-default ui-corner-all");
                p_li.setAttribute("title", info.shortText);
                var p_icon = createXmlNode("span"); info.icon = p_icon.node; {
                    p_icon.setAttribute("class", "ui-icon " + str_Icon);
                } p_li.node.appendChild(p_icon.node);
                var xml_Text = document.createTextNode(new XmlNode(xml_h3).fullText()); {
                } p_li.node.appendChild(xml_Text);
            } p_ul.node.appendChild(p_li.node);
        } xml_div.appendChild(p_ul.node);

        // Apply styles to the icon.
        $(info.ul).css({ margin: "0", padding: "0" });
        $(info.li).css({ margin: "2px", position: "relative", padding: "4px 0", cursor: "pointer", float: "right", listStyle: "none", width: 70 });
        $(info.icon).css({ float: "left", margin: "0 4px" });
        // Ensure infos do not mask menu items.
        $(info.div).css({ zIndex: 0 });
        $(info.ul).css({ zIndex: 0 });
        $(info.li).css({ zIndex: 0 });
        $(info.icon).css({ zIndex: 0 });

        // Make the info change of color when mouse is over.
        $(info.li).hover(
            function() { $(this).addClass('ui-state-hover'); }, 
            function() { $(this).removeClass('ui-state-hover'); }
        );
        // Display the dialog on click.
        $(info.li).click(
            function() { info.dlg.open(); }
        );
    }

    this.debug = function() {
        window.setTimeout(function() {
            trace.info("p_Info.shortText = " + info.shortText);
            trace.info("p_Info.div.zIndex = " + info.div.style.zIndex);
            trace.info("p_Info.ul.zIndex = " + info.ul.style.zIndex);
            trace.info("p_Info.li.zIndex = " + info.li.style.zIndex);
            trace.info("p_Info.icon.zIndex = " + info.icon.style.zIndex);
        }, 10);
    }

    // Initialization calls.
    try {
        this.create();
    } catch (err) {
        trace.error("UserGuideInfo.&lt;init&gt;: " + err); throw err;
    }
}


//! @brief Handler called when the page is loaded.
function onLoad() {
    try {
        trace.profile("onLoad()");

        // Display blackbird logger.
        window.setTimeout(function() {
            try {
                var b_Display = false;
                $("#blackbird").css({ display: (b_Display ? "block" : "none") });
            } catch (err) {
                trace.error("onLoad()/timeout[display blackbird]: " + err);
                throw err;
            }
        }, 10);

        // Abort javascript if required.
        if (! /nojs/.exec(location)) {
            // Create dynamic menu.
            document.menu = new UserGuideMenu();

            // Other transformations.
            function makeInfos(str_Class, str_Icon, str_DlgTitle) {
                try {
                    trace.profile("onLoad()/makeInfos(str_Class = " + str_Class + ")");
                    // For each note, create a UserGuideInfo object that will transform the page.
                    $("." + str_Class).each(function() {
                        try {
                            this.info = new UserGuideInfo(this, str_Icon, str_DlgTitle);
                        } catch (err) {
                            trace.warn("onLoad()/makeInfos(): div processing error: this = " + this + ", err = " + err);
                        }
                    });
                    trace.profile("onLoad()/makeInfos(str_Class = " + str_Class + ")");
                } catch (err) {
                    trace.error("onLoad()/makeInfos(): " + err); throw err;
                }
            }
            window.setTimeout(function() {
                makeInfos("note", "ui-icon-document", "Note");
                makeInfos("tip", "ui-icon-lightbulb", "Tip");
            }, 100);

            if (document.all) {
                // IE fixes: make the scrolling move in order to ensure the menu to be displayed.
                window.setTimeout(function() { document.body.scrollTop = document.body.scrollTop - 1; }, 5000);
            }
        }

        trace.profile("onLoad() done");
    } catch (err) {
        trace.error("onLoad(): " + err); throw err;
    }
}

//! @brief Handler called when the page is scrolled.
function onScroll() {
    try {
        trace.profile("onScroll()");

        if (document.all) {
            var xml_BlackBird = document.getElementById("blackbird");
            if (xml_BlackBird) {
                xml_BlackBird.style.position = "absolute";
                xml_BlackBird.style.top = document.body.scrollTop + window.height() - xml_BlackBird.offsetHeight;
                xml_BlackBird.style.left = document.body.scrollLeft + window.width() - xml_BlackBird.offsetWidth;
            }
        }

        trace.profile("onScroll()");
    } catch (err) {
        trace.error("onScroll(): " + err);
        throw err;
    }
}

