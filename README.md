# Design Patterns

# By Artem Shabalin and Barak Sharabi
Sixth assigment in Operating System Course

This assigment is about design patterns and thread protected queue.

## Shortly about the implemented patterns:
> </b>`Active Object:`
* A proxy, which provides an interface towards clients with publicly accessible methods.
* An interface which defines the method reques on an active object.
* A list of pending requests from clients.
* A scheduler, which decides which request to execute next.
* The implementation of the active object method.
* A callback or variable for the client to receive the result.

> </b>`Guard:` a guard is a boolean expression that must evaluate to true if the program execution is to continue in the branch in question. The guard provides an early exit from a subroutine, and is a commonly used deviation from structured programming, removing one level of nesting and resulting in flatter code.

> </b>`Singelton:` the singelton pattern restricts the instantiation of a class to one "single" instance. This is useful when exactly one object is needed to coordinate actions across the sytem.

> </b>`Reactor:` the reactor design pattern is an event handling pattern for handling service requests delieverd concurrently to a service handler ny one or more inputs. The service handler then demultiplexes the incoming requests and dispatches them synchronously to the associated request handlers.