# -*- coding: ISO-8859-1 -*-

# Copyright (c) 2006-2013, Alexis Royer, http://alexis.royer.free.fr/CLI
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


""" CLI to Java compiler. """

# Imports
import modlog
import xmldoc
import clicommon


# Main function
def main():
    """ Main function. """
    #modlog.engine().set_filter(Cli2Java.MODLOG_FILTER, modlog.DEBUG)
    import sys
    ctx = Cli2Java()
    if (ctx.args.parse_args(ctx)):
        if (ctx.xml.load_file(ctx.args.xml_resource_file())):
            ctx.out.set_encoding(ctx.xml.encoding())
            ctx.xml.set_ns_shortcut("cli", "http://alexis.royer.free.fr/CLI")
            if (ctx.clic.execute(ctx)):
                sys.exit(0)
    modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "Execution error")
    sys.exit(-1)

# Classes

class Args:
    """ Program arguments management. """
    
    def __init__(self):
        """ Constructor. """
        self._res = None
    
    def parse_args(self, ctx):
        """ Parse program arguments.
            @param ctx (Cli2Java) Program execution context.
            @return (bool) True for success. """
        import argparse
        
        _args = argparse.ArgumentParser(prog="cli2java")
        _args.add_argument("--version", action="version", version=("%(prog)s - " + clicommon.CtxUtils.version()))
        _args.add_argument("--cli-class-name", help="Main CLI class name (default is based on <class-prefix> and @name attribute).")
        _args.add_argument("--cli-class-scope", help="Main CLI class scope (default is empty).")
        _args.add_argument("--class-prefix", help="Class prefix (default is empty).")
        _args.add_argument("--var-prefix", help="Variable prefix (default is 'm_cli').")
        _args.add_argument("--indent", help="Indentation pattern (default is 4 spaces).")
        _args.add_argument("--user-indent", help="User-code indentation pattern (default is '/* > */ ').")
        _args.add_argument("xml-res", type=file, help="CLI XML resource file")
        
        self._res = _args.parse_args()
        #modlog.log(ctx.MODLOG_FILTER, modlog.DEBUG, vars(self._res))
        
        #modlog.log(ctx.MODLOG_FILTER, modlog.WARNING, "Only one output device managed: Console")
        ctx.out = clicommon.Console()
        
        return True
    
    @staticmethod
    def program_name():
        """ Program name accessor.
            @return (str) Program name. """
        return "cli2java"
    
    def cli_class_name(self, ctx):
        """ Main CLI class name.
            @param ctx (Cli2Java) Execution context.

            @return (str) Main CLI class name. """
        _cli_class_name = vars(self._res)["cli_class_name"]
        if (_cli_class_name == None):

            _xml_cli = ctx.xml.root_node()
            _cli_class_name = self.class_prefix() + ctx.Utils.mk_java_name(ctx, None, ctx.xml.attr_value(_xml_cli, "@name"))
        return _cli_class_name

    def cli_class_scope(self):
        """ Main CLI class scope.
            @return (str) Class scope. """
        _class_scope = vars(self._res)["cli_class_scope"]
        if (_class_scope == None):
            _class_scope = ""
        return _class_scope
    
    def class_prefix(self):
        """ Class prefix pattern.
            @return (str) Class prefix pattern. """
        _class_prefix = vars(self._res)["class_prefix"]
        if (_class_prefix == None):
            _class_prefix = ""
        return _class_prefix
    
    def var_prefix(self):
        """ Variable prefix pattern.
            @return (str) Variable prefix pattern. """
        _var_prefix = vars(self._res)["var_prefix"]
        if (_var_prefix == None):
            _var_name = "m_cli"
        return _var_name
    
    def indent(self):
        """ Indentation pattern.
            @return (str) Indentation pattern. """
        _indent = vars(self._res)["indent"]
        if (_indent == None):
            _indent = "    "
        return _indent
    
    def user_indent(self):
        """ User-code indentation pattern.
            @return (str) User-code indentation pattern. """
        _user_indent = vars(self._res)["user_indent"]
        if (_user_indent == None):
            _user_indent = "/* > */ "
        return _user_indent
    
    def xml_resource_file(self):
        """ Resource file name accessor.
            @return (str) XML resource file name. """
        return vars(self._res)["xml-res"].name

