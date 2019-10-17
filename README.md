# simple-linux-device-driver
Device Driver (module) that turns off the computer immediately or at a scheduled time by keyboard key combination.

This is an Operational System course assignment, the goal was to provide a little experience in programming at "Kernel Space". The module works on Linux.
Attach the module:
  <br><br>1- Run make:
    <code>$ make</code>
  <br>2- Attach module:
    <code>$ sudo insmod keyshut.ko</code>
  <br>3- Removing:
    <code>$ sudo rmmod keyshut.ko</code>
  <br>4- Check the last message of the module.:
    <code>$ dmesg | tail -1</code>
     
<br><br>Running:
  After attaching the module you can use it to shut down your computer in the following ways: immediately or scheduled.
  1. To immediately shut down press SHIFT + C simultaneously;
  2. To schedule press SHIFT + T simultaneously and choose a time in the following format:
    Ex: 2h15m12s, to shutdown your computer in 2 hours, 15 minutes e 12 seconds. Just the seconds are mandatory.
