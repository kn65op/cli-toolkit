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


//! @brief Dynamic menu item class.
//! @param menu Dynamic menu owning this item.
//! @param p_ParentMenuItem Parent menu item reference.
//! @param str_Text Text of the item.
//! @param str_Url URL of the item.
function DynamicMenuItem(menu, p_ParentMenuItem, str_Text, str_Url) {
    var menuItem = this;

    //! Parent menu item.
    this.parent = p_ParentMenuItem;
    //! LI top element.
    this.li = null;
    //! A link element.
    this.a = null;
    //! UL element ready for receiving child nodes.
    this.ul = null;

    //! Background color.
    this.backgroundColor = "#666666";

    //! @brief Dynamic menu item creation routine.
    this.create = function() {
        try {
            var p_li = createXmlNode("li");
            menuItem.li = p_li.node; {
                // Set the internal hyperlink.
                var p_a = createXmlNode("a");
                menuItem.a = p_a.node; {
                    // Create @href attribute.
                    if ((str_Url != null) && (typeof str_Url != undefined) && (str_Url != "#")) {
                        p_a.setAttribute("href", str_Url);
                    } else {
                        p_a.setAttribute("href", "#_");
                    }
                    // Set the item text.
                    var xml_Text = document.createTextNode(str_Text);
                    p_a.node.appendChild(xml_Text);
                }
                p_li.node.appendChild(p_a.node);
                // Add an unsigned list node for possible future sub-items.
                var p_ul = createXmlNode("ul");
                menuItem.ul = p_ul.node; {
                }
                p_li.node.appendChild(p_ul.node);
            }
            if (menuItem.parent != null) {
                menuItem.parent.ul.appendChild(p_li.node);
            } else {
                menu.ulMain.appendChild(p_li.node);
            }

            // General styles.

            // Align our list elements horizontally.
            // Positions 2nd rank sub-menus right next to their parent.
            $(menuItem.li).css({
                float: "left",
                position: "relative",
                display: "block"
            });

            // Style each hyper link in our menu a little bit.
            $(menuItem.a).css({
                display: "block",
                padding: "0px 5px",
                border: "1px solid #333333",
                color: "#ffffff",
                textDecoration: "none",
                backgroundColor: menuItem.backgroundColor
                // Using opacity slows a lot menu displays
                // opacity: "0.95",
                // filter: "alpha(opacity='95')" // IE < 8
            });
            if (menuItem.parent != null) {
                // Set the width of the hyper links
                // (which in combination with the width of the UL set above results in a horizontally displayed sub menu, despite of the ongoing float:left).
                $(menuItem.a).css({
                    // width: "300", // Do not set the item width right now, it will be set on display.
                    float: "left"
                });
            }

            $(menuItem.ul).css({
                margin: 0,
                padding: 0,
                listStyleType: "none",
                listStylePosition: "outside",
                // width: "300", // Do not set the item width right now, it will be set on display.
                display: "none"
            });

            // Force z-indexes so that the menu is always on top
            menuItem.li.parentNode.style.zIndex = 99;
            menuItem.li.style.zIndex = 99;
            menuItem.a.style.zIndex = 99;
            menuItem.ul.style.zIndex = 99;

            // Set animations.

            // Enlight menu items when the mouse is over.
            $(menuItem.li).hover(
                // 'on' effect
                function() {
                    $(this).addClass('dynamic-menu-on');
                    menu.getItem(this).hilight();
                    // If a timeout for hiding the menu is set, kill it.
                    menu.clearHideMenuTimeout("jQuery.hover(on)");
                },
                // 'out' effect
                function() {
                    $(this).removeClass('dynamic-menu-on');
                    menu.getItem(this).unhilight();
                    // Set a timer for auto hiding the menu.
                    var b_HideMenu = true;
                    for (var p_Item = menu.lastMenuItem; p_Item != null; p_Item = p_Item.parent) {
                        if ($(p_Item.li).hasClass('dynamic-menu-on')) {
                            // The mouse pointer is still covering a parent menu item => Do not hide the menu.
                            b_HideMenu = false;
                            break;
                        }
                    }
                    if (b_HideMenu) {
                        menu.setHideMenuTimeout("jQuery.hover(off)", 1000);
                    }
                }
            );

            $(menuItem.a).click(
                function() {
                    if (menu.getItem(this).getChildCount() == 0) {
                        // Final menu item.
                        menu.getItem(this).activate();
                    } else {
                        // Non-final menu item.
                        if (menu.lastMenuItem == menu.getItem(this)) {
                            // When clicked twice on the menu item: unexpand the sub-menu.
                            menu.lastMenuItem.unexpand();
                            menu.lastMenuItem = menu.lastMenuItem.parent;
                        } else {
                            // Expand the menu item.
                            menu.getItem(this).expand();
                        }
                        // Also activate in order to make the page scroll smoothly.
                        menu.activateLink(this);
                    }
                }
            );
        } catch (err) {
            trace.error("DynamicMenu.DynamicMenuItem.create(): " + err); throw err;
        }
    }

    //! @brief Attach an identifier to the item.
    //! @param str_Id Identifier.
    this.setId = function(str_Id) {
        menuItem.li.setAttribute("id", str_Id);
    }

    //! @brief Colorize a menu item.
    //! @param str_Color Color to be used.
    this.setColor = function(str_Color) {
        trace.profile("DynamicMenuItem.setBackgroundColor(this = " + menuItem.a + ", str_Color = " + str_Color + ")");

        menuItem.backgroundColor = str_Color;
        $(menuItem.a).css({
            border: "1px solid " + menuItem.backgroundColor,
            backgroundColor: menuItem.backgroundColor
        });
    }

    //! Child item width.
    this.childWidth = "300";

    //! @brief Resize children in width.
    //! @param str_Width New width.
    this.setChildWidth = function(str_Width) {
        trace.profile("DynamicMenuItem.setChildWidth(this = " + menuItem.a + ", str_Width = " + str_Width + ")");

        menuItem.childWidth = str_Width;
        $(menuItem.ul).css({ width: menuItem.childWidth });
        $(menuItem.ul).find("li").css({ width: menuItem.childWidth });
        $(menuItem.ul).find("li").find("a").css({ width: menuItem.childWidth });
    }

    //! @brief Child count accessor.
    //! @return Child count.
    this.getChildCount = function() {
        i_Count = 0;
        for (var p_child = menuItem.ul.firstChild; p_child != null; p_child = p_child.nextSibling) {
            if (p_child.nodeName == "LI") {
                i_Count ++;
            }
        }
        trace.profile("DynamicMenuItem.getChildCount(this = " + menuItem.a + ") -> " + i_Count);
        return i_Count;
    }

    //! @brief Determines whether the item is an ancestor of another item.
    //! @param p_item Another menu item.
    //! @return true if the item is an ancestor the other item, false otherwise.
    this.isAncestorOf = function(p_item) {
        try {
            if (p_item == null) {
                return false;
            } else if (p_item == this) {
                return true;
            } else {
                return menuItem.isAncestorOf(p_item.parent);
            }
        } catch (err) {
            trace.error("DynamicMenuItem.isAncestorOf(): " + err); throw err;
        }
    }

    //! @brief Hilight the menu item.
    this.hilight = function() {
        trace.profile("DynamicMenuItem.hilight(this = " + menuItem.a + ")");

        $(menuItem.a).css({
            backgroundColor: "#ffffff",
            color: "#333333"
        });
    }
    //! @brief Unhilight the menu item.
    this.unhilight = function() {
        trace.profile("DynamicMenuItem.unhilight(this = " + menuItem.a + ")");
        $(menuItem.a).css({
            backgroundColor: menuItem.backgroundColor,
            color: "#ffffff"
        });
    }

    //! @brief Expand the menu item.
    this.expand = function() {
        try {
            trace.profile("DynamicMenuItem.expand(this = " + menuItem.a + ")");

            // Unexpand the previous menu items.
            menu.unexpand(this);

            if (menuItem.parent == null) {
                // Position the first nested Lists right beyond the main menu and give them a width of 18em.
                $(menuItem.ul).css({
                    position: "absolute",
                    top: "1.5em"
                });
            } else {
                // Define where we display the sub menus.
                $(menuItem.ul).css({
                    position: "absolute",
                    top: "auto",
                    left: menuItem.parent.childWidth,
                    margin: "0px 0 0 10px"
                });
            }

            // Ensure the menu is visible at the top position.
            $(menu.ul).css({
                top: 0
            });
            if (document.all) {
                $(menu.ul).css({
                    top: document.body.scrollTop
                });
            }
            // Display the direct sub-menu.
            $(menuItem.ul).css({
                visibility: "visible",
                display: "none"
            });
            if (document.all) {
                $(menuItem.ul).css({
                    display: "block"
                });
            }

            // Ensure menu width.
            menuItem.setChildWidth(menuItem.childWidth);

            // Show the menu
            $(menuItem.ul).show(400);
            window.setTimeout(function() { menuItem.fitToWindow(); }, 450);
        } catch (err) {
            trace.error("DynamicMenu.onItemMouseOver(): " + err); throw err;
        }
    }
    //! @brief Reduce the menu item.
    this.unexpand = function() {
        try {
            trace.profile("DynamicMenuItem.unexpand(this = " + menuItem.a + ")");

            // Hide the direct sub-menu.
            $(menuItem.ul).css({
                visibility: "hidden"
            });
            // Also undo what has possibly done to fit the sub-menu in the window.
            menuItem.unfitToWindow();
        } catch (err) {
            trace.error("DynamicMenu.unexpand(): " + err); throw err;
        }
    }
    //! @brief Ensure the menu fits in the window.
    this.fitToWindow = function() {
        try {
            var p_ul = new XmlNode(menuItem.ul);
            i_WindowHeight = window.height() - 24; // Removal of 24px in order to avoid the bottom slider to mask the last menu.
            if (document.all) {
                // IE: p_ul.bottom() retrieves the absolute bottom position from the top of the page (not of the window)
                // Let's add the equivalent of the current scrolling position to i_WindowHeight in order to compare similar values.
                i_WindowHeight += document.body.scrollTop;
            }
            if (p_ul.bottom() > i_WindowHeight) {
                // Need to clip the menu into the window.

                function hideItemsFromStart() {
                    var p_li = menuItem.ul.firstChild;
                    for (p_li = p_li.nextSibling; (p_li != null) && (p_ul.bottom() > i_WindowHeight); p_li = p_li.nextSibling) {
                        $(p_li).css({ display: "none" });
                    }
                }
                function hideItemsFromEnd() {
                    var p_li = menuItem.ul.lastChild;
                    for (p_li = p_li.previousSibling; (p_li != null) && (p_ul.bottom() > i_WindowHeight); p_li = p_li.previousSibling) {
                        $(p_li).css({ display: "none" });
                    }
                }

                function createTempMenuItem(b_Top) {
                    var p_li = createXmlNode("li"); {
                        // Create @class attribute.
                        p_li.setAttribute("class", "temp");

                        function create_a(str_Text) {
                            // Create a link.
                            var p_a = createXmlNode("a"); {
                                // Create @href attribute.
                                p_a.setAttribute("href", "#_");
                                // Set the item text.
                                var xml_Text = null;
                                if (str_Text == "<<") { xml_Text = document.createTextNode("Move up"); }
                                if (str_Text == ">>") { xml_Text = document.createTextNode("Move down"); }
                                p_a.node.appendChild(xml_Text);
                            } p_li.node.appendChild(p_a.node);

                            // Style each hyper link in our menu a little bit.
                            $(p_a.node).css({
                                display: "block",
                                padding: "0px 5px",
                                border: "1px dotted #aaaaaa",
                                color: "#000000",
                                textDecoration: "none",
                                fontStyle: "italic",
                                fontSize: "x-small",
                                height: "20",
                                backgroundColor: "#f0f0f0",
                                // Using opacity slows a lot menu displays
                                // opacity: "0.95",
                                // filter: "alpha(opacity='95')" // IE < 8
                                float: "left",
                                textAlign: "center"
                            });
                            if (str_Text == "<<") { $(p_a.node).css({ textAlign: "left" }); }
                            if (str_Text == ">>") { $(p_a.node).css({ textAlign: "right" }); }

                            $(p_a.node).click(function() {
                                // Click effect
                                var p_li = null;
                                if (str_Text == "<<") { p_li = menuItem.ul.lastChild; }
                                if (str_Text == ">>") { p_li = menuItem.ul.firstChild; }
                                function next_li(li) {
                                    if (li == null) { return null; }
                                    if (str_Text == "<<") { return li.previousSibling; }
                                    if (str_Text == ">>") { return li.nextSibling; }
                                }
                                // Move backward to the visible items
                                for (p_li = next_li(p_li); p_li != null; p_li = next_li(p_li)) {
                                    if (p_li.style.display == "block") {
                                        // Continue until we reach the first hidden item.
                                        for (p_li = next_li(p_li); p_li != null; p_li = next_li(p_li)) {
                                            if (p_li.style.display == "none") {
                                                // Show it
                                                $(p_li).css({ display: "block" });
                                                // Hide menu items from end/start
                                                if (str_Text == "<<") { hideItemsFromEnd(); }
                                                if (str_Text == ">>") { hideItemsFromStart(); }
                                                // End there.
                                                return;
                                            }
                                        }
                                    }
                                }
                            });
                        }
                        if (b_Top) {
                            create_a("<<");
                        } else {
                            create_a(">>");
                        }
                    }
                    if (b_Top) {
                        p_ul.node.insertBefore(p_li.node, p_ul.node.firstChild);
                    } else {
                        p_ul.node.appendChild(p_li.node);
                    }

                    // Animations.
                    $(p_li.node).hover(
                        // 'on' effect
                        function() {
                            // If a timeout for hiding the menu is set, kill it.
                            menu.clearHideMenuTimeout("jQuery.hover(on)");
                        },
                        // 'out' effect
                        function() {
                            // Set a timer for auto hiding the menu.
                            menu.setHideMenuTimeout("jQuery.hover(off)", 1000);
                        }
                    );
                }
                // Create a first menu item to scroll the menu up.
                createTempMenuItem(true);
                // Create a last menu item to scroll the menu down.
                createTempMenuItem(false);

                menuItem.setChildWidth(menuItem.childWidth);

                hideItemsFromEnd();
            }
        } catch (err) {
            trace.error("DynamicMenu.fitToWindow(): " + err); throw err;
        }
    }
    //! @brief Undo fitToWindow() operations.
    this.unfitToWindow = function() {
        try {
            if (menuItem.ul.firstChild != null) {
                if (menuItem.ul.firstChild.getAttribute("class") == "temp") {
                    menuItem.ul.removeChild(menuItem.ul.firstChild);
                }
            }
            if (menuItem.ul.lastChild != null) {
                if (menuItem.ul.lastChild.getAttribute("class") == "temp") {
                    menuItem.ul.removeChild(menuItem.ul.lastChild);
                }
            }
            for (var p_li = menuItem.ul.firstChild; p_li != null; p_li = p_li.nextSibling) {
                $(p_li).css({ display: "block" });
            }
        } catch (err) {
            trace.error("DynamicMenu.fitToWindow(): " + err); throw err;
        }
    }

    //! @brief Activate the link.
    this.activate = function() {
        trace.profile("DynamicMenuItem.activate(this = " + menuItem.a + ")");

        menu.activateLink(menuItem.a);
    }

    // Initialization calls.
    try {
        menuItem.create();
    } catch (err) {
        trace.error("DynamicMenu.DynamicMenuItem.&lt;init&gt;: " + err); throw err;
    }
}