class Cli2Java:
    """ CLI XML resource file to C++ transformation. """
    
    MODLOG_FILTER = "cli2java"
    
    def __init__(self):
        """ Constructor. """
        self.args = Args()
        self.xml = xmldoc.InputDocument()
        self.clic = self
        self.out = None
        self.cache = clicommon.Cache()
    
    @staticmethod
    def execute(ctx):
        """ XML 2 C++ transformation execution.
            @param ctx (Cli2Java) Execution context.
            @return (bool) True for success, False otherwise. """
        modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "execute()")
        try:
            ctx.cache.execute(ctx)
            ctx.Sections.execute(ctx)
            return True
        except ctx.Utils.ExecutionStoppedException:
            pass
        return False
    
    class Sections:
        """ Section generation routines. """
        
        def __init__(self):
            """ Constructor. """
            # Static class, nothing to be done
            pass
        
        @staticmethod
        def execute(ctx):
            """ Executes the class generation routines.
                @param ctx (Cli2Java) Execution context. """
            modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "Generating head section...")
            ctx.Sections.head_section(ctx)
            modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "Generating import section...")
            ctx.Sections.import_section(ctx)
            modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "Generating comment section...")
            ctx.Sections.comment_section(ctx)
            modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "Generating cli section...")
            ctx.Sections.cli_section(ctx)
            modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "Generating tail section...")
            ctx.Sections.tail_section(ctx)
            modlog.log(ctx.MODLOG_FILTER, modlog.TRACE, "ok")
        
        @staticmethod
        def head_section(ctx):
            """ Generate head section.
                @param ctx (Cli2Java) Execution context. """
            ctx.Utils.extra_source(ctx, xml_node = None, option = "head")
            ctx.out.endl()
        
        @staticmethod
        def import_section(ctx):
            """ Generate import section.
                @param ctx (Cli2Java) Execution context. """
            # Other includes
            ctx.out.put("// ----- Imports -----").endl()
            ctx.out.endl()
            ctx.Utils.extra_source(ctx, xml_node = None, option = "import")
            ctx.out.endl()
        
        @staticmethod
        def comment_section(ctx):
            """ Generate comment section.
                @param ctx (Cli2Java) Execution context. """
            import datetime
            ctx.out.put("/**").endl()
            ctx.out.put(" * Class auto-generated by 'cli2java.py' - Do not edit!").endl()
            ctx.out.put(" * @author cli2java.py - %s" % clicommon.CtxUtils.version()).endl()
            ctx.out.put(" * @date %s" % datetime.datetime.today().isoformat()).endl()
            ctx.out.put(" */").endl()
            ctx.out.endl()
        
        @staticmethod
        def cli_section(ctx):
            """ CLI code generation (main section).
                @param ctx (Cli2Java) Execution context. """
            # Cli definition
            ctx.Menu.execute(ctx)
        
        @staticmethod
        def tail_section(ctx):
            """ Generate tail section.
                @param ctx (Cli2Java) Proggram execution context.
                @return True for success, False otherwise. """
            ctx.Utils.extra_source(ctx, xml_node = None, option = "tail")
            ctx.out.endl()
            return True
    
    class Menu:
        """ Cli (which is the main menu) or menu code generation routines. """
        
        def __init__(self):
            """ Constructor. """
            # Static class, nothing to be done
            pass
        
        @staticmethod
        def execute(ctx):
            """ Executes the class generation routines for the Cli.
                @param ctx (Cli2Java) Execution context. """
            ctx.Menu.menu(ctx, ctx.xml.root_node())
        
        @staticmethod
        def menu(ctx, xml_menu):
            """ Cli (which is the main menu) or menu code generation.
                @param ctx (Cli2Java) Execution context.
                @param xml_menu (XML node) cli:cli or cli:menu node.
                @return True for success, False otherwise. """
            _class_name = ctx.Utils.node2class(ctx, xml_menu)
            _super_class = "cli.Menu"
            _scope = "private"
            
            if (ctx.xml.is_node(xml_menu, "cli:cli")):
                ctx.Utils.indent(ctx, xml_menu, 0).put("// ----- Cli class definition -----").endl()
                _super_class = "cli.Cli"
                _scope = ctx.args.cli_class_scope()
            
            ctx.Utils.indent(ctx, xml_menu, 0).put("%s class %s extends %s {" % (_scope, _class_name, _super_class)).endl()
            
            ctx.Menu.SubMenus.execute(ctx, xml_menu)    # Sub-menus at root only.
            ctx.Menu.Members.execute(ctx, xml_menu)     # Member declarations.
            ctx.Menu.Constructor.execute(ctx, xml_menu) # Constructor.
            ctx.Menu.Populate.execute(ctx, xml_menu)    # Populating nodes
            ctx.Menu.Execute.execute(ctx, xml_menu)     # Menu implementation.
            ctx.Menu.Handlers.execute(ctx, xml_menu)    # Menu handlers.
            
            ctx.Utils.indent(ctx, xml_menu, 0).put("}").endl()
            
            if (ctx.xml.is_node(xml_menu, "cli:cli")):
                ctx.out.endl()
            
            return True
        
        class SubMenus:
            """ Menu.SubMenus code generation routines. """
            
            def __init__(self):
                """ Constructor. """
                # Static class, nothing to be done
                pass
            
            @staticmethod
            def execute(ctx, xml_menu):
                """ Executes the Menu.SubMenus code generation routines.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Menu.SubMenus.sub_menus(ctx, xml_menu)
            
            @staticmethod
            def sub_menus(ctx, xml_menu):
                """ Propagate code generation over sub-menus (cli only), and set a reference to the main menu.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                # Sub menus: only for CLI object
                if (ctx.xml.is_node(xml_menu, "cli:cli")):
                    _xml_cli = xml_menu
                    ctx.Utils.indent(ctx, _xml_cli, 1).put("// ----- Sub-menus -----").endl()
                    for _xml_sub_menu in ctx.cache.menus:
                        # Filter-out cli:cli XML nodes
                        if (ctx.xml.is_node(_xml_sub_menu, "cli:menu")):
                            # Call code generation for the sub-menu.
                            ctx.Menu.menu(ctx, _xml_sub_menu)
                            ctx.out.endl()
                    ctx.out.endl()
                # Any menu: reference to the owner CLI object
                ctx.Utils.indent(ctx, xml_menu, 1).put("// ----- Owner CLI -----").endl()
                ctx.Utils.indent(ctx, xml_menu, 1).put("private %s m_cliOwnerCli;" % ctx.args.cli_class_name(ctx)).endl()
        
        class Members:
            """ Menu.Members code generation routines. """
            
            def __init__(self):
                """ Constructor. """
                # Static class, nothing to be done
                pass
            
            @staticmethod
            def execute(ctx, xml_menu):
                """ Executes the Menu.Members code generation routines.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Menu.Members.members(ctx, xml_menu)
            
            @staticmethod
            def members(ctx, xml_menu):
                """ Menu members declaration: menu members (cli only), sub-nodes and extra 'member' source.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                _class_name = ctx.Utils.node2class(ctx, xml_menu)
                _var_name = ctx.Utils.node2var(ctx, xml_menu)
                
                # Menus
                if (ctx.xml.is_node(xml_menu, "cli:cli")):
                    _xml_cli = xml_menu
                    ctx.Utils.indent(ctx, _xml_cli, 1).put("// ----- Menus -----").endl()
                    # Self
                    ctx.Menu.Members.declare_node(ctx, _xml_cli, _xml_cli, indent_count = 1)
                    # Effective menus
                    for _xml_sub_menu in ctx.cache.menus:
                        # Filter-out cli:cli XML nodes
                        if (ctx.xml.is_node(_xml_sub_menu, "cli:menu")):
                            ctx.Menu.Members.declare_node(ctx, _xml_cli, _xml_sub_menu, indent_count= 1)
                
                # Sub-nodes
                ctx.Utils.indent(ctx, xml_menu, 1).put("// ----- Node members -----").endl()
                ctx.Menu.Members.walk(ctx, xml_menu, xml_menu, indent_count = 1)
                
                # Members section
                ctx.Utils.extra_source(ctx, xml_node = xml_menu, option = "members")
                
                ctx.out.endl()
            
            @staticmethod
            def walk(ctx, xml_menu, xml_node, indent_count = 0):
                """ Recursive node declaration routine.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) Current menu being processed.
                    @param xml_node (XML node) Current cli:* node.
                    @param indent_count (int) Indentation depth. """
                for _xml_child in ctx.xml.children(xml_node):
                    if (ctx.xml.is_node(_xml_child, "cli:help")):
                        # No member for help nodes
                        pass
                    elif (ctx.xml.is_node(_xml_child, "cli:cpp") or ctx.xml.is_node(_xml_child, "cli:java") or ctx.xml.is_node(_xml_child, "cli:handler")):
                        # No member for source nodes.
                        pass
                    elif (ctx.xml.is_node(_xml_child, "cli:menu")):
                        # Menus are declare only in the CLI and have been declared at first. Do not declare them twice.
                        pass
                    elif (ctx.xml.is_node_with_attr(_xml_child, "cli:tag", "@ref")):
                        # No member for tag references
                        pass
                    elif (ctx.xml.is_node(_xml_child, "cli:*")):
                        ctx.Menu.Members.declare_node(ctx, xml_menu, _xml_child, indent_count = indent_count)
                        ctx.Menu.Members.walk(ctx, xml_menu, _xml_child, indent_count = indent_count + 1)
            
            @staticmethod
            def declare_node(ctx, xml_menu, xml_node, indent_count = 0):
                """ Member declaration for a given node.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) Current menu being processed.
                    @param xml_node (XML node) Current cli:* node.
                    @param indent_count (int) Indentation depth. """
                _class_name = ctx.Utils.node2class(ctx, xml_node)
                _var_name = ctx.Utils.node2var(ctx, xml_node, menu_with_cli_pointer = False)
                ctx.Utils.indent(ctx, xml_menu, indent_count).put("private %s %s;" % (_class_name, _var_name)).endl()
        
        class Constructor:
            """ Menu.Constructor code generation routines. """
            
            def __init__(self):
                """ Constructor. """
                # Static class, nothing to be done
                pass
            
            @staticmethod
            def execute(ctx, xml_menu):
                """ Executes the Menu.Constructor code generation routines.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Menu.Constructor.constructor(ctx, xml_menu)
            
            @staticmethod
            def constructor(ctx, xml_menu):
                """ Menu constructor code generation.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Utils.indent(ctx, xml_menu, 1).put("// ----- Constructor -----").endl()
                ctx.Utils.indent(ctx, xml_menu, 1).put("public %s() {" % ctx.Utils.node2class(ctx, xml_menu)).endl()
                # Initialization
                ctx.Utils.indent(ctx, xml_menu, 2).put("super(")
                ctx.Menu.Constructor.params(ctx, xml_menu)
                ctx.out.put(");").endl()
                
                # Populate
                if (ctx.xml.is_node(xml_menu, "cli:cli")):
                    ctx.Utils.indent(ctx, xml_menu, 2).put("this.populate();").endl()
                # Constructor section
                ctx.Utils.extra_source(ctx, xml_node = xml_menu, option = "constructor")
                
                ctx.Utils.indent(ctx, xml_menu, 1).put("}").endl()
                ctx.out.endl()
            
            @staticmethod
            def params(ctx, xml_node):
                """ Creation parameters for a given node.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_node (XML node) Node to output creation parameters for. """
                # tag[@id] node only need to know whether they are hollow or not.
                if (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@id")):
                    if (ctx.xml.is_node_with_attr_value(xml_node, "cli:tag", "@hollow", "yes")):
                        ctx.out.put("true")
                    else:
                        ctx.out.put("false")
                    return True
                # tag[@ref] only need to be passed their tag[@id] target
                elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                    _xml_tag = ctx.cache.node(xml_node).cli_Cli2xxx_tag_id
                    if (_xml_tag != None):
                        ctx.out.put(ctx.Utils.node2var(ctx, _xml_tag))
                        return True
                    else:
                        ctx.Utils.abort(ctx, xml_node, "unknown tag reference '%s'" % ctx.xml.attr_value(xml_node, "@ref"))
                # menu[@ref] only need to be passed their menu[@name] target
                elif (ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@ref")):
                    _xml_menu = ctx.cache.node(xml_node).cli_Cli2xxx_menu
                    if (_xml_menu != None):
                        ctx.out.put(ctx.Utils.node2var(ctx, _xml_menu))
                        return True
                    else:
                        ctx.Utils.abort(ctx, xml_node, "unknown menu reference '%s'" % ctx.xml.attr_value(xml_node, "@ref"))
                else:
                    _keyword = None
                    if (ctx.xml.is_node(xml_node, "cli:cli") or ctx.xml.is_node(xml_node, "cli:menu")):
                        _keyword = ctx.xml.attr_value(xml_node, "@name")
                    if (ctx.xml.is_node(xml_node, "cli:keyword")):
                        _keyword = ctx.xml.attr_value(xml_node, "@string")
                    # Determine whether help should be set on that node.
                    _set_help = True
                    if (ctx.xml.is_node(xml_node, "cli:tag")):
                        _set_help = False
                    if (ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@ref")):
                        _set_help = False
                    # Generate "keyword" and help parameters.
                    if (_keyword != None):
                        ctx.out.put("\"%s\"" % _keyword)
                        if (_set_help):
                            ctx.out.put(", ")
                    if (_set_help):
                        ctx.out.put("new cli.Help()")
                        for _xml_help in ctx.xml.children(xml_node):
                            if (ctx.xml.is_node(_xml_help, "cli:help")):
                                ctx.out.put(".addHelp(")
                                if (ctx.xml.attr_value(_xml_help, "@lang") == None):
                                    modlog.log(ctx.MODLOG_FILTER, modlog.WARNING, "Missing @lang attribute for cli:help node, using 'en' instead.")
                                    ctx.Utils.abort(ctx, _xml_help, "missing @lang attribute")
                                    ctx.out.put("cli.Help.LANG_EN, ")
                                elif (ctx.xml.attr_value(_xml_help, "@lang") == "en"):
                                    ctx.out.put("cli.Help.LANG_EN, ")
                                elif (ctx.xml.attr_value(_xml_help, "@lang") == "fr"):
                                    ctx.out.put("cli.Help.LANG_FR, ")
                                else:
                                    modlog.log(ctx.MODLOG_FILTER, modlog.WARNING, "Unknown @lang value '%s' for cli:help node, using 'en' instead.")
                                    ctx.out.put("cli.Help.LANG_EN, ")
                                ctx.out.put("\"%s\"" % ctx.xml.content(_xml_help))
                                ctx.out.put(")")
        
        class Populate:
            """ Menu.Populate code generation routines. """
            
            def __init__(self):
                """ Constructor. """
                # Static class, nothing to be done
                pass
            
            @staticmethod
            def execute(ctx, xml_menu):
                """ Executes the Menu.Populate code generation routines.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Menu.Populate.populate(ctx, xml_menu)
            
            @staticmethod
            def populate(ctx, xml_menu):
                """ Menu constructor code generation.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                _str_class = ctx.Utils.node2class(ctx, xml_menu)
                _str_var = ctx.Utils.node2var(ctx, xml_menu)
                
                ctx.Utils.indent(ctx, xml_menu, 1).put("// ----- Populate -----").endl()
                ctx.Utils.indent(ctx, xml_menu, 1).put("public void populate() {").endl()
                
                # CLI reference
                ctx.Utils.indent(ctx, xml_menu, 2).put("// CLI reference").endl()
                ctx.Utils.indent(ctx, xml_menu, 2).put("m_cliOwnerCli = (%s) getCli();" % ctx.args.cli_class_name(ctx)).endl()
                
                # Populate menus
                if (ctx.xml.is_node(xml_menu, "cli:cli")):
                    ctx.Utils.indent(ctx, xml_menu, 2).put("// Create menus and populate").endl()
                    # Self CLI reference setting
                    ctx.Utils.indent(ctx, xml_menu, 2).put("%s = this;" % ctx.Utils.node2var(ctx, xml_menu)).endl()
                    # Create the menus
                    for _xml_menu in ctx.cache.menus:
                        # Filter-out cli:cli XML nodes
                        if (ctx.xml.is_node(_xml_menu, "cli:menu")):
                            _var_name = ctx.Utils.node2var(ctx, _xml_menu)
                            _class_name = ctx.Utils.node2class(ctx, _xml_menu)
                            ctx.Utils.indent(ctx, xml_menu, 2).put("%s = (%s) addMenu(new %s());" % (_var_name, _class_name, _class_name)).endl()
                    # Populate them
                    for _xml_menu in ctx.cache.menus:
                        # Filter-out cli:cli XML nodes
                        if (ctx.xml.is_node(_xml_menu, "cli:menu")):
                            ctx.Utils.indent(ctx, xml_menu, 2).put("%s.populate();" % ctx.Utils.node2var(ctx, _xml_menu)).endl()
                
                # Sub-nodes
                ctx.Utils.indent(ctx, xml_menu, 2).put("// Local nodes").endl()
                ctx.Menu.Populate.walk(ctx, xml_menu, xml_menu, indent_count = 2)
                # tag[@ref] items are processed at the very end
                for _xml_tag_ref in ctx.cache.node(xml_menu).cli_Cli2xxx_tag_refs:
                    # Check this tag reference is not in a sub-menu of the current one.
                    _set_tag_ref = True
                    _xml_ancestor = _xml_tag_ref
                    while (_xml_ancestor != None):
                        if (ctx.xml.is_node_with_attr(_xml_ancestor, "cli:menu", "@name") and (_xml_ancestor != xml_menu)):
                            _set_tag_ref = False
                            _xml_ancestor = None
                        _xml_ancestor = ctx.xml.parent_node(_xml_ancestor)
                    if (_set_tag_ref):
                        ctx.Utils.indent(ctx, xml_menu, 2).put("// tag[@ref] -> tag[@id] connections").endl()
                        ctx.Menu.Populate.create_node(ctx, xml_menu, _xml_tag_ref, indent_count = 2)
                
                ctx.Utils.indent(ctx, xml_menu, 1).put("}").endl()
                ctx.out.endl()
            
            @staticmethod
            def walk(ctx, xml_menu, xml_node, indent_count = 0):
                """ Propagate population throughout child nodes.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) Current menu.
                    @param xml_node (XML node) Current node.
                    @indent_count (int) Indentation depth. """
                for _xml_sub_node in ctx.xml.children(xml_node):
                    if (ctx.xml.is_node(_xml_sub_node, "cli:help")):
                        # No population for help nodes. Help nodes generate help arguments.
                        pass
                    elif (ctx.xml.is_node(_xml_sub_node, "cli:cpp")
                          or ctx.xml.is_node(_xml_sub_node, "cli:java")
                          or ctx.xml.is_node(_xml_sub_node, "cli:handler")):
                        # No population for target language nodes.
                        pass
                    elif (ctx.xml.is_node_with_attr(_xml_sub_node, "cli:menu", "@name") and ctx.xml.is_node(xml_node, "cli:cli")):
                        # Do not populate menus which are defined at the cli level.
                        pass
                    elif (ctx.xml.is_node_with_attr(_xml_sub_node, "cli:tag", "@ref")):
                        # No population for tag[@ref] right now.
                        # Because tag[@id] could be processed later, tag[@ref] will be processed at the very end,
                        # in the main populate() routine above.
                        pass
                    elif (ctx.xml.is_node(_xml_sub_node, "cli:*")):
                        ctx.Menu.Populate.create_node(ctx, xml_menu, _xml_sub_node, indent_count = indent_count)
                        if (ctx.xml.is_node(_xml_sub_node, "cli:cli")):
                            # Should never occure.
                            pass
                        elif (ctx.xml.is_node_with_attr(_xml_sub_node, "cli:menu", "@name")):
                            # Do not populate menus, this is done directly at the cli level.
                            pass
                        else:
                            ctx.Menu.Populate.walk(ctx, xml_menu, _xml_sub_node, indent_count = indent_count + 1)
            
            @staticmethod
            def create_node(ctx, xml_menu, xml_node, indent_count = 0, with_creation_params = True):
                """ Create the node for population.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) Current menu.
                    @param xml_node (XML node) Current node.
                    @param indent_count (int) Indentation depth.
                    @param with_creation_params (bool) Insert creation parameters or not (default is True). """
                _class_name = ctx.Utils.node2class(ctx, xml_node)
                _var_name = ctx.Utils.node2var(ctx, xml_node)
                _parent_var = ctx.Utils.node2var(ctx, ctx.xml.parent_node(xml_node))
                
                ctx.Utils.indent(ctx, xml_menu, indent_count)
                _do_not_cast = ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref") or ctx.xml.is_node(xml_node, "cli:menu")
                if (not(_do_not_cast)):
                    ctx.out.put("%s = (%s) " % (_var_name, _class_name))
                # Attach the new object to the parent node.
                ctx.out.put("%s" % _parent_var)
                if (ctx.xml.is_node(xml_node, "cli:menu")):
                    # Populate menu[@ref] item.
                    ctx.out.put(".setMenuRef(new cli.MenuRef(")
                    _menu_ref_set = False
                    if (ctx.xml.attr_value(xml_node, "@name") != None):
                        ctx.out.put("%s" % _var_name)
                        _menu_ref_set = True
                    elif (ctx.xml.attr_value(xml_node, "@ref") != None):
                        _xml_menu = ctx.cache.node(xml_node).cli_Cli2xxx_menu
                        if (_xml_menu != None):
                            ctx.out.put("%s" % ctx.Utils.node2var(ctx, _xml_menu))
                            _menu_ref_set = True
                    if (not(_menu_ref_set)):
                        ctx.Utils.abort(ctx, xml_node, "missing menu/@name or menu/@ref attribute, or invalid menu/@ref reference")
                    ctx.out.put("))")
                else:
                    # Populate other kind of item.
                    ctx.out.put(".addElement(new %s(" % _class_name)
                    if (with_creation_params):
                        ctx.Menu.Constructor.params(ctx, xml_node)
                    ctx.out.put("))")
                # Terminate the line.
                ctx.out.put(";").endl()
        
        class Execute:
            """ Menu.Execution code generation routines. """
            
            def __init__(self):
                """ Constructor. """
                # Static class, nothing to be done
                pass
            
            @staticmethod
            def execute(ctx, xml_menu):
                """ Menu execution code generation.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Utils.indent(ctx, xml_menu, 1).put("// ----- Menu execution -----").endl()
                
                # Execution method
                ctx.Utils.indent(ctx, xml_menu, 1).put("public boolean execute(cli.CommandLine CLI_CmdLine) {").endl()
                
                ctx.Utils.indent(ctx, xml_menu, 2).put("try {").endl()
                
                # Trace
                ctx.Utils.indent(ctx, xml_menu, 3).put("cli.TraceClass CLI_EXECUTION = new cli.TraceClass(\"CLI_EXECUTION\", new cli.Help()")
                ctx.out.put(".addHelp(cli.Help.LANG_EN, \"CLI Execution traces\")")
                ctx.out.put(u".addHelp(cli.Help.LANG_FR, \"Traces d'ex�cution du CLI\")")
                ctx.out.put(");").endl()
                
                # Step variables
                ctx.Utils.indent(ctx, xml_menu, 3).put("java.util.Iterator<cli.Element> cli_Elements = CLI_CmdLine.iterator();").endl()
                ctx.Utils.indent(ctx, xml_menu, 3).put("cli.Element cli_Element = null;").endl()
                
                # Call implementation on the menu object
                ctx.Menu.Execute.execute_node(ctx, xml_menu, xml_menu, indent_count = 3)
                
                # Finishing
                ctx.Utils.indent(ctx, xml_menu, 2).put("} catch (java.util.NoSuchElementException e1) {").endl()
                ctx.Utils.indent(ctx, xml_menu, 2).put("} catch (java.lang.Exception e) {").endl()
                ctx.Utils.indent(ctx, xml_menu, 3).put(    "getErrorStream().printStackTrace(e);").endl()
                ctx.Utils.indent(ctx, xml_menu, 2).put("}").endl()
                ctx.Utils.indent(ctx, xml_menu, 2).put("return false;").endl()
                
                ctx.Utils.indent(ctx, xml_menu, 1).put("}").endl()
                ctx.out.endl()
            
            @staticmethod
            def execute_node(ctx, xml_menu, xml_node, indent_count):
                """ Recursive execution code generation routine.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node.
                    @param xml_node (XML node) Current node which execution code is generated for.
                    @param indent_count (int) Number of indentation from the menu offset. """
                # Top comment
                ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("// " + ctx.Utils.node2desc(ctx, xml_node)).endl()
                
                # Block label
                ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("%s:" % ctx.Utils.node2lbl(ctx, xml_node)).endl()
                
                # Start the block
                if (ctx.xml.is_node(xml_node, "cli:cli") or ctx.xml.is_node(xml_node, "cli:menu") or ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("{").endl()
                elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@id")):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("do {").endl()
                elif (ctx.xml.is_node(xml_node, "cli:param")):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("if (%s.matches(cli_Element)) {" % ctx.Utils.node2var(ctx, xml_node)).endl()
                else:
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("if (cli_Element == %s) {" % ctx.Utils.node2var(ctx, xml_node)).endl()
                
                # Step it
                if (not(ctx.xml.is_node(xml_node, "cli:tag")) and not(ctx.xml.is_node(xml_node, "cli:endl"))):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 1).put("cli_Element = cli_Elements.next();").endl()
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 1).put("if (cli_Element == null) return false;").endl()
                
                # Trace current keyword
                ctx.Utils.indent(ctx, xml_menu, indent_count + 1).put("cli.Traces.trace(CLI_EXECUTION, ")
                ctx.out.put("\"context = \\\"%s\\\", \"" % ctx.Utils.node2desc(ctx, xml_node))
                ctx.out.put("+ \"word = \" + (")
                ctx.out.put(    "cli_Element instanceof cli.Endl ")
                ctx.out.put(    "? \"<CR>\" ")
                ctx.out.put(    ": cli_Element.getKeyword()")
                ctx.out.put("));").endl()
                
                # Execution
                _xml_sources = []
                for _xml_source in ctx.xml.children(xml_node):
                    if (ctx.xml.is_node(_xml_source, "cli:java")):
                        if (ctx.xml.attr_value(_xml_source, "@option") == None):
                            _xml_sources.append(_xml_source)
                if (len(_xml_sources) > 0):
                    ctx.out.put(ctx.args.user_indent()).endl()
                    for _xml_source in _xml_sources:
                        ctx.Utils.indent(ctx, xml_menu, indent_count + 1, with_user_indent = True)
                        ctx.Utils.source_node(ctx, _xml_source)
                        ctx.out.endl()
                    ctx.out.put(ctx.args.user_indent()).endl()
                
                # Sub-elements
                ctx.Menu.Execute.walk(ctx, xml_menu, xml_node, indent_count = indent_count + 1)
                
                # Final jump
                if (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@id")):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 1).put("break %s;" % ctx.Utils.node2lbl(ctx, xml_node)).endl()
                elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                    pass
                elif (ctx.xml.is_node(xml_node, "cli:endl")):
                    pass
                else:
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 1).put("return false;").endl()
                
                # End the block
                if (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@id")):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("} while(true);").endl()
                else:
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("}").endl()
            
            @staticmethod
            def walk(ctx, xml_menu, xml_node, indent_count, xml_tag = None):
                """ Second part of recursive execution code generation routine.
                    Propagate over sub-nodes.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node.
                    @param xml_node (XML node) Current node which execution code is generated for.
                    @param xml_tag (XML node) cli:tag[@id] reference being processed: only set jumps.
                    @param indent_count (int) Number of indentation from the menu offset. """
                if (ctx.xml.is_node(xml_node, "cli:endl")):
                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("return true;").endl()
                elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                    # Tag reference
                    _tag_name = ctx.xml.attr_value(xml_node, "@ref")
                    _xml_target = None
                    _xml_target = ctx.cache.node(xml_node).cli_Cli2xxx_tag_id
                    #modlog.log(ctx.MODLOG_FILTER, modlog.DEBUG, "Menu.Execute.walk()|cli:tag[@ref]: _xml_ancestor = %s, _xml_target = %s"
                    #                                            % (ctx.Utils.node2desc(ctx, _xml_ancestor), ctx.Utils.node2desc(ctx, _xml_target)))
                    if (_xml_target == ctx.xml.parent_node(xml_node)):
                        ctx.Utils.abort(ctx, xml_node, "tag reference '%s' directly located in the tag" % _tag_name)
                    elif (_xml_target == None):
                        ctx.Utils.abort(ctx, xml_node, "unknown tag reference '%s'" % _tag_name)
                    elif (_xml_target.cli_Cli2xxx_is_backward):
                        # The referenced tag is an ancestor.
                        # Use a 'continue' statement to avoid inifinite loop generations.
                        #modlog.log(ctx.MODLOG_FILTER, modlog.DEBUG, "Menu.Execute.walk()|cli:tag[@ref]: ancestor")
                        if (xml_tag != None):
                            ctx.Menu.Execute.walk(ctx, xml_menu, _xml_target, indent_count, xml_tag)
                        else:
                            ctx.Menu.Execute.walk(ctx, xml_menu, _xml_target, indent_count, _xml_target)
                    else:
                        # The referenced tag is not an ancestor.
                        # Implementation is required here.
                        ctx.Menu.Execute.walk(ctx, xml_menu, _xml_target, indent_count, None)
                else:
                    for _xml_sub_node in ctx.xml.children(xml_node):
                        if (    ctx.xml.is_node(_xml_sub_node, "cli:keyword") or ctx.xml.is_node(_xml_sub_node, "cli:param")
                                or (ctx.xml.is_node(_xml_sub_node, "cli:tag") and (ctx.xml.attr_value(_xml_sub_node, "@hollow") != "yes"))
                                or ctx.xml.is_node(_xml_sub_node, "cli:endl")):
                            if (xml_tag == None):
                                ctx.Menu.Execute.execute_node(ctx, xml_menu, _xml_sub_node, indent_count)
                            else:
                                if (ctx.xml.is_node_with_attr(_xml_sub_node, "cli:tag", "@ref")):
                                    ctx.Menu.Execute.walk(ctx, xml_menu, _xml_sub_node, indent_count, xml_tag)
                                elif (ctx.xml.is_node_with_attr(_xml_sub_node, "cli:tag", "@id")):
                                    ctx.Menu.Execute.walk(ctx, xml_menu, _xml_sub_node, indent_count, xml_tag)
                                elif (ctx.xml.is_node(_xml_sub_node, "cli:param")):
                                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("if (%s.matches(cli_Element)) "
                                                                                          % ctx.Utils.node2var(ctx, _xml_sub_node))
                                    ctx.out.put("continue %s;" % ctx.Utils.node2lbl(ctx, xml_tag)).endl()
                                else:
                                    ctx.Utils.indent(ctx, xml_menu, indent_count + 0).put("if (cli_Element == %s) "
                                                                                          % ctx.Utils.node2var(ctx, _xml_sub_node))
                                    ctx.out.put("continue %s;" % ctx.Utils.node2lbl(ctx, xml_tag)).endl()
        
        # Handlers
        class Handlers:
            """ Menu.Handlers code generation routines. """
            
            def __init__(self):
                """ Constructor. """
                # Static class, nothing to be done
                pass
            
            @staticmethod
            def execute(ctx, xml_menu):
                """ Executes the Menu.Execute code generation routines.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Menu.Handlers.error_handler(ctx, xml_menu) # cli only
                ctx.Menu.Handlers.exit_handler(ctx, xml_menu)
                ctx.Menu.Handlers.prompt_handler(ctx, xml_menu)
            
            @staticmethod
            def error_handler(ctx, xml_menu):
                """ Error handler code generation.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                # Error handler
                if (ctx.xml.is_node(xml_menu, "cli:cli")):
                    _xml_extra_sources = ctx.xml.xpath_set(xml_menu, "cli:handler[@name='error']/cli:java")
                    if (len(_xml_extra_sources) > 0):
                        ctx.Utils.indent(ctx, xml_menu, 1).put("public boolean onError(")
                        ctx.out                           .put(    "cli.ResourceString location, ")
                        ctx.out                           .put(    "cli.ResourceString message")
                        ctx.out                           .put(") {").endl()
                        ctx.out.put(ctx.args.user_indent()).endl()
                        for _xml_extra_source in _xml_extra_sources:
                            ctx.Utils.indent(ctx, xml_menu, 2, with_user_indent = True)
                            ctx.Utils.source_node(ctx, _xml_extra_source)
                            ctx.out.endl()
                        ctx.out.put(ctx.args.user_indent()).endl()
                        #ctx.Utils.indent(ctx, xml_menu, 2).put("return super.onError(location, message);").endl()
                        ctx.Utils.indent(ctx, xml_menu, 1).put("}").endl()
                        ctx.out.endl()
            
            @staticmethod
            def exit_handler(ctx, xml_menu):
                """ Exit handler code generation.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Utils.indent(ctx, xml_menu, 1).put("public void onExit() {").endl()
                _xml_extra_sources = ctx.xml.xpath_set(xml_menu, "cli:handler[@name='exit']/cli:java")
                if (len(_xml_extra_sources) > 0):
                    ctx.out.put(ctx.args.user_indent()).endl()
                    for _xml_extra_source in _xml_extra_sources:
                        ctx.Utils.indent(ctx, xml_menu, 2, with_user_indent = True)
                        ctx.Utils.source_node(ctx, _xml_extra_source)
                        ctx.out.endl()
                    ctx.out.put(ctx.args.user_indent()).endl()
                ctx.Utils.indent(ctx, xml_menu, 1).put("}").endl()
                ctx.out.endl()
            
            @staticmethod
            def prompt_handler(ctx, xml_menu):
                """ Prompt handler code generation.
                    @param ctx (Cli2Java) Execution context.
                    @param xml_menu (XML node) cli:cli or cli:menu node. """
                ctx.Utils.indent(ctx, xml_menu, 1).put("public java.lang.String onPrompt() {").endl()
                _xml_extra_sources = ctx.xml.xpath_set(xml_menu, "cli:handler[@name='prompt']/cli:java")
                if (len(_xml_extra_sources) > 0):
                    ctx.out.put(ctx.args.user_indent()).endl()
                    for _xml_extra_source in _xml_extra_sources:
                        ctx.Utils.indent(ctx, xml_menu, 2, with_user_indent = True)
                        ctx.Utils.source_node(ctx, _xml_extra_source)
                        ctx.out.endl()
                    ctx.out.put(ctx.args.user_indent()).endl()
                else:
                    ctx.Utils.indent(ctx, xml_menu, 2).put("return super.onPrompt();").endl()
                ctx.Utils.indent(ctx, xml_menu, 1).put("}").endl()
                ctx.out.endl()
    
    class Utils (clicommon.CtxUtils) :
        """ Utils routines. """
        
        def __init__(self):
            """ Constructor. """
            # Static class, nothing to be done
            clicommon.CtxUtils.__init__(self)
        
        @staticmethod
        def extra_source(ctx, xml_node, option):
            """ Extra source generation.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Focus node to generate extra source section for. May be None.
                @param option (str) Extra source @option attribute value.
                @return True for success, False otherwise. """
            #modlog.log(ctx.MODLOG_FILTER, modlog.DEBUG, "_extra_source(option = %s)" % option)
            # Determine indentation count
            _indent_count = 0
            if (option == "constructor"):
                _indent_count = 2
            elif (option == "members"):
                _indent_count = 1
            #modlog.log(ctx.MODLOG_FILTER, modlog.DEBUG, "_extra_source(): _indent_count = %d" % _indent_count)
            # Begin extra code section
            ctx.Utils.indent(ctx, xml_node, _indent_count).put("// ----- Extra java (option='%s') -----" % option).endl()
            # Find out extra code nodes
            _xml_extra_sources = ctx.xml.xpath_set(xml_node, "cli:java[@option='%s']" % option)
            if (len(_xml_extra_sources) > 0):
                # Determine whether user-code should be spotted
                _show_user_indent = ((option =='members') or (option == "constructor"))
                # Generate extra code
                if (_show_user_indent):
                    ctx.out.put(ctx.args.user_indent()).endl()
                for _xml_extra_source in _xml_extra_sources:
                    ctx.Utils.indent(ctx, None, _indent_count, with_user_indent = _show_user_indent)
                    ctx.Utils.source_node(ctx, _xml_extra_source)
                    ctx.out.endl()
                if (_show_user_indent):
                    ctx.out.put(ctx.args.user_indent()).endl()
            # Finish
            ctx.out.endl()
            return True
        
        @staticmethod
        def source_node(ctx, xml_source):
            """ Put out the code for a source node.
                @param ctx (Cli2Java) Execution context.
                @param xml_source (XML node) Source node to process. """
            for _xml_child in ctx.xml.children(xml_source):
                if (ctx.xml.is_node(_xml_child, "cli:out")):
                    ctx.out.put("getOutputStream()")
                elif (ctx.xml.is_node(_xml_child, "cli:err")):
                    ctx.out.put("getErrorStream()")
                elif (ctx.xml.is_node_with_attr(_xml_child, "cli:value-of", "@param")):
                    _param_id = ctx.xml.attr_value(_xml_child, "@param")
                    _xml_param = xml_source
                    while (_xml_param != None):
                        if (ctx.xml.is_node_with_attr_value(_xml_param, "cli:param", "@id", _param_id)):
                            break
                        _xml_param = ctx.xml.parent_node(_xml_param)
                    if (_xml_param != None):
                        ctx.out.put("%s.getValue()" % ctx.Utils.node2var(ctx, _xml_param))
                    else:
                        ctx.Utils.abort(ctx, _xml_child, "unknown parameter reference '%s'" % _param_id)
                else:
                    ctx.out.put(ctx.xml.content(_xml_child))
        
        @staticmethod
        def indent(ctx, xml_node, count, with_user_indent = False):
            """ Output indentation.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Context node.
                @param count (int) Number of indentations to output.
                @param with_user_indent (boolean) True to display user-indent indentation pattern, False otherwise.
                @return (Output) Output device. """
            # Ensure the reference of the XML node for caching
            if (xml_node == None):
                xml_node = ctx.xml.root_node()
            xml_node = ctx.cache.node(xml_node)
            
            # Check whether the indent computation is already cached
            _cache_attr_name = "cli_Cli2Java_indent" + str(count) + str(with_user_indent)
            _str_indent = getattr(xml_node, _cache_attr_name, None)
            
            if (_str_indent == None):
                _str_indent = ""
                
                _offset = ctx.Utils.indent_offset(ctx, xml_node)
                
                #_filter = ctx.MODLOG_FILTER + ".Utils.indent"
                #modlog.log(_filter, modlog.DEBUG, "Utils.indent(_offset = %d, count = %d, with_user_indent = %d)" % (_offset, count, with_user_indent))
                
                _count = _offset + count
                _indent_length = _count * len(ctx.args.indent())
                
                if (with_user_indent):
                    _str_indent += ctx.args.user_indent()
                    _indent_length -= len(ctx.args.user_indent())
                while (_indent_length > 0):
                    _str_indent += ctx.args.indent()
                    _indent_length -= len(ctx.args.indent())
                
                setattr(xml_node, _cache_attr_name, _str_indent)
            
            ctx.out.put(_str_indent)
            return ctx.out
        
        @staticmethod
        def indent_offset(ctx, xml_node):
            """ Indentation offset computation.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Context node.
                @return (int) Indentation offset. """
            _indent_offset = 0
            if (xml_node != None):
                #try:
                xml_node = ctx.cache.node(xml_node)
                _indent_offset = getattr(xml_node, "cli_Cli2Java_indent_offset", None)
                if (_indent_offset == None):
                    if (ctx.xml.is_node(xml_node, "cli:cli")):
                        _indent_offset = 0
                    elif (ctx.xml.is_node(xml_node, "cli:menu")):
                        _indent_offset = 1
                    else:
                        # Default: rely on the parent's computation value.
                        _indent_offset = ctx.Utils.indent_offset(ctx, ctx.xml.parent_node(xml_node))
                    # Cache result
                    setattr(xml_node, "cli_Cli2Java_indent_offset", _indent_offset)
            return _indent_offset
        
        @staticmethod
        def node2desc(ctx, xml_node):
            """ Description computation.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Node to compute a description for.
                @return (str) Description computed. """
            
            _desc = ""
            
            if (xml_node == None):
                _desc = "nil"
            else:
                xml_node = ctx.cache.node(xml_node)
                _desc = getattr(xml_node, "cli_Cli2Java_desc", None)
                if (_desc == None):
                    _desc = ""
                    
                    # Recursive call
                    if (not(ctx.xml.is_node(xml_node, "cli:cli")) and not(ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@name"))):
                        _desc += (ctx.Utils.node2desc(ctx, ctx.xml.parent_node(xml_node)) + " ")
                    
                    # Current item description
                    if (ctx.xml.is_node(xml_node, "cli:cli") or ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@name")):
                        _desc += (ctx.xml.attr_value(xml_node, "@name") + ">")
                    elif (ctx.xml.is_node(xml_node, "cli:keyword")):
                        _desc += ctx.xml.attr_value(xml_node, "@string")
                    elif (ctx.xml.is_node(xml_node, "cli:param")):
                        _desc += ("$" + ctx.xml.attr_value(xml_node, "@id"))
                    elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@id")):
                        _desc += ("[" + ctx.xml.attr_value(xml_node, "@id") + ":]")
                    elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                        _desc += ("[-> " + ctx.xml.attr_value(xml_node, "@ref") + "]")
                    elif (ctx.xml.is_node(xml_node, "cli:endl")):
                        _desc += "<CR>"
                    else:
                        _desc += "???"
                    
                    # Cache result
                    setattr(xml_node, "cli_Cli2Java_desc", _desc)
            
            return _desc
        
        @staticmethod
        def node2var(ctx, xml_node, menu_with_cli_pointer = True):
            """ Determine a variable for the given node.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Node to find a variable name for.
                @param menu_with_cli_pointer (bool) Include owner cli pointer access for menus in the variable name.
                                                    Rem: all menu variables are members of the CLI object.
                @return (str) Variable name. """
            _with_cli_pointer = False
            _var_name = ""
            
            if (xml_node != None):
                #try:
                xml_node = ctx.cache.node(xml_node)
                _with_cli_pointer = getattr(xml_node, "cli_Cli2Java_cli_pointer", None)
                _var_name = getattr(xml_node, "cli_Cli2Java_var_name", None)
                if ((_with_cli_pointer == None) or (_var_name == None)):
                    _with_cli_pointer = False
                    _var_name = ""
                    
                    # Menu: access through the m_cliOwnerCli member, unless menu_with_cli_pointer is disabled.
                    if (ctx.xml.is_node(xml_node, "cli:cli") or ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@name")):
                        _with_cli_pointer = True
                    
                    # Variable prefix
                    _var_name += ctx.args.var_prefix()
                    
                    # Element type
                    if (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                        _var_name += "tagref"
                    elif (ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@ref")):
                        _var_name += "menuref"
                    else:
                        _var_name += ctx.Utils.mk_java_name(ctx, xml_node, ctx.xml.node_name(xml_node, with_ns = False))
                    
                    # Element name
                    if (ctx.xml.is_node(xml_node, "cli:keyword")):
                        _var_name += "_" + ctx.Utils.mk_java_name(ctx, xml_node, ctx.xml.attr_value(xml_node, "@string"))
                    elif (ctx.xml.is_node(xml_node, "cli:param")):
                        _var_name += "_" + ctx.Utils.mk_java_name(ctx, xml_node, ctx.xml.attr_value(xml_node, "@id"))
                    
                    # Id
                    _var_name += "_" + ctx.Utils.node2id(ctx, xml_node)
                    
                    # Cache result
                    setattr(xml_node, "cli_Cli2Java_cli_pointer", _with_cli_pointer)
                    setattr(xml_node, "cli_Cli2Java_var_name", _var_name)
            
            if (menu_with_cli_pointer and _with_cli_pointer):
                _var_name = ("m_cliOwnerCli." + _var_name)
            return _var_name
        
        @staticmethod
        def node2id(ctx, xml_node, separator = ""):
            """ Determine an identifier for the given node.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Node to find a variable name for.
                @param separator (str) Separator pattern.
                @return (str) Node identifier. """
            # Ensure the reference of the XML node for caching
            if (xml_node == None):
                xml_node = ctx.xml.root_node()
            xml_node = ctx.cache.node(xml_node)
            
            _id = getattr(xml_node, "cli_Cli2Java_id", None)
            if (_id == None):
                _preceding_count = xml_node.cli_Cli2xxx_child_index
                
                # Recursive call.
                _parent_id = ""
                _xml_parent = ctx.xml.parent_node(xml_node)
                if (_xml_parent != None):
                    if (ctx.xml.is_node(_xml_parent, "cli:*")):
                        if (not(ctx.xml.is_node(_xml_parent, "cli:cli"))):
                            _separator = ""
                            if (_preceding_count >= 26+26):
                                _separator = "_"
                            _parent_id = ctx.Utils.node2id(ctx, _xml_parent, separator = _separator)
                # Generate id part based on the position of the node regarding its preceding-sibling.
                # Start with a,b,c...x,y,z,A,B,C...X,Y,Z, then numbers from 0 seperated with '_' if numbers follow each others.
                _this_id = ""
                if (_preceding_count < 26):
                    _this_id = chr(97 + _preceding_count)
                elif (_preceding_count < 26+26):
                    _this_id = chr(65 + _preceding_count - 26)
                else:
                    _this_id = str(_preceding_count-26-26) + "_"
                
                # Cache result.
                _id = _parent_id + _this_id
                setattr(xml_node, "cli_Cli2Java_id", _parent_id + _this_id)
            
            if ((separator == "") and _id.endswith("_")):
                _id = _id[:-1]
            return _id
        
        @staticmethod
        def node2class(ctx, xml_node):
            """ Determine a class name for the given node.
                @param ctx (Cli2Java) Execution context.
                @param (XML node) Node to find a class name for.
                @return (str) Class name. """
            _class_name = ""
            # Root node <=> Main CLI class
            if (ctx.xml.is_node(xml_node, "cli:cli")):
                _class_name = ctx.args.cli_class_name(ctx)
            # Menu and menu references
            elif (ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@name")):
                _class_name = ctx.args.class_prefix() + ctx.Utils.mk_java_name(ctx, xml_node, ctx.xml.attr_value(xml_node, "@name"))
            elif (ctx.xml.is_node_with_attr(xml_node, "cli:menu", "@ref")):
                _class_name = "cli.MenuRef"
            elif (ctx.xml.is_node(xml_node, "cli:menu")):
                ctx.Utils.abort(ctx, xml_node, "menu node should either have @name or @ref attribute.")
            # Keywords
            elif (ctx.xml.is_node(xml_node, "cli:keyword")):
                _class_name = "cli.Keyword"
            # Parameters
            elif (ctx.xml.is_node_with_attr(xml_node, "cli:param", "@type")):
                _type = ctx.xml.attr_value(xml_node, "@type")
                if (_type == "string"):
                    _class_name = "cli.ParamString"
                elif (_type == "int"):
                    _class_name = "cli.ParamInt"
                elif (_type == "float"):
                    _class_name = "cli.ParamFloat"
                elif (_type == "host"):
                    _class_name = "cli.ParamHost"
                else:
                    ctx.Utils.abort(ctx, xml_node, "unknown param type '%s'" % _type)
            elif (ctx.xml.is_node(xml_node, "cli:param")):
                ctx.Utils.abort(ctx, xml_node, "missing @type attribute")
            # End of line
            elif (ctx.xml.is_node(xml_node, "cli:endl")):
                _class_name = "cli.Endl"
            # Tags
            elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@id")):
                _class_name = "cli.SyntaxTag"
            elif (ctx.xml.is_node_with_attr(xml_node, "cli:tag", "@ref")):
                _class_name = "cli.SyntaxRef"
            elif (ctx.xml.is_node(xml_node, "cli:tag")):
                ctx.Utils.abort(ctx, xml_node, "tag node should either have @id or @ref attribute.")
            # Fallback
            else:
                ctx.Utils.abort(xml_node, "unknown cli node '%s'" % ctx.xml.node_name(xml_node))
            
            return _class_name
        
        @staticmethod
        def node2lbl(ctx, xml_node):
            """ Top label name computation.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Node to compute a label name for.
                @return (str) Top label name. """
            return (ctx.Utils.node2var(ctx, xml_node, menu_with_cli_pointer = False) + "_lbl")
        
        @staticmethod
        def mk_java_name(ctx, xml_node, symbol):
            """ Build a regular C++ symbol from a given resource.
                @param ctx (Cli2Java) Execution context.
                @param xml_node (XML node) Current node being processed.
                @param symbol (str) Text resource to build a regular C++ symbol from.
                @return (str) Regular C++ symbol. """
            _filter = ctx.MODLOG_FILTER + ".Utils.mk_java_name"
            if ((symbol == None) or (symbol == "")):
                ctx.Utils.abort(ctx, xml_node, "Cannot make a C++ symbol from an empty string.")
            _java_name = symbol
            # Replace every special character with an underscore.
            for _special in " ,:<>*|+-()#.":
                _java_name = _java_name.replace(_special, "_")
            # Replace every accented character with its non-accented equivalent.
            _tr = {}
            _tr["a"] = [ "á", "à", "â", "ä"]
            _tr["c"] = [ "ç" ]
            _tr["e"] = [ "�", "è", "ê", "ë" ]
            _tr["i"] = [ "í", "ì", "î", "ï" ]
            _tr["o"] = [ "ó", "ò", "ô", "ö" ]
            _tr["u"] = [ "ú", "ù", "û", "ü" ]
            for _no_accent in _tr.keys():
                for _accent in _tr[_no_accent]:
                    _java_name = _java_name.replace(_accent, _no_accent)
            
            return _java_name
        
# Main call
main()
