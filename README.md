# TrakRun

A small utility that disables sleep and keeps the screen on while Traktor is running.

## WHY?

If you're using an external controller, you might not touch the mouse or keyboard for a long time.
Since Windows doesn't consider the MIDI inputs from your controller active use it will eventually
turn off your screen or even go into sleep mode unless you disabled them in your power plan.

There's a handy Windows API function for programs to tell it they want the screen kept on and the
PC staying active. For reasons unknown, Traktor doesn't appear to call that API, at least not up
to v3.11.1.

## WHAT?

TrakRun will launch Traktor and call the correct Windows API in its stead, then it will wait until
Traktor exits. This way Windows will keep the PC and monitor on as long as TrakRun, and by extension
Traktor are running.

## HOW?

Place TrakRun.exe in the same directory as Traktor.exe and run it instead of Traktor.exe. That's all.
It's probably a good idea to change the Start Menu shortcut to run TrakRun.exe.

If Traktor is already running, TrakRun will latch onto the running instance instead of launching a
new one; so if for whatever reason Traktor is running without TrakRun and you only find out because
your screen turns off 30 minutes into your gig, you can fix this by adding TrakRun to the mix without
having to restart Traktor.
