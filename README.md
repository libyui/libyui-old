# LibYUI - The Base Library

[![Build Status](https://travis-ci.org/libyui/libyui.svg?branch=master)](https://travis-ci.org/libyui/libyui)

Libyui is a widget abstraction library providing Qt, GTK and ncurses
frontends. Originally it was developed for [YaST](https://yast.github.io/)
but it can be used in any independent project.

This part contains the base abstraction layer which is implemented in several
target frontends.


### Selecting the used UI-plugin

By default LibYUI tries to load any of the available UI-plugins in this order:

* Qt:
  - if $DISPLAY is set
  - NCurses is user-selected and stdout is *not* a TTY

* Gtk:
  - if $DISPLAY is set and Qt is not available,
  - a GTK-based desktop environment is detected from the environment variable
    XDG_CURRENT_DESKTOP
  - any of the above pre-conditions are met and NCurses is user-selected, but
    stdout is *not* a TTY

* NCurses:
  - if $DISPLAY is *not* set and stdout is a TTY
  - Qt and Gtk are not available and stdout is a TTY

This can be overridden by either:

* specifing one of the switches on the command-line of the program
  - `--gtk`,
  - `--ncurses`, or
  - `--qt`

* setting the environment variable YUI_PREFERED_BACKEND to one of
  - `gtk`,
  - `ncurses`, or
  - `qt`

If a command-line switch is given to the program, the setting from the environment
variable will be overridden by the UI-plugin chosen with the switch.

If the user-selected UI-plugin is not installed on the system, an installed
UI-plugin will be chosen by the above criteria.


### Building

Libyui uses CMake, driven by a slightly complex set of
[CMakefiles](https://github.com/libyui/libyui/tree/master/buildtools). For
reproducible builds it is best to use the [libyui-rake](
https://github.com/libyui/libyui-rake) Ruby gem like the [Jenkins CI](
https://ci.opensuse.org/view/libyui/) jobs do.

It can be installed from [rubygems.org](https://rubygems.org/gems/libyui-rake/)
using this command (Ruby needs to be installed in the system):

```
gem install libyui-rake
```

Then to build the package run:

```
rake osc:build
```
