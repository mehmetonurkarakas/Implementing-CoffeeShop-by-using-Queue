# Implementing-CoffeeShop-by-using-Queue
Data Stuctures-Queue

Heraclitus once said “The only constant in life is change”. Heraclitus was a wise man, indeed,
but he missed an exception. If you take BBM 203, you solve a Discrete Event Simulation
(DES) project!
Well, but what is DES?
Enter, Wikipedia: “A discrete-event simulation (DES) models the operation of a system as
a (discrete) sequence of events in time. Each event occurs at a particular instant in time
and marks a change of state in the system. Between consecutive events, no change in the
system is assumed to occur; thus the simulation time can directly jump to the occurrence
time of the next event”. In other words, DES is the representation of a certain process by
simulating certain events which occur at certain times. Hence, time is not continuous in DES
and progress according to the events. As a demonstrative example, a process of airplane
boarding can be simulated as follows:

• TIME: 09:00 Go into the luggage queue.

• TIME: 09:15 Give your luggage and take your boarding pass.

• TIME: 09:20 Go into the security queue.

• TIME: 09:30 Wait for your boarding time and enjoy your free time.

• TIME: 10:00 Go into the boarding queue.

• TIME: 10.30 Have a nice flight!

Note that in DES the only things that count are events and everything else is ignored. In the
case of the passenger, until 10:31 only 6 things happened. Additionally, the time progressed
in a discrete fashion (i.e. jumped from one event to the next one).
In this experiment, you are expected to simulate HUCoffeeShop, which splits and executes
tasks simultaneously in a way (by using queue data structure) that will not affect the outcome.
