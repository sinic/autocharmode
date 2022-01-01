This proof of concept is in an unpolished state,
but turned out to be so useful in day-to-day usage
that I'm publishing it regardless.

# The Problem

In short:
[EXWM's simulation keys](https://github.com/ch11ng/exwm/wiki#simulation-keys)
can remap an X application's keybindings, so that it is possible to,
for example, control Firefox using vi-like keybindings,
purely through the window manager and without resorting to any add-ons.

One unfortunate drawback compared to browser add-ons, however, is
that state internal to applications is invisible to EXWM:
with the cursor in the address bar or in a web page's text field,
one would expect input to show up verbatim, but for that,
one has to switch from `line-mode` (i.e., simulation keys) to `char-mode`
*manually*!
It's easy to imagine (and experience!) that
forgetting about toggling input modes wreaks havoc on a browsing session.

# The Solution

This program (ab)uses GTK's input method mechanism,
which is normally used to open an on-screen keyboard
when text is about to be entered anywhere in a GTK application,
and to close it afterwards.
Here, instead,
`emacs-client` is called to toggle `char-mode` for the appropriate window.
Now it is hardly ever necessary to toggle input modes manually!

### Usage

1. Make sure that an Emacs server runs
   by executing the `server-start` command in your EXWM session.

2. Build this project with `make` and
   start Firefox (or any other GTK 3 application of your choice)
   so that it uses the new input method module, adapt the path as necessary:
```
GTK_IM_MODULE_FILE=$HOME/charmode/immodules.cache GTK_IM_MODULE=charmode firefox
```

3. Confirm that the input mode toggles automatically,
   for example by selecting the address bar.