//! @brief Dynamic menu management.
function DynamicMenu(id) {
    trace.profile("DynamicMenu(id = " + id + ")");

    //! Self reference.
    var menu = this;


    //! Menu identifier.
    this.id = id;
    //! Main UL element.
    this.ulMain = null;

    //! @brief Dynamic menu creation.
    this.create = function() {
        try {
            trace.profile("DynamicMenu.create()");

            // Add the UL element that will constitute the main menu.
            var p_ul = createXmlNode("ul");
            p_ul.setAttribute("id", menu.id);
            $(p_ul.node).css({ display: "none" });
            document.body.insertBefore(p_ul.node, document.body.firstChild);
            menu.ulMain = p_ul.node;

            // Also add a blank DIV that will do nothing but leave enough space to let the menu visible without masking the top of the page.
            var p_blank = createXmlNode("div");
            $(p_blank.node).css({
                //backgroundColor: "red",
                top: "0px",
                height: "18px" //new XmlNode(menu.ulMain).height(),
            });
            document.body.insertBefore(p_blank.node, document.body.firstChild);

            // Let's position menu items at the top left corner, attached to the navigator window.
            $(menu.ulMain).css({
                margin: 0,
                padding: 0,
                listStyleType: "none",
                listStylePosition: "outside",
                position: "fixed", // fixed for a fixed position in the navigator window, whatever the scrolling is.
                top: 0,
                left: 0,
                lineHeight: "1.5em" // Line-height defines the height of each list item.
            });
            if (document.all) {
                // IE fixes.
                $(menu.ulMain).css({
                    position: "absolute",
                    top: document.body.scrollTop,
                    left: document.body.scrollLeft
                });
                $(window).scroll(function() {
                    $(menu.ulMain).css({
                        display: "block",
                        position: "absolute",
                        top: document.body.scrollTop - menu.hideOffset,
                        left: document.body.scrollLeft
                    });
                });
            }

            // Set a timer for auto hiding the menu.
            menu.setHideMenuTimeout("DynamicMenu.create()", 1000);

            trace.profile("DynamicMenu.create()");
        } catch (err) {
            trace.error("DynamicMenu.create(): " + err); throw err;
        }
    }

    //! List of items
    this.items = [];

    //! @brief Menu item creation.
    //! @param p_ParentMenuItem Parent menu item reference. null for creating an item at the top level.
    //! @param str_Text Text of menu item.
    //! @param str_Url URL the menu item should redirect to.
    //! @return New DynamicMenuItem object.
    this.createItem = function(p_ParentMenuItem, str_Text, str_Url) {
        try {
            trace.profile("DynamicMenu.createItem(str_Text = " + str_Text + ", str_Url = " + str_Url + ")");

            // Create a list item.
            var p_NewItem = new DynamicMenuItem(menu, p_ParentMenuItem, str_Text, str_Url);
            menu.items.push(p_NewItem);

            return p_NewItem;
        } catch (err) {
            trace.error("DynamicMenu.createItem(): " + err); throw err;
        }
    }
    //! @brief Item accessor.
    //! @param p_x LI, A or UL element corresponding to the item.
    //! @return DynamicItem reference if found, null otherwise.
    this.getItem = function(p_x) {
        for (i=0; i<menu.items.length; i++) {
            if ((p_x == menu.items[i].li) || (p_x == menu.items[i].a) || (p_x == menu.items[i].ul)) {
                return menu.items[i];
            }
        }
        trace.warn("Could not find menu item");
        return null;
    }

    //! Last menu item being processed.
    this.lastMenuItem = null;

    //! @brief Unexpand menu items.
    //! @param p_TargetItem Optional. Target item for expansion.
    this.unexpand = function(p_TargetItem) {
        try {
            trace.profile("DynamicMenu.unexpand(p_TargetItem = " + p_TargetItem + ")");

            while (menu.lastMenuItem != null) {
                if (p_TargetItem != null) {
                    if (menu.lastMenuItem.isAncestorOf(p_TargetItem)) {
                        // The menu is ready for expansion of the target item.
                        // Stop unexpanding menu items.
                        break;
                    }
                }
                // Unexpand the last menu item and move to the parent.
                menu.lastMenuItem.unexpand();
                menu.lastMenuItem = menu.lastMenuItem.parent;
            }

            // Remember the target menu item if any.
            if (p_TargetItem) {
                menu.lastMenuItem = p_TargetItem;
            } else {
                menu.lastMenuItem = null;
            }
        } catch (err) {
            trace.error("DynamicMenu.unexpand(): " + err); throw err;
        }
    }

    //! First show flag. Fall at false as soon as the menu is shown for the first time.
    this.firstShow = true;

    //! @brief Shows the menu previously created.
    //! @param cause Cause of showing (useful for debug only)
    this.show = function(cause) {
        try {
            trace.profile("DynamicMenu.show(cause = " + cause + ")");

            if (menu.firstShow) {
                // This style is applicable to all A elements of the page (but menu items).
                // In case it is an inner reference, it scrolls the page as the menu would,
                // and applies the appropriate offset so that the menu will not disturb the elements displayed.
                $("a").click(
                    function() { // 'click' effect
                        // If it is not a link for a menu item
                        if (menu.getItem(this) == null) {
                            menu.activateLink(this);
                        }
                    }
                );
                menu.firstShow = false;
            }

            // Clear the hide menu timeout if any.
            menu.clearHideMenuTimeout("DynamicMenu.show()");

            // Ensure the menu is visible at the top position.
            $(menu.ulMain).css({
                display: "block",
                top: 0
            });
            if (document.all) {
                // IE fixes
                $(menu.ulMain).css({
                    top: document.body.scrollTop
                });
            }
        } catch (err) {
            trace.error("DynamicMenu.show(): " + err); throw err;
        }
    }

    //! Hiding offset.
    this.hideOffset = 0; // Default

    //! @brief Hides the menu.
    //! @param cause Cause of hiding (useful for debug only)
    //! @param delay Time of effect. Optional, can be not set, default behaviour is then adopted.
    this.hide = function(cause, delay) {
        try {
            trace.profile("DynamicMenu.hide(cause = " + cause + ", delay = " + delay + ")");

            // Clear the hide menu timeout if any.
            menu.clearHideMenuTimeout("DynamicMenu.hide()");

            // Unexpand the last menu item if any.
            menu.unexpand();

            // Hide the menu.
            if (! document.all) {
                $(menu.ulMain).animate({ top: - menu.hideOffset }, delay);
            } else {
                // IE only
                if (menu.hideOffset > 0) {
                    $(menu.ulMain).animate({
                        position: "absolute",
                        top: document.body.scrollTop - menu.hideOffset,
                        left: document.body.scrollLeft
                    }, delay);
                }
            }
        } catch (err) {
            trace.error("DynamicMenu.hide(cause = " + cause + "): " + err); throw err;
        }
    }

    //! Hide menu timeout.
    this.hHideMenu = null;

    //! @brief Program a timeout to hide the menu.
    //! @param cause Cause of hiding (useful for debug only).
    //! @param timeout Delay before hiding the menu.
    this.setHideMenuTimeout = function(cause, timeout) {
        trace.profile("DynamicMenu.setHideMenuTimeout(cause = " + cause + ", timeout = " + timeout + ")");

        // Clear the hide menu timeout if any.
        menu.clearHideMenuTimeout(cause);

        // Program a timeout function.
        menu.hHideMenu = window.setTimeout(function() { menu.hide(cause); }, timeout);
    }

    //! @brief clearHideMenuTimeout
    this.clearHideMenuTimeout = function(cause) {
        trace.profile("DynamicMenu.clearHideMenuTimeout(cause = " + cause + ")");

        // Kill and unset the reference of the current timeout function.
        if (menu.hHideMenu != null) {
            window.clearTimeout(menu.hHideMenu);
            menu.hHideMenu = null;
        }
    }

    //! @brief Activate a hyperlink and manages menu in consequence.
    //! @param p_a A element which link to activate.
    this.activateLink = function(p_a) {
        // Hide the main menu
        hideMainMenu = true;
        p_item = menu.getItem(p_a);
        if (p_item != null) {
            // Exception for non-final A elements of the menu when we use a "click" animation mode.
            // Do not hide the menu.
            if (p_item.getChildCount() > 0) {
                hideMainMenu = false;
            }
        }
        if (hideMainMenu) {
            menu.hide("DynamicMenu.activateLink()");
        }

        // Inner anchors.
        if (p_a.getAttribute("href") != null) {
            if (p_a.getAttribute("href").charAt(0) == '#') {
                var str_Anchor = p_a.getAttribute("href").replace(/#/, "");
                if (str_Anchor.length > 0) {
                    // Find out the target anchor in the page
                    var p_Anchor = null;
                    var xml_Anchors = document.getElementsByName(str_Anchor);
                    if (xml_Anchors.length > 0) {
                        p_Anchor = new XmlNode(xml_Anchors[0]);
                    } else if (document.all) {
                        // IE fixes.
                        xml_Anchors = document.getElementsByTagName("a");
                        for (var i=0; i<xml_Anchors.length; i++) {
                            if (xml_Anchors[i].getAttribute("name") == str_Anchor) {
                                p_Anchor = new XmlNode(xml_Anchors[i]);
                                break;
                            }
                        }
                    }

                    // Scroll to the target anchor.
                    if (p_Anchor != null) {
                        // If the anchor is located in a title, move up to the title element.
                        if (p_Anchor.parent().name().charAt(0) == 'h') {
                            p_Anchor = p_Anchor.parent();
                        }
                        // Apply some more offset because of the menu.
                        var moreOffset = new XmlNode(menu.ulMain).height() + 5 - menu.hideOffset;
                        // Make the window move smoothly to the given position.
                        $(document.body).animate({ scrollTop: p_Anchor.top() - moreOffset });
                    }
                }
            }
        }
    }

    // Initialization calls.
    try {
        menu.create();
    } catch (err) {
        trace.error("DynamicMenu.&lt;int&gt;: " + err); throw err;
    }

    trace.profile("DynamicMenu(id = " + id + ")");
}
