Tcl4bb [Tickle for Blackbox]
 - Written by Brian "tresni" Hartvigsen
 - a plugin that allows users to utilize Tcl [Tool Command Language] in xoblite/bblean

0. Foreword
 I make no gaurentees about the functionality of this plugin.  I've fixed
 every crash I've encountered and it seems to be stable.  I am sure the usage
 of this plugin is severly limited, I wrote this a long time ago because I
 always wondered why shells like LiteStep used their own scripting language
 [mzScript] instead of a readily avialable one.  This was written [and shown to
 qwilk] before LSLua ever saw the light of day.  It was never released as there
 was still a crash if the plugin was unloaded and reloaded, and I didn't think
 anyone would want it.  The only reason I'm releasing this now is that
 SpyderBlade commented that "lua scripting would rock."  Well, it's not lua,
 but I hope it still rocks.

 The huge plugin size is due to the fact that I have staticly linked the Tcl
 library, this makes it so users will not need to install Tcl for the plugin to
 work.  Hope that's worth the extra file size, otherwise this compiles to a
 more normal size.
 
1. Settings
 Settings are stored in the tcl4bb.rc file.
	
  tcl-bb.start.eval: <path/to/script.tcl>
  tcl-bb.var.<name>: <value>

 Variables are not read in on startup, they must be read in with tcl-bb:readvar
 and can be saved with tcl-bb:writevar;
 
2. Bro@ms
 Currently there are only two bro@ms.
 
  @tcl-bb evalfile <path/to/script.tcl>
  @tcl-bb eval     <valid tcl statement>
  
3. Using Tcl4bb in scripts
 All of Tcl4bb is encapsulated in the tcl-bb namespace.  The entire namespace,
 or certain functions, may be imported in a script for ease of use.  This is not
 required as functions may be called with the tcl-bb:: prefix.
 
  ex. namespace import tcl-bb::*
      namespace import tcl-bb::*menu

4. Avialable Tcl4bb functions
  tcl-bb::editstyle
  tcl-bb::editmenu
  tcl-bb::editplugins
  tcl-bb::editextension
  tcl-bb::editblackbox
    Opens the corresponding rc files for editing for editing.
    
  tcl-bb::mainmenu
  tcl-bb::workspacemenu
  tcl-bb::toolbarmenu
  tcl-bb::systembarmenu
  tcl-bb::slitmenu
  tcl-bb::hidemenu
    Show or hide the various menus
    
  tcl-bb::toggletray
  tcl-bb::togglesystembar
  tcl-bb::toggleslit
  tcl-bb::toggletoolbar
  tcl-bb::toggleplugins
    Toggle corresponding system components
    
  tcl-bb::deskleft
  tcl-bb::deskright
    Change workspace in appropriate direction
    
  tcl-bb::adddesk
  tcl-bb::deletedesk [alias: tcl-bb::deldesk]
    Manage workspaces
  
  tcl-bb::shutdown
  tcl-bb::reboot
  tcl-bb::logoff
  tcl-bb::hibernate
  tcl-bb::suspend
  tcl-bb::lockworkstation
    Various shutdown options
    
  tcl-bb::quit
  tcl-bb::restart
  tcl-bb::reconfig
    Control xoblite/bblean
  
  tcl-bb::run
    Opens the run dialog
    
  tcl-bb::windowgather [alias: tcl-bb::gatherwindows]
    Gather windows to current workspace
    
  tcl-bb::aboutstyle
  tcl-bb::aboutplugins
    Open dialog showing information about style/plugins
    
  tcl-bb::pause (<seconds>) [alias: tcl-bb::softpause]
  tcl-bb::hardpause
    Cause the execution of the script to pause.  Pauses for <seconds> or until
    the message box is dismissed if <seconds> is <= 0 or non-present.  Can be a
    fractional number (e.g. 1.5 seconds or 0.123 seconds)
    
  tcl-bb::bro@m <broam> [alias: tcl-bb::broam]
    Send <broam> as bro@m to xoblite/bblean
    
  tcl-bb::label <label>
    Change toolbar label to <label>
  
  tcl-bb::style <style file>
    Change to <style file>
    
  tcl-bb::shellexecute <operation> <file> (<params> (<working directory>))
    Essentially the same as typing a command or filename into a run dialog.
    Operation is usually "open" or "exec"
   tcl-bb::exec is the same as "tcl-bb::shellexecute exec"
   tcl-bb::open is the same as "tcl-bb::shellexecute open"
  
  tcl-bb::sound <file> <sync*|async>
    Play wav file.  sync cause the execution of the thread to wait till the wav
    file is done playing.
   tcl-bb::syncsound  is the same as "tcl-bb::sound <file> sync"
   tcl-bb::asyncsound is the same as "tcl-bb::sound <file> async"
  
  tcl-bb::readvar <name>
  tcl-bb::writevar <name> <value>
    Load/Save variable information.
    
  tcl-bb::log <message>
  tcl-bb::mbox <message>
    Save information to the blackbox log.  Optionally show a messabe box of the msg
    to the user (using mbox)

5. Plans
 I would like to add the ability to link a bro@m to a script or function much
 like what LSLua allows for now.  This is a little more complicated in BB4Win
 then in LiteStep though, not impossible mind you.

 If you have any ideas feel free to leave them in the Tcl4bb thread on
 http://www.lostinthebox.com
 
6. Changelog
 1.0.0 - Initial Release
 1.2.0
   + added tcl-bb::mbox & tcl-bb::log
   ! no longer fails silently if @tcl-bb eval/evalfile or tcl-bb.start.eval fails
     also throws an error if a bro@m is formatted incorrectly
 2.3.2
   ! tcl-bb::pause now takes decimal numbers